/*
 * stdio.c
 */

#include <stdarg.h>
#include <kernel/tty.h>

#define VIDPTR ((uint8_t*)0xb8000)
#define VID_LOC(x, y) (x + y * SCREEN_WIDTH)
#define MAX_VID_LOC (SCREEN_WIDTH * SCREEN_HEIGHT)

uint16_t vid_loc = 0;
uint8_t curs_x = 0;
uint8_t curs_y = 0;
uint8_t color = 0x07;	// light gray on black

static inline void write_character(char c, uint8_t attr, uint16_t loc)
{
	loc = loc << 1;
	VIDPTR[loc] = c;
	VIDPTR[loc + 1] = attr;
}

void term_putc(const char c)
{
	if (c == '\n')
	{
		curs_y++;
		curs_x = 0;
	}
	else
	{
		write_character(c, color, VID_LOC(curs_x++, curs_y));
	}

	if (curs_x >= SCREEN_WIDTH)
	{
		curs_x = 0;
		curs_y++;
	}

	if (curs_y >= SCREEN_HEIGHT)
	{
		curs_y = 0;
	}
}

static inline char to_upper_hex(uint8_t num)
{
	if (num < 10)
	{
		return '0' + num;
	}
	return 'A' + num - 10;
}

static inline char to_lower_hex(uint8_t num)
{
	if (num < 10)
	{
		return '0' + num;
	}
	return 'a' + num - 10;
}

size_t puti(unsigned int num, uint8_t base, bool upper)
{
	if (!num)
	{
		term_putc('0');
		return 1;
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

	size_t len = digits;

	while (digits--)
	{
		if (upper)
		{
			term_putc(to_upper_hex(res % base));
		}
		else
		{
			term_putc(to_lower_hex(res % base));
		}
		res = res / base;
	}

	return len;
}

void term_set_cursor(uint8_t loc_x, uint8_t loc_y)
{
	if (loc_x >= 0 && loc_x < SCREEN_WIDTH)
	{
		curs_x = loc_x;
	}

	if (loc_y >= 0 && loc_y < SCREEN_HEIGHT)
	{
		curs_y = loc_y;
	}
}

void term_set_color(uint8_t new_color)
{
	color = new_color;
}

size_t puts(const char* str)
{
	size_t len = 0;
	while (*str)
	{
		term_putc(*str);
		str++;
		len++;
	}
	return len;
}

void term_cls()
{
	uint16_t j = 0;
	while(j < MAX_VID_LOC)
	{
		write_character(' ', 0x07, j++);
	}
}

static inline bool is_digit(char c)
{
	return '0' <= c && c <= '9';
}

static inline bool is_alpha(char c)
{
	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

static inline bool is_flag(char c)
{
	return c == '-' || c == '+' || c == ' ' || c == '0';
}

size_t printf(const char* format, ...)
{
	va_list parameters;
	va_start(parameters, format);

	size_t written = 0;
	while (*format)
	{
		if (format[0] == '%')
		{
			switch(format[1])
			{
				case 'd':
				{
					int arg = va_arg(parameters, int);
					if (arg < 0)
					{
						term_putc('-');
						arg = -arg;
					}
					written += puti((unsigned int) arg, 10, false);
				}
				break;
				case 'u':
				{
					unsigned int arg = va_arg(parameters, unsigned int);
					written += puti(arg, 10, false);
				}
				break;
				case 'o':
				{
					int arg = va_arg(parameters, int);
					written += puti(arg, 8, false);
				}
				break;
				case 'x':
				{
					int arg = va_arg(parameters, int);
					written += puti(arg, 16, false);
				}
				break;
				case 'X':
				{
					int arg = va_arg(parameters, int);
					written += puti(arg, 16, true);
				}
				break;
				case 'f':
					break;
				case 'c':
				{
					char arg = (char) va_arg(parameters, int);
					term_putc(arg);
					written++;
				}
				break;
				case 's':
				{
					const char* arg = va_arg(parameters, const char*);
					written += puts(arg);
				}
				break;
				case 'p':
				{
					const void* arg = va_arg(parameters, const void*);
					written += puti((int) arg, 16, true);
				}
				break;
				case 'n':
				{
					int* arg = va_arg(parameters, int*);
					*arg = written;
				}
				break;
				case 'b':
				{
					int arg = va_arg(parameters, int);
					if (arg)
					{
						written += puts("true");
					}
					else
					{
						written += puts("false");
					}
				}
				break;
				case '%':
					term_putc('%');
					written++;
					break;
				default:
					break;
			}
			format++;
		}
		else
		{
			term_putc(*format);
			written++;
		}

		format++;
	}

	va_end(parameters);
	return written;
}
