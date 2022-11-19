//
// check.s -- low level access to special registers
//


//
// int checkH(Word psw, Word val);
//
	.CODE
	.ALIGN	4
	.GLOBAL	checkH
checkH:
	PUTS	R1,3		// psw -> PSW
	PUTS	R2,1		// val -> H
	GETS	R4,3		// PSW -> R4
	GETS	R5,1		// H -> R5
	SUB	R6,R1,R4
	BEQ	checkH1
	MOV	R0,2		// PSW clobbered
	B	checkHx
checkH1:
	SUB	R6,R2,R5
	BEQ	checkH2
	MOV	R0,1		// wrong H value
	B	checkHx
checkH2:
	MOV	R0,0		// ok
checkHx:
	B	R15


//
// void checkPSW(Word psw, Word *p, Word *q);
//
	.CODE
	.ALIGN	4
	.GLOBAL	checkPSW
checkPSW:
	PUTS	R1,3		// psw -> PSW
	GETS	R4,3		// PSW -> R4
	GETS	R5,3		// PSW -> R5
	STW	R4,R2,0
	STW	R5,R3,0
	B	R15
