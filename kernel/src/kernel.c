/*
 * kernel.c
 */

#include <stdint.h>
#include <boot.h>
#include <kernel/asm.h>
#include <kernel/tty.h>
#include <kernel/vga.h>
#include <kernel/idt.h>
#include <kernel/keyboard.h>
#include <libc/stdio.h>

const uint8_t reset = 0x07;

void interrupt_handler()
{

}

void demo()
{
	const char *str = "my first kernel\n";
	puts(str);

	terminal_set_color(reset);
	printf("Lets demo some cool features of printf\n");
	size_t len = printf("Like numbers %% d%-3d d%04d u%u x%x X%03X and characters %c %s cool? %b\n", 10, -10, 10, 0xe0f, 0xff3, 'c', "string", true);
	int store;
	printf("That was %d characters!%n And that sentance was ", len, &store);
	printf("%d\n", store);
	printf("Last one, here is are two pointers 0x%p 0x%p\n", &str, &store);

	printf("Now for the real test, colors:\n");
	int i, j;
	for (i = 0; i <= 0xF; i++)
	{
		for (j = 0; j <= 0xF; j++)
		{
			terminal_set_color(vga_entry_color(j, i));
			printf("%X%X ", j, i);
		}
		puts("\n");
	}

	terminal_set_color(reset);
	//~ for (i = 0; i <= 0xF; i++)
	//~ {
		//~ for (j = 0; j <= 0xF; j++)
		//~ {
			//~ printf(" %c ", j | i << 4);
		//~ }
		//~ puts("\n");
	//~ }

	printf("Testing some assembly: 0x%X", inb(0x60));

	terminal_disable_cursor();
	terminal_enable_cursor(14, 15);
}

uint8_t print_time(uint8_t last_sec)
{
	outb(0x70, 0x00);
	uint8_t sec = inb(0x71);

	if (sec != last_sec)
	{
		uint8_t last_x = terminal_get_cursor_x();
		uint8_t last_y = terminal_get_cursor_y();
		terminal_disable_cursor();
		terminal_set_cursor(60, 0);
		outb(0x70, 0x04);
		uint8_t hour = inb(0x71);

		outb(0x70, 0x02);
		uint8_t min = inb(0x71);

		printf("The time is %d:%d:%d", hour, min, sec);
		terminal_set_cursor(last_x, last_y);
		terminal_enable_cursor(14, 15);
	}

	return sec;
}

void kmain(void)
{
	idt_init();

	terminal_set_color(reset);
	terminal_clear();

	demo();

	uint8_t last_sec = 0;
	uint8_t last_scancode = 0xFA;

	bool lshift = false;
	bool rshift = false;

	for (;;)
	{
		outb(0x20, 0x20);   // Send EOI
		uint8_t scancode = inb(0x60);
		if (scancode != last_scancode)  
		{
			last_scancode = scancode;
			bool press = !(scancode & 0x80);
			uint8_t key = scancode & 0x7F;
			switch(key)
			{
				// LShift
				case 0x2A:
					lshift = press;
					break;
				// RShift
				case 0x36:
					rshift = press;
					break;
				// arrow keys
				case 0x48:
					terminal_set_cursor(terminal_get_cursor_x(), terminal_get_cursor_y() - 1);
					break;
				case 0x50:
					terminal_set_cursor(terminal_get_cursor_x(), terminal_get_cursor_y() + 1);
					break;
				case 0x4B:
					terminal_set_cursor(terminal_get_cursor_x() - 1, terminal_get_cursor_y());
					break;
				case 0x4D:
					terminal_set_cursor(terminal_get_cursor_x() + 1, terminal_get_cursor_y());
					break;
				default:
					if (press)
						putchar(asChar(key, lshift || rshift));
			}
			// printf("Key 0x%X is %s\n", key, press ? "Pressed" : "Released");
		}
		
		last_sec = print_time(last_sec);
	}

	terminal_set_color(0x07);
	terminal_clear();
	printf("System Halting!");
	terminal_disable_cursor();
	return;
}
