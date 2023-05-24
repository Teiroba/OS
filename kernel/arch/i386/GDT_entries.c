// Used for creating GDT segment descriptors in 64-bit integer form.
 
#include <stdio.h>
#include <stdint.h>
#include <kernel/GDT_entries.h>
 
struct gdt_entry gdt[3];
struct gdp gp;
extern void gdt_flush();

/* Setup a descriptor in the Global Descriptor Table */
void encodeGdtEntry(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
    /* Setup the descriptor base address */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void gdt_init(void)
{
    /* Setup the GDT pointer and limit */
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = &gdt;

    /* Our NULL descriptor */
    encodeGdtEntry(0, 0, 0, 0, 0);

    // Code Segment.
    encodeGdtEntry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Data Segment.
    encodeGdtEntry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    /* Reload the segment registers */
    gdt_flush(); 
}