#include "../include/types.h"
#include "../include/idt.h"
#include "../include/pic.h"
#include "../include/timer.h"
#include "../include/vga.h"

static void (*irq_routines[16])();

void irq_register_handler(u8 irq, void (*handler)()) {
    irq_routines[irq] = handler;
}

void irq_handler(u32 irq) {
    if (irq >= 32 && irq < 48) {
        u8 irq_num = irq - 32;
        
        if (irq_num == 0) {
            timer_handler();
        }
        
        if (irq_routines[irq_num]) {
            irq_routines[irq_num]();
        }
        
        pic_send_eoi(irq_num);
    }
}
