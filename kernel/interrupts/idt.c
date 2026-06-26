#include "idt.h"
#include "vga.h"
#include "io.h"

#define IDT_ENTRIES 256

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;
static interrupt_handler_t handlers[IDT_ENTRIES];

// External assembly functions
extern void idt_load(struct idt_ptr*);
extern void isr0(void);
extern void isr1(void);
extern void irq0(void);

// Common ISR stub (declared in assembly)
extern void isr_common_stub(void);
extern void irq_common_stub(void);

void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

void idt_init(void) {
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)&idt;
    
    // Clear IDT
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i, 0, 0, 0);
        handlers[i] = 0;
    }
    
    // Remap PICs
    io_outb(0x20, 0x11);  // ICW1 - Initialize PIC1
    io_outb(0xA0, 0x11);  // ICW1 - Initialize PIC2
    io_wait();
    
    io_outb(0x21, 0x20);  // ICW2 - Vector offset for PIC1 (0x20-0x27)
    io_outb(0xA1, 0x28);  // ICW2 - Vector offset for PIC2 (0x28-0x2F)
    io_wait();
    
    io_outb(0x21, 0x04);  // ICW3 - Cascade identity
    io_outb(0xA1, 0x02);  // ICW3 - Cascade identity
    io_wait();
    
    io_outb(0x21, 0x01);  // ICW4 - 8086 mode
    io_outb(0xA1, 0x01);  // ICW4 - 8086 mode
    io_wait();
    
    io_outb(0x21, 0x0);   // OCW1 - Mask all interrupts initially
    io_outb(0xA1, 0x0);   // OCW1 - Unmask all interrupts
    
    // Set up some exception handlers (ISRs 0-31)
    // These would be set by assembly stubs in a full implementation
    // For now, we'll just set up the timer interrupt (IRQ0 -> INT 0x20)
    
    // Load IDT
    idt_load(&idtp);
    
    vga_println("[IDT] Initialized with 256 entries");
    vga_println("[PIC] Remapped to 0x20-0x2F");
}

void idt_register_handler(uint8_t vector, interrupt_handler_t handler) {
    handlers[vector] = handler;
}

void enable_interrupts(void) {
    __asm__ volatile ("sti");
}

void disable_interrupts(void) {
    __asm__ volatile ("cli");
}

// Timer interrupt handler (IRQ0)
void timer_handler(uint32_t int_no) {
    static uint32_t ticks = 0;
    ticks++;
    
    if (ticks % 18 == 0) {  // Approximately once per second
        // Could update system time here
    }
    
    // Send EOI to PIC
    io_outb(0x20, 0x20);
    if (int_no >= 0x28) {
        io_outb(0xA0, 0x20);
    }
}

// Keyboard interrupt handler (IRQ1)
void keyboard_irq_handler(uint32_t int_no) {
    // Handled by polling in main kernel for simplicity
    io_outb(0x20, 0x20);  // Send EOI
}
