/*
  This file contains various functions to emulate a terminal.
*/
#pragma once

#define TERMINAL_WIDTH  80
#define TERMINAL_HEIGHT 25

#define COLOR_BLACK           0x00
#define COLOR_BLUE            0x01
#define COLOR_GREEN           0x02
#define COLOR_CYAN            0x03
#define COLOR_RED             0x04
#define COLOR_MAGENTA         0x05
#define COLOR_YELLOW          0x06
#define COLOR_WHITE           0x07
#define COLOR_GRAY            0x08
#define COLOR_BRIGHT_BLUE     0x09
#define COLOR_BRIGHT_GREEN    0x0A
#define COLOR_BRIGHT_CYAN     0x0B
#define COLOR_BRIGHT_RED      0x0C
#define COLOR_BRIGHT_MAGENTA  0x0D
#define COLOR_BRIGHT_YELLOW   0x0E
#define COLOR_BRIGHT_WHITE    0x0F


static uint8_t terminal_cursor_x;
static uint8_t terminal_cursor_y;
static uint8_t terminal_text_color;
static uint8_t terminal_bg_color;

void terminal_init(void)
{
  terminal_cursor_x = 0;
  terminal_cursor_y = 0;
  terminal_text_color = 0x07;
  terminal_bg_color = 0x00;
}

void clear(void)
{
  uint16_t    *vga = (uint16_t*)0xB8000;
  uint16_t    i = 0;
  //clear the screen,if BIOS has not done the same before giving us control..
  for(i = 0;i<TERMINAL_WIDTH*TERMINAL_HEIGHT;++i)
    *(vga + i) =  (uint16_t)0x0007;
}

void fill_background(void)
{
  uint16_t    *vga = (uint16_t*)0xB8000;
  uint16_t    i = 0;
  //clear the screen,if BIOS has not done the same before giving us control..
  for(i = 0;i<TERMINAL_WIDTH*TERMINAL_HEIGHT;++i)
    *(vga + i) =  (uint16_t)(terminal_bg_color) << 8 ;
}

int8_t  hex2char(uint8_t value)
{
    return value <= 9 ? '0' + value : 'A' - 10 + value;
}


//prints hex value..
void debug_hex(uint32_t value)
{
  uint8_t *vga = (uint8_t*)0xB8000;

  vga += (80*(terminal_cursor_y) + terminal_cursor_x)*2;
  *(vga++) = '0';
  *(vga++) = terminal_bg_color | terminal_text_color;
  *(vga++) = 'x';
  *(vga++) = terminal_bg_color | terminal_text_color;
  *(vga++) =  hex2char(((value & 0xF0000000) >> 28));
  *(vga++) = terminal_bg_color | terminal_text_color;
  *(vga++) =  hex2char(((value & 0x0F000000) >> 24));
  *(vga++) = terminal_bg_color | terminal_text_color;
  *(vga++) =  hex2char(((value & 0x00F00000) >> 20));
  *(vga++) = terminal_bg_color | terminal_text_color;
  *(vga++) =  hex2char(((value & 0x000F0000) >> 16));
  *(vga++) = terminal_bg_color | terminal_text_color;
  *(vga++) =  hex2char(((value & 0xF000) >> 12));
  *(vga++) = terminal_bg_color | terminal_text_color;
  *(vga++) =  hex2char(((value & 0x0F00) >> 8));
  *(vga++) = terminal_bg_color | terminal_text_color;
  *(vga++) =  hex2char(((value & 0x00F0) >> 4));
  *(vga++) = terminal_bg_color | terminal_text_color;
  *(vga++) =  hex2char((value & 0x000F));
  *(vga++) = terminal_bg_color | terminal_text_color;
}

void set_cursor(uint8_t x,uint8_t y)
{
  terminal_cursor_x = x;
  terminal_cursor_y = y;

  uint16_t cpos = terminal_cursor_y * 80 + terminal_cursor_x;
  port_outb(0x3D4,14);
  port_outb(0x3D5, cpos >> 8);
  port_outb(0x3D4,15);
  port_outb(0x3D5, cpos);
}

void set_bg_color(uint8_t color)
{
  terminal_bg_color = color << 4;
}

void set_text_color(uint8_t color)
{
  terminal_text_color = color;
}

void putc(char c)
{
  uint16_t   *vga = (uint16_t*)0xB8000;
  do
  {
    if(c == '\n')
    {
      terminal_cursor_x = 0,terminal_cursor_y++;
      break;
    }

    if(c == '\t')
    {
        if(terminal_cursor_x + 5 < 80)
          terminal_cursor_x+=5;
        else
          terminal_cursor_x += ( terminal_cursor_x + 5 - 80),terminal_cursor_y++;
        break;
    }

    if(c == '\b')
    {
      if(terminal_cursor_x > 0)
        terminal_cursor_x--;
      else if(terminal_cursor_x == 0)
      {
        if(terminal_cursor_y > 0)
          terminal_cursor_y--;
        terminal_cursor_x = 79;
      }
      *(vga + 80*(terminal_cursor_y) + terminal_cursor_x) = (uint16_t)((terminal_bg_color | terminal_text_color) << 8 | ' ');
      set_cursor(terminal_cursor_x,terminal_cursor_y);
      return;
    }

    *(vga + 80*(terminal_cursor_y) + terminal_cursor_x) = (uint16_t)((terminal_bg_color | terminal_text_color) << 8 | c);

    if(terminal_cursor_x >= 79)
      terminal_cursor_x = 0,++terminal_cursor_y;
    else
      ++terminal_cursor_x;

    terminal_cursor_y > 24 ? terminal_cursor_y = 0 : 0 ;

  }while(0);

  set_cursor(terminal_cursor_x,terminal_cursor_y);
}

void print(char *str)
{
  for(uint32_t i = 0;*(str + i) != '\0';++i)
  {
    putc(*(str + i));
  }
}
