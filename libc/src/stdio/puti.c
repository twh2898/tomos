/*
 * libc/stdio/puti.c
 */
#include <libc/stdio.h>

static char to_upper_hex(uint8_t num)
{
	if (num < 10)
	{
		return '0' + num;
	}
	return 'A' + num - 10;
}

static char to_lower_hex(uint8_t num)
{
	if (num < 10)
	{
		return '0' + num;
	}
	return 'a' + num - 10;
}

size_t puti(unsigned int num, uint32_t base, bool isSigned, bool useUpper)
{
	if (!num)
	{
		putchar('0');
		return 1;
	}

	size_t len = 0;

	if (isSigned)
	{
		int tmpNum = num;
		if (tmpNum < 0)
		{
			len = 1;
			tmpNum = -tmpNum;
		}
		num = (unsigned int) tmpNum;
	}

	int digits = 0;
	int res = 0;
	while (num)
	{
		res = res * base;
		res = res + (num % base);
		num = num / base;
		digits++;
	}

	len += digits;

	while (digits--)
	{
		if (useUpper)
		{
			putchar(to_upper_hex(res % base));
		}
		else
		{
			putchar(to_lower_hex(res % base));
		}
		res = res / base;
	}

	return len;
}
