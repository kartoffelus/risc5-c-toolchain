#
# Makefile for a standalone program
#

BUILD = ../../build

SRC = start.s test-00.c
EXE = test-00
MAP = test-00.map
BIN = test-00.bin
MEM = test-00.mem

all:		$(BIN) $(MEM)

install:	$(BIN) $(MEM)

boot:
		$(BUILD)/bin/sim -p $(BUILD)/prom/boot.mem -s 003

boot-connect:	$(BIN)
		$(BUILD)/bin/sercomm $(BIN)

run:		$(MEM)
		$(BUILD)/bin/sim -i -r $(MEM) -s 003

run-connect:
		$(BUILD)/bin/sercomm

$(BIN):		$(EXE)
		$(BUILD)/bin/load -p $(EXE) $(BIN)

$(MEM):		$(BIN)
		$(BUILD)/bin/bin2mem $(BIN) $(MEM)

$(EXE):		$(SRC)
		$(BUILD)/bin/lcc -A \
		  -Wo-nostdinc -Wo-nostdlib \
		  -Wo-ldscript=standalone.lnk \
		  -Wo-ldmap=$(MAP) -o $(EXE) $(SRC)

clean:
		rm -f *~ $(EXE) $(MAP) $(BIN) $(MEM)
		rm -f serial.dev
