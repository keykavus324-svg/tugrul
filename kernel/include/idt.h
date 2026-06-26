#ifndef IDT_H
#define IDT_H

#include "types.h"

// IDT entry structure
struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

// IDT pointer structure
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Interrupt handler function type
typedef void (*interrupt_handler_t)(uint32_t);

// Initialize IDT
void idt_init(void);

// Register an interrupt handler
void idt_register_handler(uint8_t vector, interrupt_handler_t handler);

// Enable interrupts
void enable_interrupts(void);

// Disable interrupts
void disable_interrupts(void);

#endif // IDT_H
