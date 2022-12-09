/*
 * sfs.h -- simple file system constants and types
 */


#ifndef _SFS_H_
#define _SFS_H_


#define SECTOR_SIZE	512		/* in bytes */

#define LOADER		254		/* in sectors */
#define RESERVED	(2 + LOADER)	/* in sectors */

#define SFS_MAGIC_1	0x2DD43AE8	/* SFS identification */
#define SFS_MAGIC_2	0x92321AA0	/* SFS identification */


typedef enum { false = 0, true = 1 } Bool;

typedef unsigned int Word;
typedef unsigned short Half;
typedef unsigned char Byte;

typedef struct {
  char name[20];	/* name of file */
  Word start;		/* start sector */
  Word size;		/* size in bytes */
} TocEntry;

typedef struct {
  TocEntry toc[16];	/* TOC */
  Word magic_1;		/* must be SFS_MAGIC_1 */
  Word freeStart;	/* start of free space, sector */
  Word freeSize;	/* size of free space, sectors */
  Word padding[12];
  Word magic_2;		/* must be SFS_MAGIC_2 */
} TocRecord;


#endif /* _SFS_H_ */
