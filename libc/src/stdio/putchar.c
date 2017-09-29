/*
 * libc/stdio/putchar.c
 */

#include <libc/stdio.h>

extern size_t terminal_putchar(char);

size_t putchar(char c)
{
	return terminal_putchar(c);
}
