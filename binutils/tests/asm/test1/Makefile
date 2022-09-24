#
# Makefile for RISC5 assembler test
#

BUILD = ../../../../build

all:		patterns full.s

install:	all

check:		all
		$(BUILD)/bin/asm -o full.o full.s
		$(BUILD)/bin/link -s standalone.lnk -o full full.o
		$(BUILD)/bin/load -p full full.bin
		$(BUILD)/bin/bin2mem full.bin full.mem
		diff full.mem patterns

patterns:	gen1
		./gen1 >patterns

gen1:		gen1.c
		gcc -Wall -o gen1 gen1.c

full.s:		patterns gen2
		./gen2 patterns >full.s

gen2:		gen2.c
		gcc -Wall -o gen2 gen2.c

clean:
		rm -f *~ gen1 gen2
		rm -f patterns
		rm -f full.s full.o full full.bin full.mem
