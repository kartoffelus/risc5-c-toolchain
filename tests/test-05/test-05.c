/*
 * structure copy
 */


typedef struct {
  char c;
  int i;
  double d;
  unsigned char b;
} Small;


typedef struct {
  int a[9997];
} Big;


void copySmall(Small *sp1, Small *sp2) {
  *sp1 = *sp2;
}


void copyBig(Big *bp1, Big *bp2) {
  *bp1 = *bp2;
}


int main(void) {
  Small s1, s2;
  Big b1, b2;

  s1 = s2;
  b1 = b2;
  return 0;
}
