#
# Makefile for RISC5 assembler test
#

BUILD = ../../../../build

all:		test.mem

install:	all

check:		test.mem
		diff test.mem test.ref

test.mem:	test.s
		$(BUILD)/bin/asm -o test.o test.s
		$(BUILD)/bin/link -s standalone.lnk -o test test.o
		$(BUILD)/bin/load -p test test.bin
		$(BUILD)/bin/bin2mem test.bin test.mem

clean:
		rm -f *~ test.o test test.bin test.mem
