#include <stdio.h>
#include <stdint.h>
#include <kernel/idt.h>
#include <stdbool.h>
#include <string.h>
#include <kernel/irq.h>
#include <kernel/isr.h>
#include <kernel/keyboard.h>


unsigned char keymap[128] =
{
    0,  27, '&', 'é', '"', '\'', '(', '-', 'è', '_',	/* 9 */
  'ç', 'à', ')', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'a', 'z', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n',	/* Enter key */
    0,			/* 29   - LControl */
  'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm',	/* 39 */
 'ù', '²',   0,		/* Left shift */
 '*', 'w', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  ',', ';', ':', '!',   0,				/* Right shift */
  0, /* Numpad * */
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Numpad 7 */
    0,	/* Numpad 8 */
    0,	/* Numpad 9 */
    0, /* Numpad - */
    0,	/* Numpad 4 */
    0,  /* Numpad 5 */
    0,	/* Numpad 6 */
    0,  /* Numpad + */
    0,	/* Numpad 1*/
    0,	/* Numpad 2 */
    0,	/* Numpad 3 */
    0,	/* Numpad 0 */
    0,	/* Numpad . */
    0,  /* Print screen*/
    0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r)
{
    unsigned char scancode;

    /* Read from the keyboard's data buffer */
    scancode = inportb(0x60);

    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (scancode & 0x80)
    {
        //Magic happens
    }
    else
    {
        /* Here, a key was just pressed.*/
        printf("%c", keymap[scancode]);
    }
}

void keyboard_install()
{
    irq_install_handler(1, keyboard_handler);
}

