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
