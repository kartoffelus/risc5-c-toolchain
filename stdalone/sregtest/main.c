/*
 * main.c -- RISC5 special register test
 */


#include "types.h"
#include "stdarg.h"
#include "iolib.h"
#include "check.h"


/**************************************************************/


Word val[] = {
  0x00000000,
  0x000000A5,
  0x08000000,
  0x10000000,
  0x20000000,
  0x40000000,
  0x50000000,
  0x60000000,
  0x80000000,
  0xC0000000,
};


/**************************************************************/


void test1(void) {
  Word psw;
  int i;
  int err;
  char *res;

  psw = 0x00000000;
  printf("\ntesting GETS/PUTS with H register, PSW = 0x%08X\n", psw);
  for (i = 0; i < sizeof(val)/sizeof(val[0]); i++) {
    printf("%d: 0x%08X    ", i, val[i]);
    err = checkH(psw, val[i]);
    switch (err) {
      case 0:
        res = "ok";
        break;
      case 1:
        res = "wrong H value";
        break;
      case 2:
        res = "PSW clobbered";
        break;
    }
    printf("%s\n", res);
  }
  psw = 0xF8000000;
  printf("\ntesting GETS/PUTS with H register, PSW = 0x%08X\n", psw);
  for (i = 0; i < sizeof(val)/sizeof(val[0]); i++) {
    printf("%d: 0x%08X    ", i, val[i]);
    err = checkH(psw, val[i]);
    switch (err) {
      case 0:
        res = "ok";
        break;
      case 1:
        res = "error: wrong H value";
        break;
      case 2:
        res = "error: PSW clobbered";
        break;
    }
    printf("%s\n", res);
  }
}


/**************************************************************/


Word correctedFlags(Word p) {
  if (p == 0x00000000) {
    p |= 0x40000000;
  } else {
    p &= ~(unsigned)0x40000000;
  }
  return p;
}


void test2(void) {
  int i;
  Word p, q;

  printf("\ntesting GETS/PUTS with PSW register\n");
  for (i = 0; i < sizeof(val)/sizeof(val[0]); i++) {
    printf("%d: 0x%08X    ", i, val[i]);
    checkPSW(val[i], &p, &q);
    if (p == val[i] && q == correctedFlags(val[i])) {
      printf("ok\n");
    } else {
      if (p != val[i]) {
        printf("error: PSW read = 0x%08X\n", p);
      } else {
        printf("error: second read got 0x%08X, expected 0x%08X\n",
               q, correctedFlags(val[i]));
      }
    }
  }
}


/**************************************************************/


int main(void) {
  printf("\nRISC5 special register test started\n");
  test1();
  test2();
  printf("\nRISC5 special register test finished\n");
  return 0;
}
