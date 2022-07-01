#
# Makefile for plain binary to prom memory converter
#

BUILD = ../../build

SRC = bin2mem.c
EXE = bin2mem

all:		$(EXE)

install:	$(EXE)
		mkdir -p $(BUILD)/bin
		cp $(EXE) $(BUILD)/bin

$(EXE):		$(SRC)
		gcc -g -Wall -o $(EXE) $(SRC)

clean:
		rm -f *~ $(EXE)
