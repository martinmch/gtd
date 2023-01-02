#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include "clock.h"

// Commands
char* customCommand  = "clear";
char* notifyCommand  = "notify-send";
char* speakCommand   = "&>/dev/null espeak";
char* MPDCommand     = "mpc";
char* MPDArgs        = "-q toggle";
char* exename;

// Default settings
int defBreakLength   = 5;
int defWorkLength    = 25;
char* notifyWorkMsg  = "Get things done.";
char* notifyBreakMsg = "Take a break.";
char* speakWorkMsg   = "Get things done.";
char* speakBreakMsg  = "Take a break.";
int workLength;
int breakLength;

// Flags
bool notifyOnChange    = false;
bool toggleMPDonChange = false;
bool startOnBreak      = false;
bool writeToTMP        = false;
bool updateTMUX        = false;

#define WORKCLOCK false
#define BREAKCLOCK true

typedef void IO;

IO
usage(char* progname){
    fprintf(stderr,"USAGE:\n\n");
    fprintf(stderr
           , "    %s [ -bcmnst ] [ work length ] [ break  length ]\n\n"
           , progname);
    fprintf(stderr,"OPTIONS:\n\n");
    fprintf(stderr,"    -b : start on a break\n");
    fprintf(stderr,"    -c : custom command (defaults to \"clear\")\n");
    fprintf(stderr,"    -m : toggle MPD on change\n");
    fprintf(stderr,"    -n : notify on change\n");
    fprintf(stderr,"    -s : speak command\n");
    fprintf(stderr,"    -t : show time in tmux status bar\n");
    fprintf(stderr,"    -T : update time in /tmp file\n");
    fprintf(stderr,"    -e : specify time to end at (e.g. 2:45pm today)\n");
    exit(EXIT_FAILURE);
}

IO
writeToFile(char* fpath, char* timeString){
    FILE* fp = fopen(fpath, "w");
    fprintf(fp, "%s",  timeString);
    fclose(fp);
}

IO
error(char* msg, char* progname){
    fprintf(stderr,"\033[0;31m%s\033[0m",msg);
    usage(progname);
}

IO
clearScreen(){
    if(!system(customCommand)){ /* Error */ }
}

IO
countdown(struct clock* clock){
    char* clockString = toString(clock);
    // Clear the current line, and print the clock string.
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
    struct clock* old = clock;
    clock = decrementClock(clock);
    free(old);
    sleep(1);
}


IO
printHeader(int periods, int breaks, bool onBreak){
    // global workLength
    // global breakLength
    printf("%d minute sessions with %d minute breaks.\n"
        , workLength , breakLength);
    struct clock* workedClock = newClock(0,workLength*(periods-1),0);

    char* worktime = toString(workedClock);

    freeClock(workedClock);

    if (onBreak) {
        printf("Break #%d. Total working time: %s.\n", breaks, worktime);
    } else {
        printf("Period #%d. Total working time: %s\n", periods, worktime);
    }
    free(worktime);
}

int main(int argc, char *argv[])
{
    struct clock* swork;
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
                        break;
                    case 's':
                        error("No speak command supplied.\n", exename);
                        break;
                }
                break;
            case '?': //
                error("Invalid flag.\n", exename);
                break;
            default:
                abort();
        }
    }
    int argcount = argc-optind; // Remove consumed flags.

    if (!argcount) {
        workLength = defWorkLength;
        breakLength = defBreakLength;
    } else if (argcount < 3) {
        workLength = (int)strtol(argv[optind], NULL, 10);
        breakLength = (argcount == 2)
                    ? (int)strtol(argv[optind+1], NULL, 10)
                    : workLength/5;
        if (!(workLength >= 0 && breakLength >= 0)) {
            usage(exename);
        }
    } else {
        error("Only two arguments are allowed.\n", exename);
    }
    if (workLength < 1 ) {
        error("Work period must be longer than one minute.\n", exename);
    }
    if (breakLength < 1 ) {
        error("Break period must be longer than one minute.\n", exename);
    }
    swork = newClock(0,workLength,0);
    sbreak = newClock(0,breakLength,0);

    int periods = 1;
    int breaks = 1;
    bool onBreak = startOnBreak;
    bool reset = false;
    struct clock** current;

    clearScreen();
    printHeader(periods, breaks, startOnBreak);

    while(true){
        current = onBreak ? &sbreak : &swork;
        
        if(clockIsAllZeroes(*current)){
            if(!onBreak){
                if (notifyOnChange) {
                    execl(notifyCommand, notifyBreakMsg);
                }
                periods++;
            } else {
                if (notifyOnChange) {
                    execl(notifyCommand, notifyWorkMsg);
                }
                breaks++;
            }
            if (toggleMPDonChange) {
                execl(MPDCommand, MPDArgs);
            }
            reset = true;
            onBreak = !onBreak;
        }

        char* clockstring = toString(*current);
        printf("\r%s", clockstring);
        free(clockstring);
        fflush(stdout);
        
        sleep(1);
        if(reset){
            reset = !reset;
            freeClock(*current);
            *current = resetClock(onBreak, breakLength, workLength);
            clearScreen();
            printHeader(periods, breaks, onBreak);
        }

        struct clock* decrementedClock = decrementClock(*current);
        freeClock(*current);
        *current = decrementedClock;
    }
    freeClock(swork);
    freeClock(sbreak);
    return 0;
}
