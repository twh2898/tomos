#ifndef STDIO_H
#define STDIO_H

/* Includes */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Constants */

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

/* Color */

typedef enum _VGA_COLOR {
	VGA_COLOR_BLACK = 0x0,
	VGA_COLOR_BLUE = 0x1,
	VGA_COLOR_GREEN = 0x2,
	VGA_COLOR_CYAN = 0x3,
	VGA_COLOR_RED = 0x4,
	VGA_COLOR_MAGENTA = 0x5,
	VGA_COLOR_BROWN = 0x6,
	VGA_COLOR_LIGHT_GREY = 0x7,
	VGA_COLOR_DARK_GREY = 0x8,
	VGA_COLOR_LIGHT_BLUE = 0x9,
	VGA_COLOR_LIGHT_GREEN = 0xA,
	VGA_COLOR_LIGHT_CYAN = 0xB,
	VGA_COLOR_LIGHT_RED = 0xC,
	VGA_COLOR_LIGHT_MAGENTA = 0xD,
	VGA_COLOR_LIGHT_BROWN = 0xE,
	VGA_COLOR_WHITE = 0xF,
} VGA_COLOR;

static inline uint8_t vga_color(VGA_COLOR fg, VGA_COLOR bg) {
	return fg | (bg << 4);
}

/* Functions */

extern void term_set_cursor(uint8_t loc_x, uint8_t loc_y);
extern void term_set_color(uint8_t new_color);
extern size_t puti(unsigned int num, uint8_t base, bool upper);
extern void term_putc(const char c);
extern size_t puts(const char* str);
extern size_t printf(const char* format, ...);
extern void term_cls(void);

#endif // STDIO_H
