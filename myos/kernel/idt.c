#include "../include/types.h"
#include "../include/idt.h"

#define IDT_ENTRIES 256

struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idtp;

extern void idt_load(u32);
extern void irq_stub_start();

void idt_set_gate(u8 num, u32 base, u16 selector, u8 flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

void idt_init() {
    idtp.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
    idtp.base = (u32)&idt;
    
    // IRQ0 - Timer
    idt_set_gate(32, (u32)irq0, 0x08, 0x8E);
    
    // IRQ1 - Keyboard
    idt_set_gate(33, (u32)irq1, 0x08, 0x8E);
    
    idt_load((u32)&idtp);
}
