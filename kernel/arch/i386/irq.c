#include <stdio.h>
#include <stdint.h>
#include <kernel/idt.h>
#include <stdbool.h>
#include <string.h>
#include <kernel/irq.h>
#include <kernel/isr.h>

extern void irq_0();
extern void irq_1();
extern void irq_2();
extern void irq_3();
extern void irq_4();
extern void irq_5();
extern void irq_6();
extern void irq_7();
extern void irq_8();
extern void irq_9();
extern void irq_10();
extern void irq_11();
extern void irq_12();
extern void irq_13();
extern void irq_14();
extern void irq_15();

void *irq_routines[16] = {
    0, 0, 0, 0, 0, 0, 0, 0, // 0-7
    0, 0, 0, 0, 0, 0, 0, 0  // 8-15
};

void irq_install_handler(int irq, void (*handler)(struct regs *r))
{
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}

void irq_remap(void) 
{
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}

void irq_install()
{
    irq_remap();

    idt_set_descriptor(32, (uint32_t)irq_0, 0x08, 0x8E);
    idt_set_descriptor(33, (uint32_t)irq_1, 0x08, 0x8E);
    idt_set_descriptor(34, (uint32_t)irq_2, 0x08, 0x8E);
    idt_set_descriptor(35, (uint32_t)irq_3, 0x08, 0x8E);
    idt_set_descriptor(36, (uint32_t)irq_4, 0x08, 0x8E);
    idt_set_descriptor(37, (uint32_t)irq_5, 0x08, 0x8E);
    idt_set_descriptor(38, (uint32_t)irq_6, 0x08, 0x8E);
    idt_set_descriptor(39, (uint32_t)irq_7, 0x08, 0x8E);
    idt_set_descriptor(40, (uint32_t)irq_8, 0x08, 0x8E);
    idt_set_descriptor(41, (uint32_t)irq_9, 0x08, 0x8E);
    idt_set_descriptor(42, (uint32_t)irq_10, 0x08, 0x8E);
    idt_set_descriptor(43, (uint32_t)irq_11, 0x08, 0x8E);
    idt_set_descriptor(44, (uint32_t)irq_12, 0x08, 0x8E);
    idt_set_descriptor(45, (uint32_t)irq_13, 0x08, 0x8E);
    idt_set_descriptor(46, (uint32_t)irq_14, 0x08, 0x8E);
    idt_set_descriptor(47, (uint32_t)irq_15, 0x08, 0x8E);
}

void irq_handler(struct regs *r)
{
    void (*handler)(struct regs *r);

    /* Find out if we have a custom handler to run for this
    *  IRQ, and then finally, run it */
    handler = irq_routines[r->int_no - 32];
    if (handler)
    {
        handler(r);
    }

    /* EOI to slave controller (if needed) */
    if (r->int_no >= 40)
    {
        outportb(0xA0, 0x20);
    }

    /* EOI to master controller */
    outportb(0x20, 0x20);
}