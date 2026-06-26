/* kernel/src/keyboard.c - Keyboard driver */
#include "../include/keyboard.h"
#include "../include/io.h"

static const char scancode_to_ascii[] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,   'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.'
};

void keyboard_init(void) {
    /* Keyboard is initialized by BIOS, just ensure it's enabled */
}

int keyboard_has_data(void) {
    return inb(KEYBOARD_STATUS_PORT) & 0x01;
}

char keyboard_read_char(void) {
    while (!keyboard_has_data()) {
        __asm__ volatile ("hlt");
    }
    
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    
    /* Check if key press (bit 7 not set) */
    if (!(scancode & 0x80)) {
        if (scancode < sizeof(scancode_to_ascii)) {
            return scancode_to_ascii[scancode];
        }
    }
    
    return 0;
}
