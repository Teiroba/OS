#include <stdio.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/GDT_entries.h>
#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/irq.h>

void kernel_early_main(void){
	gdt_init();
	idt_init();
	set_isrs();
	irq_install();
	__asm__ __volatile__ ("sti");
}


void kernel_main(void) {
	terminal_initialize();
	printf("Hello, kernel World!\nThis is a test\n");
	printf("Testing %s %s %cnd again.\n", "again", "and again", 'a');
}
