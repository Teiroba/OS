#ifndef TIMER_H
#define TIMER_H


#include <stdio.h>
#include <stdint.h>
#include <kernel/idt.h>
#include <stdbool.h>
#include <string.h>

void timer_handler(struct regs *r);
void timer_install(void);
void timer_phase(int hz);
void timer_wait(int ticks);

