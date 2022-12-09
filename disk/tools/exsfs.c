/*
 * exsfs.c -- extract a file from simple file system to host
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


void copyFile(FILE *file, Word startSector, Word fileSize) {
  Byte buffer[SECTOR_SIZE];
  Word size;

  while (fileSize > 0) {
    readSector(startSector, buffer);
    size = (fileSize >= SECTOR_SIZE) ? SECTOR_SIZE : fileSize;
    if (fwrite(buffer, 1, size, file) != size) {
      error("cannot write output file");
    }
    startSector++;
    fileSize -= size;
  }
}


/**************************************************************/


void usage(char *myself) {
  printf("usage: %s <disk image> <entry number>\n", myself);
  exit(1);
}


int main(int argc, char *argv[]) {
  TocRecord tocRecord;
  int n;
  char *endptr;
  TocEntry *p;
  FILE *file;

  if (argc != 3) {
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
  n = strtol(argv[2], &endptr, 0);
  if (*endptr != '\0') {
    error("cannot read entry number");
  }
  if (n < 0 || n >= 16) {
    error("illegal entry number %d", n);
  }
  p = &tocRecord.toc[n];
  if (p->name[0] == '\0') {
    error("entry %d is empty", n);
  }
  file = fopen(p->name, "w");
  if (file == NULL) {
    error("cannot open output file '%s'", p->name);
  }
  copyFile(file, p->start, p->size);
  fclose(file);
  fclose(disk);
  return 0;
}
