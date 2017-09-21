#ifndef STDIO_H
#define STDIO_H

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

extern void set_cursor(char loc_x, char loc_y);
extern void putc(const char c);
extern void puts(const char* str);
extern void cls(void);

#endif // STDIO_H
