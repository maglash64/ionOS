;this file implements the multiboot header and jumps to the C function while setting the stack.
;made by ankit sharma : maglash64@gmail.com

global start
[extern kmain]
[extern handle_isr]

MAGIC     equ 0x1BADB002    ;multiboot magic number
FLAGS     equ 0x00          ;multiboot flags
CHECKSUM  equ -MAGIC        ;checksum => (magic number + flags + checksum should be 0)

section .multiboot:         ;section for the multiboot header
align 4                     ;data should be 4 byte aligned
  dd MAGIC                  ;write the magic number
  dd FLAGS                  ;and the flags
  dd CHECKSUM               ;and the checksum

section .bss
align 4
  kstack: resb 8096        ;reserve memory for stack to be used by HLL

section .data
  sleep_time: dd 0

section .text
start:
  mov   esp,kstack            ;the base of the stack
  mov   ebp,esp
  jmp   kmain                 ;jump to our C function

;***********************************************************************************************

global sleep
global timer_asm

timer_asm:
  push eax
  mov  eax,sleep_time
  or   eax,eax
  jz   timer_done
  dec  dword [sleep_time]
timer_done:
  pop  eax
  ret

sleep:
  push eax
  mov  eax,[esp + 8]
  mov  [sleep_time],eax
sleep_loop:
  cli
  mov eax,[sleep_time]
  or  eax,eax
  jz  sleep_done
  sti
  nop
  nop
  nop
  nop
  nop
  nop
  jmp sleep_loop
sleep_done:
  sti
  pop eax
  ret



;***********************************************************************************************

global port_outb
global port_inb

port_outb:
  mov al,[esp + 8]
  mov dx,[esp + 4]
  out dx,al
  ret

port_inb:
  xor eax,eax
  mov dx,[esp + 4]
  in  al,dx
  ret

;***********************************************************************************************

global gdt_install

gdt_install:
  mov   eax,[esp + 4]
  lgdt  [eax]

  mov ax,0x10
  mov ds,ax
  mov es,ax
  mov fs,ax
  mov gs,ax
  mov ss,ax
  jmp 0x8:gdt_ret   ;stimulate long jump to stall the instruction pipeline.
gdt_ret:
  ret

;************************************************************************************************

global idt_install

idt_install:
  mov eax,[esp + 4]
  lidt [eax]
  sti
  ret

;****************************************************************************
;ISR's start here
;****************************************************************************

;isr's...
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19

isr0:
  cli
  push  byte 0
  push  byte 0
  jmp   isr_stub
isr1:
  cli
  push  byte 0
  push  byte 1
  jmp   isr_stub
isr2:
  cli
  push  byte 0
  push  byte 2
  jmp   isr_stub
isr3:
  cli
  push  byte 0
  push  byte 3
  jmp   isr_stub
isr4:
  cli
  push  byte 0
  push  byte 4
  jmp   isr_stub
isr5:
  cli
  push  byte 0
  push  byte 5
  jmp   isr_stub
isr6:
  cli
  push  byte 0
  push  byte 6
  jmp   isr_stub
isr7:
  cli
  push  byte 0
  push  byte 7
  jmp   isr_stub
isr8:
  cli
  push  byte 8
  jmp   isr_stub
isr9:
  cli
  push  byte 0
  push  byte 9
  jmp   isr_stub
isr10:
  cli
  push  byte 10
  jmp   isr_stub
isr11:
  cli
  push  byte 11
  jmp   isr_stub
isr12:
  cli
  push  byte 12
  jmp   isr_stub
isr13:
  cli
  push  byte 13
  jmp   isr_stub
isr14:
  cli
  push  byte 14
  jmp   isr_stub
isr15:
  cli
  push  byte 0
  push  byte 15
  jmp   isr_stub
isr16:
  cli
  push  byte 0
  push  byte 16
  jmp   isr_stub
isr17:
  cli
  push  byte 0
  push  byte 17
  jmp   isr_stub
isr18:
  cli
  push  byte 0
  push  byte 18
  jmp   isr_stub
isr19:
  cli
  push  byte 0
  push  byte 19
  jmp   isr_stub


;****************************************************************************
;IRQ's start here
;****************************************************************************

;irq's....
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

irq0:
  cli
  push  byte 0
  push  byte 32
  jmp   isr_stub
irq1:
  cli
  push  byte 0
  push  byte 33
  jmp   isr_stub
irq2:
  cli
  push  byte 0
  push  byte 34
  jmp   isr_stub
irq3:
  cli
  push  byte 0
  push  byte 35
  jmp   isr_stub
irq4:
  cli
  push  byte 0
  push  byte 36
  jmp   isr_stub
irq5:
  cli
  push  byte 0
  push  byte 37
  jmp   isr_stub
irq6:
  cli
  push  byte 0
  push  byte 38
  jmp   isr_stub
irq7:
  cli
  push  byte 0
  push  byte 39
  jmp   isr_stub
irq8:
  cli
  push  byte 0
  push  byte 40
  jmp   isr_stub
irq9:
  cli
  push  byte 0
  push  byte 41
  jmp   isr_stub
irq10:
  cli
  push  byte 0
  push  byte 42
  jmp   isr_stub
irq11:
  cli
  push  byte 0
  push  byte 43
  jmp   isr_stub
irq12:
  cli
  push  byte 0
  push  byte 44
  jmp   isr_stub
irq13:
  cli
  push  byte 0
  push  byte 45
  jmp   isr_stub
irq14:
  cli
  push  byte 0
  push  byte 46
  jmp   isr_stub
irq15:
  cli
  push  byte 0
  push  byte 47
  jmp   isr_stub

;*********************************************************************************************

;common stub for ISR and IRQ processing..
[extern handle_isr]

isr_stub:
  pusha       ;push all the general pourpose regs onto stack

  mov  ax,ds
  push eax

  mov  ax,0x10
  mov  ds,ax
  mov  es,ax
  mov  gs,ax
  mov  fs,ax

  call handle_isr

  pop  eax
  mov  ds,ax
  mov  es,ax
  mov  gs,ax
  mov  fs,ax

  popa
  add  esp,8
  sti
  iret
