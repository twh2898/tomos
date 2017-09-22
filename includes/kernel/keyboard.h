#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

extern void initKeybaord();
extern char waitScanCode();
extern char asChar(char code, bool shift);

#endif // KEYBOARD_H
