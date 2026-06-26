/* kernel.c - MyOS Kernel ana dosyası */

#include "kernel.h"

/* VGA Buffer yapısı */
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

/* Renk kodları */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

/* VGA karakter yapısı */
struct vga_char {
    unsigned char character;
    unsigned char color;
};

/* Mevcut cursor pozisyonu */
static size_t terminal_row = 0;
static size_t terminal_column = 0;
static uint8_t terminal_color = 0;
static struct vga_char* terminal_buffer = (struct vga_char*) VGA_MEMORY;

/* Renk oluşturucu */
static inline uint8_t make_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

/* Terminali temizle */
void terminal_clear() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index].character = ' ';
            terminal_buffer[index].color = terminal_color;
        }
    }
    terminal_row = 0;
    terminal_column = 0;
}

/* Cursor pozisyonunu ayarla */
void terminal_setpos(size_t x, size_t y) {
    if (x >= VGA_WIDTH) x = VGA_WIDTH - 1;
    if (y >= VGA_HEIGHT) y = VGA_HEIGHT - 1;
    terminal_row = y;
    terminal_column = x;
}

/* Karakter yaz */
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
        return;
    }
    
    if (c == '\r') {
        terminal_column = 0;
        return;
    }
    
    if (c == '\t') {
        terminal_column += 4;
        terminal_column &= ~3; /* 4'e yuvarla */
        return;
    }
    
    if (c == '\b') {
        if (terminal_column > 0) {
            terminal_column--;
            const size_t index = terminal_row * VGA_WIDTH + terminal_column;
            terminal_buffer[index].character = ' ';
            terminal_buffer[index].color = terminal_color;
        }
        return;
    }
    
    const size_t index = terminal_row * VGA_WIDTH + terminal_column;
    terminal_buffer[index].character = c;
    terminal_buffer[index].color = terminal_color;
    
    terminal_column++;
    if (terminal_column >= VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
    }
}

/* String yaz */
void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

/* Null-terminated string yaz */
void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

/* Renk ayarla */
void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

/* Integer yazdır (basit) */
void terminal_putint(int num) {
    char buffer[12];
    int i = 0;
    int negative = 0;
    
    if (num < 0) {
        negative = 1;
        num = -num;
    }
    
    do {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);
    
    if (negative) {
        buffer[i++] = '-';
    }
    
    /* Ters çevir ve yazdır */
    while (i > 0) {
        terminal_putchar(buffer[--i]);
    }
}

/* Hex yazdır */
void terminal_puthex(unsigned int num) {
    char hex_chars[] = "0123456789ABCDEF";
    char buffer[11];
    int i = 0;
    
    terminal_writestring("0x");
    
    do {
        buffer[i++] = hex_chars[num & 0xF];
        num >>= 4;
    } while (num > 0);
    
    while (i > 0) {
        terminal_putchar(buffer[--i]);
    }
}

/* Kernel başlangıç noktası */
void kernel_main() {
    /* Terminali başlat */
    terminal_setcolor(make_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    terminal_clear();
    
    /* Karşılama mesajı */
    terminal_writestring("========================================\n");
    terminal_writestring("       MyOS - Linux Tabanlı İşletim Sistemi\n");
    terminal_writestring("========================================\n\n");
    
    terminal_writestring("Kernel basariyla yuklendi!\n");
    terminal_writestring("Sürüm: 0.0.1-alpha\n\n");
    
    terminal_writestring("Sistem bilgileri:\n");
    terminal_writestring("  - VGA Text Mode: 80x25\n");
    terminal_writestring("  - Bellek: Korunmalı mod (32-bit)\n");
    terminal_writestring("  - Boot adresi: 0x7C00\n");
    terminal_writestring("  - Kernel adresi: 0x1000\n\n");
    
    terminal_setcolor(make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    terminal_writestring("Sistem hazir...\n");
    
    /* Sonsuz döngü - şimdilik */
    while (1) {
        __asm__ volatile("hlt");
    }
}
