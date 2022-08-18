//
// blink.s -- blinking LED, interrupt-driven
//

	.CODE

start:
	B	main

	.WORD	0		// align ISR to interrupt vector

intr:
	MOV	R0,-64
	LDW	R0,R0,0
	MOV	R0,count
	LDW	R1,R0,0
	ADD	R1,R1,1
	STW	R1,R0,0
	SUB	R1,R1,500
	BNE	noblink
	MOV	R1,0
	STW	R1,R0,0
	MOV	R0,leds
	LDW	R1,R0,0
	XOR	R1,R1,1
	STW	R1,R0,0
	MOV	R0,-60
	STW	R1,R0,0
noblink:
	RTI

main:
	GETS	R0,3
	IOR	R0,R0,0x8000
	PUTS	R0,3
	STI
loop:
	B	loop

	.DATA

count:
	.WORD	0
leds:
	.WORD	0
