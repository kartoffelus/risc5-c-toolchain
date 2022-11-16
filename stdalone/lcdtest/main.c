/*
 * main.c -- RISC5 LCD test
 */


#include "types.h"
#include "stdarg.h"
#include "iolib.h"


#define LINE_LENGTH	100


/**************************************************************/


unsigned int readData(void) {
  unsigned int val;

  val = * (unsigned int *) 0xFFFF88;
  return val;
}


void writeData(unsigned int val) {
  * (unsigned int *) 0xFFFF88 = val;
}


unsigned int readCtrl(void) {
  unsigned int val;

  val = * (unsigned int *) 0xFFFF8C;
  return val;
}


void writeCtrl(unsigned int val) {
  * (unsigned int *) 0xFFFF8C = val;
}


/**************************************************************/


unsigned int getHex(char *s) {
  unsigned int val;
  unsigned int digit;

  val = 0;
  while ((*s >= 'A' && *s <= 'F') ||
         (*s >= 'a' && *s <= 'f') ||
         (*s >= '0' && *s <= '9')) {
    if (*s >= 'A' && *s <= 'F') {
      digit = *s - 'A' + 10;
    } else
    if (*s >= 'a' && *s <= 'f') {
      digit = *s - 'a' + 10;
    } else {
      digit = *s - '0';
    }
    val *= 16;
    val += digit;
    s++;
  }
  return val;
}


/**************************************************************/


int main(void) {
  char line[LINE_LENGTH];
  unsigned int val;

  printf("\nRISC5 LCD test started\n\n");
  while (1) {
    line[0] = '\0';
    getLine("rd, wd <hex>, rc, wc <hex> ? ", line, LINE_LENGTH);
    if (line[0] == 'r' && line[1] == 'd' && line[2] == '\0') {
      val = readData();
      printf("0x%08X\n", val);
    } else
    if (line[0] == 'w' && line[1] == 'd' && line[2] == ' ') {
      val = getHex(line + 3);
      writeData(val);
    } else
    if (line[0] == 'r' && line[1] == 'c' && line[2] == '\0') {
      val = readCtrl();
      printf("0x%08X\n", val);
    } else
    if (line[0] == 'w' && line[1] == 'c' && line[2] == ' ') {
      val = getHex(line + 3);
      writeCtrl(val);
    } else {
      printf("unknown command\n");
    }
  }
  printf("\nRISC5 LCD test finished\n");
  return 0;
}
