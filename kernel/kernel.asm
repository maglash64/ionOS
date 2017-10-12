;this file implements the multiboot header and jumps to the C function while setting the stack.
;made by ankit sharma : maglash64@gmail.com

global start
[extern kmain]
[extern handle_isr]

MODULEALIGN equ 1<<0
MEMINFO     equ 1<<1
MAGIC       equ 0x1BADB002        ;multiboot magic number
FLAGS       equ 0x03              ;multiboot flags
CHECKSUM    equ -(MAGIC + FLAGS)  ;checksum => (magic number + flags + checksum should be 0)

section .multiboot          ;section for the multiboot header
align 4                     ;data should be 4 byte aligned
  dd MAGIC                  ;write the magic number
  dd FLAGS                  ;and the flags
  dd CHECKSUM               ;and the checksum

section .bss
align 4
  kstack: resb 8096         ;reserve memory for stack to be used by HLL

section .data
  sleep_time: dd 0

section .text
start:
  mov   esp,kstack + 8096     ;the base of the stack
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

;***********************************************************************************************

global get_cpuid

get_cpuid:

  ;***clear all reg's
  mov eax,0
  mov ebx,0
  mov ecx,0
  mov edx,0

  ;***get version,stepping,model..
  mov eax,1
  cpuid 
  mov edx,eax               ;store value of eax for further use as eax may be overwriten by other instructions
  mov edi,[esp + 4]         ;retrive the pointer to the CPUID strcture
  mov cl,byte 00001111b     ;value used for and'ing to extract first 3 bits from eax, extract stepping
  and cx,ax                 ;do the shit
  mov [edi + 48],cl         ;save it in structure..
  mov cl,byte 11110000b     ;extract model
  and cx,ax                 
  mov [edi + 49],cl
  mov cx,0   
  mov ch,byte 00001111b     ;extract family
  and cx,ax                 
  mov [edi + 50],cl
    
  ;***get the branding string
  mov eax,0x80000002
  cpuid
  mov [edi],eax
  mov [edi + 4],ebx
  mov [edi + 8],ecx
  mov [edi + 12],edx
  mov eax,0x80000003
  cpuid
  mov [edi + 16],eax
  mov [edi + 20],ebx
  mov [edi + 24],ecx
  mov [edi + 28],edx
  mov eax,0x80000004
  cpuid
  mov [edi + 32],eax
  mov [edi + 36],ebx
  mov [edi + 40],ecx
  mov [edi + 44],edx

  ;***get extended info
  mov eax,1
  cpuid
  mov eax,edx
  and al, 00000001b       ; x87 FPU support
  mov [edi + 51],al
  mov eax,edx
  and al, 00001000b       ; PSE - Page Size Extention (4MiB) 
  mov [edi + 52],al
  mov eax,edx
  and al, 00100000b       ; PAE - Physical Address Extention (64-bit Addressing)
  mov [edi + 53],al
  mov eax,edx
  and eax, 33554432       ; SSE1 support
  mov [edi + 54],eax
  mov eax,edx
  and eax, 67108864       ; SSE2 support
  mov [edi + 55],eax
  mov eax,ecx
  and eax, 0x1            ; SSE3 support
  mov [edi + 56],eax
  mov eax,edx
  and eax, 268435456      ; HT support
  mov [edi + 57],eax
  mov eax,edx
  and eax, 4194305        ; ACPI support
  mov [edi + 58],eax
  mov eax,edx
  and eax, 8388608        ; MMX support
  mov [edi + 59],eax

  ret

  



