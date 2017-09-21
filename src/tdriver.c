/*
 * stdio.c
 */

#include <kernel/tty.h>

#define VIDPTR ((char*)0xb8000)
#define VID_LOC(x, y) (x + y * SCREEN_WIDTH)
#define MAX_VID_LOC (SCREEN_WIDTH * SCREEN_HEIGHT)

uint16_t vid_loc = 0;
uint8_t curs_x = 0;
uint8_t curs_y = 0;

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
		write_character(c, 0x07, VID_LOC(curs_x++, curs_y));
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

void term_puts(const char* str)
{
	while (*str)
	{
		term_putc(*str);
		str++;
	}
}

void term_cls()
{
	uint16_t j = 0;
	while(j < MAX_VID_LOC)
	{
		write_character(' ', 0x07, j++);
	}
}
