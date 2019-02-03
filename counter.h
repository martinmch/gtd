struct counter {
    struct clock* sessionClock;
    struct clock* breakClock;
    int periods;
    bool onBreak;
};

struct counter*
newCounter(struct clock* sp, struct clock* bp, bool startonbreak);

void
freeCounter(struct counter* cnt);

void
printHeader(struct counter* cnt, char* customCommand);
