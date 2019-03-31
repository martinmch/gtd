#ifndef COUNTERH
#define COUNTERH

#include "IO.h"

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

IO
printHeader(struct counter* cnt, char* customCommand);
#endif
