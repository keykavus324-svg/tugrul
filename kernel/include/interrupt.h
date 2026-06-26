#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "types.h"

// Register state structure for interrupt handlers
struct registers {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

// ISR handler function type
typedef void (*isr_handler_t)(struct registers*);

// IRQ handler function type  
typedef void (*irq_handler_t)(struct registers*);

// Initialize interrupt system
void interrupt_init(void);

// Register ISR and IRQ handlers
void isr_register_handler(uint8_t vector, isr_handler_t handler);
void irq_register_handler(uint8_t vector, irq_handler_t handler);

// C handlers called from assembly stubs
void isr_handler(struct registers* regs);
void irq_handler(struct registers* regs);

#endif // INTERRUPT_H
