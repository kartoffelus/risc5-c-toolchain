#
# Makefile for RISC5 assembler test
#

BUILD = ../../../../build

all:		test.mem

install:	all

check:		test.mem
		diff test.mem test.ref

test.mem:	test.s
		$(BUILD)/bin/asm -o test.mem test.s

clean:
		rm -f *~ test.mem
