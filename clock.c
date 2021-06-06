#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdbool.h>
#include "clock.h"

int secToMins(int seconds){
    return seconds/60;
}
int minToSecs(int minutes){
    return minutes*60;
}
int secToHours(int seconds){
    return seconds/3600;
}
int minToHours(int minutes){
    return minutes/60; 
}
int hourToMin(int hours){
    return hours*60;
}

struct clock*
resetClock(bool breakClock, int breakLength, int workLength){
    return breakClock
        ? newClock(0, breakLength, 0)
        : newClock(0, workLength, 0);
}

struct clock* newClock(int hour, int min, int sec){
    struct clock* cp = (struct clock*)malloc(sizeof(struct clock));
    cp->hour = cp->min = cp->sec = cp->totalmin = 0;

    if(hour < 0 || min < 0 || sec < 0){
        printf("Clock created with negative time unit");
        exit(1);
    }

    int sm = secToMins(sec);
    min += sm;
    int mh = minToHours(min);

    cp->sec = sec-minToSecs(sm);
    cp->hour = mh + hour;
    cp->min += min - hourToMin(mh);

    cp->totalmin = (cp->hour*60) + cp->min;
    return cp;
}

struct clock*
minToClock(int min){
    int minutes = min % 60;
    int hours = (min - minutes) / 60;
    return newClock(hours, minutes, 0);
}

/* Returns a char pointer. Remember to free */
char*
toString(struct clock* clock){
    char* string = (char*)malloc(10*sizeof(char));
    sprintf(string
           , "%02d:%02d:%02d"
           , clock->hour
           , clock->min
           , clock->sec);
    return string;
}

struct clock*
decrementClock(struct clock* cp){
    int hour = (cp->hour);
    int min  = (cp->min);
    int sec  = (cp->sec);
    int decSec = sec-1;
    if (decSec < 0) {
        int rollSec = 59;
        int decMin = min-1;
        if (decMin < 0) {
            int rollMin = 59;
            int decHour = hour-1;
            if (decHour < 0) {
                return newClock(0, 0, 0);
            }
            return newClock(hour, rollMin, rollSec);
        }
        return newClock(hour, decMin, rollSec);
    }
    return newClock(hour, min, decSec);
}


bool clockIsAllZeroes(struct clock* cl){
    return (cl->hour == 0 && cl->min == 0 && cl->sec == 0);
}

void
freeClock(struct clock* cp){ free(cp); }
