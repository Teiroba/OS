#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);
unsigned char inportb (unsigned short _port);
void outportb (unsigned short _port, unsigned char _data);


#ifdef __cplusplus
}
#endif

#endif
