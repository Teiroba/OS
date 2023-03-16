#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/GDT_entries.h>

void kernel_main(void) {
	terminal_initialize();
	printf("Hello, kernel World!\nThis is a test\n");
	printf("Testing %s %s %cnd again.\n", "again", "and again", 'a');
	
}
