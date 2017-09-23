/*
    This file contains mapping's to external functions those,which cannot be written with HLL's
    without using inline assembly (and i hate AT&T style asm) and some exotic routines.
*/
extern void     port_outb(uint16_t port,uint8_t data);
extern uint8_t  port_inb(uint16_t port);

typedef struct _CPUID{
    char cpu_branding[48];
    uint8_t stepping;
    uint8_t model;
    uint8_t family;
    bool fpu_supported;
    bool pse_supported;
    bool pae_supported;
    bool sse,sse2,sse3;
    bool ht_supported;
    bool acpi_supported;
    bool mmx_supported;
}__attribute__((packed)) CPUID;

extern void get_cpuid(CPUID *p);
