; kernel/start.asm - Kernel entry point (assembly stub)
[bits 32]
[global start]
[extern kernel_main]

section .text
start:
    ; Set up stack
    mov ebp, 0x90000
    mov esp, ebp
    
    ; Call kernel main
    call kernel_main
    
    ; Halt if kernel returns
    cli
    hlt

section .note.GNU-stack noalloc noexec nowrite progbits
