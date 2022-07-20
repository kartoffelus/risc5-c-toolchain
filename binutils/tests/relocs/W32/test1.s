	.GLOBAL	C_global
	.GLOBAL	C_extern
	.GLOBAL	D_global
	.GLOBAL	D_extern
	.GLOBAL	B_global
	.GLOBAL	B_extern

	.CODE

	ADD	R3,R2,R1
	ADD	R3,R2,R1
	.WORD	C_local+10
	.WORD	C_global+20
	.WORD	C_extern+30
	.WORD	D_local+40
	.WORD	D_global+50
	.WORD	D_extern+60
	.WORD	B_local+70
	.WORD	B_global+80
	.WORD	B_extern+90
	ADD	R3,R2,R1
	ADD	R3,R2,R1
C_local:
	ADD	R3,R2,R1
C_global:
	ADD	R3,R2,R1

	.DATA

	.WORD	0x55AA55AA
	.WORD	0x55AA55AA
	.WORD	C_local+10
	.WORD	C_global+20
	.WORD	C_extern+30
	.WORD	D_local+40
	.WORD	D_global+50
	.WORD	D_extern+60
	.WORD	B_local+70
	.WORD	B_global+80
	.WORD	B_extern+90
	.WORD	0x55AA55AA
	.WORD	0x55AA55AA
D_local:
	.WORD	0x55AA55AA
D_global:
	.WORD	0x55AA55AA

	.BSS

	.SPACE	0x100
B_local:
	.SPACE	0x100
B_global:
	.SPACE	0x100
