//
// mbr.s -- master boot record (just a fake one)
//

	.SET	iobase,0xFFFFC0		// I/O base address
	.SET	termdata,iobase+4*2	// terminal data
	.SET	termctrl,iobase+4*3	// terminal ctrl
	.SET	tos,0x010000		// top of stack

	// get some addresses listed in the load map
	.GLOBAL	start
	.GLOBAL	main
	.GLOBAL	out
	.GLOBAL	hello

	// minimal execution environment
start:
	MOV	R14,tos			// setup stack
	C	main			// do useful work
start1:
	B	start1			// halt by looping

	// main program
main:
	SUB	R14,R14,8		// create stack frame
	STW	R15,R14,0		// save return address
	STW	R8,R14,4		// save register variable
	MOV	R8,hello		// pointer to string
loop:
	LDB	R1,R8,0			// get char
	BEQ	stop			// null - finished
	C	out			// output char
	ADD	R8,R8,1			// bump pointer
	B	loop			// next char
stop:
	LDW	R15,R14,0		// restore return address
	LDW	R8,R14,4		// restore register variable
	ADD	R14,R14,8		// release stack frame
	B	R15			// return

	// output a character to the terminal
out:
	MOV	R4,termctrl		// terminal control
out1:
	LDW	R5,R4,0			// get status
	AND	R5,R5,2			// xmtr ready?
	BEQ	out1			// no - wait
	MOV	R4,termdata		// terminal data
	STW	R1,R4,0			// send char
	B	R15			// return

	// a very famous little string...
hello:
	.BYTE	0x0D, 0x0A
	.BYTE	"Hello, world!"
	.BYTE	0x0D, 0x0A
	.BYTE	0x0D, 0x0A, 0

	.SPACE	0x182

	.BYTE	0x55,0xAA
