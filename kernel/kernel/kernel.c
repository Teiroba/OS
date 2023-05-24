#include <stdio.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/GDT_entries.h>
#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/irq.h>
#include <kernel/timer.h>
#include <kernel/keyboard.h>

void kernel_early_main(void){
	gdt_init();
	idt_init();
	set_isrs();
	irq_install();
	__asm__ __volatile__ ("sti");
	timer_install();
	keyboard_install();
}


void kernel_main(void) {
	terminal_initialize();
	printf("Hello, kernel World!\nThis is a test\n");
	printf("Testing %s %s %cnd again.\n", "again", "and again", 'a');
	while(1);
}
