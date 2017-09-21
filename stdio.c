/*
 * stdio.c
 */
 
#include <stdio.h>

void puts(const char* str)
{
	while (*str)
	{
		str++;
	}
}
