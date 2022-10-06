/*
 *
 */


void f01(void) {
}


int f02(void) {
  return 42;
}


int f03(void) {
  int x, y;
 
  x = 1;
  y = 2;
  return x + y;
}

int f04(void) {
  int x, y;
 
  x = 1;
  f01();
  y = 2;
  return x + y;
}


int f05(int x) {
  return -x;
}


int f06(int x, int y) {
  return x + y;
}


int f07(int x, int y) {
  return x - y;
}


int f08(int x, int y) {
  return x * y;
}


int f09(int x, int y) {
  return x / y;
}


int f10(int x, int y) {
  return x % y;
}


unsigned int f11(unsigned int x, unsigned int y) {
  return x + y;
}


unsigned int f12(unsigned int x, unsigned int y) {
  return x - y;
}


unsigned int f13(unsigned int x, unsigned int y) {
  return x * y;
}


unsigned int f14(unsigned int x, unsigned int y) {
  return x / y;
}


unsigned int f15(unsigned int x, unsigned int y) {
  return x % y;
}


int f16(int x, int y) {
  f01();
  return x + y;
}


int f17(int x, int y) {
  int a, b, c;

  a = x + y;
  b = x - y;
  c = f06(a + b, a - b);
  return c;
}


int main(void) {
  return 0;
}
