#ifndef ISR_H
#define ISR_H

#include <stddef.h>
#include <stdint.h>
#include <kernel/tty.h>
#include <stdbool.h>

void exception_handler(void);
#endif