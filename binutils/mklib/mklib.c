/*
 * mklib.c -- RISC5 library manager
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "../include/lib.h"
#include "../include/exec.h"


/**************************************************************/


FILE *libFile;
LibHeader libHeader;
ModuleRecord *moduleTable;


/**************************************************************/


void error(char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  printf("Error: ");
  vprintf(fmt, ap);
  printf("\n");
  va_end(ap);
  exit(1);
}


void *memAlloc(unsigned int size) {
  void *p;

  p = malloc(size);
  if (p == NULL) {
    error("out of memory");
  }
  return p;
}


void memFree(void *p) {
  if (p == NULL) {
    error("memFree() got NULL pointer");
  }
  free(p);
}


/**************************************************************/


unsigned int read4FromEco(unsigned char *p) {
  return (unsigned int) p[0] <<  0 |
         (unsigned int) p[1] <<  8 |
         (unsigned int) p[2] << 16 |
         (unsigned int) p[3] << 24;
}


void write4ToEco(unsigned char *p, unsigned int data) {
  p[0] = data >>  0;
  p[1] = data >>  8;
  p[2] = data >> 16;
  p[3] = data >> 24;
}


void conv4FromEcoToNative(unsigned char *p) {
  unsigned int data;

  data = read4FromEco(p);
  * (unsigned int *) p = data;
}


void conv4FromNativeToEco(unsigned char *p) {
  unsigned int data;

  data = * (unsigned int *) p;
  write4ToEco(p, data);
}


/**************************************************************/


void dumpString(unsigned int offset) {
  long pos;
  int c;

  pos = ftell(libFile);
  if (fseek(libFile, libHeader.ostrs + offset, SEEK_SET) < 0) {
    error("cannot seek to string");
  }
  while (1) {
    c = fgetc(libFile);
    if (c == EOF) {
      error("unexpected end of file");
    }
    if (c == 0) {
      break;
    }
    fputc(c, stdout);
  }
  fseek(libFile, pos, SEEK_SET);
}


void dumpStrings(unsigned int offset, int n) {
  long pos;
  int i;
  int c;

  pos = ftell(libFile);
  if (fseek(libFile, libHeader.ostrs + offset, SEEK_SET) < 0) {
    error("cannot seek to strings");
  }
  for (i = 0; i < n; i++) {
    printf("                 ");
    while (1) {
      c = fgetc(libFile);
      if (c == EOF) {
        error("unexpected end of file");
      }
      if (c == 0) {
        break;
      }
      fputc(c, stdout);
    }
    printf("\n");
  }
  fseek(libFile, pos, SEEK_SET);
}


/**************************************************************/

/* create library from files */


int fileOffset;
int dataSize;
int stringSize;


void writeDummyHeader(void) {
  fwrite(&libHeader, sizeof(LibHeader), 1, libFile);
  /* update file offset */
  fileOffset += sizeof(LibHeader);
}


void writeRealHeader(void) {
  conv4FromNativeToEco((unsigned char *) &libHeader.magic);
  conv4FromNativeToEco((unsigned char *) &libHeader.omods);
  conv4FromNativeToEco((unsigned char *) &libHeader.nmods);
  conv4FromNativeToEco((unsigned char *) &libHeader.odata);
  conv4FromNativeToEco((unsigned char *) &libHeader.sdata);
  conv4FromNativeToEco((unsigned char *) &libHeader.ostrs);
  conv4FromNativeToEco((unsigned char *) &libHeader.sstrs);
  fwrite(&libHeader, sizeof(LibHeader), 1, libFile);
  conv4FromEcoToNative((unsigned char *) &libHeader.magic);
  conv4FromEcoToNative((unsigned char *) &libHeader.omods);
  conv4FromEcoToNative((unsigned char *) &libHeader.nmods);
  conv4FromEcoToNative((unsigned char *) &libHeader.odata);
  conv4FromEcoToNative((unsigned char *) &libHeader.sdata);
  conv4FromEcoToNative((unsigned char *) &libHeader.ostrs);
  conv4FromEcoToNative((unsigned char *) &libHeader.sstrs);
}


void writeDummyModuleTable(int nmods) {
  fwrite(moduleTable, sizeof(ModuleRecord), nmods, libFile);
  /* update file offset */
  fileOffset += nmods * sizeof(ModuleRecord);
}


void writeRealModuleTable(void) {
  int mn;

  for (mn = 0; mn < libHeader.nmods; mn++) {
    conv4FromNativeToEco((unsigned char *) &moduleTable[mn].name);
    conv4FromNativeToEco((unsigned char *) &moduleTable[mn].offs);
    conv4FromNativeToEco((unsigned char *) &moduleTable[mn].size);
    conv4FromNativeToEco((unsigned char *) &moduleTable[mn].fsym);
    conv4FromNativeToEco((unsigned char *) &moduleTable[mn].nsym);
    fwrite(&moduleTable[mn], sizeof(ModuleRecord), 1, libFile);
    conv4FromEcoToNative((unsigned char *) &moduleTable[mn].name);
    conv4FromEcoToNative((unsigned char *) &moduleTable[mn].offs);
    conv4FromEcoToNative((unsigned char *) &moduleTable[mn].size);
    conv4FromEcoToNative((unsigned char *) &moduleTable[mn].fsym);
    conv4FromEcoToNative((unsigned char *) &moduleTable[mn].nsym);
  }
}


unsigned int writeFile(char *fileName) {
  FILE *inFile;
  int c;
  long size;

  inFile = fopen(fileName, "r");
  if (inFile == NULL) {
    error("cannot open file '%s' for read", fileName);
  }
  while (1) {
    c = getc(inFile);
    if (c == EOF) {
      break;
    }
    fputc(c, libFile);
  }
  size = ftell(inFile);
  fclose(inFile);
  return size;
}


void writeFiles(int n, char *fileNames[]) {
  int mn;
  int size;

  for (mn = 0; mn < n; mn++) {
    moduleTable[mn].offs = dataSize;
    size = writeFile(fileNames[mn]);
    moduleTable[mn].size = size;
    dataSize += size;
    fileOffset += size;
  }
}


void readObjHeader(ExecHeader *objHeader,
                   FILE *objFile, char *objName) {
  if (fseek(objFile, 0, SEEK_SET) < 0) {
    error("cannot seek to header in object file '%s'", objName);
  }
  if (fread(objHeader, sizeof(ExecHeader), 1, objFile) != 1) {
    error("cannot read header from object file '%s'", objName);
  }
  conv4FromEcoToNative((unsigned char *) &objHeader->magic);
  conv4FromEcoToNative((unsigned char *) &objHeader->osegs);
  conv4FromEcoToNative((unsigned char *) &objHeader->nsegs);
  conv4FromEcoToNative((unsigned char *) &objHeader->osyms);
  conv4FromEcoToNative((unsigned char *) &objHeader->nsyms);
  conv4FromEcoToNative((unsigned char *) &objHeader->orels);
  conv4FromEcoToNative((unsigned char *) &objHeader->nrels);
  conv4FromEcoToNative((unsigned char *) &objHeader->odata);
  conv4FromEcoToNative((unsigned char *) &objHeader->sdata);
  conv4FromEcoToNative((unsigned char *) &objHeader->ostrs);
  conv4FromEcoToNative((unsigned char *) &objHeader->sstrs);
  conv4FromEcoToNative((unsigned char *) &objHeader->entry);
  if (objHeader->magic != EXEC_MAGIC) {
    error("wrong magic number in object file '%s'", objName);
  }
}


void readObjSymtbl(SymbolRecord *symbols, unsigned int osyms, int nsyms,
                   FILE *objFile, char *objName) {
  int sn;
  SymbolRecord *sp;

  if (fseek(objFile, osyms, SEEK_SET) < 0) {
    error("cannot seek to symbols in object file '%s'", objName);
  }
  for (sn = 0; sn < nsyms; sn++) {
    sp = &symbols[sn];
    if (fread(sp, sizeof(SymbolRecord), 1, objFile) != 1) {
      error("cannot read symbol from object file '%s'", objName);
    }
    conv4FromEcoToNative((unsigned char *) &sp->name);
    conv4FromEcoToNative((unsigned char *) &sp->val);
    conv4FromEcoToNative((unsigned char *) &sp->seg);
    conv4FromEcoToNative((unsigned char *) &sp->attr);
  }
}


void readObjStrings(char *strings, unsigned int ostrs, unsigned int sstrs,
                    FILE *objFile, char *objName) {
  if (fseek(objFile, ostrs, SEEK_SET) < 0) {
    error("cannot seek to strings in object file '%s'", objName);
  }
  if (fread(strings, 1, sstrs, objFile) != sstrs) {
    error("cannot read strings from object file '%s'", objName);
  }
}


void writeStrings(int n, char *fileNames[], int verbose) {
  int mn;
  int size;
  char *baseName;
  FILE *objFile;
  ExecHeader objHeader;
  SymbolRecord *symbols;
  char *strings;
  int sn;
  int nDefSyms;

  for (mn = 0; mn < n; mn++) {
    /* write base (!) name of module */
    moduleTable[mn].name = stringSize;
    size = strlen(fileNames[mn]) + 1;
    baseName = fileNames[mn] + size - 1;
    while (baseName != fileNames[mn] && *baseName != '/') {
      baseName--;
    }
    if (*baseName == '/') {
      baseName++;
    }
    size -= baseName - fileNames[mn];
    if (verbose) {
      printf("adding module '%s'\n", baseName);
    }
    fwrite(baseName, 1, size, libFile);
    stringSize += size;
    fileOffset += size;
    /* write names of exported symbols */
    objFile = fopen(fileNames[mn], "r");
    if (objFile == NULL) {
      error("cannot open object file '%s' for read", fileNames[mn]);
    }
    readObjHeader(&objHeader, objFile, fileNames[mn]);
    symbols = memAlloc(objHeader.nsyms * sizeof(SymbolRecord));
    readObjSymtbl(symbols, objHeader.osyms, objHeader.nsyms,
                  objFile, fileNames[mn]);
    strings = memAlloc(objHeader.sstrs);
    readObjStrings(strings, objHeader.ostrs, objHeader.sstrs,
                   objFile, fileNames[mn]);
    moduleTable[mn].fsym = stringSize;
    nDefSyms = 0;
    for (sn = 0; sn < objHeader.nsyms; sn++) {
      if (!(symbols[sn].attr & SYM_ATTR_U)) {
        nDefSyms++;
        size = strlen(strings + symbols[sn].name) + 1;
        fwrite(strings + symbols[sn].name, 1, size, libFile);
        stringSize += size;
        fileOffset += size;
      }
    }
    moduleTable[mn].nsym = nDefSyms;
    fclose(objFile);
    memFree(symbols);
    memFree(strings);
  }
}


void createLibrary(char *libName, int n, char *fileNames[], int verbose) {
  libFile = fopen(libName, "w");
  if (libFile == NULL) {
    error("cannot open library '%s' for write", libName);
  }
  fileOffset = 0;
  dataSize = 0;
  stringSize = 0;
  writeDummyHeader();
  libHeader.magic = LIB_MAGIC;
  libHeader.omods = fileOffset;
  libHeader.nmods = n;
  moduleTable = memAlloc(n * sizeof(ModuleRecord));
  writeDummyModuleTable(n);
  libHeader.odata = fileOffset;
  writeFiles(n, fileNames);
  libHeader.sdata = dataSize;
  libHeader.ostrs = fileOffset;
  writeStrings(n, fileNames, verbose);
  libHeader.sstrs = stringSize;
  rewind(libFile);
  writeRealHeader();
  writeRealModuleTable();
  fclose(libFile);
}


/**************************************************************/

/* show table of library contents */


void readHeader(void) {
  if (fseek(libFile, 0, SEEK_SET) < 0) {
    error("cannot seek to library header");
  }
  if (fread(&libHeader, sizeof(LibHeader), 1, libFile) != 1) {
    error("cannot read library header");
  }
  conv4FromEcoToNative((unsigned char *) &libHeader.magic);
  conv4FromEcoToNative((unsigned char *) &libHeader.omods);
  conv4FromEcoToNative((unsigned char *) &libHeader.nmods);
  conv4FromEcoToNative((unsigned char *) &libHeader.odata);
  conv4FromEcoToNative((unsigned char *) &libHeader.sdata);
  conv4FromEcoToNative((unsigned char *) &libHeader.ostrs);
  conv4FromEcoToNative((unsigned char *) &libHeader.sstrs);
  if (libHeader.magic != LIB_MAGIC) {
    error("wrong magic number in library header");
  }
}


void readModuleTable(void) {
  int mn;

  moduleTable = memAlloc(libHeader.nmods * sizeof(ModuleRecord));
  if (fseek(libFile, libHeader.omods, SEEK_SET) < 0) {
    error("cannot seek to module table");
  }
  for (mn = 0; mn < libHeader.nmods; mn++) {
    if (fread(&moduleTable[mn], sizeof(ModuleRecord), 1, libFile) != 1) {
      error("cannot read module record %d", mn);
    }
    conv4FromEcoToNative((unsigned char *) &moduleTable[mn].name);
    conv4FromEcoToNative((unsigned char *) &moduleTable[mn].offs);
    conv4FromEcoToNative((unsigned char *) &moduleTable[mn].size);
    conv4FromEcoToNative((unsigned char *) &moduleTable[mn].fsym);
    conv4FromEcoToNative((unsigned char *) &moduleTable[mn].nsym);
  }
}


void showTable(char *libName, int verbose) {
  int mn;

  libFile = fopen(libName, "r");
  if (libFile == NULL) {
    error("cannot open library '%s' for read", libName);
  }
  readHeader();
  printf("Library contents:\n");
  if (libHeader.nmods == 0) {
    printf("<empty>\n");
    fclose(libFile);
    return;
  }
  readModuleTable();
  for (mn = 0; mn < libHeader.nmods; mn++) {
    printf("    %d:\n", mn);
    printf("        name   = ");
    dumpString(moduleTable[mn].name);
    printf("\n");
    printf("        offset = 0x%08X\n", moduleTable[mn].offs);
    printf("        size   = 0x%08X\n", moduleTable[mn].size);
    if (verbose) {
      printf("        nsyms  = %d\n", moduleTable[mn].nsym);
      dumpStrings(moduleTable[mn].fsym, moduleTable[mn].nsym);
    }
  }
  fclose(libFile);
}


/**************************************************************/

/* extract files from library */


void extractFiles(char *libName, int verbose) {
  int mn;
  char *strings;
  char *outName;
  unsigned int modStart;
  FILE *outFile;
  int cnt;
  int c;

  libFile = fopen(libName, "r");
  if (libFile == NULL) {
    error("cannot open library '%s' for read", libName);
  }
  readHeader();
  readModuleTable();
  strings = memAlloc(libHeader.sstrs);
  if (fseek(libFile, libHeader.ostrs, SEEK_SET) < 0) {
    error("cannot seek to strings");
  }
  if (fread(strings, 1, libHeader.sstrs, libFile) != libHeader.sstrs) {
    error("cannot read strings");
  }
  for (mn = 0; mn < libHeader.nmods; mn++) {
    outName = strings + moduleTable[mn].name;
    modStart = libHeader.odata + moduleTable[mn].offs;
    if (fseek(libFile, modStart, SEEK_SET) < 0) {
      error("cannot seek to module '%s'", outName);
    }
    outFile = fopen(outName, "w");
    if (outFile == NULL) {
      error("cannot open output file '%s'", outName);
    }
    if (verbose) {
      printf("extracting module '%s'\n", outName);
    }
    for (cnt = 0; cnt < moduleTable[mn].size; cnt++) {
      c = fgetc(libFile);
      if (c == EOF) {
        error("cannot read module '%s'", outName);
      }
      c = fputc(c, outFile);
      if (c == EOF) {
        error("cannot write module '%s'", outName);
      }
    }
    fclose(outFile);
  }
  fclose(libFile);
}


/**************************************************************/


void usage(char *myself) {
  printf("Usage: %s\n", myself);
  printf("         -c  <library> <file>...   create library from files\n");
  printf("         -cv <library> <file>...   ditto, and list modules\n");
  printf("         -t  <library>             show table of contents\n");
  printf("         -tv <library>             ditto, include symbols\n");
  printf("         -x  <library>             extract files from library\n");
  printf("         -xv <library>             ditto, and list modules\n");
  exit(1);
}


int main(int argc, char *argv[]) {
  if (argc < 3) {
    usage(argv[0]);
  }
  if (strcmp(argv[1], "-c") == 0) {
    /* create library from files */
    createLibrary(argv[2], argc - 3, &argv[3], 0);
  } else
  if (strcmp(argv[1], "-cv") == 0) {
    /* create library from files, list module names */
    createLibrary(argv[2], argc - 3, &argv[3], 1);
  } else
  if (strcmp(argv[1], "-t") == 0) {
    /* show table of library contents */
    showTable(argv[2], 0);
  } else
  if (strcmp(argv[1], "-tv") == 0) {
    /* show table of library contents with symbols */
    showTable(argv[2], 1);
  } else
  if (strcmp(argv[1], "-x") == 0) {
    /* extract files from library */
    extractFiles(argv[2], 0);
  } else
  if (strcmp(argv[1], "-xv") == 0) {
    /* extract files from library, list module names */
    extractFiles(argv[2], 1);
  } else {
    usage(argv[0]);
  }
  return 0;
}
