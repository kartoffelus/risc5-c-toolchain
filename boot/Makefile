#
# Makefile
#

BUILD = ../build

SRC = start.s main.c
EXE = boot
MAP = boot.map
BIN = boot.bin
MEM = boot.mem

all:		$(MEM)

install:	$(MEM)
		mkdir -p $(BUILD)/prom
		cp boot.mem $(BUILD)/prom

run:		$(MEM)
		$(BUILD)/bin/sim -i -p $(MEM) -s 001

$(MEM):		$(BIN)
		$(BUILD)/bin/bin2mem $(BIN) $(MEM)

$(BIN):		$(EXE)
		$(BUILD)/bin/load -p $(EXE) $(BIN)

$(EXE):		$(SRC)
		$(BUILD)/bin/lcc -A \
		  -Wo-nostdinc -Wo-nostdlib \
		  -Wo-ldscript=prom.lnk \
		  -Wo-ldmap=$(MAP) -o $(EXE) $(SRC)

clean:
		rm -f *~ $(EXE) $(MAP) $(BIN) $(MEM)
		rm -f serial.dev
