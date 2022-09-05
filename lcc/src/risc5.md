%{

/*
 * risc5.md -- RISC5 back-end specification
 *
 * tree grammar terminals produced by:
 *   ops c=1 s=2 i=4 l=4 h=4 f=4 d=4 x=4 p=4
 */


#include "c.h"


#define I(f)	risc5_##f

#define NODEPTR_TYPE	Node
#define OP_LABEL(p)	((p)->op)
#define LEFT_CHILD(p)	((p)->kids[0])
#define RIGHT_CHILD(p)	((p)->kids[1])
#define STATE_LABEL(p)	((p)->x.state)


static void I(segment)(int s);

%}

%term CNSTF4=4113
%term CNSTI1=1045 CNSTI2=2069 CNSTI4=4117
%term CNSTP4=4119
%term CNSTU1=1046 CNSTU2=2070 CNSTU4=4118

%term ARGB=41
%term ARGF4=4129
%term ARGI4=4133
%term ARGP4=4135
%term ARGU4=4134

%term ASGNB=57
%term ASGNF4=4145
%term ASGNI1=1077 ASGNI2=2101 ASGNI4=4149
%term ASGNP4=4151
%term ASGNU1=1078 ASGNU2=2102 ASGNU4=4150

%term INDIRB=73
%term INDIRF4=4161
%term INDIRI1=1093 INDIRI2=2117 INDIRI4=4165
%term INDIRP4=4167
%term INDIRU1=1094 INDIRU2=2118 INDIRU4=4166

%term CVFF4=4209
%term CVFI4=4213

%term CVIF4=4225
%term CVII1=1157 CVII2=2181 CVII4=4229
%term CVIU1=1158 CVIU2=2182 CVIU4=4230

%term CVPU4=4246

%term CVUI1=1205 CVUI2=2229 CVUI4=4277
%term CVUP4=4279
%term CVUU1=1206 CVUU2=2230 CVUU4=4278

%term NEGF4=4289
%term NEGI4=4293

%term CALLB=217
%term CALLF4=4305
%term CALLI4=4309
%term CALLP4=4311
%term CALLU4=4310
%term CALLV=216

%term RETF4=4337
%term RETI4=4341
%term RETP4=4343
%term RETU4=4342
%term RETV=248

%term ADDRGP4=4359

%term ADDRFP4=4375

%term ADDRLP4=4391

%term ADDF4=4401
%term ADDI4=4405
%term ADDP4=4407
%term ADDU4=4406

%term SUBF4=4417
%term SUBI4=4421
%term SUBP4=4423
%term SUBU4=4422

%term LSHI4=4437
%term LSHU4=4438

%term MODI4=4453
%term MODU4=4454

%term RSHI4=4469
%term RSHU4=4470

%term BANDI4=4485
%term BANDU4=4486

%term BCOMI4=4501
%term BCOMU4=4502

%term BORI4=4517
%term BORU4=4518

%term BXORI4=4533
%term BXORU4=4534

%term DIVF4=4545
%term DIVI4=4549
%term DIVU4=4550

%term MULF4=4561
%term MULI4=4565
%term MULU4=4566

%term EQF4=4577
%term EQI4=4581
%term EQU4=4582

%term GEF4=4593
%term GEI4=4597
%term GEU4=4598

%term GTF4=4609
%term GTI4=4613
%term GTU4=4614

%term LEF4=4625
%term LEI4=4629
%term LEU4=4630

%term LTF4=4641
%term LTI4=4645
%term LTU4=4646

%term NEF4=4657
%term NEI4=4661
%term NEU4=4662

%term JUMPV=584

%term LABELV=600


%%


%%


/*
 * void address(Symbol q, Symbol p, long n) -- initialize q for addressing
 *                                             expression p+n
 */
static void I(address)(Symbol q, Symbol p, long n) {
  if (p->scope == GLOBAL ||
      p->sclass == STATIC ||
      p->sclass == EXTERN) {
    q->x.name = stringf("%s%s%D", p->x.name, n >= 0 ? "+" : "", n);
  } else {
    assert(n >= INT_MIN && n <= INT_MAX);
    q->x.offset = p->x.offset + n;
    q->x.name = stringd(q->x.offset);
  }
}


/*
 * void defaddress(Symbol p) -- initialize an address
 */
static void I(defaddress)(Symbol p) {
  print("\t.WORD\t%s\n", p->x.name);
}


/*
 * void defconst(int suffix, int size, Value v) -- define a constant
 */
static void I(defconst)(int suffix, int size, Value v) {
  float f;

  if (suffix == F) {
    assert(size == 4);
    f = v.d;
    print("\t.WORD\t0x%x\n", * (unsigned *) &f);
    return;
  }
  if (suffix == P) {
    assert(size == 4);
    print("\t.WORD\t0x%X\n", (unsigned long) v.p);
    return;
  }
  assert(suffix == I || suffix == U);
  if (size == 1) {
    print("\t.BYTE\t0x%x\n",
          (unsigned) ((unsigned char) (suffix == I ? v.i : v.u)));
    return;
  }
  if (size == 2) {
    print("\t.HALF\t0x%x\n",
          (unsigned) ((unsigned short) (suffix == I ? v.i : v.u)));
    return;
  }
  if (size == 4) {
    print("\t.WORD\t0x%x\n", (unsigned) (suffix == I ? v.i : v.u));
    return;
  }
  assert(0);
}


/*
 * void defstring(int len, char *s) -- emit a string constant
 */
static void I(defstring)(int len, char *s) {
  char *t;

  for (t = s; t < s + len; t++) {
    print("\t.BYTE\t0x%x\n", (*t) & 0xFF);
  }
}


/*
 * void defsymbol(Symbol p) -- define a symbol: initialize p->x
 */
static void I(defsymbol)(Symbol p) {
  if (p->scope >= LOCAL && p->sclass == STATIC) {
    p->x.name = stringf("L.%d", genlabel(1));
  } else
  if (p->generated) {
    p->x.name = stringf("L.%s", p->name);
  } else {
    assert(p->scope != CONSTANTS || isint(p->type) || isptr(p->type));
    p->x.name = p->name;
  }
}


static void I(emit)(Node p) {
  printf("// emit\n");
}


/*
 * void export(Symbol p) -- announce p as exported
 */
static void I(export)(Symbol p) {
  print("\t.GLOBAL\t%s\n", p->name);
}


static void I(function)(Symbol f, Symbol caller[],
                        Symbol callee[], int ncalls) {
  /* initialize */
  /* call gencode */
  gencode(caller, callee);
  /* emit prologue */
  I(segment)(CODE);
  print("\t.ALIGN\t4\n");
  print("%s:\n", f->x.name);
  /* call emitcode */
  emitcode();
  /* emit epilogue */
  print("\tB\tR15\n");
  print("\n");
}


static Node I(gen)(Node p) {
  printf("// gen\n");
  return NULL;
}


/*
 * void global(Symbol p) -- announce a global
 */
static void I(global)(Symbol p) {
  if (p->type->align != 0) {
    print("\t.ALIGN\t%d\n", p->type->align);
  } else {
    print("\t.ALIGN\t%d\n", 4);
  }
  print("%s:\n", p->x.name);
}


/*
 * void import(Symbol p) -- import a symbol
 */
static void I(import)(Symbol p) {
  print("\t.GLOBAL\t%s\n", p->name);
}


static void I(local)(Symbol p) {
  printf("// local\n");
}


static void setSwap(void) {
  union {
    char c;
    int i;
  } u;

  u.i = 0;
  u.c = 1;
  swap = ((u.i == 1) != IR->little_endian);
}


/*
 * void progbeg(int argc, char *argv[]) -- beginning of program
 */
static void I(progbeg)(int argc, char *argv[]) {
  setSwap();
  I(segment)(CODE);
  parseflags(argc, argv);
}


/*
 * void progend(void) -- end of program
 */
static void I(progend)(void) {
  /* nothing to do here */
}


/*
 * void segment(int s) -- switch to segment s
 */
static void I(segment)(int s) {
  static int currSeg = -1;
  int newSeg;

  switch (s) {
    case CODE:
      newSeg = CODE;
      break;
    case BSS:
      newSeg = BSS;
      break;
    case DATA:
      newSeg = DATA;
      break;
    case LIT:
      newSeg = DATA;
      break;
  }
  if (currSeg == newSeg) {
    return;
  }
  switch (newSeg) {
    case CODE:
      print("\t.CODE\n");
      break;
    case BSS:
      print("\t.BSS\n");
      break;
    case DATA:
      print("\t.DATA\n");
      break;
  }
  currSeg = newSeg;
}


/*
 * void space(int n) -- initialize n bytes of space
 */
static void I(space)(int n) {
  print("\t.SPACE\t%d\n", n);
}


Interface risc5IR = {
  1, 1, 0,        /* char */
  2, 2, 0,        /* short */
  4, 4, 0,        /* int */
  4, 4, 0,        /* long */
  4, 4, 0,        /* long long */
  4, 4, 1,        /* float */
  4, 4, 1,        /* double */
  4, 4, 1,        /* long double */
  4, 4, 0,        /* T* */
  0, 1, 0,        /* struct */
  1,              /* little_endian */
  0,              /* mulops_calls */
  0,              /* wants_callb */
  0,              /* wants_argb */
  1,              /* left_to_right */
  0,              /* wants_dag */
  0,              /* unsigned_char */
  /* ------------ */
  I(address),     /* address */
  blockbeg,       /* blockbeg */
  blockend,       /* blockend */
  I(defaddress),  /* defaddress */
  I(defconst),    /* defconst */
  I(defstring),   /* defstring */
  I(defsymbol),   /* defsymbol */
  I(emit),        /* emit */
  I(export),      /* export */
  I(function),    /* function */
  I(gen),         /* gen */
  I(global),      /* global */
  I(import),      /* import */
  I(local),       /* local */
  I(progbeg),     /* progbeg */
  I(progend),     /* progend */
  I(segment),     /* segment */
  I(space),       /* space */
  0,              /* stabblock */
  0,              /* stabend */
  0,              /* stabfend */
  0,              /* stabinit */
  0,              /* stabline */
  0,              /* stabsym */
  0,              /* stabtype */
};
