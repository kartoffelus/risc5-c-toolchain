#
# Makefile for a standalone program
#

BUILD = ../../build

SRC = blinkintr.s
OBJ = blinkintr.o
EXE = blinkintr
MAP = blinkintr.map
BIN = blinkintr.bin
MEM = blinkintr.mem

all:		$(BIN) $(MEM)

install:	$(BIN) $(MEM)

boot:
		$(BUILD)/bin/sim -p $(BUILD)/prom/boot.mem -s 001

boot-connect:	$(BIN)
		$(BUILD)/bin/sercomm $(BIN)

run:		$(MEM)
		$(BUILD)/bin/sim -i -r $(MEM) -s 001

run-connect:
		$(BUILD)/bin/sercomm

$(MEM):		$(BIN)
		$(BUILD)/bin/bin2mem $(BIN) $(MEM)

$(BIN):		$(EXE)
		$(BUILD)/bin/load -p $(EXE) $(BIN)

$(EXE):		$(OBJ)
		$(BUILD)/bin/link -s standalone.lnk \
		  -m $(MAP) -o $(EXE) $(OBJ)

$(OBJ):		$(SRC)
		$(BUILD)/bin/asm -o $(OBJ) $(SRC)

clean:
		rm -f *~ $(OBJ) $(EXE) $(MAP) $(BIN) $(MEM)
		rm -f serial.dev
