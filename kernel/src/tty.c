/*
 * stdio.c
 */

#include <kernel/asm.h>
#include <kernel/tty.h>
#include <kernel/vga.h>

static size_t terminal_row = 0;
static size_t terminal_column = 0;
static uint8_t terminal_color = 0x07;
static uint16_t* terminal_buffer = VGA_MEMORY;

void terminal_write(char c, uint8_t color, size_t x, size_t y)
{
	size_t index = vga_index(x, y);
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_set_color(uint8_t color)
{
	terminal_color = color;
}

void terminal_clear()
{
	size_t x, y;
	for (x = 0; x < VGA_WIDTH; x++)
		for (y = 0; y < VGA_HEIGHT; y++)
			terminal_write(' ', terminal_color, x, y);
	
	terminal_set_cursor(0, 0);
}

void terminal_enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
        outb(0x3D4, 0x0A);
        outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

        outb(0x3D4, 0x0B);
        outb(0x3D5, (inb(0x3E0) & 0xE0) | cursor_end);
}

void terminal_disable_cursor()
{
	outb(0x3D4, 0x0A); // LOW cursor shape port to vga INDEX register
	outb(0x3D5, 0x3f); //bits 6-7 must be 0 , if bit 5 set the cursor is disable  , bits 0-4 controll the cursor shape .
}

static void update_cursor()
{
	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned char) vga_index(terminal_column, terminal_row) & 0xFF);
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char) (vga_index(terminal_column, terminal_row) >> 8) & 0xFF);
}

void terminal_set_cursor(size_t x, size_t y)
{
	if (x >= 0 && x < VGA_WIDTH)
		terminal_column = x;

	if (y >= 0 && y < VGA_HEIGHT)
		terminal_row = y;
	
	update_cursor();
}

size_t terminal_get_cursor_x()
{
	return terminal_column;
}

size_t terminal_get_cursor_y()
{
	return terminal_row;
}

static void shift_lines()
{
	size_t x, y;
	for (y = 0; y < VGA_HEIGHT - 1; y++)
	{
		for (x = 0; x < VGA_WIDTH; x++)
		{
			size_t from_index = vga_index(x, y + 1);
			size_t to_index = vga_index(x, y);
			terminal_buffer[to_index] = terminal_buffer[from_index];
		}
	}
	
	for (y = VGA_HEIGHT - 1; y < VGA_HEIGHT; y++)
		for (x = 0; x < VGA_WIDTH; x++)
			terminal_write(' ', 0x07, x, y);
}

void terminal_putchar(char c)
{
	switch (c)
	{
		case '\n':
		{
			terminal_row++;
			terminal_column = 0;
		}
		break;
		case '\b':
		{
			if (terminal_column == 0)
			{
				if (terminal_row == 0)
				{
					terminal_column = 1;
				}
				else
				{
					terminal_column = VGA_WIDTH;
					terminal_row--;
				}
			}

			terminal_write(' ', terminal_color, --terminal_column, terminal_row);
		}
		break;
		case '\t':
		{
			terminal_write(' ', terminal_color, terminal_column++, terminal_row);
			while (terminal_column % 4 != 0 && terminal_column >= VGA_WIDTH)
				terminal_write(' ', terminal_color, terminal_column++, terminal_row);
		}
		break;
		default:
			terminal_write(c, terminal_color, terminal_column++, terminal_row);
		}

	if (terminal_column >= VGA_WIDTH)
	{
		terminal_column = 0;
		terminal_row++;
	}

	if (terminal_row >= VGA_HEIGHT)
	{
		terminal_row = VGA_HEIGHT - 1;
		shift_lines();
	}

	update_cursor();
}
