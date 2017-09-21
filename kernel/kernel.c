/*
  MAIN KERNEL SOURCE
  Made By Ankit Sharma : maglash64@gmail.com
*/
#include "../include/system/system.h"
#include "../driver/timer/timer.h"
#include "../driver/keyboard/keyboard.h"

void kmain(void)
{
  terminal_init();
  setup_gdt();
  setup_idt();
  setup_timer(1000);
  setup_keyboard();

  set_bg_color(COLOR_BLACK);
  fill_background();
  set_text_color(COLOR_WHITE);
  set_cursor(0,0);

  print("Hello! Welcome to ion-OS !\n");
  print("root@1337:~");
  set_cursor(terminal_cursor_x,terminal_cursor_y);

  uint8_t old = 0;
  while(1)
  {
    uint8_t key = kbd_read();
    if(key)
    {
      if(key == old)
        continue;
      putc(key);
      sleep(10);
    }
    old = key;
  }
}
