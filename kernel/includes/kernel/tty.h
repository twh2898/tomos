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

/**
 * Set the cursor location in the terminal.
 * Pre:
 *     loc_x and loc_y must be withing the screen bounds defined by
 *     SCREEN_WIDTH and SCREEN_HEIGHT
 */
extern void term_set_cursor(uint8_t loc_x, uint8_t loc_y);

extern uint8_t get_cursor_x(void);
extern uint8_t get_cursor_y(void);

extern void term_enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
extern void term_disable_cursor();

/**
 * Set the terminal foreground and background colors. These colors can
 * be choosen using the helper function vga_color(fg, bg). This color
 * will not update the entire terminal, but will be use for future print
 * statements.
 */
extern void term_set_color(uint8_t new_color);

/**
 * Print a number to the terminal with the specified base. The parameter
 * upper defines weather upper case letters should be used for
 * bases > 10.
 * Pre:
 *     base is a valid number between 0 and 16.
 * Return:
 *     the number of characters written to the terminal
 */
extern size_t puti(unsigned int num, uint8_t base, bool upper);

/**
 * Put a single character at the current cursor location in the terminal
 * and advance the cursor by 1.
 */
extern void term_putc(const char c);

/**
 * Print a string to the terminal starting at the current cursor
 * location. If the string passes the right edge of the terminal, it
 * will be wrapped to the next line starting at the 0'th character. If
 * the next line is off the bottom edge of the terminal, the contents of
 * the screen will be shifted up by 1 line.
 */
extern size_t puts(const char* str);

/**
 * Print a formated string to the console. Much like the regular C
 * printf, this takes a list of parameters as arguments. Unlike the
 * regular C printf, no formatting is currenlty available, and the only
 * flags agailable are %, d, u, o, x, X, c, s, n, p, b.
 */
extern size_t printf(const char* format, ...);

/**
 * Clears all characters from the console using the current foreground
 * and background colors.
 */
extern void term_cls(void);

#endif // STDIO_H
