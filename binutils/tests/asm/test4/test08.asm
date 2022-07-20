//
// test08.asm -- check W32 relocations
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

	// .code/.data/W32

	// .code/.bss/W32

	// .code/symbol/W32

	.WORD	l_3 + 6

	.DATA

	// .data/.code/W32

	// .data/.data/W32

l_d_1:
	.WORD	l_d_2 + 2
	.SPACE	0x10
	.WORD	l_d_1 + 4
	.SPACE	0x10
l_d_2:

	// .data/.bss/W32

	// .data/symbol/W32

	.BSS

	//.GLOBAL	l_1, l_2
	//.GLOBAL	l_3
