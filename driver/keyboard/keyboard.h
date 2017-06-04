/*
  This file contains functions that help map the US style keyboard.
*/

#include "../../include/system/system.h"


uint8_t kbd_us[128] =
{
  0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
  0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
  '\'', '`',   0,		/* Left shift */
  '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,
};



static uint8_t  kbd_buffer[256];
static uint16_t kbd_buffer_len;

void keyboard_callback(void)
{
  //read from keyboard contrller data register.
  uint8_t scode = port_inb(0x60);

  if(scode & 0x80)  //if top bit of the read byte is set, it means the key has been released.
  {
    kbd_buffer[0] = 0;
  }
  else              // otherwise the key has been pressed.
  {
    /*
    kbd_buffer[kbd_buffer_len] = kbd_us[scode];
    kbd_buffer_len++;
    if(kbd_buffer_len > 255)
      kbd_buffer_len = 0;
    */
    kbd_buffer[0] = kbd_us[scode];
  }
}

void setup_keyboard(void)
{
  zeromem(kbd_buffer,256);
  kbd_buffer_len = 0;
  set_irq_callback_fn(1,keyboard_callback);
}


uint8_t kbd_read(void)
{
  uint8_t ret = kbd_buffer[0];
  if(ret == 0)
    return 0;
  for(uint8_t i = 0;i<kbd_buffer_len;i++)
    kbd_buffer[i] = kbd_buffer[i + 1];
  return ret;
}
