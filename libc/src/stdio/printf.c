/*
 * libc/stdio/printf.c
 */
#include <stdarg.h>
#include <libc/string.h>
#include <libc/stdio.h>

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
						written += putchar('-');
						arg = -arg;
						numWidth++;
					}
					if (width > numWidth && !left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							written += putchar(lead_zero ? '0' : ' ');
						}
					}
					written += puti((unsigned int) arg, 10, false, false);
					if (width > numWidth && left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							written += putchar(' ');
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
							written += putchar(lead_zero ? '0' : ' ');
						}
					}
					written += puti(arg, 10, false, false);
					if (width > numWidth && left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							written += putchar(' ');
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
							written += putchar(lead_zero ? '0' : ' ');
						}
					}
					written += puti(arg, 8, false, false);
					if (width > numWidth && left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							written += putchar(' ');
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
							written += putchar(lead_zero ? '0' : ' ');
						}
					}
					written += puti(arg, 16, false, false);
					if (width > numWidth && left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							written += putchar(' ');
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
							written += putchar(lead_zero ? '0' : ' ');
						}
					}
					written += puti(arg, 16, false, true);
					if (width > numWidth && left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							written += putchar(' ');
						}
					}
				}
				break;
				case 'c':
				{
					char arg = (char) va_arg(parameters, int);
					written += putchar(arg);
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
							written += putchar(' ');
						}
					}
					written += puts(arg);
					if (width > strWidth && left_align)
					{
						int i;
						for (i = strWidth; i < width; i++)
						{
							written += putchar(' ');
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
							written += putchar(lead_zero ? '0' : ' ');
						}
					}
					written += puti((unsigned int) arg, 16, false, true);
					if (width > numWidth && left_align)
					{
						int i;
						for (i = numWidth; i < width; i++)
						{
							written += putchar(' ');
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
					written += puts(arg ? "true" : "false");
				}
				break;
				case '%':
				{
					written += putchar('%');
				}
				break;
				default:
					break;
			}
		}
		else
		{
			written += putchar(*format);
		}

		format++;
	}

	va_end(parameters);
	return written;
}
