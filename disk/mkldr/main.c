/*
 * main.c -- main program
 */


#include "stdarg.h"
#include "iolib.h"
#include "sfs.h"
#include "promlib.h"


#define LINE_SIZE	100


/**************************************************************/


TocRecord tocRecord;


/**************************************************************/


void halt(void) {
  while (1) ;
}


void error(char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  printf("error: ");
  vprintf(fmt, ap);
  printf("\n");
  va_end(ap);
  halt();
}


/**************************************************************/


void list(void) {
  int i;

  printf("entry   name                    size\n");
  for (i = 0; i < 16; i++) {
    printf("  %2d    ", i);
    if (tocRecord.toc[i].name[0] != '\0') {
      printf("%-24s", tocRecord.toc[i].name);
      printf("%d", tocRecord.toc[i].size);
    }
    printf("\n");
  }
  printf("free : %u bytes\n", tocRecord.freeSize * SECTOR_SIZE);
}


/**************************************************************/


void load(Word start, Word size) {
  Word sector;
  Word *address;
  Word loaded;

  sector = start;
  address = (Word *) 0;
  loaded = 0;
  while (loaded < size) {
    sdcardRead(sector, address);
    sector++;
    address += SECTOR_SIZE / sizeof(Word);
    loaded += SECTOR_SIZE;
  }
}


/**************************************************************/


int main(void) {
  char line[LINE_SIZE];
  char *endptr;
  int n;

  printf("Loader executing...\n");
  printf("\n");
  sdcardRead(1, (Word *) &tocRecord);
  if (tocRecord.magic_1 != SFS_MAGIC_1 ||
      tocRecord.magic_2 != SFS_MAGIC_2) {
    error("the disk does not host a proper simple file system");
  }
  while (1) {
    printf("\nFiles:\n\n");
    list();
    printf("\n");
    getLine("Please choose an entry: ", line, LINE_SIZE);
    n = str2int(line, &endptr);
    if (*endptr != '\0' && *endptr != '\n') {
      printf("cannot read entry number\n");
      continue;
    }
    if (n < 0 || n >= 16) {
      printf("illegal entry number %d\n", n);
      continue;
    }
    if (tocRecord.toc[n].name[0] == '\0') {
      printf("entry %d is empty\n", n);
      continue;
    }
    printf("loading file '%s'%s\n", tocRecord.toc[n].name);
    load(tocRecord.toc[n].start, tocRecord.toc[n].size);
    printf("loading done, now executing...\n");
    (* (void (*)(void)) 0)();
  }
  return 0;
}
