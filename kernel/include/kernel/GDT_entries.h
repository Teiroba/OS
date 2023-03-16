#ifndef _KERNEL_GDT_ENTRIES_H
#define _KERNEL_GDT_ENTRIES_H

#include <stddef.h>
#include <stdint.h>

// The GDT entry structure
struct gdt_entry
{
    uint8_t entry0;
    uint8_t entry1;
    uint8_t entry2;
    uint8_t entry3;
    uint8_t entry4;
    uint8_t entry5;
    uint8_t entry6;
    uint8_t entry7;

};

// The GDT pointer structure
struct gdp
{
  uint32_t limit;
  uint32_t base;
};
void gdt_init(void);
void encodeGdtEntry(uint8_t target, uint32_t base, uint32_t limit, uint16_t flag);

#endif
