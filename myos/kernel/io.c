#include "../include/types.h"

u8 inb(u16 port) {
    u8 ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outb(u16 port, u8 data) {
    __asm__ volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}

u16 inw(u16 port) {
    u16 ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outw(u16 port, u16 data) {
    __asm__ volatile ("outw %0, %1" : : "a"(data), "Nd"(port));
}

void io_wait() {
    __asm__ volatile ("outb %%al, $0x80" : : "a"(0));
}
