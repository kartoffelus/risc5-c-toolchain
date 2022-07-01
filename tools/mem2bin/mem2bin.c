/*
 * mem2bin.c -- convert prom memory format to plain binary
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


#define LINE_SIZE	150


void error(char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  printf("Error: ");
  vprintf(fmt, ap);
  printf("\n");
  va_end(ap);
  exit(1);
}


int main(int argc, char *argv[]) {
  FILE *in, *out;
  int lineno;
  char line[LINE_SIZE];
  char *p;
  unsigned int w;
  char *endp;
  unsigned char b;

  if (argc != 3) {
    printf("Usage: %s <input file> <output file>\n", argv[0]);
    return 1;
  }
  in = fopen(argv[1], "r");
  if (in == NULL) {
    error("cannot open input file '%s'", argv[1]);
  }
  out = fopen(argv[2], "w");
  if (out == NULL) {
    error("cannot open output file '%s'", argv[2]);
  }
  lineno = 0;
  while (fgets(line, LINE_SIZE, in) != NULL) {
    lineno++;
    p = line;
    while (*p == ' ' || *p == '\t') {
      p++;
    }
    if (*p == '\n') {
      continue;
    }
    if (*(p + 0) == '/' &&
        *(p + 1) == '/') {
      continue;
    }
    w = strtoul(p, &endp, 16);
    b = (w >> 0) & 0xFF;
    if (fwrite(&b, 1, 1, out) != 1) {
      error("cannot write output file");
    }
    b = (w >> 8) & 0xFF;
    if (fwrite(&b, 1, 1, out) != 1) {
      error("cannot write output file");
    }
    b = (w >> 16) & 0xFF;
    if (fwrite(&b, 1, 1, out) != 1) {
      error("cannot write output file");
    }
    b = (w >> 24) & 0xFF;
    if (fwrite(&b, 1, 1, out) != 1) {
      error("cannot write output file");
    }
    p = endp;
    while (*p == ' ' || *p == '\t') {
      p++;
    }
    if (*p == '\n') {
      continue;
    }
    if (*(p + 0) == '/' &&
        *(p + 1) == '/') {
      continue;
    }
    error("garbage at end of line %d", lineno);
  }
  fclose(in);
  fclose(out);
  return 0;
}
