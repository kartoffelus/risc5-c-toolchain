/*
 * test01.c -- behavior of caller-save vs. callee-save registers
 *
 * insight:
 *   - Callee-save registers are saved by the prolog of the callee,
 *     but only if they are in fact used.
 *   - Caller-save registers are never saved by the callee.
 */


int g1(void);
int g2(void);
int g3(void);
int g4(void);


int f1(void) {
  int n1, n2, n3, n4;

  n1 = 1;
  n2 = 2;
  n3 = 3;
  n4 = 4;
  n1 += n2;
  n3 += n4;
  n2 = n1 + n3;
  return n2;
}


int f2(void) {
  return (g1() + g2()) + (g3() + g4());
}
