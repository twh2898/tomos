#ifndef IDT_H
#define IDT_H

#define IDT_SIZE 256

struct IDT_entry{
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

extern struct IDT_entry IDT[IDT_SIZE];

extern void idt_init(void);

#endif // IDT_H
