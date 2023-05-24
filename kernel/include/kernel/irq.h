#ifndef IRQ_H
#define IRQ_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/isr.h>
#include <stdio.h>
#include <kernel/idt.h>
#include <string.h>

void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void irq_remap(void);
void irq_install(void);
void irq_handler(struct regs *r);

#endif