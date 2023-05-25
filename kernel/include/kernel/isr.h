#ifndef ISR_H
#define ISR_H

#include <stddef.h>
#include <stdint.h>
#include <kernel/tty.h>
#include <stdbool.h>

//state of the registers when an interrupt occurs
struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

void set_isrs(void);
void exception_handler(struct regs *r);
#endif