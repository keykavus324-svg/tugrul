/* kernel/src/kernel.c - Main kernel entry point */
#include "../include/types.h"
#include "../include/vga.h"
#include "../include/keyboard.h"
#include "../include/gdt.h"

extern void gdt_flush();

void kernel_main(void) {
    vga_init();
    keyboard_init();
    
    /* Initialize GDT */
    gdt_install();
    
    /* Welcome message */
    vga_set_color(vga_color(VGA_WHITE, VGA_BLUE));
    vga_putstr("========================================\n");
    vga_putstr("     Welcome to MyOS v0.2\n");
    vga_putstr("========================================\n");
    vga_set_color(vga_color(VGA_LIGHT_GREY, VGA_BLACK));
    vga_putstr("\nSystem initialized successfully!\n");
    vga_putstr("GDT installed.\n");
    vga_putstr("Type 'help' for available commands.\n\n");
    
    /* Simple command loop */
    char buffer[256];
    size_t pos = 0;
    
    while (1) {
        vga_putstr("> ");
        pos = 0;
        
        /* Read input */
        while (1) {
            char c = keyboard_read_char();
            
            if (c == '\n' || c == '\r') {
                vga_putchar('\n');
                break;
            } else if (c == '\b') {
                if (pos > 0) {
                    pos--;
                    vga_putstr("\b \b");
                }
            } else if (c >= ' ' && c < 127) {
                if (pos < 255) {
                    buffer[pos++] = c;
                    vga_putchar(c);
                }
            }
        }
        
        buffer[pos] = '\0';
        
        /* Process command */
        if (pos > 0) {
            if (buffer[0] == 'h' && buffer[1] == 'e' && buffer[2] == 'l' && 
                buffer[3] == 'p' && buffer[4] == '\0') {
                vga_putstr("\nAvailable commands:\n");
                vga_putstr("  help    - Show this help\n");
                vga_putstr("  clear   - Clear screen\n");
                vga_putstr("  echo    - Print text\n");
                vga_putstr("  color   - Change text color\n");
                vga_putstr("  reboot  - Reboot system\n");
                vga_putstr("  mem     - Show memory info\n");
                vga_putstr("  gdt     - Show GDT status\n\n");
            } else if (buffer[0] == 'c' && buffer[1] == 'l' && buffer[2] == 'e' && 
                       buffer[3] == 'a' && buffer[4] == 'r' && buffer[5] == '\0') {
                vga_clear();
            } else if (buffer[0] == 'e' && buffer[1] == 'c' && buffer[2] == 'h' && 
                       buffer[3] == 'o' && buffer[4] == ' ') {
                vga_putstr(&buffer[5]);
                vga_putstr("\n");
            } else if (buffer[0] == 'c' && buffer[1] == 'o' && buffer[2] == 'l' && 
                       buffer[3] == 'o' && buffer[4] == 'r' && buffer[5] == ' ') {
                /* Simple color change: color <fg> <bg> */
                int fg = buffer[6] - '0';
                int bg = buffer[8] - '0';
                if (fg >= 0 && fg <= 15 && bg >= 0 && bg <= 15) {
                    vga_set_color((uint8_t)(fg | (bg << 4)));
                    vga_putstr("Color changed!\n");
                } else {
                    vga_putstr("Usage: color <fg> <bg> (0-15)\n");
                }
            } else if (buffer[0] == 'r' && buffer[1] == 'e' && buffer[2] == 'b' && 
                       buffer[3] == 'o' && buffer[4] == 'o' && buffer[5] == 't' && 
                       buffer[6] == '\0') {
                vga_putstr("Rebooting...\n");
                __asm__ volatile ("cli; hlt");
            } else if (buffer[0] == 'm' && buffer[1] == 'e' && buffer[2] == 'm' && 
                       buffer[3] == '\0') {
                vga_putstr("Memory Information:\n");
                vga_putstr("  Mode: 32-bit Protected Mode\n");
                vga_putstr("  Kernel loaded at: 0x100000\n");
                vga_putstr("  GDT entries: 3 (Null, Code, Data)\n\n");
            } else if (buffer[0] == 'g' && buffer[1] == 'd' && buffer[2] == 't' && 
                       buffer[3] == '\0') {
                vga_putstr("GDT Status: Initialized\n");
                vga_putstr("  Entry 0: Null Descriptor\n");
                vga_putstr("  Entry 1: Code Segment (Ring 0)\n");
                vga_putstr("  Entry 2: Data Segment (Ring 0)\n\n");
            } else {
                vga_putstr("Unknown command: ");
                vga_putstr(buffer);
                vga_putstr("\n");
            }
        }
    }
}
