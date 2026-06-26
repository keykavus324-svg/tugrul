#include "../include/types.h"
#include "../include/vga.h"
#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/pic.h"
#include "../include/timer.h"
#include "../include/keyboard.h"
#include "../include/io.h"

void kernel_main() {
    vga_init();
    vga_writestring("MYOS - Mini Linux-like OS", VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    vga_putchar('\n', VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    vga_writestring("Booting...", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_putchar('\n', VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    
    gdt_init();
    vga_writestring("[OK] GDT initialized", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_putchar('\n', VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    
    idt_init();
    vga_writestring("[OK] IDT initialized", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_putchar('\n', VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    
    timer_init();
    vga_writestring("[OK] Timer initialized", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_putchar('\n', VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    
    keyboard_init();
    vga_writestring("[OK] Keyboard initialized", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_putchar('\n', VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    
    vga_putchar('\n', VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    vga_writestring("System ready! Type on keyboard...", VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
    vga_putchar('\n', VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
    
    enable_interrupts();
    
    while(1) {
        __asm__ volatile ("hlt");
    }
}
