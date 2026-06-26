; boot/boot.asm - 32-bit bootloader
[bits 16]
[org 0x7c00]

KERNEL_OFFSET equ 0x1000

start:
    ; Setup segments
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    
    ; Save boot drive
    mov [BOOT_DRIVE], dl
    
    ; Print loading message
    mov si, MSG_LOADING
    call print_string
    
    ; Load kernel from disk
    mov dh, 32          ; Number of sectors to load
    mov dl, [BOOT_DRIVE]
    mov bx, KERNEL_OFFSET
    call disk_load
    
    ; Print OK message
    mov si, MSG_OK
    call print_string
    
    ; Switch to protected mode
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp CODE_SEG:init_pm

[bits 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp
    
    ; Jump to kernel
    call KERNEL_OFFSET
    
    ; Halt if kernel returns
    hlt

; Disk read routine
disk_load:
    pusha
    push dx
    
    ; Calculate CHS values
    mov ah, 0x02        ; BIOS read sectors
    mov al, dh          ; Number of sectors
    mov ch, 0           ; Cylinder 0
    mov cl, 2           ; Start from sector 2 (sector 1 is boot)
    mov dh, 0           ; Head 0
    
    int 0x13            ; BIOS disk interrupt
    jc disk_error
    
    pop dx
    popa
    ret

disk_error:
    mov si, MSG_DISK_ERROR
    call print_string
    hlt

; Print string in real mode
print_string:
    pusha
    mov ah, 0x0e
.print_loop:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .print_loop
.done:
    popa
    ret

; GDT
gdt_start:
gdt_null:
    dq 0
gdt_code:
    dw 0xffff
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0
gdt_data:
    dw 0xffff
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; Messages
MSG_LOADING db "Loading OS...", 0
MSG_OK db " OK!", 13, 10, 0
MSG_DISK_ERROR db "Disk error!", 0
BOOT_DRIVE db 0

; Padding and boot signature
times 510-($-$$) db 0
dw 0xaa55
