#ifndef BOOT_H
#define BOOT_H

#include <stdint.h>

extern void interrupt_handler(void);
extern void load_idt(unsigned long *idt_ptr);
extern void keyboard_handler(void);
extern void keyboard_handler_main(void);

#endif // BOOT_H
