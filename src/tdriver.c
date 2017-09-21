/*
 * stdio.c
 */
 
#include <terminal.h>

#define VIDPTR ((char*)0xb8000)
#define VID_LOC(x, y) (x + y * SCREEN_WIDTH)
#define MAX_VID_LOC (SCREEN_WIDTH * SCREEN_HEIGHT)

int vid_loc = 0;
int curs_x = 0;
int curs_y = 0;

static inline void write_character(char c, char attr, int loc)
{
	loc = loc << 1;
	VIDPTR[loc] = c;
	VIDPTR[loc + 1] = attr;
}

void putc(const char c)
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

void set_cursor(char loc_x, char loc_y)
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

void puts(const char* str)
{
	while (*str)
	{
		putc(*str);
		str++;
	}
}

void cls()
{
	int j = 0;
	while(j < MAX_VID_LOC) 
	{
		write_character(' ', 0x07, j++);
	}
}
