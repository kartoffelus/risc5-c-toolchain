#
# Makefile for binutils tests
#

BUILD = ../../build

#DIRS = abs alloc artest cycle errors relocs simple statlib
DIRS = abi abs cycle

all:
		for i in $(DIRS) ; do \
		  $(MAKE) -C $$i all ; \
		done

clean:
		for i in $(DIRS) ; do \
		  $(MAKE) -C $$i clean ; \
		done
		rm -f *~
