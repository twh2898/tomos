#ifndef STDIO_H
#define STDIO_H

/* Includes */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <kernel/vga.h>

/* Constants */

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

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
 * Clears all characters from the console using the current foreground
 * and background colors.
 */
extern void term_cls(void);

#endif // STDIO_H
