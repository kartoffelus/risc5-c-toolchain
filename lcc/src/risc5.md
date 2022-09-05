%{

/*
 * risc5.md -- RISC5 back-end specification
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
