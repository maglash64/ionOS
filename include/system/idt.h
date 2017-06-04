/*
  this file contains the functions that help setup the IDT.
  it also contains ISR's definations & Interupt Service Routines
  that will be called if an Interupt is raised.
  Made By : Ankit Sharma : maglash64@gmail.com
*/

#pragma once

#define ISR_DIVIDE_BY_ZERO    "Divide By Zero exception!"
#define ISR_DEBUG             "Debug exception!"
#define ISR_NMI               "Non Maskable Interupt exception!"
#define ISR_BREAKPOINT        "Breakpoint exception!"
#define ISR_OVERFLOW          "Into detected overflow exception!"
#define ISR_OUT_OF_BOUND      "Out of bound exception!"
#define ISR_INVALID_OPCODE    "Invalid Opcode!"
#define ISR_NO_COP            "No CoProcessor!"
#define ISR_DOUBLE_FAULT      "Double Fault occured!"
#define ISR_COP_SOR           "CoProcessor Segment Overrun!"
#define ISR_BAD_TSS           "Bad TSS!"
#define ISR_SEGMENT_ERROR     "Segment not present!"
#define ISR_STACK_FAULT       "Stack Fault!"
#define ISR_GPF               "General Protection Fault!"
#define ISR_PAGE_FAULT        "Page Fault!"
#define ISR_UNKNOWN           "Unknown Interupt exception!"
#define ISR_COP_FAULT         "CoProcessor Fault!"
#define ISR_ALIGNMENT_FAULT   "Alignment Fault!"
#define ISR_MCE               "Machine Check Exception!"
#define ISR_RESERVED          "Reserved exception thrown!"

#define   PIC_MASTER          0x20
#define   PIC_SLAVE           0xA0
#define   PIC_MASTER_COMMAND  PIC_MASTER
#define   PIC_SLAVE_COMMAND   PIC_SLAVE
#define   PIC_MASTER_DATA     0x21
#define   PIC_SLAVE_DATA      0xA1
#define   PIC_INIT            0x11          // 0x10 = init , 0x01 = 8086/88 mode ,INIT = 0x10 | 0x01

typedef struct _IDT_ENTRY
{
  uint16_t  base_low;
  uint16_t  selector;
  uint8_t   zero;
  uint8_t   flags;
  uint16_t  base_high;
}__attribute__((packed)) IDT_ENTRY;

typedef struct _IDT_PTR
{
  uint16_t  limit;
  uint32_t  base;
}__attribute__((packed)) IDT_PTR;

//a data structure that stores CPU state after an interupt occurs..
typedef struct _REGS
{
  uint32_t  ds;
  uint32_t  edi,esi,ebp,esp,ebx,edx,ecx,eax;  //in accordance with pusha
  uint32_t  interupt_no,error_no;
  uint32_t  eip,cs,efalgs,useresp,ss;         //pushed bu cpu
}__attribute__((packed)) REGS;


extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);

extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

extern void idt_install(uint32_t pointer);

typedef void (*interupt_callback_fn)(void);



//these store function pointer to callback functions of respected IRQ handlers.
static interupt_callback_fn irq_callback[16];
static IDT_ENTRY idt_entry[256];
static IDT_PTR   idt_ptr;



void idt_set_gate(uint8_t num,uint32_t base,uint16_t selector,uint8_t flags)
{
  idt_entry[num].base_low = base & 0xFFFF;
  idt_entry[num].base_high = (base >> 16) & 0xFFFF;
  idt_entry[num].selector = selector;
  idt_entry[num].flags = flags;
  idt_entry[num].zero = 0;
}

void setup_idt()
{
  idt_ptr.limit = sizeof(IDT_ENTRY)*256 - 1;
  idt_ptr.base  = (uint32_t)&idt_entry;

  zeromem((void*)&idt_entry,256);

  //remap the PIC
  port_outb(PIC_MASTER_COMMAND,PIC_INIT);
  port_outb(PIC_SLAVE_COMMAND,PIC_INIT);
  port_outb(PIC_MASTER_DATA,0x20);       //offset to remap irq0 - irq7 to isr32 - isr39
  port_outb(PIC_SLAVE_DATA,0x28);        //offset to remap irq8 - irq15 to isr40 - isr47
  port_outb(PIC_MASTER_DATA,0x04);       //tell master PIC there is a slave PIC
  port_outb(PIC_SLAVE_DATA,0x02);        //assign cascade identity to slave PIC
  port_outb(PIC_MASTER_DATA,0x01);       //8086/88 mode
  port_outb(PIC_SLAVE_DATA,0x01);        //8086/88 mode
  port_outb(PIC_MASTER_DATA,0);
  port_outb(PIC_SLAVE_DATA,0);

  //here comes ISR's
  idt_set_gate(0,(uint32_t)isr0,0x08,0x8E);
  idt_set_gate(1,(uint32_t)isr1,0x08,0x8E);
  idt_set_gate(2,(uint32_t)isr2,0x08,0x8E);
  idt_set_gate(3,(uint32_t)isr3,0x08,0x8E);
  idt_set_gate(4,(uint32_t)isr4,0x08,0x8E);
  idt_set_gate(5,(uint32_t)isr5,0x08,0x8E);
  idt_set_gate(6,(uint32_t)isr6,0x08,0x8E);
  idt_set_gate(7,(uint32_t)isr7,0x08,0x8E);
  idt_set_gate(8,(uint32_t)isr8,0x08,0x8E);
  idt_set_gate(9,(uint32_t)isr9,0x08,0x8E);
  idt_set_gate(10,(uint32_t)isr10,0x08,0x8E);
  idt_set_gate(11,(uint32_t)isr11,0x08,0x8E);
  idt_set_gate(12,(uint32_t)isr12,0x08,0x8E);
  idt_set_gate(13,(uint32_t)isr13,0x08,0x8E);
  idt_set_gate(14,(uint32_t)isr14,0x08,0x8E);
  idt_set_gate(15,(uint32_t)isr15,0x08,0x8E);
  idt_set_gate(16,(uint32_t)isr16,0x08,0x8E);
  idt_set_gate(17,(uint32_t)isr17,0x08,0x8E);
  idt_set_gate(18,(uint32_t)isr18,0x08,0x8E);
  for(uint8_t i = 19; i < 32 ; i++)               //all these are Reserved Exception's
    idt_set_gate(i,(uint32_t)isr19,0x08,0x8E);
  //here comes the IRQs
  idt_set_gate(32,(uint32_t)irq0,0x8,0x8E);
  idt_set_gate(33,(uint32_t)irq1,0x8,0x8E);
  idt_set_gate(34,(uint32_t)irq2,0x8,0x8E);
  idt_set_gate(35,(uint32_t)irq3,0x8,0x8E);
  idt_set_gate(36,(uint32_t)irq4,0x8,0x8E);
  idt_set_gate(37,(uint32_t)irq5,0x8,0x8E);
  idt_set_gate(38,(uint32_t)irq6,0x8,0x8E);
  idt_set_gate(39,(uint32_t)irq7,0x8,0x8E);
  idt_set_gate(40,(uint32_t)irq8,0x8,0x8E);
  idt_set_gate(41,(uint32_t)irq9,0x8,0x8E);
  idt_set_gate(42,(uint32_t)irq10,0x8,0x8E);
  idt_set_gate(43,(uint32_t)irq11,0x8,0x8E);
  idt_set_gate(44,(uint32_t)irq12,0x8,0x8E);
  idt_set_gate(45,(uint32_t)irq13,0x8,0x8E);
  idt_set_gate(46,(uint32_t)irq14,0x8,0x8E);
  idt_set_gate(47,(uint32_t)irq15,0x8,0x8E);

  //default the callback fn's to null
  for(uint8_t i = 0;i<16;i++)
    irq_callback[i] = 0;

  idt_install((uint32_t)&idt_ptr);
}

void set_irq_callback_fn(uint8_t irq_num,interupt_callback_fn fn)
{
  if(irq_num > 16 || fn == 0)
    return;
  irq_callback[irq_num] = fn;
}


void handle_isr(REGS rp)
{
  if(rp.interupt_no < 32)
  {
    set_cursor(0,0);
    set_bg_color(COLOR_BLUE);
    fill_background();
    set_text_color(COLOR_BRIGHT_WHITE);

    if(rp.interupt_no == 0)
      print(ISR_DIVIDE_BY_ZERO);
    else if(rp.interupt_no == 1)
      print(ISR_DEBUG);
    else if(rp.interupt_no == 2)
      print(ISR_NMI);
    else if(rp.interupt_no == 3)
      print(ISR_BREAKPOINT);
    else if(rp.interupt_no == 4)
      print(ISR_OVERFLOW);
    else if(rp.interupt_no == 5)
      print(ISR_OUT_OF_BOUND);
    else if(rp.interupt_no == 6)
      print(ISR_INVALID_OPCODE);
    else if(rp.interupt_no == 7)
      print(ISR_NO_COP);
    else if(rp.interupt_no == 8)
      print(ISR_DOUBLE_FAULT);
    else if(rp.interupt_no == 9)
      print(ISR_COP_SOR);
    else if(rp.interupt_no == 10)
      print(ISR_BAD_TSS);
    else if(rp.interupt_no == 11)
        print(ISR_SEGMENT_ERROR);
    else if(rp.interupt_no == 12)
      print(ISR_STACK_FAULT);
    else if(rp.interupt_no == 13)
      print(ISR_GPF);
    else if(rp.interupt_no == 14)
      print(ISR_PAGE_FAULT);
    else if(rp.interupt_no == 15)
      print(ISR_UNKNOWN);
    else if(rp.interupt_no == 16)
      print(ISR_COP_FAULT);
    else if(rp.interupt_no == 17)
      print(ISR_ALIGNMENT_FAULT);
    else if(rp.interupt_no == 18)
      print(ISR_MCE);
    else if(rp.interupt_no == 19)
      print(ISR_RESERVED);
    else
      print("Undefined CPU exception occured!");

    while(1); //halt the CPU

  }
  if(rp.interupt_no >= 32 && rp.interupt_no <=47)
  {
      if(rp.interupt_no > 39)
      {
          //send End Of Interrupt (EOI) to slave PIC
        port_outb(PIC_SLAVE_COMMAND,PIC_SLAVE);
      }
      //send End Of Interrupt (EOI) to master PIC
      port_outb(PIC_MASTER_COMMAND,PIC_MASTER);


      if(irq_callback[rp.interupt_no - 32] != 0)
        irq_callback[rp.interupt_no - 32]();

  }
}
