global loader
extern kmain

MAGIC_NUMBER equ 0x1BADB002
FLAGS equ 0
CHECKSUM equ -(MAGIC_NUMBER + FLAGS)

section .multiboot
align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
align 4
loader:
    mov esp, stack_top
    call kmain
.hang:
    cli
    hlt
    jmp .hang

