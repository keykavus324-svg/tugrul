; gdt_flush.asm - GDT flush routine
[bits 32]

global gdt_flush

gdt_flush:
    mov eax, [esp + 4]      ; Get the pointer to the GDT
    lgdt [eax]              ; Load the new GDT
    
    mov ax, 0x10            ; Kernel data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    jmp 0x08:.flush         ; Far jump to kernel code segment
.flush:
    ret
