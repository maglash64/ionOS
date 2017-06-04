/*
  This file contains sub routine's to program the system timer.
*/

#include "../../include/system/system.h"

static uint32_t system_timer;
static uint16_t timer_frequency;

extern void sleep(uint32_t time);
extern void timer_asm(void);

void timer_callback(void)
{
  system_timer++;
  timer_asm();
}

void setup_timer(uint16_t frequency)
{
  port_outb(0x43,0x36);

  timer_frequency = frequency;

  uint32_t div = 1193180/frequency;

  uint8_t l = (uint8_t)(div & 0xFF);
  uint8_t h = (uint8_t)((div >> 8) & 0xFF);

  port_outb(0x40,l);
  port_outb(0x40,h);

  set_irq_callback_fn(0,timer_callback);
}

uint32_t get_timer_ticks()
{
  return system_timer;
}
