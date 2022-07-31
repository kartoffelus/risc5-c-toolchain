#
# Makefile for asm/link cyclic reference tests
#

BUILD = ../../../build

all:
	$(BUILD)/bin/asm -o src1.o src1.s
	$(BUILD)/bin/asm -o src2.o src2.s
	$(BUILD)/bin/asm -o src3.o src3.s
	$(BUILD)/bin/link -s standalone.lnk -o cycle src1.o src2.o src3.o
	$(BUILD)/bin/dump -a cycle >cycle.dump

clean:
	rm -f *~ src1.o src2.o src3.o cycle cycle.dump
