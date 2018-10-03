#
# Makefile for TINY
# Gnu C Version
# K. Louden 2/3/98
#
MAKEFILE = Makefile

CC = gcc

CFLAGS = 

SRCS := $(wildcard src/*.c)
SRCS := $(filter-out src/tm.c, $(SRCS))

HEADERS := $(wildcard src/*.h)

OBJS := $(SRCS:.c=.o)

BIN =./bin/

tiny: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o ${BIN}tiny

clean:
	-rm -f ./bin/tiny
	-rm -f ./bin/tm
	-rm -f ./src/*.o
	-rm -f *.log

tm: tm.c
	$(CC) $(CFLAGS) tm.c -o ./bin/tm

all: tiny tm

