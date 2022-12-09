/*
 * mksfs.c -- make a simple file system
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "sfs.h"


/**************************************************************/


void error(char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  printf("error: ");
  vprintf(fmt, ap);
  printf("\n");
  va_end(ap);
  exit(1);
}


/**************************************************************/


FILE *disk;


void readSector(Word sector, Byte *buffer) {
  fseek(disk, sector * SECTOR_SIZE, SEEK_SET);
  if (fread(buffer, SECTOR_SIZE, 1, disk) != 1) {
    error("cannot read sector 0x%08X\n", sector);
  }
}


void writeSector(Word sector, Byte *buffer) {
  fseek(disk, sector * SECTOR_SIZE, SEEK_SET);
  if (fwrite(buffer, SECTOR_SIZE, 1, disk) != 1) {
    error("cannot write sector 0x%08X\n", sector);
  }
}


/**************************************************************/


void usage(char *myself) {
  printf("usage: %s <disk image>\n", myself);
  exit(1);
}


int main(int argc, char *argv[]) {
  Word size;
  int i;
  TocRecord tocRecord;
  Byte *p;

  if (argc != 2) {
    usage(argv[0]);
  }
  disk = fopen(argv[1], "r+");
  if (disk == NULL) {
    error("cannot open disk image '%s'", argv[1]);
  }
  fseek(disk, 0, SEEK_END);
  size = (Word) ftell(disk);
  fseek(disk, 0, SEEK_SET);
  if (size % SECTOR_SIZE != 0) {
    error("disk size is not an integral multiple of sector size");
  }
  size /= SECTOR_SIZE;
  p = (Byte *) &tocRecord;
  for (i = 0; i < SECTOR_SIZE; i++) {
    *p++ = '\0';
  }
  tocRecord.freeStart = 0 + RESERVED;
  tocRecord.freeSize = size - RESERVED;
  tocRecord.magic_1 = SFS_MAGIC_1;
  tocRecord.magic_2 = SFS_MAGIC_2;
  writeSector(1, (Byte *) &tocRecord);
  fclose(disk);
  return 0;
}
