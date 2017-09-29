#ifndef TTY_H
#define TTY_H

#include <stddef.h>
#include <stdint.h>

#ifdef _cplusplus
extern "C" {
#endif

extern void terminal_write(char c, uint8_t color, size_t x, size_t y);
extern void terminal_set_color(uint8_t color);
extern void terminal_clear(void);
extern void terminal_enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
extern void terminal_disable_cursor(void);
extern void terminal_set_cursor(size_t x, size_t y);
extern size_t terminal_get_cursor_x(void);
extern size_t terminal_get_cursor_y(void);
extern void terminal_putchar(char c);

#ifdef _cplusplus
}
#endif

#endif // TTY_H
