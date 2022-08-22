#
# Makefile for a standalone program
#

BUILD = ../../build

all:		blink.mem

install:	blink.mem

run-from-ram:	blink.mem
		$(BUILD)/bin/sim -r blink.mem -s 003

run-from-ser:	$(BUILD)/prom/boot.mem
		$(BUILD)/bin/sim -p $(BUILD)/prom/boot.mem -s 003

send-to-ser:	blink.bin
		$(BUILD)/bin/sercomm blink.bin

blink.mem:	blink.bin
		$(BUILD)/bin/bin2mem blink.bin blink.mem

blink.bin:	blink
		$(BUILD)/bin/load blink blink.bin

blink:		blink.o
		$(BUILD)/bin/link -s standalone.lnk -o blink blink.o

blink.o:	blink.s
		$(BUILD)/bin/asm -o blink.o blink.s

clean:
		rm -f *~ blink.o blink blink.bin blink.mem
		rm -f serial.dev
