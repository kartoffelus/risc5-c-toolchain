#
# Makefile for a standalone program
#

BUILD = ../../build

SRC = echo.s
OBJ = echo.o
EXE = echo
MAP = echo.map
BIN = echo.bin
MEM = echo.mem

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
