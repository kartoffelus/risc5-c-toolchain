#
# Makefile to copy Makefiles to the run directories
#

BUILD=../build

all:

install:
		mkdir -p $(BUILD)/run-sim
		cp Makefile.sim $(BUILD)/run-sim/Makefile
		mkdir -p $(BUILD)/run-fpga
		cp Makefile.fpga $(BUILD)/run-fpga/Makefile
		cp serial.dev $(BUILD)/run-fpga

clean:
		rm -f *~
