/*
 * libc/string/memset.c
 */
#include <stdint.h>
#include <libc/string.h>

void* memset(void* bufptr, int value, size_t n)
{
	uint8_t* buf = (uint8_t*) bufptr;
	size_t i;
	for (i = 0; i < n; i++)
		*buf++ = (uint8_t) value;
	return bufptr;
}
