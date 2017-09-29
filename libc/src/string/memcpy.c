/*
 * libc/string/memcpy.c
 */
#include <stdint.h>
#include <libc/string.h>

void* memcpy(void* __restrict destptr, const void* __restrict srcptr, size_t n)
{
	uint8_t* dest = (uint8_t*) destptr;
	const uint8_t* src = (uint8_t*) srcptr;
	size_t i;
	for (i = 0; i < n; i++, dest++, src++)
	{
		*dest = *src;
	}
	return destptr;
}
