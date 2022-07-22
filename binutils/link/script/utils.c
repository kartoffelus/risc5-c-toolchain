/*
 * utils.c -- utility functions
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "utils.h"


/**************************************************************/


void error(char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  fprintf(stderr, "Error: ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
  exit(1);
}


void warning(char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  fprintf(stderr, "Warning: ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}


/**************************************************************/


void *memAlloc(unsigned int size) {
  void *p;

  p = malloc(size);
  if (p == NULL) {
    error("out of memory");
  }
  return p;
}


void memFree(void *p) {
  if (p == NULL) {
    error("memFree() got NULL pointer");
  }
  free(p);
}


/**************************************************************/


unsigned int read4FromTarget(unsigned char *p) {
  return (unsigned int) p[0] <<  0 |
         (unsigned int) p[1] <<  8 |
         (unsigned int) p[2] << 16 |
         (unsigned int) p[3] << 24;
}


void write4ToTarget(unsigned char *p, unsigned int data) {
  p[0] = data >>  0;
  p[1] = data >>  8;
  p[2] = data >> 16;
  p[3] = data >> 24;
}


void conv4FromTargetToNative(unsigned char *p) {
  unsigned int data;

  data = read4FromTarget(p);
  * (unsigned int *) p = data;
}


void conv4FromNativeToTarget(unsigned char *p) {
  unsigned int data;

  data = * (unsigned int *) p;
  write4ToTarget(p, data);
}
