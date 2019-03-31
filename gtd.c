#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "clock.h"
#include "counter.h"
#include "IO.h"

void usage(char* progname){
    fprintf(stderr,"USAGE:\n");
    fprintf(stderr,"    %s [ -bcmnst ] [ work length ] [ break  length ]\n",progname);
    /* TODO: Implement options.
    fprintf(stderr,"OPTIONS:\n");
    fprintf(stderr,"    -b : start on a break\n");
    fprintf(stderr,"    -c : custom command (defaults to \"clear\")\n");
    fprintf(stderr,"    -m : toggle MPD on change\n");
    fprintf(stderr,"    -n : notify on change\n");
    fprintf(stderr,"    -s : speak command\n");
    fprintf(stderr,"    -t : show time in tmux status bar\n");
    fprintf(stderr,"    -T : update time in /tmp file\n");
    fprintf(stderr,"    -e : specify time to end at (e.g. 2:45pm today)\n");
    */
    exit(EXIT_FAILURE);
}

void
countdown(struct clock* cl){
    char* clockString = toString(cl);
    printf("\r%s", clockString);
    fflush(stdout);
    free(clockString);
    sleep(1);
    struct clock* decClock = decrementClock(cl);
    if(clockIsAllZeroes(decClock)){
        return;
    }
    countdown(decClock);
}

int main(int argc, char *argv[]) {
    struct clock* session;
    struct clock* sbreak;
    bool startonbreak = false;
    char* customCommand = "clear";
    char* notifyCommand = "notify-send";
    char* notifyWork = "Get things done.";
    char* notifyBreak = "Take a break.";

    /* Parse commandline arguments */
    if (argc == 3) {
        int sessionMin = (int)strtol(argv[1], NULL, 10);
        int sbreakMin  = (int)strtol(argv[2], NULL, 10);
        if (!(sessionMin > 0 && sbreakMin > 0)) {
            usage(argv[0]);
        }
        session = minToClock(sessionMin);
        sbreak = minToClock(sbreakMin);
    } else {
        // Default work length
        session = newClock(0,25,0);
        // Default break length
        sbreak = newClock(0,5,0);
    }

    struct counter* cnt = newCounter(session, sbreak, startonbreak);

    while (true){
        printHeader(cnt, customCommand);
        if(cnt->onBreak){
            countdown(&(*cnt->breakClock));
        } else {
            countdown(&(*cnt->sessionClock));
            cnt->periods++;
        }
        cnt->onBreak = !cnt->onBreak;
        cnt->sessionClock = session;
        cnt->breakClock = sbreak;
    }

    freeCounter(instance);
    return 0;
}
