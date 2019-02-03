#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "clock.h"

struct counter {
    struct clock* sessionClock;
    struct clock* breakClock;
    int periods;
    bool onBreak;
};

struct counter*
newCounter(struct clock* sp, struct clock* bp, bool startonbreak){
    struct counter* cntp = malloc(sizeof(struct counter));
    cntp->sessionClock = sp;
    cntp->breakClock = bp;
    cntp->periods = 1;
    cntp->onBreak = startonbreak;
    return cntp;
}

void
freeCounter(struct counter* cnt){
    free(cnt);
}

void
printHeader(struct counter* cnt){
    // TODO: Make clear portable
    system("clear");
    struct clock* workedClock = timeWorked(cnt->sessionClock, cnt->periods);
    printf("%d minute sessions with %d minute breaks.\n"
            , cnt->sessionClock->totalmin
            , cnt->breakClock->totalmin);
    // TODO: Remove code duplication
    if(cnt->onBreak){
        printf("Break. Total working time: %s\n"
                , toString(workedClock));
    } else {
        printf("Period #%d. Total working time: %s\n"
                , cnt->periods
                , toString(workedClock));
    }
    freeClock(workedClock);
}

void usage(char* progname){
    fprintf(stderr,"USAGE:\n");
    fprintf(stderr,"    %s [ -bcmnst ] [ work length] [ break  length]\n",progname);
    fprintf(stderr,"OPTIONS:\n");
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


int main(int argc, char *argv[])
{
    if (argc == 2 || argc > 3) {
        usage(argv[0]);
    }

    struct clock* session;
    struct clock* sbreak;

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
        session = newClock(0,1,0);
        sbreak = newClock(0,1,0);
    }
    bool startonbreak = false;
    /* End parse */

    struct counter* cnt = newCounter(session, sbreak, startonbreak);

    while (true){
        printHeader(cnt);
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

    freeClock(session);
    freeClock(sbreak);
    freeCounter(cnt);
    return 0;
}
