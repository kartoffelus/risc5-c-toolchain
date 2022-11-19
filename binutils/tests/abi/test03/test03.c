/*
 * test03.c -- argument register save/restore
 *
 * insight:
 *   - The argument value registers R1..R3 are treated like caller-save
 *     registers, i.e., they can be overwritten without saving.
 */


void g(int n1, int n2, int n3);


void f(void) {
  g(1, 2, 3);
}


void g(int n1, int n2, int n3) {
}
