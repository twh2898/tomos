/*
 * kernel.c
 */

#include <kernel/tty.h>

static inline void outb(unsigned short int port, unsigned char val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

static inline unsigned char inb(unsigned short int port)
{
    unsigned char ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

void puti(int num)
{
	if (!num)
	{
		term_puts("0");
		return;
	}

	int digits = 0;
	int res = 0;
	while (num)
	{
		res = res * 10;
		res = res + (num % 10);
		num = num / 10;
		digits++;
	}
	while (digits--)
	{
		term_putc('0' + (res % 10));
		res = res / 10;
	}
}

void kmain(void)
{
	const char *str = "my super first kernel";

	term_cls();
	term_puts(str);
	term_puts("\n");
	term_puts("Hi");
	term_puts("\nThe time is ");

	for (;;)
	{
		term_set_cursor(12, 2);
		outb(0x70, 0x04);
		unsigned char hour = inb(0x71);
		puti(hour);
		term_putc(':');

		outb(0x70, 0x02);
		unsigned char min = inb(0x71);
		puti(min);
		term_putc(':');

		outb(0x70, 0x00);
		unsigned char sec = inb(0x71);
		puti(sec);
	}


	return;
}
