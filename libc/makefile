#
# makefile
#

CC = gcc
CFLAGS = -O2 -m32 -c -I../includes -nostdlib -fno-builtin -ffreestanding -fno-stack-protector

SDIR = src/
BDIR = build/

SOURCES := $(shell find $(SDIR) -type f -name '*.c')
OBJECTS := $(patsubst $(SDIR)%.c, $(BDIR)%.o, $(SOURCES))

BUILD_NAME = libc

all: init compile copy

dbg:
	@echo Sources: $(SOURCES)
	@echo Objects: $(OBJECTS)

clean:
	@echo :$(BUILD_NAME):Cleaning build directory
	@if [ -d $(BDIR) ]; then rm -r $(BDIR); fi

init: clean
	@mkdir -p src
	@mkdir -p build

$(BDIR)%.o: $(SDIR)%.c
	$(foreach d,$(subst /, ,${@D}),mkdir -p $d && cd $d && ):
	$(CC) $(CFLAGS) -o $@ $<

compile: .compile_message $(OBJECTS)

.compile_message:
	@echo :$(BUILD_NAME):Compiling

copy:
	@echo :$(BUILD_NAME):Copying binaries to linking directory
	cp -r build/ ../build/$(BUILD_NAME)/
