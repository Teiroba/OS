#include <stdio.h>
#include <limits.h>

#include <kernel/tty.h>
#include <kernel/GDT_entries.h>

void kernel_early_main(void){
	gdt_init();
}


void kernel_main(void) {
	terminal_initialize();
	printf("Hello, kernel World!\nThis is a test\n");
	printf("Testing %s %s %cnd again.\n", "again", "and again", 'a');

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

	
}
