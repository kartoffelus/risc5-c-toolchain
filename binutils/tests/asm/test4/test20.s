//
// test20.s -- check W32 relocations
//

	.CODE
	.SPACE	3*4

	.DATA
	.SPACE	5*4

	.BSS
	.SPACE	7*4

	.CODE

	// .code/.code/W32

l_c_1:
	.WORD	l_c_2 + 2
	.SPACE	0x10
	.WORD	l_c_1 + 4
	.SPACE	0x10
l_c_2:
