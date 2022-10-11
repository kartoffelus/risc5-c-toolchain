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


int loadFromLine(void) {
  unsigned int len;
  unsigned char *addr;

  while (1) {
    len = rcvInt();
    if (len == 0) {
      break;
    }
    addr = (unsigned char *) rcvInt();
    do {
      *addr++ = rcvByte();
      len--;
    } while (len != 0);
  }
  sndByte(ACK);
  return 1;
}


/**************************************************************/


#define SPI_DATA	((unsigned int *) 0xFFFFD0)
#define SPI_CTRL	((unsigned int *) 0xFFFFD4)


/*
 * send n FFs slowly with no card selected
 */
void spiIdle(int n) {
  *SPI_CTRL = 0;
  while (n > 0) {
    n--;
    *SPI_DATA = 0xFFFFFFFF;
    while ((*SPI_CTRL & 1) == 0) ;
  }
}


/*
 * send & rcv byte slowly with card selected
 */
void spi(int n) {
  *SPI_CTRL = 1;
  *SPI_DATA = n;
  while ((*SPI_CTRL & 1) == 0) ;
}


/*
 * send command
 */
void spiCmd(int n, int arg) {
  int data;
  int crc;
  int i;

  /* flush while deselected */
  do {
    spiIdle(1);
    data = *SPI_DATA;
  } while (data != 255) ;
  /* flush while selected */
  do {
    spi(255);
    data = *SPI_DATA;
  } while (data != 255) ;
  /* select precomputed CRC */
  crc = (n == 8) ? 135 : ((n == 0) ? 149 : 255);
  /* send command */
  spi(0x40 | (n & 0x3F));
  /* send arg */
  for (i = 24; i >= 0; i -= 8) {
    spi(arg >> i);
  }
  /* send CRC */
  spi(crc);
  /* wait for response */
  i = 32;
  do {
    spi(255);
    data = *SPI_DATA;
    i--;
  } while (data >= 0x80 && i != 0);
}


/*
 * initialize SPI
 */
void spiInit(void) {
  int res;

  /* first, idle for at least 80 clks */
  spiIdle(9);
  /* CMD0 when card selected, sets MMC SPI mode */
  spiCmd(0, 0);
  /* CMD8 for SD cards */
  spiCmd(8, 0x1AA);
  spi(-1);
  spi(-1);
  spi(-1);
  /* wait for card getting ready */
  do {
    /* ACMD41, optionally with high-capacity (HCS) bit set, starts init */
    spiCmd(55, 0);  /* APP cmd follows */
    spiCmd(41, 1 << 30);  /* HCS */
    res = *SPI_DATA;
    spi(-1);
    spi(-1);
    spi(-1);
    spiIdle(10000);
  } while (res != 0);
  /* CMD16: set block size as a precaution (should default) */
  spiCmd(16, 512);
  spiIdle(1);
}


/**************************************************************/


void sdShift(int *p) {
  int data;

  /* CMD58: get card capacity bit */
  spiCmd(58, 0);
  data = *SPI_DATA;
  spi(-1);
  if (data != 0 || (*SPI_DATA & 0x40) == 0) {
    *p *= 512;
  }
  spi(-1);
  spi(-1);
  spiIdle(1);
}


void sdRead(int src, int dst) {
  int data;
  int i;

  sdShift(&src);
  /* CMD17: read one block */
  spiCmd(17, src);
  /* wait for start data marker */
  do {
    spi(-1);
    data = *SPI_DATA;
  } while (data != 254);
  /* switch to fast */
  *SPI_CTRL = 4 + 1;
  /* get data */
  for (i = 0; i <= 508; i += 4) {
    *SPI_DATA = 0xFFFFFFFF;
    while ((*SPI_CTRL & 1) == 0) ;
    data = *SPI_DATA;
    * (unsigned int *) dst = data;
    dst += 4;
  }
  spi(255);
  spi(255);
  spiIdle(1);
}


int loadFromDisk(void) {
  unsigned char sb0, sb1;

  /* read boot sector */
  sdRead(0, 0);
  /* check boot signature */
  sb0 = * (unsigned char *) 0x1FE;
  sb1 = * (unsigned char *) 0x1FF;
  if (sb0 == 0x55 && sb1 == 0xAA) {
    /* success */
    return 1;
  }
  /* failure*/
  return 0;
}


/**************************************************************/


void main(void) {
  int loaded;

  wrled(0x80);
  spiInit();
  if (rdswt() & 1) {
    /* loading from serial line requested */
    wrled(0x81);
    loaded = loadFromLine();
  } else {
    /* loading from disk requested */
    wrled(0x82);
    loaded = loadFromDisk();
  }
  if (loaded) {
    /* loading succeeded, start program */
    wrled(0x83);
    (* (void (*)(void)) 0)();
  }
  /* loading failed */
  wrled(0xFF);
}
