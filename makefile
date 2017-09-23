#
# makefile
#

ASM = nasm
ASMFLAGS = -f elf32
CC = gcc
CFLAGS = -O2 -m32 -c -Iincludes -nostdlib -fno-builtin -ffreestanding -fno-stack-protector
LD = ld
LDFLAGS = -m elf_i386 -T link.ld -nostdlib
NAME = kernel.bin

SDIR = src/
ODIR = build/

all: clean build run

build: compile
	$(LD) $(LDFLAGS) -o $(NAME) $(ODIR)boot.o $(ODIR)kernel.o $(ODIR)tdriver.o $(ODIR)keyboard.o $(ODIR)idt.o

$(ODIR)%.o: $(SDIR)%.c
	$(CC) $(CFLAGS) -o $@ $<

compile: $(ODIR)tdriver.o $(ODIR)kernel.o $(ODIR)keyboard.o $(ODIR)idt.o
	$(ASM) $(ASMFLAGS) $(SDIR)boot.asm -o $(ODIR)boot.o

run:
	qemu-system-i386 -kernel $(NAME)

clean:
	if ls $(ODIR)* 1> /dev/null 2>&1; then rm $(ODIR)*; fi
	if [ -f $(NAME) ]; then rm $(NAME); fi
