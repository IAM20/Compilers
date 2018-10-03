MAKEFILE	= Makefile

CC				= gcc

CFLAGS		= -lfl\

SRCS			:= $(wildcard src/*.c)

SRCS_FLEX = ./src/lex.yy.c

HEADERS		:= $(wildcard src/*.h)

OBJS := $(SRCS:.c=.o)\

OBJS_FLEX = ./src/lex.yy.o

BIN				= ./bin/

CMINUSL		= src/cminus.l

TARGET		= cminus_flex

$(TARGET): $(OBJS) $(HEADER) $(OBJS_FLEX)
	$(CC) $(CFLAGS) $(OBJS) $(OBJS_FLEX) -o $(BIN)$(TARGET)


#
#main.o: main.c util.h util.c scan.h cminus.l globals.h
#	$(CC) $(CFLAGS) -c main.c

$(OBJS_FLEX): $(CMINUSL) $(HEADERS)
	flex $(CMINUSL)
	mv lex.yy.c src/
	$(CC) $(CFLAGS) -c $(SRCS_FLEX)
	mv lex.yy.o src/

#util.o: util.c globals.h util.h
#	$(CC) $(CFLAGS) -c util.c

clean:
	rm -f ./src/lex.yy.c ./bin/* ./src/*.o *.log

