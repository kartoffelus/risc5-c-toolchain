/*
 * main.c -- main program
 */


#define IO_BASE		0xFFFFC0


void putchar(char c) {
  unsigned int *data;
  unsigned int *ctrl;

  if (c == '\n') {
    putchar('\r');
  }
  data = (unsigned int *) (IO_BASE + 4*2);
  ctrl = (unsigned int *) (IO_BASE + 4*3);
  while ((*ctrl & 2) == 0) ;
  *data = c;
}


void puts(char *s) {
  char c;

  while ((c = *s++) != '\0') {
    putchar(c);
  }
}


int main(void) {
  puts("\nHello, world!\n\n");
  return 0;
}
