#
# Makefile for ABI test
#

BUILD = ../../../../build

SRC = test00.c
ASM = $(patsubst %.c,%.s,$(SRC))

all:		$(ASM)

install:	$(ASM)

$(ASM):		$(SRC)
		$(BUILD)/bin/lcc -S $(SRC)

clean:
		rm -f *~ $(ASM)
