/* kernel.h - Kernel başlık dosyası */

#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>
#include <stdint.h>

/* Terminal fonksiyonları */
void terminal_clear(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_setcolor(uint8_t color);
void terminal_putint(int num);
void terminal_puthex(unsigned int num);
void terminal_setpos(size_t x, size_t y);

/* String uzunluğu */
static inline size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

#endif /* KERNEL_H */
