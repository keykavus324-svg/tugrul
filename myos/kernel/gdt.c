#include "../include/types.h"
#include "../include/gdt.h"

#define GDT_ENTRIES 5

struct gdt_entry gdt[GDT_ENTRIES];
struct gdt_ptr gdtp;

u32 KERNEL_CS = 0x08;
u32 KERNEL_DS = 0x10;
u32 USER_CS = 0x1B;
u32 USER_DS = 0x23;

extern void gdt_flush(u32);

void gdt_set_gate(int num, u32 base, u32 limit, u8 access, u8 gran) {
    gdt[num].base_low = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;
    
    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    
    gdt[num].access = access;
}

void gdt_init() {
    gdtp.limit = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1;
    gdtp.base = (u32)&gdt;
    
    // Null segment
    gdt_set_gate(0, 0, 0, 0, 0);
    
    // Kernel code segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    
    // Kernel data segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    
    // User code segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    
    // User data segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
    
    gdt_flush((u32)&gdtp);
}
