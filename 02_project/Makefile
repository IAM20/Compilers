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
SRCS			:= $(filter-out src/cminus.tab.c, $(SRCS))

SRCS_FLEX = ./src/lex.yy.c
SRCS_BISON = ./src/cminus.tab.c

HEADERS		:= $(wildcard src/*.h)
HEADER_BISON = ./src/cminus.tab.h

OBJS := $(SRCS:.c=.o)\

OBJS_FLEX = ./src/lex.yy.o
OBJS_BISON = ./src/cminus.tab.o

BIN				= ./bin/

CMINUSL		= src/cminus.l
CMINUSY		= src/cminus.y

TARGET		= cminus_flex

$(TARGET): $(HEADER_BISON) $(OBJS) $(HEADER) $(OBJS_FLEX) $(OBJS_BISON)
	$(CC) $(CFLAGS) $(OBJS) $(OBJS_FLEX) $(OBJS_BISON) -o $(BIN)$(TARGET)

$(OBJS_FLEX): $(CMINUSL) $(HEADERS)
	flex $(CMINUSL)
	mv lex.yy.c src/
	$(CC) $(CFLAGS) -c $(SRCS_FLEX)
	mv lex.yy.o src/

$(HEADER_BISON) : $(CMINUSY)
	bison -d $(CMINUSY)
	mv cminus.tab.h src/

$(OBJS_BISON): $(CMINUSY) $(HEADERS)
	bison $(CMINUSY)
	mv cminus.tab.c src/
	$(CC) $(CFLAGS) -c $(SRCS_BISON)
	mv cminus.tab.o src/

clean:
	rm -f ./src/cminus.tab.h ./src/cminus.tab.c ./src/lex.yy.c ./bin/* ./src/*.o *.log

