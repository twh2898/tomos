#
# makefile
#

all: clean compile build run

build: 
	ld -m elf_i386 -T link.ld -o kernel kasm.o kc.o stdio.o

compile:
	nasm -f elf32 kernel.asm -o kasm.o
	gcc -m32 -c kernel.c -o kc.o -nostdinc -Iincludes -fno-builtin
	gcc -m32 -c stdio.c -o stdio.o -nostdinc -Iincludes -fno-builtin

run:
	qemu-system-i386 -kernel kernel

clean:
	if [ -f *.o ]; then rm *.o; fi
	if [ -f kernel ]; then rm kernel; fi
