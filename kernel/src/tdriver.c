/*
 * stdio.c
 */

#include <stdarg.h>
#include <stdint.h>
#include <kernel/asm.h>
#include <kernel/tty.h>

#define VIDPTR ((uint8_t*)0xb8000)
#define MAX_VID_LOC (SCREEN_WIDTH * SCREEN_HEIGHT)

uint8_t curs_x = 0;
uint8_t curs_y = 0;
uint8_t color = 0x07;	// light gray on black

static inline uint16_t vid_loc(uint8_t x, uint8_t y)
{
	return x + SCREEN_WIDTH * y;
}

static inline void write_character(char c, uint8_t attr, uint16_t loc)
{
	loc = loc << 1;
	VIDPTR[loc] = c;
	VIDPTR[loc + 1] = attr;
}

static inline void update_cursor()
{
	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned char) vid_loc(curs_x, curs_y) & 0xFF);
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char) (vid_loc(curs_x, curs_y) >> 8) & 0xFF);
}

void term_enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
        outb(0x3D4, 0x0A);
        outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

        outb(0x3D4, 0x0B);
        outb(0x3D5, (inb(0x3E0) & 0xE0) | cursor_end);
}

void term_disable_cursor()
{
	outb(0x3D4, 0x0A); // LOW cursor shape port to vga INDEX register
	outb(0x3D5, 0x3f); //bits 6-7 must be 0 , if bit 5 set the cursor is disable  , bits 0-4 controll the cursor shape .
}

uint8_t get_cursor_x(void)
{
	return curs_x;
}

uint8_t get_cursor_y(void)
{
	return curs_y;
}

static inline void shift_lines()
{
	int x, y;
	for (y = 0; y < SCREEN_HEIGHT - 1; y++)
	{
		for (x = 0; x < SCREEN_WIDTH; x++)
		{
			VIDPTR[vid_loc(x, y) << 1] = VIDPTR[vid_loc(x, y + 1) << 1];
			VIDPTR[(vid_loc(x, y) << 1) + 1] = VIDPTR[(vid_loc(x, y + 1) << 1) + 1];
		}
	}
	for (y = SCREEN_HEIGHT - 1; y < SCREEN_HEIGHT; y++)
	{
		for (x = 0; x < SCREEN_WIDTH; x++)
		{
			write_character(' ', color, vid_loc(x, y));
		}
	}
}

size_t terminal_putchar(char c)
{
	term_putc(c);
	return 1;
}

void term_putc(const char c)
{
	switch (c)
	{
		case '\n':
		{
			curs_y++;
			curs_x = 0;
		}
		break;
		case '\b':
		{
			if (curs_x == 0)
			{
				if (curs_y == 0)
				{
					curs_x = 1;
				}
				else
				{
					curs_x = SCREEN_WIDTH;
					curs_y--;
				}
			}
			curs_x--;

			write_character(' ', color, vid_loc(curs_x, curs_y));
		}
		break;
		case '\t':
		{
			write_character(' ', color, vid_loc(curs_x++, curs_y));
			while (curs_x % 4 != 0)
			{
				if (curs_x >= SCREEN_WIDTH)
				{
					break;
				}
				write_character(' ', color, vid_loc(curs_x++, curs_y));
			}
		}
		break;
		default:
			write_character(c, color, vid_loc(curs_x++, curs_y));
		}

	if (curs_x >= SCREEN_WIDTH)
	{
		curs_x = 0;
		curs_y++;
	}

	if (curs_y >= SCREEN_HEIGHT)
	{
		curs_y = SCREEN_HEIGHT - 1;
		shift_lines();
	}

	update_cursor();
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
		write_character(' ', color, j++);
	}

	curs_x = 0;
	curs_y = 0;
	update_cursor();
}

static size_t numSizeu(uint32_t num, uint32_t base)
{
	size_t digits = 0;
	while (num)
	{
		num /= base;
	}
	return digits;
}

static size_t numSizei(int32_t num, int32_t base)
{
	size_t digits = 0;
	while (num)
	{
		num /= base;
	}
	return digits;
}

static size_t strlen(const char* str)
{
	size_t count = 0;
	while (*str++);
		count++;
	return count;
}

size_t printf(const char* format, ...)
{
	va_list parameters;
	va_start(parameters, format);

	size_t written = 0;
	while (*format)
	{
		if (*format == '%')
		{
			int width = 0;
			bool lead_zero = format[1] == '0';
			bool left_align = format[1] == '-';

			if (left_align)
				lead_zero = false;

			start_format:
			format++;
			switch(*format)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					width *= 10;
					width += (*format - '0');
				case '-':
					goto start_format;
				case 'd':
				{
					int arg = va_arg(parameters, int);
					size_t numWidth = numSizei(arg, 10);
					if (arg < 0)
					{
						term_putc('-');
						arg = -arg;
						numWidth++;
					}
					if (width > numWidth && !left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							term_putc(lead_zero ? '0' : ' ');
							written++;
						}
					}
					written += puti((unsigned int) arg, 10, false);
					if (width > numWidth && left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							term_putc(' ');
							written++;
						}
					}
				}
				break;
				case 'u':
				{
					unsigned int arg = va_arg(parameters, unsigned int);
					size_t numWidth = numSizeu(arg, 10u);
					if (width > numWidth && !left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							term_putc(lead_zero ? '0' : ' ');
							written++;
						}
					}
					written += puti(arg, 10, false);
					if (width > numWidth && left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							term_putc(' ');
							written++;
						}
					}
				}
				break;
				case 'o':
				{
					unsigned int arg = va_arg(parameters, unsigned int);
					size_t numWidth = numSizeu(arg, 8u);
					if (width > numWidth && !left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							term_putc(lead_zero ? '0' : ' ');
							written++;
						}
					}
					written += puti(arg, 8, false);
					if (width > numWidth && left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							term_putc(' ');
							written++;
						}
					}
				}
				break;
				case 'x':
				{
					unsigned int arg = va_arg(parameters, unsigned int);
					size_t numWidth = numSizeu(arg, 16u);
					if (width > numWidth && !left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							term_putc(lead_zero ? '0' : ' ');
							written++;
						}
					}
					written += puti(arg, 16, false);
					if (width > numWidth && left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							term_putc(' ');
							written++;
						}
					}
				}
				break;
				case 'X':
				{
					unsigned int arg = va_arg(parameters, unsigned int);
					size_t numWidth = numSizeu(arg, 10u);
					if (width > numWidth && !left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							term_putc(lead_zero ? '0' : ' ');
							written++;
						}
					}
					written += puti(arg, 16, true);
					if (width > numWidth && left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							term_putc(' ');
							written++;
						}
					}
				}
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
					size_t strWidth = strlen(arg);
					if (width > strWidth && !left_align)
					{
						int i;
						for (i = strWidth; i < width; i++)
						{
							term_putc(' ');
							written++;
						}
					}
					written += puts(arg);
					if (width > strWidth && left_align)
					{
						int i;
						for (i = strWidth; i < width; i++)
						{
							term_putc(' ');
							written++;
						}
					}
				}
				break;
				case 'p':
				{
					unsigned int arg = va_arg(parameters, unsigned int);
					size_t numWidth = numSizeu(arg, 10u);
					if (width > numWidth && !left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							term_putc(lead_zero ? '0' : ' ');
							written++;
						}
					}
					written += puti((unsigned int) arg, 16, true);
					if (width > numWidth && left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							term_putc(' ');
							written++;
						}
					}
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
