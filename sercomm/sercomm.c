/*
 * sercomm.c -- serial line communication support
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>


#define SERDEV_FILE	"serial.dev"

#define BLOCK_SIZE	512

#define LINE_SIZE	200

#define ACK		((unsigned char) 0x10)


static int sfd = -1;
static struct termios origOptions;
static struct termios currOptions;


void serialClose(void);


/**************************************************************/


void error(char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  printf("Error: ");
  vprintf(fmt, ap);
  printf("\n");
  va_end(ap);
  serialClose();
  exit(1);
}


/**************************************************************/


void serialOpen(char *serialPort) {
  sfd = open(serialPort, O_RDWR | O_NOCTTY | O_NDELAY);
  if (sfd == -1) {
    error("cannot open serial port '%s'", serialPort);
  }
  tcgetattr(sfd, &origOptions);
  currOptions = origOptions;
  cfsetispeed(&currOptions, B9600);
  cfsetospeed(&currOptions, B9600);
  currOptions.c_cflag |= (CLOCAL | CREAD);
  currOptions.c_cflag &= ~PARENB;
  currOptions.c_cflag &= ~CSTOPB;
  currOptions.c_cflag &= ~CSIZE;
  currOptions.c_cflag |= CS8;
  currOptions.c_cflag &= ~CRTSCTS;
  currOptions.c_lflag &= ~(ICANON | ECHO | ECHONL | ISIG | IEXTEN);
  currOptions.c_iflag &= ~(IGNBRK | BRKINT | IGNPAR | PARMRK);
  currOptions.c_iflag &= ~(INPCK | ISTRIP | INLCR | IGNCR | ICRNL);
  currOptions.c_iflag &= ~(IXON | IXOFF | IXANY);
  currOptions.c_oflag &= ~(OPOST | ONLCR | OCRNL | ONOCR | ONLRET);
  tcsetattr(sfd, TCSANOW, &currOptions);
}


void serialClose(void) {
  if (sfd < 0) {
    return;
  }
  tcsetattr(sfd, TCSANOW, &origOptions);
  close(sfd);
  sfd = -1;
}


int serialSnd(unsigned char b) {
  int n;

  n = write(sfd, &b, 1);
  return n == 1;
}


int serialRcv(unsigned char *bp) {
  int n;

  n = read(sfd, bp, 1);
  return n == 1;
}


/**************************************************************/


unsigned char rcvByte(void) {
  unsigned char b;

  while (!serialRcv(&b)) ;
  return b;
}


int rcvInt(void) {
  int i;
  unsigned char b;

  i = 0;
  while (!serialRcv(&b)) ;
  i |= (unsigned int) b <<  0;
  while (!serialRcv(&b)) ;
  i |= (unsigned int) b <<  8;
  while (!serialRcv(&b)) ;
  i |= (unsigned int) b << 16;
  while (!serialRcv(&b)) ;
  i |= (unsigned int) b << 24;
  return i;
}


char *rcvStr(void) {
  static char buf[100];
  int i;
  unsigned char b;

  i = 0;
  do {
    while (!serialRcv(&b)) ;
    buf[i] = b;
    i++;
  } while (b != '\0');
  return buf;
}


void sndByte(unsigned char b) {
  while (!serialSnd(b)) ;
}


void sndInt(unsigned int i) {
  while (!serialSnd((i >>  0) & 0xFF)) ;
  while (!serialSnd((i >>  8) & 0xFF)) ;
  while (!serialSnd((i >> 16) & 0xFF)) ;
  while (!serialSnd((i >> 24) & 0xFF)) ;
}


void sndStr(char *s) {
  while (*s != '\0') {
    sndByte(*s);
    s++;
  }
  sndByte(0);
}


/**************************************************************/


int sendBootFile(FILE *bootFile, unsigned int addr) {
  unsigned char buf[BLOCK_SIZE];
  int n, i;
  unsigned char b;

  while (1) {
    n = fread(buf, 1, BLOCK_SIZE, bootFile);
    if (n < 0) {
      error("cannot read boot file");
    }
    if (n == 0) {
      break;
    }
    sndInt(n);
    sndInt(addr);
    for (i = 0; i < n; i++) {
      sndByte(buf[i]);
    }
    addr += n;
    if (n < BLOCK_SIZE) {
      break;
    }
  }
  sndInt(0);
  b = rcvByte();
  return b == ACK;
}


/**************************************************************/


void usage(char *myself) {
  printf("Usage: %s [<boot file>]\n", myself);
  exit(1);
}


int main(int argc, char *argv[]) {
  FILE *serdevFile;
  char serialPort[LINE_SIZE];
  int n;
  char *bootName;
  unsigned char b;
  FILE *bootFile;

  if (argc != 1 && argc != 2) {
    usage(argv[0]);
  }
  serdevFile = fopen(SERDEV_FILE, "r");
  if (serdevFile == NULL) {
    error("cannot open file '%s' for reading the\npath to "
          "the serial device. Please create this file.",
          SERDEV_FILE);
  }
  if (fgets(serialPort, LINE_SIZE, serdevFile) == NULL) {
    error("cannot read file '%s' (should contain a valid path).",
          SERDEV_FILE);
  }
  fclose(serdevFile);
  n = strlen(serialPort) - 1;
  if (serialPort[n] == '\n') {
    serialPort[n] = '\0';
  }
  if (argc == 1) {
    bootName = NULL;
  } else {
    bootName = argv[1];
  }
  serialOpen(serialPort);
  while (serialRcv(&b)) ;
  if (bootName != NULL) {
    bootFile = fopen(bootName, "r");
    if (bootFile == NULL) {
      error("cannot open boot file '%s'", bootName);
    }
    printf("Sending boot file '%s', please wait...\n", bootName);
    if (sendBootFile(bootFile, 0)) {
      printf("Sending boot file succeeded.\n");
    } else {
      printf("Sending boot file failed.\n");
    }
    fclose(bootFile);
  }
  return 0;
}
