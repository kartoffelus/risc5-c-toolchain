/*
 * utils.h -- utility functions
 */


#ifndef _UTILS_H_
#define _UTILS_H_


void error(char *fmt, ...);
void warning(char *fmt, ...);

void *memAlloc(unsigned int size);
void memFree(void *p);

unsigned int read4FromTarget(unsigned char *p);
void write4ToTarget(unsigned char *p, unsigned int data);
void conv4FromTargetToNative(unsigned char *p);
void conv4FromNativeToTarget(unsigned char *p);


#endif /* _UTILS_H_ */
