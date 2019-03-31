BIN = gtd
SRCS = gtd.c clock.c counter.c

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

dep.dot: $(SRCS)
	c2dot 2>/dev/null > dep.dot
	head -n -1 dep.dot > temp.txt ; mv temp.txt dep.dot
	echo -n "\t" >> dep.dot
	echo -n *.h | sed 's@\([a-zA-Z]*.h\)@\"\1\";@g; s@^@\{ rank=same; @;' >> dep.dot
	echo " }"    >> dep.dot
	echo -n "\t" >> dep.dot
	echo -n *.c | sed 's@\([a-zA-Z]*.c\)@\"\1\";@g; s@^@\{ rank=same; @;' >> dep.dot
	echo " }" >> dep.dot
	echo "}" >> dep.dot

dep.pdf: dep.dot
	dot -Tpdf dep.dot > dep.pdf

-include *.d

.PHONY: clean

clean:
	rm -f $(BIN) $(OBJS) $(DEPS) tags
