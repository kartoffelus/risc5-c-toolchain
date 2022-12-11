//
// start.s -- startup code
//

	.GLOBAL	_bcode
	.GLOBAL	_ecode
	.GLOBAL	_bdata
	.GLOBAL	_edata
	.GLOBAL	_bbss
	.GLOBAL	_ebss
	.GLOBAL	main

	.CODE

start:
	MOV	R6,_bdata	// copy data segment
	MOV	R4,_edata
	SUB	R5,R4,R6
	ADD	R5,R5,_ecode
	B	cpytest
cpyloop:
	LDW	R7,R5,0
	STW	R7,R4,0
cpytest:
	SUB	R4,R4,4
	SUB	R5,R5,4
	SUB	R12,R4,R6
	BCC	cpyloop
	MOV	R4,_bbss	// clear bss
	MOV	R5,_ebss
	MOV	R0,0
	B	clrtest
clrloop:
	STW	R0,R4,0
	ADD	R4,R4,4
clrtest:
	SUB	R12,R4,R5
	BCS	clrloop
	C	main		// do some useful work
start1:
	B	start1		// loop
