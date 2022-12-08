#include <string.h>

void* memcpy( void *dest, const void *src, size_t count )
{
    while(count--)
    {
        ((unsigned char*)dest)[count] = ((const unsigned char*)src)[count];
    }
    return dest;
}

void* memset( void *dest, int ch, size_t count ){
    unsigned char *destB = (unsigned char*)dest;
    
    if (count >= 8)
    {
        unsigned int ch4 = (unsigned int)((unsigned char)ch << 24) | (unsigned int)((unsigned char)ch << 16) | (unsigned int)((unsigned char)ch << 8) | (unsigned char)ch;

        unsigned int *destW;
        size_t countW;

        /* set 0-3 bytes to align to 4 byte boundary */
        /* x & 3 == x % 4 */
        while((((size_t)destB) & 3)) 
        {   
            *(destB++) = (unsigned char)ch;
            --count;
        }

        /* set 4 bytes at once */
        /* x >> 2 == x / 4 */
        destW = (unsigned int*)destB;
        countW = count >> 2;
        while(countW--){
            *(destW++) = ch4;
        }

        /* leftover bytes*/
        count = count & 3;
        destB = (unsigned char*)destW;
    }
    
    /* set leftover bytes */
    while(count--){
        *(destB++) = (unsigned char)ch;
    }

    return dest;
}
