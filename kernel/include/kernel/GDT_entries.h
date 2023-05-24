#ifndef _KERNEL_GDT_ENTRIES_H
#define _KERNEL_GDT_ENTRIES_H

#include <stddef.h>
#include <stdint.h>

// The GDT entry structure
struct gdt_entry
{
    unsigned short limit_low;           // The lower 16 bits of the limit.
    unsigned short base_low;            // The lower 16 bits of the base.
    unsigned char  base_middle;         // The next 8 bits of the base.
    unsigned char  access;              // Access flags, determine what ring this segment can be used in.
    unsigned char  granularity;
    unsigned char  base_high;           // The last 8 bits of the base.

} __attribute__((packed));

// The GDT pointer structure
struct gdp
{
  unsigned short limit;
  unsigned int base;
} __attribute__((packed));
void gdt_init(void);
void encodeGdtEntry(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);

#endif
