#ifndef __TICK_US_H
#define __TICK_US_H

#include "main.h"

uint32_t tick_delta_time_tick(uint32_t start, uint32_t end);
uint8_t tick_expire_us(uint32_t *last_tick, uint32_t time_us);
void tick_wait_us(uint32_t wait_us);

#endif