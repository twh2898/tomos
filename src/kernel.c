/*
 * kernel.c
 */

#include <stdint.h>
#include <kernel/tty.h>

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

void kmain(void)
{
	term_cls();

	const char *str = "my super first kernel\n";
	term_set_color(vga_color(VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BROWN));
	puts(str);

	term_set_color(0x07);
	printf("Lets demo some cool features of printf\n");
	size_t len = printf("Like numbers %% d%d d%d u%u x%x X%X and characters %c %s cool? %b\n", 10, -10, 10, 0xe0f, 0xff3, 'c', "string", true);
	int store;
	printf("That was %d characters!%n And that sentance was ", len, &store);
	printf("%d\n", store);
	printf("Last one, here is are two pointers 0x%p 0x%p\n", &str, &store);

	for (;;)
	{
		term_set_cursor(60, 0);
		outb(0x70, 0x04);
		uint8_t hour = inb(0x71);
		//~ term_puti(hour, 10, false);
		//~ term_putc(':');

		outb(0x70, 0x02);
		uint8_t min = inb(0x71);
		//~ term_puti(min, 10, false);
		//~ term_putc(':');

		outb(0x70, 0x00);
		uint8_t sec = inb(0x71);
		//~ term_puti(sec, 10, false);

		printf("The time is %d:%d:%d", hour, min, sec);
	}


	return;
}
