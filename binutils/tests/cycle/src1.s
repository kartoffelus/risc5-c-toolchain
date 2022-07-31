	.CODE
	.GLOBAL	fptr
	.GLOBAL	call
	.WORD	0x11111111
call:	.WORD	fptr
