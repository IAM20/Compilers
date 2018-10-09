MAKEFILE	= Makefile

CC				= gcc

UNAME_S   := $(shell uname -s)

CFLAGS		:= 
ifeq ($(UNAME_S),Darwin)
	CFLAGS += -ll
else
	CFLAGS += -lfl
endif

SRCS			:= $(wildcard src/*.c)
SRCS			:= $(filter-out src/lex.yy.c, $(SRCS))

SRCS_FLEX = ./src/lex.yy.c

HEADERS		:= $(wildcard src/*.h)

OBJS := $(SRCS:.c=.o)\

OBJS_FLEX = ./src/lex.yy.o

BIN				= ./bin/

CMINUSL		= src/cminus.l

TARGET		= cminus_flex

$(TARGET): $(OBJS) $(HEADER) $(OBJS_FLEX)
	$(CC) $(CFLAGS) $(OBJS) $(OBJS_FLEX) -o $(BIN)$(TARGET)

$(OBJS_FLEX): $(CMINUSL) $(HEADERS)
	flex $(CMINUSL)
	mv lex.yy.c src/
	$(CC) $(CFLAGS) -c $(SRCS_FLEX)
	mv lex.yy.o src/

clean:
	rm -f ./src/lex.yy.c ./bin/* ./src/*.o *.log

