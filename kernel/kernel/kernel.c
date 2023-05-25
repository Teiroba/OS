#include <stdio.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/GDT_entries.h>
#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/irq.h>
#include <kernel/timer.h>
#include <kernel/keyboard.h>
#include <limits.h>
#include <kernel/filesystem.h>

extern void jump_usermode();
extern void gdt_init();



void kernel_early_main(void){
	gdt_init();
	idt_init();
	set_isrs();
	irq_install();
	__asm__ __volatile__ ("sti");
	timer_install();
	keyboard_install();
	init_filesystem();
}


void kernel_main(void) {
	terminal_initialize();
	printf("Welcome to ")

	/*printf("Hello, kernel World!\nThis is a test\n");
	printf("Testing %s %s %cnd again.\n", "again", "and again", 'a');

	int* xs = malloc(100 * sizeof(int));
	printf("Allocated 400 bytes.\n");
	for (int i = 0; i < 100; i++) {
		xs[i] = i;
	}
	printf("Wrote integers 0 to 99 to allocated memory.\n");
	printf("Reading allocated memory: ");
	for (int i = 0; i < 100; i++) {
		printf("%d ", xs[i]);
	}
	printf("\n");*/

	while (1) {}
	//jump_usermode();

	// while (1) {}


}
