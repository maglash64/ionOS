/*
  THIS FILE CONTAINS DATA STRUCTURES TO CREATE A VALID GDT ENTRY AND ROUTINES TO SETUP GDT.
  Made By : Ankit Sharma : maglash64@gmail.com
*/

typedef struct _GDT_ENTRY
{
  uint16_t  limit_low;
  uint16_t  base_low;
  uint8_t   base_mid;
  uint8_t   access;
  uint8_t   granuality;
  uint8_t   base_high;
}__attribute__((packed)) GDT_ENTRY;

typedef struct _GDT_PTR
{
  uint16_t  limit;
  uint32_t  base;
}__attribute__((packed)) GDT_PTR;

static GDT_ENTRY  gdt_entry[5];
static GDT_PTR    gdt_ptr;

//function written in assembly -> kernel.asm
extern void gdt_install(uint32_t pointer);

void gdt_set_gate(uint8_t num,uint32_t base,uint32_t limit,uint8_t access,uint8_t granuality)
{
  gdt_entry[num].base_low   = base & 0xFFFF;
  gdt_entry[num].base_mid   = (base >> 16) & 0xFF;
  gdt_entry[num].base_high  = (base >> 24) & 0xFF;
  gdt_entry[num].limit_low  = limit & 0xFFFF;
  gdt_entry[num].granuality = (limit >> 16) & 0x0F;
  gdt_entry[num].granuality = granuality & 0xF0;
  gdt_entry[num].access     = access;
}

void setup_gdt()
{
  gdt_ptr.limit = (sizeof(GDT_ENTRY)*5 -1);
  gdt_ptr.base  = (uint32_t)&gdt_entry;

  gdt_set_gate(0,0,0,0,0);                  //null segment
  gdt_set_gate(1,0,0xFFFFFFFF,0x9A,0xCF);   //kernel mode code segment
  gdt_set_gate(2,0,0xFFFFFFFF,0x92,0xCF);   //kernel mode data segment
  gdt_set_gate(3,0,0xFFFFFFFF,0xFA,0xCF);   //user mode code segment
  gdt_set_gate(4,0,0xFFFFFFFF,0xF2,0xCF);   //user mode data segment

  gdt_install((uint32_t)&gdt_ptr);
}
