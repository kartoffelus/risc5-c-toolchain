/*
 * bin2mem.c -- convert plain binary to prom memory format
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


#define WORDS_PER_KB	256


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
  char *endptr;
  int memSize;
  int totalWords;
  int numBytes;
  int c;
  unsigned char lineData[16];

  if (argc != 3 && argc != 4) {
    printf("Usage: %s <input file> <output file> [memory size in KB]\n",
           argv[0]);
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
  if (argc == 3) {
    /* choose memory size just big enough */
    memSize = 0;
  } else {
    memSize = strtol(argv[3], &endptr, 0);
    if (*endptr != '\0') {
      error("cannot read memory size");
    }
    if (memSize <= 0) {
      error("illegal memory size %d", memSize);
    }
    memSize *= WORDS_PER_KB;
  }
  totalWords = 0;
  while (1) {
    for (numBytes = 0; numBytes < 4; numBytes++) {
      c = fgetc(in);
      if (c == EOF) {
        break;
      }
      lineData[numBytes] = c;
    }
    if (numBytes == 0) {
      break;
    }
    for (; numBytes < 4; numBytes++) {
      lineData[numBytes] = 0;
    }
    fprintf(out, "%02X", lineData[3]);
    fprintf(out, "%02X", lineData[2]);
    fprintf(out, "%02X", lineData[1]);
    fprintf(out, "%02X", lineData[0]);
    fprintf(out, "\n");
    totalWords++;
    if (c == EOF) {
      break;
    }
  }
  if (memSize != 0) {
    if (totalWords > memSize) {
      error("binary size (%d words) exceeds memory size (%d words)",
            totalWords, memSize);
    }
    while (totalWords < memSize) {
      fprintf(out, "00000000\n");
      totalWords++;
    }
  }
  fclose(in);
  fclose(out);
  return 0;
}
