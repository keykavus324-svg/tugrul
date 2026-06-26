/* kernel/include/keyboard.h */
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define KEY_ESCAPE 0x01
#define KEY_BACKSPACE 0x0E
#define KEY_TAB 0x0F
#define KEY_ENTER 0x1C
#define KEY_CTRL 0x1D
#define KEY_SHIFT 0x2A
#define KEY_ALT 0x38
#define KEY_CAPSLOCK 0x3A
#define KEY_F1 0x3B
#define KEY_F12 0x57

void keyboard_init(void);
char keyboard_read_char(void);
int keyboard_has_data(void);

#endif
