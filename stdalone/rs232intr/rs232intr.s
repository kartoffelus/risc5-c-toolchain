//
// rs232intr.s -- serial receive, interrupt-driven
//

	.CODE

	// program starts here, at location 0
start:
	B	main

	// interrupts arrive here, at location 4
intr:
	MOV	R0,0xFFFFC8	// RS232 base address
	LDW	R1,R0,0		// get character
wait:
	LDW	R2,R0,4		// get status
	AND	R2,R2,2		// check xmt ready
	BEQ	wait		// wait if not ready
	STW	R1,R0,0		// send character
	RTI			// return from interrupt

main:
	MOV	R0,0xFFFFC8	// RS232 base address
	MOV	R1,1		// enable rcv interrupt
	STW	R1,R0,4
	GETS	R0,3		// get PSW
	IOR	R0,R0,1<<7	// enable serial rcv interrupt
	PUTS	R0,3		// put PSW
	STI			// enable interrupts
loop:
	B	loop		// and wait indefinitely
