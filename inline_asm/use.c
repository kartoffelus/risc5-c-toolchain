/*
 * use.c -- use inline assembler
 */


#include "asm.h"


volatile int global = 42;


void f(int x) {
  int r;

  _asm_CLI();
  r = x + global;
  global = 2 * r;
  _asm_STI();
}


void g(void) {
  _asm_INC_GLOBAL();
}


int main(int argc, char *argv[]) {
  f(5);
  g();
  return 0;
}
