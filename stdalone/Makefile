#
# Makefile for building standalone programs
#

BUILD = ../build

DIRS = blinkintr blinkintr2 blinkpoll echo hello hello2 \
       lcdtest memtest promtest sregtest

.PHONY:		all install clean

all:
		for i in $(DIRS) ; do \
		  $(MAKE) -C $$i all ; \
		done

install:
		for i in $(DIRS) ; do \
		  $(MAKE) -C $$i install ; \
		done

clean:
		for i in $(DIRS) ; do \
		  $(MAKE) -C $$i clean ; \
		done
		rm -f *~
