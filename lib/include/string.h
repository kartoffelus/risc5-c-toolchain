/*
 * string.h -- string operations
 */

#ifndef __STRING_H
#define __STRING_H

/*********************************************
 * NOTE: This is a not a full implementation 
 *       of string.h, just the bare minimum
 *********************************************/

#include <stddef.h>

void* memcpy( void *dest, const void *src, size_t count );

void *memset( void *dest, int ch, size_t count );

#endif //__STRING_H
