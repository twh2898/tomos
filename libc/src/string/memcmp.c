/*
 * libc/string/memcmp.c
 */
#include <stdint.h>
#include <libc/string.h>

int memcmp(const void* aptr, const void* bptr, size_t n)
{
	const uint8_t* a = (uint8_t*) aptr;
	const uint8_t* b = (uint8_t*) bptr;
	size_t i;
	for (i = 0; i < n; i++, a++, b++)
	{
		if (*a != *b)
			return *a - *b;
	}
	return 0;
}
