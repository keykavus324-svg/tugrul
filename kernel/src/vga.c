/* kernel/src/vga.c - VGA text mode driver */
#include "../include/vga.h"
#include "../include/io.h"

static uint16_t* vga_buffer = (uint16_t*)0xB8000;
static size_t vga_row = 0;
static size_t vga_col = 0;
static uint8_t vga_current_color = 0x0F; /* White on black */

void vga_init(void) {
    vga_clear();
}

void vga_clear(void) {
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = vga_entry(' ', vga_current_color);
    }
    vga_row = 0;
    vga_col = 0;
}

void vga_set_color(uint8_t color) {
    vga_current_color = color;
}

void vga_putchar(char c) {
    if (c == '\n') {
        vga_col = 0;
        vga_row++;
    } else if (c == '\r') {
        vga_col = 0;
    } else if (c == '\t') {
        vga_col = (vga_col + 8) & ~7;
    } else if (c >= ' ') {
        vga_buffer[vga_row * VGA_WIDTH + vga_col] = vga_entry(c, vga_current_color);
        vga_col++;
    }
    
    if (vga_col >= VGA_WIDTH) {
        vga_col = 0;
        vga_row++;
    }
    
    if (vga_row >= VGA_HEIGHT) {
        vga_scroll();
        vga_row = VGA_HEIGHT - 1;
    }
}

void vga_putstr(const char* str) {
    while (*str) {
        vga_putchar(*str++);
    }
}

void vga_scroll(void) {
    /* Move all lines up by one */
    for (size_t i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
        vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
    }
    
    /* Clear the last line */
    for (size_t i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        vga_buffer[i] = vga_entry(' ', vga_current_color);
    }
}
