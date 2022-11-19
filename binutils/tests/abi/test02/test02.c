/*
 * test02.c -- return value register save/restore
 *
 * insight:
 *   - The return value register R0 is treated like a caller-save
 *     register, i.e., it can be overwritten without saving.
 */


int g(void);


void f(void) {
  int n;

  n = g();
  n++;
}


int g(void) {
  return 42;
}
