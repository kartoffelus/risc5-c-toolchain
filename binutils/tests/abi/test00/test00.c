/*
 * test00.c -- direction of stack growth
 *             exact location pointed to by the stack pointer
 *
 * insight:
 *   - The stack grows downward, toward smaller adresses.
 *   - SP points to the last occupied stack location.
 */


int f(void) {
  int a[256];

  a[0] = 123;
  a[255] = 456;
  return a[0] + a[255];
}
