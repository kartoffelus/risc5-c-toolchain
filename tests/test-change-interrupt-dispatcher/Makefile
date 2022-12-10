#
# Makefile for a libc program
#

BUILD = ../../build

SRC = test-change-interrupt-dispatcher.c
EXE = test-change-interrupt-dispatcher
MAP = test-change-interrupt-dispatcher.map
BIN = test-change-interrupt-dispatcher.bin
MEM = test-change-interrupt-dispatcher.mem

all:		$(BIN) $(MEM)

install:	$(BIN) $(MEM)

boot:
		$(BUILD)/bin/sim -p $(BUILD)/prom/boot.mem -s 001

boot-connect:	$(BIN)
		$(BUILD)/bin/sercomm $(BIN)

run:		$(MEM)
		$(BUILD)/bin/sim -i -r $(MEM) -s 001

test:	$(MEM)
		$(BUILD)/bin/sim -r $(MEM) -s 001

run-connect:
		$(BUILD)/bin/sercomm

$(BIN):		$(EXE)
		$(BUILD)/bin/load -p $(EXE) $(BIN)

$(MEM):		$(BIN)
		$(BUILD)/bin/bin2mem $(BIN) $(MEM)

$(EXE):		$(SRC)
		$(BUILD)/bin/lcc -A -Wo-ldmap=$(MAP) -o $(EXE) $(SRC)

clean:
		rm -f *~ $(EXE) $(MAP) $(BIN) $(MEM)
		rm -f serial.dev
