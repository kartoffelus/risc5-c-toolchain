/*
 * floating-point operations
 */


double f(double x) {
  double y;

  y = 2.0 * x;
  return y + 1.0;
}


int testint;
double testdouble;


int main(void) {
  double z;
  int r;

  testint = 0x1234;
  testdouble = 1.0;
  testint += 2;
  testdouble += 2.2;
  z = f(3.1415);
  z = -z;
  z += testint;
  testint = z;
  if (z != testdouble) r = 1; else
  if (z == testdouble) r = 2; else
  if (z > testdouble) r = 3; else
  if (z >= testdouble) r = 4; else
  if (z < testdouble) r = 5; else
  if (z <= testdouble) r = 6;
  return r;
}
