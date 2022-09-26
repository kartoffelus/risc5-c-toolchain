#
# Makefile
#

BUILD = ../../../../build

all:		symbol.o
		@echo "choose one of the following targets:"
		@echo "    make check-mov"
		@echo "    make check-add"
		@echo "    make check-ldst"

symbol.o:	symbol.s
		$(BUILD)/bin/asm -o symbol.o symbol.s

check-mov:	test00.s test01.s test02.s test03.s test04.s
		$(BUILD)/bin/asm -o test00.o test00.s
		$(BUILD)/bin/link -s standalone.lnk -o test00 test00.o symbol.o
		$(BUILD)/bin/load -p test00 test00.bin
		$(BUILD)/bin/bin2mem test00.bin test00.mem
		$(BUILD)/bin/sim -i -r test00.mem
		#
		$(BUILD)/bin/asm -o test01.o test01.s
		$(BUILD)/bin/link -s standalone.lnk -o test01 test01.o symbol.o
		$(BUILD)/bin/load -p test01 test01.bin
		$(BUILD)/bin/bin2mem test01.bin test01.mem
		$(BUILD)/bin/sim -i -r test01.mem
		#
		$(BUILD)/bin/asm -o test02.o test02.s
		$(BUILD)/bin/link -s standalone.lnk -o test02 test02.o symbol.o
		$(BUILD)/bin/load -p test02 test02.bin
		$(BUILD)/bin/bin2mem test02.bin test02.mem
		$(BUILD)/bin/sim -i -r test02.mem
		#
		$(BUILD)/bin/asm -o test03.o test03.s
		$(BUILD)/bin/link -s standalone.lnk -o test03 test03.o symbol.o
		$(BUILD)/bin/load -p test03 test03.bin
		$(BUILD)/bin/bin2mem test03.bin test03.mem
		$(BUILD)/bin/sim -i -r test03.mem
		#
		$(BUILD)/bin/asm -o test04.o test04.s
		$(BUILD)/bin/link -s standalone.lnk -o test04 test04.o symbol.o
		$(BUILD)/bin/load -p test04 test04.bin
		$(BUILD)/bin/bin2mem test04.bin test04.mem
		$(BUILD)/bin/sim -i -r test04.mem

check-add:	test10.s test11.s test12.s test13.s test14.s
		$(BUILD)/bin/asm -o test10.o test10.s
		$(BUILD)/bin/link -s standalone.lnk -o test10 test10.o symbol.o
		$(BUILD)/bin/load -p test10 test10.bin
		$(BUILD)/bin/bin2mem test10.bin test10.mem
		$(BUILD)/bin/sim -i -r test10.mem
		#
		$(BUILD)/bin/asm -o test11.o test11.s
		$(BUILD)/bin/link -s standalone.lnk -o test11 test11.o symbol.o
		$(BUILD)/bin/load -p test11 test11.bin
		$(BUILD)/bin/bin2mem test11.bin test11.mem
		$(BUILD)/bin/sim -i -r test11.mem
		#
		$(BUILD)/bin/asm -o test12.o test12.s
		$(BUILD)/bin/link -s standalone.lnk -o test12 test12.o symbol.o
		$(BUILD)/bin/load -p test12 test12.bin
		$(BUILD)/bin/bin2mem test12.bin test12.mem
		$(BUILD)/bin/sim -i -r test12.mem
		#
		$(BUILD)/bin/asm -o test13.o test13.s
		$(BUILD)/bin/link -s standalone.lnk -o test13 test13.o symbol.o
		$(BUILD)/bin/load -p test13 test13.bin
		$(BUILD)/bin/bin2mem test13.bin test13.mem
		$(BUILD)/bin/sim -i -r test13.mem
		#
		$(BUILD)/bin/asm -o test14.o test14.s
		$(BUILD)/bin/link -s standalone.lnk -o test14 test14.o symbol.o
		$(BUILD)/bin/load -p test14 test14.bin
		$(BUILD)/bin/bin2mem test14.bin test14.mem
		$(BUILD)/bin/sim -i -r test14.mem

check-ldst:	test20.s test21.s test22.s test23.s test24.s test25.s
		$(BUILD)/bin/asm -o test20.o test20.s
		$(BUILD)/bin/link -s standalone.lnk -o test20 test20.o symbol.o
		$(BUILD)/bin/load -p test20 test20.bin
		$(BUILD)/bin/bin2mem test20.bin test20.mem
		$(BUILD)/bin/sim -i -r test20.mem
		#
		$(BUILD)/bin/asm -o test21.o test21.s
		$(BUILD)/bin/link -s standalone.lnk -o test21 test21.o symbol.o
		$(BUILD)/bin/load -p test21 test21.bin
		$(BUILD)/bin/bin2mem test21.bin test21.mem
		$(BUILD)/bin/sim -i -r test21.mem
		#
		$(BUILD)/bin/asm -o test22.o test22.s
		$(BUILD)/bin/link -s standalone.lnk -o test22 test22.o symbol.o
		$(BUILD)/bin/load -p test22 test22.bin
		$(BUILD)/bin/bin2mem test22.bin test22.mem
		$(BUILD)/bin/sim -i -r test22.mem
		#
		$(BUILD)/bin/asm -o test23.o test23.s
		$(BUILD)/bin/link -s standalone.lnk -o test23 test23.o symbol.o
		$(BUILD)/bin/load -p test23 test23.bin
		$(BUILD)/bin/bin2mem test23.bin test23.mem
		$(BUILD)/bin/sim -i -r test23.mem
		#
		$(BUILD)/bin/asm -o test24.o test24.s
		$(BUILD)/bin/link -s standalone.lnk -o test24 test24.o symbol.o
		$(BUILD)/bin/load -p test24 test24.bin
		$(BUILD)/bin/bin2mem test24.bin test24.mem
		$(BUILD)/bin/sim -i -r test24.mem
		#
		$(BUILD)/bin/asm -o test25.o test25.s
		$(BUILD)/bin/link -s standalone.lnk -o test25 test25.o symbol.o
		$(BUILD)/bin/load -p test25 test25.bin
		$(BUILD)/bin/bin2mem test25.bin test25.mem
		$(BUILD)/bin/sim -i -r test25.mem

clean:
		rm -f *~
		rm -f symbol.o serial.dev
		#
		rm -f test00.o test00 test00.bin test00.mem
		rm -f test01.o test01 test01.bin test01.mem
		rm -f test02.o test02 test02.bin test02.mem
		rm -f test03.o test03 test03.bin test03.mem
		rm -f test04.o test04 test04.bin test04.mem
		#
		rm -f test10.o test10 test10.bin test10.mem
		rm -f test11.o test11 test11.bin test11.mem
		rm -f test12.o test12 test12.bin test12.mem
		rm -f test13.o test13 test13.bin test13.mem
		rm -f test14.o test14 test14.bin test14.mem
		#
		rm -f test20.o test20 test20.bin test20.mem
		rm -f test21.o test21 test21.bin test21.mem
		rm -f test22.o test22 test22.bin test22.mem
		rm -f test23.o test23 test23.bin test23.mem
		rm -f test24.o test24 test24.bin test24.mem
		rm -f test25.o test25 test25.bin test25.mem
