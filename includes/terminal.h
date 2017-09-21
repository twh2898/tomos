#ifndef STDIO_H
#define STDIO_H

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

extern void term_set_cursor(char loc_x, char loc_y);
extern void term_putc(const char c);
extern void term_puts(const char* str);
extern void term_cls(void);

#endif // STDIO_H
