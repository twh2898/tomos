;
; kernel.asm
;

global start
global load_idt
global keyboard_handler
extern kmain	; kmain is defined in the c file
extern keyboard_handler_main

; nasm directive - 32 bit
bits 32
section .multiboot
        ;multiboot spec
        align 4
        dd 0x1BADB002            ; magic
        dd 0x00                  ; flags
        dd - (0x1BADB002 + 0x00) ; checksum. m+f+c should be zero

section .text
	start:
	  cli					; block interrupts
	  mov esp, stack_space	; set stack pointer
	  call kmain			; call the main in c

	hang:
		cli			; disable interupts
		hlt			; halt the cpu
		jmp hang	; if that didn't work, try again

	load_idt:
		mov edx, [esp + 4]
		lidt [edx]
		sti
		ret

	keyboard_handler:
		call    keyboard_handler_main
		iretd

section .bss
	resb 8192		;8KB for stack
	stack_space:
