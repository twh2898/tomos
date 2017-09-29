#
# makefile
#

LD = ld
LDFLAGS = -m elf_i386 -T link.ld -nostdlib
NAME = kernel.bin

SDIR = build/

all: init compile link run

dbg:
	@echo $(SDIR)
	@echo $(shell find $(SDIR) -type f -name '*.o')

clean:
	@echo ::Cleaning build directory
	@if [ -d includes ]; then rm -r includes/; fi
	@if [ -d build ]; then rm -r build/; fi
	@if [ -f $(NAME) ]; then rm $(NAME); fi

init: clean
	@mkdir -p build
	@mkdir -p includes

compile:
	@echo :libc:Compiling
	@make -C kernel
	@make -C libc

link:
	@echo ::Linking
	$(LD) $(LDFLAGS) -o $(NAME) $(shell find $(SDIR) -type f -name '*.o')

run:
	@echo ::Running
	qemu-system-x86_64 -kernel $(NAME)
