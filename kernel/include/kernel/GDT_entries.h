#ifndef _KERNEL_GDT_ENTRIES_H
#define _KERNEL_GDT_ENTRIES_H

#include <stddef.h>
#include <stdint.h>

// The GDT entry structure
struct gdt_entry
{
  uint64_t
} __attribute__((packed));

// The GDT pointer structure
struct gdt_ptr
{
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

void gdt_init(void);
void encodeGdtEntry(uint8_t *target, uint32_t base, uint32_t limit, uint16_t flag);

#endif
