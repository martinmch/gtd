struct clock {
    int hour;
    int min;
    int sec;
};

struct clock*
newClock(int hour, int min, int sec);

char*
toString(struct clock* cl);

int
decrementClock(struct clock* cp);

struct clock*
timeWorked(struct clock* sessionClock, int periods);
