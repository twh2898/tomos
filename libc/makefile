#
# makefile
#

CC = gcc
CFLAGS = -O2 -m32 -c -I../includes -nostdlib -fno-builtin -ffreestanding -fno-stack-protector

SDIR = src/
BDIR = build/

SOURCES := $(wildcard $(SDIR)*.c)
OBJECTS := $(patsubst $(SDIR)%.c, $(BDIR)%.o, $(SOURCES))

all: init compile copy

clean:
	@echo :libc:Cleaning build directory
	@if [ -d $(BDIR) ]; then rm -r $(BDIR); fi

init: clean
	@mkdir -p src
	@mkdir -p includes
	@mkdir -p build

$(BDIR)%.o: $(SDIR)%.c
	$(CC) $(CFLAGS) -o $@ $<

compile: $(OBJECTS)
	@echo :libc:Compiling

copy:
	@echo :libc:Copying libc binaries to linking directory
	cp -r build/ ../build/libc/
