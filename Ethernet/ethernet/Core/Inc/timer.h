#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

uint8_t config_clock(void);
uint32_t get_tick(void);
void init_timebase(void);
void delay(uint32_t d_ms);

#endif
