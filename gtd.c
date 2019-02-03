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
};

struct counter*
newCounter(struct clock* sp, struct clock* bp){
    struct counter* cntp = malloc(sizeof(struct counter));
    cntp->sessionClock = sp;
    cntp->breakClock = bp;
    cntp->periods = 1;
    return cntp;
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
    // TODO: When session runs out
    // clear screen.
    // increment period counter
    // print break header.
    //
    exit(0);
    }
    countdown(decClock);
}

void
printHeader(struct counter* cnt){
    int sessionMinutes = cnt->sessionClock->hour*60 + cnt->sessionClock->min;
    int breakMinutes   = cnt->breakClock->hour*60 + cnt->breakClock->min;
    printf("%d minute sessions with %d minute breaks.\n"
            , sessionMinutes
            , breakMinutes);
    printf("Period #%d. Total working time: %s\n"
            , cnt->periods
            , toString(timeWorked(cnt->sessionClock, cnt->periods)));
}

void usage(char* progname){
    fprintf(stderr,"Usage: %s [session time] [break time]\n",progname);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc == 2 || argc > 3) {
        usage(argv[0]);
    }
    struct clock* session;
    struct clock* sbreak;
    if (argc == 3) {
        int sessionMin = (int)strtol(argv[1], NULL, 10);
        int sbreakMin  = (int)strtol(argv[2], NULL, 10);
        if (!(sessionMin > 0 && sbreakMin > 0)) {
            usage(argv[0]);
        }
        session = minToClock(sessionMin);
        sbreak = minToClock(sbreakMin);
    } else {
        session = newClock(0,25,0);
        sbreak = newClock(0,5,0);
    }

    struct counter* cnt = newCounter(session, sbreak);
    printHeader(cnt);
    countdown(session);
    freeClock(session);
    freeClock(sbreak);
    return 0;
}
