#include "../include/types.h"
#include "../include/io.h"
#include "../include/vga.h"
#include "../include/pic.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_STATUS_READY 0x01

static char keymap_us[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
    0, ' '
};

static char last_char = 0;

char keyboard_get_last_char() {
    return last_char;
}

void keyboard_handler() {
    u8 scancode = inb(KEYBOARD_DATA_PORT);
    
    // Tuş bırakma olayını yoksay (en yüksek bit 1)
    if (scancode & 0x80) {
        return;
    }
    
    if (scancode < sizeof(keymap_us)) {
        char c = keymap_us[scancode];
        if (c != 0) {
            last_char = c;
            vga_putchar(c, VGA_COLOR_WHITE, VGA_COLOR_BLACK);
        }
    }
    
    pic_send_eoi(1);
}

void keyboard_init() {
    last_char = 0;
    pic_clear_mask(1);
}
