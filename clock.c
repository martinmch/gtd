#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdbool.h>
#include "clock.h"

struct clock*
newClock(int hour, int min, int sec){
    struct clock* cp;
    cp = (struct clock*)malloc(sizeof(struct clock));
    cp->hour = hour;
    cp->min = min;
    cp->sec = sec;
    cp->totalmin = hour*60 + min;
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
toString(struct clock* cl){
    char* string = (char*)malloc(9*sizeof(char));
    sprintf(string
            , "%02d:%02d:%02d"
            , cl->hour
            , cl->min
            , cl->sec);
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


bool
clockIsAllZeroes(struct clock* cl){
    if(cl->hour == 0 && cl->min == 0 && cl->sec == 0){
        return true;
    }
    return false;
}

struct clock*
timeWorked(struct clock* sessionClock, int periods){
    int hoursWorked   = sessionClock->hour * (periods-1);
    int minutesWorked = sessionClock->min  * (periods-1);
    int secondsWorked = sessionClock->sec  * (periods-1);
    return newClock(hoursWorked, minutesWorked, secondsWorked);
}

void
freeClock(struct clock* cp){
    free(cp);
}
