/*
 * kernel.c
 */

#include <stdint.h>
#include <kernel/asm.h>
#include <kernel/tty.h>

void kmain(void)
{
	uint8_t reset = 0x17;
	term_set_color(reset);
	term_cls();

	const char *str = "my super first kernel\n";
	puts(str);

	term_set_color(reset);
	printf("Lets demo some cool features of printf\n");
	size_t len = printf("Like numbers %% d%d d%d u%u x%x X%X and characters %c %s cool? %b\n", 10, -10, 10, 0xe0f, 0xff3, 'c', "string", true);
	int store;
	printf("That was %d characters!%n And that sentance was ", len, &store);
	printf("%d\n", store);
	printf("Last one, here is are two pointers 0x%p 0x%p\n", &str, &store);

	printf("Now for the real test, colors:\n");
	int i, j;
	//~ for (i = 0; i <= 0xF; i++)
	//~ {
		//~ for (j = 0; j <= 0xF; j++)
		//~ {
			//~ term_set_color(vga_color(j, i));
			//~ printf("%X%X ", j, i);
		//~ }
		//~ puts("\n");
	//~ }

	term_set_color(reset);
	//~ for (i = 0; i <= 0xF; i++)
	//~ {
		//~ for (j = 0; j <= 0xF; j++)
		//~ {
			//~ printf(" %c ", j | i << 4);
		//~ }
		//~ puts("\n");
	//~ }

	term_disable_cursor();
	term_enable_cursor(14, 15);

/*
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
*/

	return;
}
