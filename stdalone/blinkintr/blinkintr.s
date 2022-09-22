//
// blinkintr.s -- blinking LED, interrupt-driven
//

	.CODE

	// program starts here, at location 0
start:
	B	main

	// interrupts arrive here, at location 4
intr:
	MOV	R0,-64		// reset timer interrupt
	LDW	R0,R0,0		// by reading its counter
	MOV	R0,count	// increment our counter
	LDW	R1,R0,0
	ADD	R1,R1,1
	STW	R1,R0,0
	SUB	R1,R1,500	// check if count of 500 reached
	BNE	noblink		// no - don't toggle LED state
	MOV	R1,0		// yes - reset our count
	STW	R1,R0,0
	MOV	R0,leds		// and toggle LED state (LSB)
	LDW	R1,R0,0
	XOR	R1,R1,1
	STW	R1,R0,0
	MOV	R0,-60
	STW	R1,R0,0
noblink:
	RTI			// return from interrupt

main:
	GETS	R0,3		// get PSW
	IOR	R0,R0,0x8000	// enable timer interrupt
	PUTS	R0,3		// put PSW
	STI			// enable interrupts
loop:
	B	loop		// and wait indefinitely

	.DATA

count:
	.WORD	0		// interrupt counter
leds:
	.WORD	0		// LED state
