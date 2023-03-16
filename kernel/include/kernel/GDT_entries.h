#ifndef _KERNEL_GDT_ENTRIES_H
#define _KERNEL_GDT_ENTRIES_H

#include <stddef.h>
#include <stdint.h>

void gdt_init(void);
void encode_gdt_entry(uint8_t *target, uint32_t base, uint32_t limit, uint16_t flag);

#endif
