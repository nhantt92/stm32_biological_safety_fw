#include "tick_us.h"

uint32_t tick_delta_time_tick(uint32_t start, uint32_t end)
{

  uint32_t reload_val = SysTick->VAL - 1;
  uint32_t diff;
  if (end > start)
  {
    diff = end - start;
  }
  else
  {
    //diff = reload_val - (start - end) + 1;
    diff = start - end;
  }
  return diff;
}


uint8_t tick_expire_us(uint32_t *last_tick, uint32_t time_us)
{
  uint32_t tickus, end;
  time_us *= 72;
  end = SysTick->VAL;
  tickus = tick_delta_time_tick(end, *last_tick);
  //tickus = tick_delta_time_tick(*last_tick, end);
  if (tickus < time_us) return 0;
  *last_tick = SysTick->VAL;
  return 1;
}

void tick_wait_us(uint32_t wait_us)
{
  uint32_t start;

  start = SysTick->VAL;
  while(tick_expire_us(&start, wait_us) == 0);
}
