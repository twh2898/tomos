/*
 * kernel.c
 */

#include <stdio.h>

void kmain(void)
{
	const char *str = "my super first kernel";
	
	cls();
	puts(str);
	puts("\n");
	puts("Hi");
	
	return;
}
