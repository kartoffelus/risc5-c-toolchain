#
# Makefile for RISC5 linker
#

BUILD = ../../build

CC = gcc
CFLAGS = -g -Wall -Iscript
LDFLAGS = -g -L$(LIBDIR)
LDLIBS = -lreadscript -lm

SRCS = link.c
OBJS = $(patsubst %.c,%.o,$(SRCS))
BIN = link

LIBDIR = script
LIB = libreadscript.a

all:		$(LIBDIR)/$(LIB) $(BIN)

$(LIBDIR)/$(LIB):
		$(MAKE) -C $(LIBDIR) all

install:	all
		mkdir -p $(BUILD)/bin
		cp $(BIN) $(BUILD)/bin

$(BIN):		$(OBJS)
		$(CC) $(LDFLAGS) -o $(BIN) $(OBJS) $(LDLIBS)

%.o:		%.c
		$(CC) $(CFLAGS) -o $@ -c $<

depend.mak:
		$(CC) -MM -MG $(CFLAGS) $(SRCS) >depend.mak

-include depend.mak

clean:
		$(MAKE) -C $(LIBDIR) clean
		rm -f *~ $(OBJS) $(BIN) depend.mak
