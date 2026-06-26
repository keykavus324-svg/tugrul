#include "../include/types.h"
#include "../include/vga.h"
#include "../include/pic.h"

static u32 ticks = 0;

void timer_handler() {
    ticks++;
    
    // Her 18 tickte bir (yaklaşık 1 saniye)
    if (ticks % 18 == 0) {
        vga_set_cursor(79, 0);
        vga_putchar('0' + (ticks / 18) % 10, VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    }
}

void timer_init() {
    ticks = 0;
    // PIC'i başlat
    pic_init();
    // IRQ0 maskesini kaldır (timer)
    pic_clear_mask(0);
}

u32 timer_get_ticks() {
    return ticks;
}
