#ifndef VGA_H
#define VGA_H

#ifdef _cplusplus
extern "C" {
#endif

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((uint16_t*) 0xb8000)

#define VGA_COLOR_BLACK 0x0
#define VGA_COLOR_BLUE 0x1
#define VGA_COLOR_GREEN 0x2
#define VGA_COLOR_CYAN 0x3
#define VGA_COLOR_RED 0x4
#define VGA_COLOR_MAGENTA 0x5
#define VGA_COLOR_BROWN 0x6
#define VGA_COLOR_LIGHT_GREY 0x7
#define VGA_COLOR_DARK_GREY 0x8
#define VGA_COLOR_LIGHT_BLUE 0x9
#define VGA_COLOR_LIGHT_GREEN 0xA
#define VGA_COLOR_LIGHT_CYAN 0xB
#define VGA_COLOR_LIGHT_RED 0xC
#define VGA_COLOR_LIGHT_MAGENTA 0xD
#define VGA_COLOR_LIGHT_BROWN 0xE
#define VGA_COLOR_WHITE 0xF

static inline size_t vga_index(size_t x, size_t y)
{
	return y * VGA_WIDTH + x;
}

static inline uint16_t vga_entry(uint8_t uc, uint8_t color)
{
	return (uint16_t) uc | ((uint16_t) color << 8);
}

static inline uint8_t vga_entry_color(uint8_t fg, uint8_t bg) {
	return fg | (bg << 4);
}

#ifdef _cplusplus
}
#endif

#endif // VGA_H
