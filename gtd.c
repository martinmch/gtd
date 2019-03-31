#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include "clock.h"
#include "counter.h"
#include "IO.h"

// Commands
char* customCommand  = "clear";
char* notifyCommand  = "notify-send";
char* speakCommand   = "&>/dev/null espeak";
char* MPDCommand     = "mpc -q toggle";
char* exename;

// Default settings
int defBreakLength   = 5;
int defSessionLength = 25;
char* notifyWorkMsg  = "Get things done.";
char* notifyBreakMsg = "Take a break.";
char* speakWorkMsg   = "Get things done.";
char* speakBreakMsg  = "Take a break.";

// Flags
bool notifyOnChange = false;
bool toggleMPDonChange = false;
bool startOnBreak = false;
bool writeToTMP = false;
bool updateTMUX = false;

IO
usage(char* progname){
    fprintf(stderr,"USAGE:\n\n");
    fprintf(stderr
           , "    %s [ -bcmnst ] [ work length ] [ break  length ]\n\n"
           , progname);
    fprintf(stderr,"OPTIONS:\n\n");
    fprintf(stderr,"    -b : start on a break\n"); // startOnBreak
    fprintf(stderr,"    -c : custom command (defaults to \"clear\")\n");
    fprintf(stderr,"    -m : toggle MPD on change\n"); // toggleMPDonChange
    fprintf(stderr,"    -n : notify on change\n"); // notifyOnChange
    fprintf(stderr,"    -s : speak command\n");
    fprintf(stderr,"    -t : show time in tmux status bar\n"); // updateTMUX
    fprintf(stderr,"    -T : update time in /tmp file\n"); // writeToTMP
    fprintf(stderr,"    -e : specify time to end at (e.g. 2:45pm today)\n");
    exit(EXIT_FAILURE);
}

IO
error(char* msg, char* progname){
    fprintf(stderr,"\033[0;31m%s\033[0m",msg);
    usage(progname);
}

IO
writeToFile(char* fpath, char* timeString){
    FILE* fp = fopen(fpath, "w");
    fprintf(fp, timeString);
    fclose(fp);
}

IO
countdown(struct clock* cl, bool isBreak){
    char* clockString = toString(cl);
    printf("\r%s", clockString);
    fflush(stdout);
    if(writeToTMP){
        writeToFile("/tmp/gtd", clockString);
    }
    if(updateTMUX){
        writeToFile("/tmp/gtd-tmux", clockString);
        execl("tmux","refresh-client -S");
    }
    free(clockString);
    sleep(1);
    struct clock* decClock = decrementClock(cl);
    free(cl);
    if(clockIsAllZeroes(decClock)){
        if(notifyOnChange){
            if(isBreak){
                execl(notifyCommand, notifyBreakMsg);
            } else {
                execl(notifyCommand, notifyWorkMsg);
            }
        }
        free(decClock);
        return;
    }
    countdown(decClock, isBreak);
}

int main(int argc, char *argv[]) {
    struct clock* session;
    struct clock* sbreak;
    exename = argv[0];

    int opt;
    while((opt = getopt(argc, argv, ":bmnTtec:s:")) != -1)
    {
        switch (opt) {
            case 'b': // start on a break
                startOnBreak = true;
                break;
            case 'm': // toggle MPD on change
                toggleMPDonChange = true;
                break;
            case 'n': // notify on change
                notifyOnChange = true;
                break;
            case 'c': // custom command
                customCommand = optarg;
                break;
            case 's': // speak command
                speakCommand = optarg;
                break;
            case 'T': // update time in /tmp file
                writeToTMP = true;
                break;
            case 't': // show time in tmux status bar
                updateTMUX = true;
                break;
            case 'e': // specify time to end
                // Check if 'at' is installed
                // execute `at "END_TIME"<<<"kill $$"`
            case ':': // If value isn't supplied
                switch (optopt) {
                    case 'c':
                        error("No custom command supplied.\n", exename);
                    case 's':
                        error("No speak command supplied.\n", exename);
                }
                usage(exename);
                break;
            case '?': //
                error("Invalid flag.\n", exename);
                break;
            default:
                abort();
        }
    }

    if (!(argc-optind)) { // If no arguments supplied
        session = newClock(0,defSessionLength,0);
        sbreak = newClock(0,defBreakLength,0);
    } else if (argc-optind == 2) { // If two arguments are supplied
        int sessionMin = (int)strtol(argv[optind], NULL, 10);
        int sbreakMin  = (int)strtol(argv[optind+1], NULL, 10);
        if (!(sessionMin > 0 && sbreakMin > 0)) {
            usage(exename);
        }
        session = minToClock(sessionMin);
        sbreak = minToClock(sbreakMin);
    } else { // If more than two arguments are supplied
        error("Only two arguments are allowed.\n", exename);
    }

    struct counter* instance = newCounter(session, sbreak, startOnBreak);

    while (true){
        printHeader(instance, customCommand);
        bool isBreak = instance->onBreak;
        if(isBreak){
            countdown(instance->breakClock, isBreak);
        } else {
            countdown(instance->sessionClock, isBreak);
            instance->periods++;
        }
        instance->onBreak = !isBreak;
        instance->sessionClock = session;
        instance->breakClock = sbreak;
    }

    freeCounter(instance);
    return 0;
}
