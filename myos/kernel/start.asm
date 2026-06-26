[bits 32]

extern kernel_main

global _start
global _stop

_start:
    cli
    
    ; Stack pointer'ı ayarla
    mov esp, stack_top
    
    ; Kernel ana fonksiyonunu çağır
    call kernel_main
    
_stop:
    hlt
    jmp _stop

; 8KB stack alanı
section .bss
align 16
stack_bottom:
    resb 8192
stack_top:
