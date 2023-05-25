#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <stdio.h>

#include "vga.h"






void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;
 
	outportb(0x3D4, 0x0F);
	outportb(0x3D5, (uint8_t) (pos & 0xFF));
	outportb(0x3D4, 0x0E);
	outportb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll()
{
	for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			const size_t index2 = (y + 1) * VGA_WIDTH + x;
			terminal_buffer[index] = terminal_buffer[index2];
		}
	}
	for (size_t x = 0; x < VGA_WIDTH; x++) {
		const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
		terminal_buffer[index] = vga_entry(' ', terminal_color);
	}
	terminal_row = VGA_HEIGHT - 1;
	terminal_column = 0;
	update_cursor(terminal_column, terminal_row);

}

void terminal_putchar(char c) {
	unsigned char uc = c;
	if (uc == '\n') {
		terminal_column = 0;
		terminal_row = terminal_row + 1;
		if (terminal_row == VGA_HEIGHT)
			terminal_scroll();
		update_cursor(terminal_column, terminal_row);
		return;
	}
	if (uc == '\t') {
		terminal_column = terminal_column + 4;
		if (terminal_column >= VGA_WIDTH) {
			terminal_column = 0;
			terminal_row = terminal_row + 1;
			if (terminal_row == VGA_HEIGHT)
				terminal_scroll();
		}
		update_cursor(terminal_column, terminal_row);
		return;
	}
	if (uc == '\r') {
		terminal_column = 0;
		update_cursor(terminal_column, terminal_row);
		return;
	}
	if (uc == '\b') {
		if (terminal_column > 0) {
			terminal_column = terminal_column - 1;
			terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
			update_cursor(terminal_column, terminal_row);
		}
		if (terminal_column == 0 && terminal_row > 0) {
			terminal_column = VGA_WIDTH - 1;
			terminal_row = terminal_row - 1;
			terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
			update_cursor(terminal_column, terminal_row);
		}
		return;
	}
	terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_scroll();
	}
	update_cursor(terminal_column, terminal_row);
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}
