#
# makefile
#

ASM = nasm
ASMFLAGS = -f elf32
CC = gcc
CFLAGS = -O2 -m32 -c -I../includes -nostdlib -fno-builtin -ffreestanding -fno-stack-protector

SDIR = src/
BDIR = build/

SOURCES := $(wildcard $(SDIR)*.asm) $(wildcard $(SDIR)*.c)
OBJECTS := $(patsubst $(SDIR)%.asm, $(BDIR)%.o, $(SOURCES)) $(patsubst $(SDIR)%.c, $(BDIR)%.o, $(SOURCES))

all: init compile copy

clean:
	@echo :kernel:Cleaning build directory
	@if [ -d $(BDIR) ]; then rm -r $(BDIR); fi

init: clean
	@mkdir -p src
	@mkdir -p includes
	@mkdir -p build

$(BDIR)%.o: $(SDIR)%.c
	$(CC) $(CFLAGS) -o $@ $<

$(BDIR)%.o: $(SDIR)%.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

compile: $(OBJECTS)
	@echo :kernel:Compiling

copy:
	@echo :kernel:Copying binaries to linking directory
	cp -r build/ ../build/kernel/
