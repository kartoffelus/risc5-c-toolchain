/*
 *
 */


void f1(void) {
}


int f2(void) {
  return 42;
}


int f3(void) {
  int x, y;
 
  x = 1;
  y = 2;
  return x + y;
}

int f4(void) {
  int x, y;
 
  x = 1;
  f1();
  y = 2;
  return x + y;
}


int neg(int x) {
  return -x;
}


int f5(int x, int y) {
  return x + y;
}


int f6(int x, int y) {
  f1();
  return x + y;
}


int f7(int x, int y) {
  int a, b, c;

  a = x + y;
  b = x - y;
  c = f5(a + b, a - b);
  return c;
}


int main(void) {
  return 0;
}
