#ifndef STDIO_H
#define STDIO_H

#include <stdint.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

typedef enum _VGA_COLOR {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} VGA_COLOR;

extern void term_set_cursor(uint8_t loc_x, uint8_t loc_y);
extern void term_putc(const char c);
extern void term_puts(const char* str);
extern void term_cls(void);

#endif // STDIO_H
