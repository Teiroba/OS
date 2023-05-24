#include <stdio.h>
#include <stdint.h>
#include <kernel/idt.h>
#include <stdbool.h>
#include <string.h>
#include <kernel/irq.h>
#include <kernel/isr.h>
#include <kernel/timer.h>

int timer_ticks = 0;

/* Handles the timer.*/

void timer_handler(struct regs *r)
{
    timer_ticks++;

    //Testing if the clock works

    /*if (timer_ticks % 100 == 0)
    {
        printf("One second has passed\n");
    }*/
}

void timer_phase(int hz) //Updates the timer frequency
{
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    outportb(0x43, 0x36);             /* Set our command byte 0x36 */
    outportb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    outportb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

void timer_install()
{
    timer_phase(100); //Updates the timer frequency to 100Hz 
    /* Installs 'timer_handler' to IRQ0 */
    irq_install_handler(0, timer_handler);
}

void timer_wait(int ticks)
{
    unsigned long eticks;
    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}