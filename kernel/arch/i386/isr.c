#include <stdio.h>
#include <stdint.h>
#include <kernel/isr.h>
#include <kernel/idt.h>
#include <kernel/tty.h>
#include <stdbool.h>

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void set_isrs()
{
    idt_set_descriptor(0, (unsigned)isr0, 0x08, 0x8E);
    idt_set_descriptor(1, (unsigned)isr1, 0x08, 0x8E);
    idt_set_descriptor(2, (unsigned)isr2, 0x08, 0x8E);
    idt_set_descriptor(3, (unsigned)isr3, 0x08, 0x8E);
    idt_set_descriptor(4, (unsigned)isr4, 0x08, 0x8E);
    idt_set_descriptor(5, (unsigned)isr5, 0x08, 0x8E);
    idt_set_descriptor(6, (unsigned)isr6, 0x08, 0x8E);
    idt_set_descriptor(7, (unsigned)isr7, 0x08, 0x8E);
    idt_set_descriptor(8, (unsigned)isr8, 0x08, 0x8E);
    idt_set_descriptor(9, (unsigned)isr9, 0x08, 0x8E);
    idt_set_descriptor(10, (unsigned)isr10, 0x08, 0x8E);
    idt_set_descriptor(11, (unsigned)isr11, 0x08, 0x8E);
    idt_set_descriptor(12, (unsigned)isr12, 0x08, 0x8E);
    idt_set_descriptor(13, (unsigned)isr13, 0x08, 0x8E);
    idt_set_descriptor(14, (unsigned)isr14, 0x08, 0x8E);
    idt_set_descriptor(15, (unsigned)isr15, 0x08, 0x8E);
    idt_set_descriptor(16, (unsigned)isr16, 0x08, 0x8E);
    idt_set_descriptor(17, (unsigned)isr17, 0x08, 0x8E);
    idt_set_descriptor(18, (unsigned)isr18, 0x08, 0x8E);  
    idt_set_descriptor(19, (unsigned)isr19, 0x08, 0x8E);
    idt_set_descriptor(20, (unsigned)isr20, 0x08, 0x8E);
    idt_set_descriptor(21, (unsigned)isr21, 0x08, 0x8E);
    idt_set_descriptor(22, (unsigned)isr22, 0x08, 0x8E);
    idt_set_descriptor(23, (unsigned)isr23, 0x08, 0x8E);
    idt_set_descriptor(24, (unsigned)isr24, 0x08, 0x8E);
    idt_set_descriptor(25, (unsigned)isr25, 0x08, 0x8E);
    idt_set_descriptor(26, (unsigned)isr26, 0x08, 0x8E);
    idt_set_descriptor(27, (unsigned)isr27, 0x08, 0x8E);
    idt_set_descriptor(28, (unsigned)isr28, 0x08, 0x8E);
    idt_set_descriptor(29, (unsigned)isr29, 0x08, 0x8E);
    idt_set_descriptor(30, (unsigned)isr30, 0x08, 0x8E);
    idt_set_descriptor(31, (unsigned)isr31, 0x08, 0x8E);
}

unsigned char *exception_messages[] =
{
    "Division by Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS Exception",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check Exception",
    "Machine Check Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};


void exception_handler(struct regs *r) {
    if (r -> int_no<32)
    {
        printf("%s Exception. System Halted!\n", exception_messages[r->int_no]);
        for (;;);}

}