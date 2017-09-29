#ifndef STRING_H
#define STRING_H

#include <stddef.h>

#ifdef _cplusplus
extern "C" {
#endif
 
extern int memcmp(const void*, const void*, size_t);
extern void* memcpy(void* __restrict, const void* __restrict, size_t);
extern void* memmove(void*, const void*, size_t);
extern void* memset(void*, int, size_t);
extern size_t strlen(const char*);

#ifdef _cplusplus
}
#endif

#endif // STRING_H
