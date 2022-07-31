#
# Makefile for asm/link absolute symbol tests
#

BUILD = ../../../build

all:
	$(BUILD)/bin/asm -o abs1.o abs1.s
	$(BUILD)/bin/asm -o abs2.o abs2.s
	$(BUILD)/bin/link -s standalone.lnk -o abs12 abs1.o abs2.o
	$(BUILD)/bin/link -s standalone.lnk -o abs21 abs2.o abs1.o
	@echo -n "Comparing abs12 and abs21: "
	@if cmp abs12 abs21 ; \
	  then echo "Files are equal." ; \
	  else echo "Error found!" ; \
	fi

clean:
	rm -f *~ abs1.o abs2.o abs12 abs21
