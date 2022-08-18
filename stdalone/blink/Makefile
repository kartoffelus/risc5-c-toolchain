#
# Makefile for a standalone program
#

BUILD = ../../build

all:		blink.bin

install:	blink.bin

run-sim:
		$(BUILD)/bin/sim -p $(BUILD)/prom/boot.mem -s 003

run-blink:	blink.bin
		$(BUILD)/bin/sercomm blink.bin

blink.bin:	blink
		$(BUILD)/bin/load blink blink.bin

blink:		blink.o
		$(BUILD)/bin/link -s standalone.lnk -o blink blink.o

blink.o:	blink.s
		$(BUILD)/bin/asm -o blink.o blink.s

clean:
		rm -f *~ blink.o blink blink.bin
		rm -f serial.dev
