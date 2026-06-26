#include "interrupt.h"
#include "idt.h"
#include "vga.h"
#include "io.h"

#define ISR_COUNT 32
#define IRQ_COUNT 16

static isr_handler_t isr_handlers[ISR_COUNT];
static irq_handler_t irq_handlers[IRQ_COUNT];

const char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 FPU Error",
    "Alignment Check",
    "Machine Check",
    "SIMD FPU Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(struct registers* regs) {
    if (isr_handlers[regs->int_no] != 0) {
        isr_handlers[regs->int_no](regs);
    } else {
        // Default exception handler
        vga_set_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
        vga_println("!!! EXCEPTION !!!");
        vga_print("Exception: ");
        vga_println(exception_messages[regs->int_no]);
        vga_print("Error Code: ");
        
        char hex[9];
        for (int i = 0; i < 8; i++) {
            uint8_t nibble = (regs->err_code >> ((7 - i) * 4)) & 0xF;
            hex[i] = nibble < 10 ? '0' + nibble : 'A' + (nibble - 10);
        }
        hex[8] = '\0';
        vga_println(hex);
        
        vga_print("EIP: 0x");
        // Simplified hex print for EIP
        vga_print("????????");
        
        vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
        vga_println("\nSystem halted. Please reboot.");
        
        while(1) {
            __asm__ volatile ("hlt");
        }
    }
}

void irq_handler(struct registers* regs) {
    // Handle timer interrupt (IRQ0)
    if (regs->int_no == 32) {
        static uint32_t ticks = 0;
        ticks++;
        
        // Send EOI to PIC
        io_outb(0x20, 0x20);
        
        return;
    }
    
    // Handle keyboard interrupt (IRQ1)
    if (regs->int_no == 33) {
        // Keyboard handling done via polling in main kernel
        io_outb(0x20, 0x20);
        
        return;
    }
    
    // Call registered handler if exists
    uint8_t irq = regs->int_no - 32;
    if (irq < IRQ_COUNT && irq_handlers[irq] != 0) {
        irq_handlers[irq](regs);
    }
    
    // Send EOI to PIC
    if (irq >= 8) {
        io_outb(0xA0, 0x20);
    }
    io_outb(0x20, 0x20);
}

void interrupt_init(void) {
    // Initialize IDT first
    idt_init();
    
    // Clear all handlers
    for (int i = 0; i < ISR_COUNT; i++) {
        isr_handlers[i] = 0;
    }
    for (int i = 0; i < IRQ_COUNT; i++) {
        irq_handlers[i] = 0;
    }
    
    vga_println("[INT] Interrupt system initialized");
}

void isr_register_handler(uint8_t vector, isr_handler_t handler) {
    if (vector < ISR_COUNT) {
        isr_handlers[vector] = handler;
    }
}

void irq_register_handler(uint8_t vector, irq_handler_t handler) {
    if (vector < IRQ_COUNT) {
        irq_handlers[vector] = handler;
    }
}
