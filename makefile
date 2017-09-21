#
# makefile
#

ASM = nasm
ASMFLAGS = -f elf32
CC = gcc
CFLAGS = -O2 -m32 -c -Iincludes -fno-builtin -ffreestanding
LD = ld
LDFLAGS = -m elf_i386 -T link.ld
NAME = kernel.bin

SDIR = src/
ODIR = build/

all: clean build run

build: compile
	$(LD) $(LDFLAGS) -o $(NAME) $(ODIR)boot.o $(ODIR)kernel.o $(ODIR)tdriver.o

$(ODIR)%.o: $(SDIR)%.c
	$(CC) $(CFLAGS) -o $@ $<

compile: $(ODIR)tdriver.o $(ODIR)kernel.o
	$(ASM) $(ASMFLAGS) $(SDIR)boot.asm -o $(ODIR)boot.o

run:
	qemu-system-i386 -kernel $(NAME)

clean:
	if ls $(ODIR)* 1> /dev/null 2>&1; then rm $(ODIR)*; fi
	if [ -f $(NAME) ]; then rm $(NAME); fi
