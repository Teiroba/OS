#ifndef _KERNEL_GDT_ENTRIES_H
#define _KERNEL_GDT_ENTRIES_H



void gdt_init(void);
void encodeGdtEntry(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
void set_kernel_stack(uint32_t stack);


#endif