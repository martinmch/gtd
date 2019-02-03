#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct counter {
    struct clock* sessionClock;
    struct clock* breakClock;
    int periods;
};

struct clock {
    int hour;
    int min;
    int sec;
};

struct counter*
newCounter(struct clock* sp, struct clock* bp){
    struct counter* cntp = malloc(sizeof(struct counter));
    cntp->sessionClock = sp;
    cntp->breakClock = bp;
    cntp->periods = 1;
    return cntp;
}

struct clock*
newClock(int hour, int min, int sec){
    struct clock* cp;
    cp = malloc(sizeof(struct clock));
    cp->hour = hour;
    cp->min = min;
    cp->sec = sec;
    return cp;
}

void
printClock(struct clock* cl){
    printf("%02d:%02d:%02d\n"
           , cl->hour
           , cl->min
           , cl->sec);
}

int
decrementClock(struct clock* cp){
    cp->sec--;
    if (cp->sec < 0) {
        cp->sec = 59;
        cp->min--;
        if (cp->min < 0) {
            cp->min = 59;
            cp->hour--;
            if (cp->hour < 0) {
                return 0;
            }
        }
    }
    return 1;
}


void
countdown(struct clock* cl){
    printClock(cl);
    if(!decrementClock(cl)){
        return;
    }
    sleep(1);
    countdown(cl);
}

struct clock* timeWorked(struct clock* sessionClock, int periods){
    int hoursWorked   = sessionClock->hour * (periods-1);
    int minutesWorked = sessionClock->min  * (periods-1);
    int secondsWorked = sessionClock->sec  * (periods-1);
    return newClock(hoursWorked, minutesWorked, secondsWorked);
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
