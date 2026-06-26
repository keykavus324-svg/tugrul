#ifndef TIMER_H
#define TIMER_H

#include "types.h"

void timer_init();
void timer_handler();
u32 timer_get_ticks();

#endif
