//
// blinkpoll.s -- blinking LED, polled
//

	.CODE

	// program starts here, at location 0
start:
	MOV	R0,-64		// read timer
	LDW	R1,R0,0
	MOV	R2,last		// get last reading
	LDW	R3,R2,0
	SUB	R4,R1,R3	// compare
	BEQ	start		// equal - wait
	STW	R1,R2,0		// else store new value as last
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
	B	start		// and start all over again

	.DATA

last:
	.WORD	0		// last timer value read
count:
	.WORD	0		// our counter
leds:
	.WORD	0		// LED state
