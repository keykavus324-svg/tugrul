#include "../include/types.h"
#include "../include/vga.h"

static u16* vga_buffer = (u16*)0xB8000;
static int cursor_x = 0;
static int cursor_y = 0;

void vga_init() {
    vga_clear();
}

void vga_putchar(char c, u8 fg, u8 bg) {
    u8 color = (bg << 4) | (fg & 0x0F);
    u16 entry = (color << 8) | (u8)c;
    
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\r') {
        cursor_x = 0;
    } else if (c == '\t') {
        cursor_x = (cursor_x + 8) & ~7;
    } else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
            vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = (color << 8) | ' ';
        }
    } else {
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = entry;
        cursor_x++;
    }
    
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    while (cursor_y >= VGA_HEIGHT) {
        vga_scroll();
        cursor_y = VGA_HEIGHT - 1;
    }
}

void vga_writestring(const char* str, u8 fg, u8 bg) {
    while (*str) {
        vga_putchar(*str++, fg, bg);
    }
}

void vga_clear() {
    u8 color = (VGA_COLOR_BLACK << 4) | VGA_COLOR_LIGHT_GREY;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = (color << 8) | ' ';
    }
    cursor_x = 0;
    cursor_y = 0;
    vga_set_cursor(0, 0);
}

void vga_scroll() {
    for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
        vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
    }
    
    u8 color = (VGA_COLOR_BLACK << 4) | VGA_COLOR_LIGHT_GREY;
    for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = (color << 8) | ' ';
    }
}

void vga_set_cursor(int x, int y) {
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        cursor_x = x;
        cursor_y = y;
        u16 pos = y * VGA_WIDTH + x;
        
        outb(0x3D4, 0x0F);
        outb(0x3D5, (u8)(pos & 0xFF));
        outb(0x3D4, 0x0E);
        outb(0x3D5, (u8)((pos >> 8) & 0xFF));
    }
}
