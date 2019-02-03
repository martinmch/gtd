#include <stdio.h>
#include <stdlib.h>
#include "clock.h"

struct clock*
newClock(int hour, int min, int sec){
    struct clock* cp;
    cp = (struct clock*)malloc(sizeof(struct clock));
    cp->hour = hour;
    cp->min = min;
    cp->sec = sec;
    return cp;
}

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


struct clock*
timeWorked(struct clock* sessionClock, int periods){
    int hoursWorked   = sessionClock->hour * (periods-1);
    int minutesWorked = sessionClock->min  * (periods-1);
    int secondsWorked = sessionClock->sec  * (periods-1);
    return newClock(hoursWorked, minutesWorked, secondsWorked);
}
