struct clock {
    int hour;
    int min;
    int sec;
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

struct clock*
minToClock(int min);

bool
clockIsAllZeroes(struct clock* cl);
