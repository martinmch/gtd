#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    printf("\r%s", toString(cl));
    fflush(stdout);
    if(!decrementClock(cl)){
        return;
    }
    sleep(1);
    countdown(cl);
}

void
printHeader(struct counter* cnt){
    int sessionMinutes = cnt->sessionClock->hour*60 + cnt->sessionClock->min;
    int breakMinutes   = cnt->breakClock->hour*60 + cnt->breakClock->min;
    printf("%d minute sessions with %d minute breaks.\n"
            , sessionMinutes
            , breakMinutes);
    printf("Period #%d. Total working time: "
            , cnt->periods);
    printClock(timeWorked(cnt->sessionClock, cnt->periods));
}


int main(int argc, char *argv[])
{
    struct clock* session = newClock(0,25,0);
    struct clock* sbreak = newClock(0,5,0);
    struct counter* cnt = newCounter(session, sbreak);
    printHeader(cnt);
    countdown(session);
    return 0;
}
