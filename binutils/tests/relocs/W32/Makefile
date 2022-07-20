#
# Makefile for asm/link relocation test
#

BUILD = ../../../../build

all:
	$(BUILD)/bin/asm -o test0.o test0.s
	$(BUILD)/bin/asm -o test1.o test1.s
	$(BUILD)/bin/asm -o test2.o test2.s
	$(BUILD)/bin/link -s standalone.lnk -o test -m test.map \
	  test0.o test1.o test2.o
	$(BUILD)/bin/dump -a test1.o >test1.dump
	$(BUILD)/bin/dump -a test >test.dump

clean:
	rm -f *~ test0.o test1.o test2.o test test.map
	rm -f test1.dump test.dump
