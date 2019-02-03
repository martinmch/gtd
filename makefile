BIN = gtd
SRCS = gtd.c clock.c

OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
CC = gcc

CFLAGS = -std=c99 -Wshadow -Wall -pedantic -Wextra -g -O3 -flto -march=native

LDFLAGS = -lm


$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(BIN)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -MMD -MP -MT $@ -MF $*.td -c $<
	mv -f $*.td $*.d

tags: $(SRCS)
	ctags -R

-include *.d

.PHONY: clean

clean:
	rm -f $(BIN) $(OBJS) $(DEPS) tags
