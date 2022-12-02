/*
 * main.c -- main program
 */


#include "types.h"
#include "stdarg.h"
#include "iolib.h"
#include "promlib.h"


void delayTest(void) {
  printf("please check the delay, it should last 30 seconds\n");
  delay(30000);
  printf("\n");
}


void ledTest(void) {
  int i;

  printf("please watch the LEDs\n");
  for (i = 0; i < 5; i++) {
    ledWrite(0xFF);
    delay(500);
    ledWrite(0x00);
    delay(500);
  }
  printf("\n");
}


void switchTest(void) {
  Word pat;
  int i;

  pat = switchRead();
  printf("switches : ");
  for (i = 7; i >= 0; i--) {
    printf("%c ", (pat & (1 << i)) ? '1' : '0');
  }
  printf("\n");
  printf("\n");
}


void sdcardTest(void) {
  Word buffer[512/4];
  Byte *p;
  int i;

  /* read sector 0 */
  printf("read sector 0\n");
  sdcardRead(0, buffer);
  printf("0x%08X  0x%08X  0x%08X  0x%08X\n",
         buffer[0], buffer[1], buffer[2], buffer[3]);
  /* read sector 123 */
  printf("read sector 123\n");
  sdcardRead(123, buffer);
  printf("0x%08X  0x%08X  0x%08X  0x%08X\n",
         buffer[0], buffer[1], buffer[2], buffer[3]);
  /* write sector 123 */
  printf("write sector 123\n");
  p = (Byte *) buffer;
  for (i = 0; i < 512; i++) {
    *p++ = i;
  }
  sdcardWrite(123, buffer);
  /* read sector 0 */
  printf("read sector 0\n");
  sdcardRead(0, buffer);
  printf("0x%08X  0x%08X  0x%08X  0x%08X\n",
         buffer[0], buffer[1], buffer[2], buffer[3]);
  /* read sector 123 */
  printf("read sector 123\n");
  sdcardRead(123, buffer);
  printf("0x%08X  0x%08X  0x%08X  0x%08X\n",
         buffer[0], buffer[1], buffer[2], buffer[3]);
  printf("\n");
}


int main(void) {
  char c;

  printf("\nPROM Functions Test\n\n");
  while (1) {
    printf("please choose one of the following actions:\n");
    printf("    0   quit\n");
    printf("    1   delay test\n");
    printf("    2   LED test\n");
    printf("    3   switch test\n");
    printf("    4   SD card test\n");
    c = serialRead();
    printf("%c\n\n", c);
    if (c == '0') {
      break;
    }
    if (c == '1') {
      delayTest();
      continue;
    }
    if (c == '2') {
      ledTest();
      continue;
    }
    if (c == '3') {
      switchTest();
      continue;
    }
    if (c == '4') {
      sdcardTest();
      continue;
    }
    printf("unknown action, please try again\n");
  }
  printf("End of PROM Functions Test\n");
  return 0;
}
