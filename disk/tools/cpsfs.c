/*
 * cpsfs.c -- copy a file from host to simple file system
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


char *basename(char *path) {
  char *p;

  p = path + strlen(path);
  while (*p != '/' && p != path) {
    p--;
  }
  if (*p == '/') {
    p++;
  }
  return p;
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


int findFreeEntry(TocRecord *p) {
  int i;

  for (i = 0; i < 16; i++) {
    if (p->toc[i].name[0] == '\0') {
      return i;
    }
  }
  return -1;
}


/**************************************************************/


void copyFile(FILE *file, Word startSector, Word numSectors) {
  Byte buffer[SECTOR_SIZE];

  while (numSectors > 0) {
    fread(buffer, 1, SECTOR_SIZE, file);
    writeSector(startSector, buffer);
    startSector++;
    numSectors--;
  }
}


/**************************************************************/


void usage(char *myself) {
  printf("usage: %s <disk image> <host file>\n", myself);
  exit(1);
}


int main(int argc, char *argv[]) {
  TocRecord tocRecord;
  int n;
  TocEntry *p;
  FILE *file;
  Word fileSize;
  Word fileSizeSectors;

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
  n = findFreeEntry(&tocRecord);
  if (n < 0) {
    error("no free TOC entry found");
  }
  p = &tocRecord.toc[n];
  file = fopen(argv[2], "r");
  if (file == NULL) {
    error("cannot open input file '%s'", argv[2]);
  }
  fseek(file, 0, SEEK_END);
  fileSize = (Word) ftell(file);
  fseek(file, 0, SEEK_SET);
  fileSizeSectors = (fileSize + SECTOR_SIZE - 1) / SECTOR_SIZE;
  if (fileSizeSectors > tocRecord.freeSize) {
    error("not enough space on disk");
  }
  strcpy(p->name, basename(argv[2]));
  p->start = tocRecord.freeStart;
  p->size = fileSize;
  copyFile(file, tocRecord.freeStart, fileSizeSectors);
  tocRecord.freeStart += fileSizeSectors;
  tocRecord.freeSize -= fileSizeSectors;
  writeSector(1, (Byte *) &tocRecord);
  fclose(file);
  fclose(disk);
  return 0;
}
