/*
 * libc/stdio/puts.c
 */

 #include <libc/stdio.h>

size_t puts(const char* str)
{
	while (*str)
		putchar(*str++);
}
