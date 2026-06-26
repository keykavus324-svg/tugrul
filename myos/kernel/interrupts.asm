[bits 32]

extern isr_handler
extern irq_handler

global gdt_flush
global idt_load
global irq0
global irq1
global enable_interrupts

; GDT'yi yükle ve segment kaydedicilerini güncelle
gdt_flush:
    mov eax, [esp+4]
    lgdt [eax]
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    jmp 0x08:.flush_cs
.flush_cs:
    ret

; IDT'yi yükle
idt_load:
    mov eax, [esp+4]
    lidt [eax]
    sti
    ret

; Kesmeleri etkinleştir
enable_interrupts:
    sti
    ret

; IRQ0 - Timer kesmesi
irq0:
    push 0
    push 32
    jmp irq_common_stub

; IRQ1 - Keyboard kesmesi
irq1:
    push 0
    push 33
    jmp irq_common_stub

irq_common_stub:
    pusha
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    push esp
    call irq_handler
    add esp, 4
    
    popa
    iret

; Genel koruma hatası
global gpf_handler
gpf_handler:
    pusha
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    iret
