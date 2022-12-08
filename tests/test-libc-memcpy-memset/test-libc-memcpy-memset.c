/*
 * libc memcpy and memset
 */

#include <string.h>
#include <stddef.h>
#include <risc5.h>

void vAssertCalled( const char *fileName, int line );
#define Assert(expr) if (expr) {} else vAssertCalled(__FILE__, __LINE__)

static void clear(char* buf, size_t len);
static void checkMemcpy(char* buf1, char* buf2, size_t len);
static void checkMemset(char* buf, char ch, size_t len);

int main(void) {
    char ref[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    char __dummy1[32];
    char src[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    char __dummy2[32];
    char dest[16] = {0};

    size_t i = 0;
    char *result = NULL;

    // memset full buffer
    clear(dest, 16);
    result = memset(dest, 0, 16);
    checkMemset(dest, 0, 16);
    Assert(result == dest);

    // memcpy full buffer
    result = memcpy(dest, src, 16);
    checkMemcpy(dest, src, 16);
    checkMemcpy(ref, src, 16);
    Assert(result == dest);

    // memset full buffer nonzero
    clear(dest, 16);
    result = memset(dest, 0x5a, 16);
    checkMemset(dest, 0x5a, 16);
    Assert(result == dest);

    // memcpy check for one-off bugs
    result = memcpy(dest+1, src+1, 14);
    checkMemcpy(dest+1, src+1, 14);
    checkMemcpy(ref, src, 16);
    Assert((dest[0] == 0x5a) && (dest[15] == 0x5a));
    Assert(result == dest+1);

    // memset check for one-off bugs
    result = memset(dest+1, 0, 14);
    checkMemset(dest+1, 0, 14);
    Assert((dest[0] == 0x5a) && (dest[15] == 0x5a));
    Assert(result == dest+1);

    // memset check for alignment bugs
    while (i < 16)
    {
        size_t j = i;
        while (j < 16)
        { 
            clear(dest, 16);
            result = memset(dest+i, 0x55, j-i);
            checkMemset(dest, 0, i);
            checkMemset(dest+i, 0x55, j-i);
            checkMemset(dest+j, 0, 15-j);
            Assert(result == dest+i);
            ++j;
        }
        ++i;
    }

    *RISC5_IO_SIM_SHUTDOWN = 0x0;
}

static void clear(char* buf, size_t len)
{
    size_t i = 0;
    for (i = 0; i < len; i++)
    {
        buf[i] = 0;
    }
}

static void checkMemcpy(char* buf1, char* buf2, size_t len)
{
    size_t i = 0;
    for (i = 0; i < len; i++)
    {
        Assert(buf1[i] == buf2[i]);
    }
}

static void checkMemset(char* buf, char ch, size_t len)
{
    size_t i = 0;
    for (i = 0; i < len; i++)
    {
        Assert(buf[i] == ch);
    }
}

void vAssertCalled( const char *fileName, int line )
{
    RISC5_DISABLE_INTERRUPTS();
	( void ) fileName;
	( void ) line;
    *RISC5_IO_SIM_SHUTDOWN = 0x1;
}
