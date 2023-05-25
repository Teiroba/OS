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
#include <kernel/malloc.h>

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
	
}


void kernel_main(void) {
	terminal_initialize();
	printf("Hello, kernel World!\nThis is a test\n");
	printf("Testing %s %s %cnd again.\n", "again", "and again", 'a');
	while(1);

	printf("Printing 0: %d.\n", 0);
	printf("Printing 1: %d.\n", 1);
	printf("Printing 2: %d.\n", 2);
	printf("Printing 9: %d.\n", 9);
	printf("Printing 10: %d.\n", 10);
	printf("Printing 11: %d.\n", 11);
	printf("Printing -1: %d.\n", -1);
	printf("Printing -2: %d.\n", -2);
	printf("Printing -9: %d.\n", -9);
	printf("Printing -10: %d.\n", -10);
	printf("Printing -11: %d.\n", -11);
	printf("Printing INT_MAX: %d.\n", INT_MAX);
	printf("Printing INT_MIN: %d.\n", INT_MIN);
	printf("Printing 1234: %d.\n", 1234);
	printf("Printing -1234: %d.\n", -1234);

	// int* xs = sbrk(100 * sizeof(int));
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
	printf("\n");
	jump_usermode();

	// while (1) {}


}
