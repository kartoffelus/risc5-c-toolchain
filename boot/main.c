/*
 * main.c -- bootstrap loader
 */


/**************************************************************/


#define LED_ADDR	((unsigned int *) 0xFFFFC4)
#define SWT_ADDR	((unsigned int *) 0xFFFFC4)


void wrled(unsigned int val) {
  *LED_ADDR = val;
}


unsigned int rdswt(void) {
  return *SWT_ADDR;
}


/**************************************************************/


#define SER_DATA	((unsigned int *) 0xFFFFC8)
#define SER_CTRL	((unsigned int *) 0xFFFFCC)


unsigned char rcvByte(void) {
  while ((*SER_CTRL & 1) == 0) ;
  return *SER_DATA;
}


void sndByte(unsigned char b) {
  while ((*SER_CTRL & 2) == 0) ;
  *(SER_DATA) = b;
}


/**************************************************************/


#define ACK		((unsigned char) 0x10)
#define NAK		((unsigned char) 0x11)


unsigned int rcvInt(void) {
  unsigned int val;

  val = 0;
  val |= ((unsigned int) rcvByte() <<  0);
  val |= ((unsigned int) rcvByte() <<  8);
  val |= ((unsigned int) rcvByte() << 16);
  val |= ((unsigned int) rcvByte() << 24);
  return val;
}


unsigned int loadFromLine(void) {
  unsigned int len;
  unsigned char *addr;
  unsigned int start;

  start = (unsigned int) 1;
  while (1) {
    len = rcvInt();
    if (len == 0) {
      break;
    }
    addr = (unsigned char *) rcvInt();
    if (start == (unsigned int) 1) {
      start = (unsigned int) addr;
    }
    do {
      *addr++ = rcvByte();
      len--;
    } while (len != 0);
  }
  sndByte(ACK);
  return start;
}


/**************************************************************/


unsigned int loadFromDisk(void) {
  return (unsigned int) 1;
}


/**************************************************************/


void main(void) {
  unsigned int start;

  wrled(0x80);
  if (rdswt() & 1) {
    /* loading from serial line requested */
    wrled(0x81);
    start = loadFromLine();
  } else {
    /* loading from disk requested */
    wrled(0x82);
    start = loadFromDisk();
  }
  if (start != (unsigned int) 1) {
    /* loading succeeded, start program */
    wrled(0x83);
    (* (void (*)(void)) start)();
  }
  /* loading failed */
  wrled(0xFF);
}
