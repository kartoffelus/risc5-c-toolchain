#
# Makefile for RISC5 project
#

VERSION = 0.0

BUILD = `pwd`/build

DIRS = sim sercomm tools binutils boot stdalone fpga doc

all:		compiler
		for i in $(DIRS) ; do \
		  $(MAKE) -C $$i install ; \
		done

compiler:	builddir
		$(MAKE) -C lcc BUILDDIR=$(BUILD)/bin \
		  HOSTFILE=etc/risc5-linux.c lcc
		$(MAKE) -C lcc BUILDDIR=$(BUILD)/bin all
		rm -f $(BUILD)/bin/*.c
		rm -f $(BUILD)/bin/*.o
		rm -f $(BUILD)/bin/*.a

builddir:
		mkdir -p $(BUILD)
		mkdir -p $(BUILD)/bin

clean:
		rm -f lcc/lburg/gram.c
		for i in $(DIRS) ; do \
		  $(MAKE) -C $$i clean ; \
		done
		rm -rf $(BUILD)
		rm -f *~

dist:		clean
		(cd .. ; \
		 tar --exclude-vcs -cvf \
		   risc5-c-toolchain-$(VERSION).tar \
		   risc5-c-toolchain/* ; \
		 gzip -f risc5-c-toolchain-$(VERSION).tar)
