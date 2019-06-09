#ifndef CLOCKH
#define CLOCKH
struct clock {
    int hour;
    int min;
    int sec;
    int totalmin;
};

struct clock*
newClock(int hour, int min, int sec);

void
freeClock(struct clock* cp);

char*
toString(struct clock* cl);

struct clock*
decrementClock(struct clock* cp);

struct clock*
timeWorked(struct clock* sessionClock, int periods);

bool
clockIsAllZeroes(struct clock* cl);
#endif
