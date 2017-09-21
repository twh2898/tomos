#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

extern void initKeybaord();
extern char waitScanCode();
extern char asChar(char code);

#endif // KEYBOARD_H
