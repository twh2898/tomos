/*
 * libc/stdio.h
 */
#ifndef LIBC_STDIO_H
#define LIBC_STDIO_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef _cplusplus
extern "C" {
#endif

extern size_t putchar(char);
extern size_t puts(const char*);
extern size_t puti(uint32_t, uint32_t, bool, bool);
extern size_t printf(const char*, ...);

#ifdef _cplusplus
}
#endif

#endif // LIBC_STDIO_H
