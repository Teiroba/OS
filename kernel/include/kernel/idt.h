#ifndef IDT_H
#define IDT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


struct idt_entry_t {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t     reserved;     // Set to zero
	uint8_t     attributes;   // Type and attributes
	uint16_t    isr_high;     // The higher 16 bits of the ISR's address
} __attribute__((packed));


struct idtr_t{
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed));

void idt_set_descriptor(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_init(void);
#endif