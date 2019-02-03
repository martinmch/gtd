#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
printHeader(struct counter* cnt, char* customCommand){
    // TODO: Make clear portable
    if(!system(customCommand)){
        // Error
    }
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
