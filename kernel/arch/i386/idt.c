#include <stdio.h>
#include <stdint.h>
#include <kernel/idt.h>
#include <stdbool.h>
#include <string.h>

#define IDT_MAX_DESCRIPTORS 256 

__attribute__((aligned(0x10))) 
struct idt_entry_t idt[IDT_MAX_DESCRIPTORS]; // Create an array of IDT entries; aligned for performance

struct idtr_t idtp; // Create an IDTR
extern void idt_load(); // Load the IDTR

void idt_set_descriptor(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].isr_low        = base & 0xFFFF;
    idt[num].kernel_cs      = sel;
    idt[num].attributes     = flags;
    idt[num].isr_high       = base >> 16;
    idt[num].reserved       = 0;
}

void idt_init(void);
void idt_init() {
    idtp.base = (uintptr_t)&idt[0];
    idtp.limit = (uint16_t)sizeof(struct idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;
 
    memset(&idt, 0, sizeof(struct idt_entry_t) * 256); //clear IDT memory

    idt_load(); // load the IDT
}