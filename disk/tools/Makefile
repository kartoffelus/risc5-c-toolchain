#
# Makefile for disk tools
#

BUILD = ../../build

all:		mksfs lssfs cpsfs rmsfs exsfs

install:	mksfs lssfs cpsfs rmsfs exsfs
		mkdir -p $(BUILD)/bin
		cp mksfs $(BUILD)/bin
		cp lssfs $(BUILD)/bin
		cp cpsfs $(BUILD)/bin
		cp rmsfs $(BUILD)/bin
		cp exsfs $(BUILD)/bin

mksfs:		mksfs.c sfs.h
		gcc -Wall -o mksfs mksfs.c

lssfs:		lssfs.c sfs.h
		gcc -Wall -o lssfs lssfs.c

cpsfs:		cpsfs.c sfs.h
		gcc -Wall -o cpsfs cpsfs.c

rmsfs:		rmsfs.c sfs.h
		gcc -Wall -o rmsfs rmsfs.c

exsfs:		exsfs.c sfs.h
		gcc -Wall -o exsfs exsfs.c

clean:
		rm -f *~ mksfs lssfs cpsfs rmsfs exsfs
