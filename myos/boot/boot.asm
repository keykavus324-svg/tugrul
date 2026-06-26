[bits 16]
[org 0x7c00]

KERNEL_OFFSET equ 0x1000

start:
    cli
    
    ; Segment kaydedicilerini ayarla
    mov ax, 0x0000
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    
    sti
    
    ; Disk okuma için BIOS çağrısı
    mov si, disk_message
    call print_string
    
    ; Kernel'i yükle
    mov dh, 32        ; 32 sektör oku
    mov dl, 0x00      ; Drive 0
    mov ah, 0x02      ; Read sectors
    mov al, dh        ; Sector count
    mov ch, 0x00      ; Cylinder 0
    mov cl, 0x02      ; Start from sector 2
    mov dh, 0x00      ; Head 0
    mov bx, KERNEL_OFFSET
    int 0x13
    
    jc disk_error
    
    mov si, load_success
    call print_string
    
    ; Korumalı moda geç
    jmp start_protected_mode
    
disk_error:
    mov si, disk_error_message
    call print_string
    jmp $

print_string:
    pusha
.loop:
    lodsb
    test al, al
    jz .done
    mov ah, 0x0e
    int 0x10
    jmp .loop
.done:
    popa
    ret

[bits 32]
start_protected_mode:
    mov esi, kernel_loaded_msg
    call print_string_32
    
    ; Kernel'i çalıştır
    call KERNEL_OFFSET
    jmp $

print_string_32:
    pushad
    mov edx, 0xb8000
.loop:
    lodsb
    test al, al
    jz .done
    mov ah, 0x0f
    mov [edx], ax
    add edx, 2
    jmp .loop
.done:
    popad
    ret

disk_message db "Loading kernel...", 0
load_success db " OK!", 13, 10, 0
disk_error_message db "Disk error!", 0
kernel_loaded_msg db "Kernel loaded, starting...", 0

times 510-($-$$) db 0
dw 0xaa55
