#include <stdio.h>
#include <stdint.h>
#include <kernel/isr.h>
#include <kernel/tty.h>
#include <stdbool.h>

__attribute__((noreturn))
void exception_handler(void);
void exception_handler() {
    printf("Exception!\n");
   __asm__ volatile ("cli; hlt"); // Completely hangs the computer

}