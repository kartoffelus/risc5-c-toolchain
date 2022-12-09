/*
 * lssfs.c -- list directory of simple file system
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
  TocRecord tocRecord;
  int i;

  if (argc != 2) {
    usage(argv[0]);
  }
  disk = fopen(argv[1], "r+");
  if (disk == NULL) {
    error("cannot open disk image '%s'", argv[1]);
  }
  readSector(1, (Byte *) &tocRecord);
  if (tocRecord.magic_1 != SFS_MAGIC_1 ||
      tocRecord.magic_2 != SFS_MAGIC_2) {
    fclose(disk);
    error("'%s' is not a proper simple file system", argv[1]);
  }
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
  fclose(disk);
  return 0;
}
