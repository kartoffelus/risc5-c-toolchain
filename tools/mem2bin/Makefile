#
# Makefile for prom memory to plain binary converter
#

BUILD = ../../build

SRC = mem2bin.c
EXE = mem2bin

all:		$(EXE)

install:	$(EXE)
		mkdir -p $(BUILD)/bin
		cp $(EXE) $(BUILD)/bin

$(EXE):		$(SRC)
		gcc -g -Wall -o $(EXE) $(SRC)

clean:
		rm -f *~ $(EXE)
