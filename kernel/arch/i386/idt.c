#include <stdio.h>
#include <stdint.h>
#include <kernel/idt.h>
#include <stdbool.h>

#define IDT_MAX_DESCRIPTORS 256 

__attribute__((aligned(0x10))) 
static idt_entry_t idt[IDT_MAX_DESCRIPTORS]; // Create an array of IDT entries; aligned for performance

static idtr_t idtr; // Create an IDTR
extern void idt_load(); // Load the IDTR

static bool vectors[IDT_MAX_DESCRIPTORS]; // Create an array of booleans to keep track of which vectors are in use

void idt_set_descriptor(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entry_t* descriptor = &idt[num];
 
    descriptor->isr_low        = base & 0xFFFF;
    descriptor->kernel_cs      = sel;
    descriptor->attributes     = flags;
    descriptor->isr_high       = base >> 16;
    descriptor->reserved       = 0;
}

void idt_init(void);
void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;
 
    memset(&idt, 0, sizeof(struct idt_entry) * 256); //clear IDT memory

    idt_load(); // load the IDT
}