#
# Makefile for a standalone program
#

BUILD = ../../build

SRC = start.s main.c iolib.c promlib.c
EXE = ldr
MAP = ldr.map
BIN = ldr.bin

all:		$(BIN)

install:	$(BIN)
		mkdir -p $(BUILD)/disk
		cp $(BIN) $(BUILD)/disk
		cp $(MAP) $(BUILD)/disk

$(BIN):		$(EXE)
		$(BUILD)/bin/load -p $(EXE) $(BIN)

$(EXE):		$(SRC)
		$(BUILD)/bin/lcc -A \
		  -Wo-nostdinc -Wo-nostdlib \
		  -Wo-ldscript=loader.lnk \
		  -Wo-ldmap=$(MAP) -o $(EXE) $(SRC)

clean:
		rm -f *~ $(EXE) $(MAP) $(BIN) $(MEM)
		rm -f serial.dev
