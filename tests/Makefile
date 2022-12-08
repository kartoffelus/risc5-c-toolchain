#
# Makefile for building test programs
#

BUILD = ../build

DIRS = test-00 test-01 test-02 test-03 test-04 test-05 test-06 test-libc-memcpy-memset

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
