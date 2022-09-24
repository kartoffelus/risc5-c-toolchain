/*
 * comparisons
 */


/* #define HOSTED */


#ifdef HOSTED
#include <stdio.h>
#endif


/*
 * EQ, signed, false
 */
int f01_0(void) {
  int i, j;

  i = 3;
  j = -4;
  if (i == j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * EQ, signed, true
 */
int f01_1(void) {
  int i, j;

  i = 3;
  j = 3;
  if (i == j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * EQ, unsigned, false
 */
int f02_0(void) {
  unsigned int i, j;

  i = 3;
  j = 4;
  if (i == j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * EQ, unsigned, true
 */
int f02_1(void) {
  unsigned int i, j;

  i = 3;
  j = 3;
  if (i == j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * NE, signed, false
 */
int f03_0(void) {
  int i, j;

  i = 3;
  j = 3;
  if (i != j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * NE, signed, true
 */
int f03_1(void) {
  int i, j;

  i = 3;
  j = -4;
  if (i != j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * NE, unsigned, false
 */
int f04_0(void) {
  unsigned int i, j;

  i = 3;
  j = 3;
  if (i != j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * NE, unsigned, true
 */
int f04_1(void) {
  unsigned int i, j;

  i = 3;
  j = 4;
  if (i != j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * LT, signed, false
 */
int f05_0(void) {
  int i, j;

  i = 3;
  j = -4;
  if (i < j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * LT, signed, true
 */
int f05_1(void) {
  int i, j;

  i = -4;
  j = 3;
  if (i < j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * LT, unsigned, false
 */
int f06_0(void) {
  unsigned int i, j;

  i = 4;
  j = 3;
  if (i < j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * LT, unsigned, true
 */
int f06_1(void) {
  unsigned int i, j;

  i = 3;
  j = 4;
  if (i < j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * LE, signed, false
 */
int f07_0(void) {
  int i, j;

  i = 3;
  j = -4;
  if (i <= j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * LE, signed, true
 */
int f07_1(void) {
  int i, j;

  i = -4;
  j = 3;
  if (i <= j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * LE, unsigned, false
 */
int f08_0(void) {
  unsigned int i, j;

  i = 4;
  j = 3;
  if (i <= j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * LE, unsigned, true
 */
int f08_1(void) {
  unsigned int i, j;

  i = 3;
  j = 4;
  if (i <= j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * GT, signed, false
 */
int f09_0(void) {
  int i, j;

  i = -4;
  j = 3;
  if (i > j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * GT, signed, true
 */
int f09_1(void) {
  int i, j;

  i = 3;
  j = -4;
  if (i > j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * GT, unsigned, false
 */
int f10_0(void) {
  unsigned int i, j;

  i = 3;
  j = 4;
  if (i > j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * GT, unsigned, true
 */
int f10_1(void) {
  unsigned int i, j;

  i = 4;
  j = 3;
  if (i > j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * GE, signed, false
 */
int f11_0(void) {
  int i, j;

  i = -4;
  j = 3;
  if (i >= j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * GE, signed, true
 */
int f11_1(void) {
  int i, j;

  i = 3;
  j = -4;
  if (i >= j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * GE, unsigned, false
 */
int f12_0(void) {
  unsigned int i, j;

  i = 3;
  j = 4;
  if (i >= j) {
    return 1;
  } else {
    return 0;
  }
}


/*
 * GE, unsigned, true
 */
int f12_1(void) {
  unsigned int i, j;

  i = 4;
  j = 3;
  if (i >= j) {
    return 1;
  } else {
    return 0;
  }
}


typedef int (*Fptr)(void);


Fptr functions[] = {
  /*  0,  1 */  f01_0, f01_1,
  /*  2,  3 */  f02_0, f02_1,
  /*  4,  5 */  f03_0, f03_1,
  /*  6,  7 */  f04_0, f04_1,
  /*  8,  9 */  f05_0, f05_1,
  /* 10, 11 */  f06_0, f06_1,
  /* 12, 13 */  f07_0, f07_1,
  /* 14, 15 */  f08_0, f08_1,
  /* 16, 17 */  f09_0, f09_1,
  /* 18, 19 */  f10_0, f10_1,
  /* 20, 21 */  f11_0, f11_1,
  /* 22, 23 */  f12_0, f12_1,
};


int main(void) {
  int i;

  for (i = 0; i < sizeof(functions)/sizeof(functions[0]); i++) {
    if ((*functions[i])() != (i & 1)) {
#ifdef HOSTED
      printf("%d\n", i);
#endif
      return i;
    }
  }
#ifdef HOSTED
  printf("0\n");
#endif
  return 0;
}
