/*
 * libc/string/memmove.c
 */
#include <stdint.h>
#include <libc/string.h>

void* memmove(void* destptr, const void* srcptr, size_t n)
{
	uint8_t* dest = (uint8_t*) destptr;
	const uint8_t* src = (uint8_t*) srcptr;
	size_t i;
	if (dest < src)
	{
		for (i = 0; i < n; i++, dest++, src++)
			*dest = *src;
	}
	else
	{
		dest = dest + n;
		src = src + n;
		for (i = 0; i < n; i++, dest--, src--)
			*dest = *src;
	}
	return destptr;
}
