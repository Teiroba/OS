#ifndef _KERNEL_ARCH_I386_GDT_ENTRIES_C_GUARD
#define _KERNEL_ARCH_I386_GDT_ENTRIES_C_GUARD

// Used for creating GDT segment descriptors in 64-bit integer form.
 
#include <stdio.h>
#include <stdint.h>
#include <kernel/GDT_entries.h>
#include <string.h>

// The GDT entry structure
struct gdt_entry
{
    unsigned short limit_low;           // The lower 16 bits of the limit.
    unsigned short base_low;            // The lower 16 bits of the base.
    unsigned char  base_middle;         // The next 8 bits of the base.
    unsigned char  access;              // Access flags, determine what ring this segment can be used in.
    unsigned char  granularity;
    unsigned char  base_high;           // The last 8 bits of the base.

} __attribute__((packed));

// The GDT pointer structure
struct gdp
{
  unsigned short limit;
  unsigned int base;
} __attribute__((packed));
 
struct gdt_entry gdt[6];
struct gdp gp;
extern void gdt_flush();
extern void* _begin_data;
extern void* _begin_bss;
extern void flush_tss();
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

struct tss_entry_struct {
	uint32_t prev_tss; // The previous TSS - with hardware task switching these form a kind of backward linked list.
	uint32_t esp0;     // The stack pointer to load when changing to kernel mode.
	uint32_t ss0;      // The stack segment to load when changing to kernel mode.
	// Everything below here is unused.
	uint32_t esp1; // esp and ss 1 and 2 would be used when switching to rings 1 or 2.
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldt;
	uint16_t trap;
	uint16_t iomap_base;
} __attribute__((packed));
 
typedef struct tss_entry_struct tss_entry_t;

// Note: some of the GDT entry struct field names may not match perfectly to the TSS entries.
tss_entry_t tss_entry;

void gdt_init(void)
{
    /* Setup the GDT pointer and limit */
    gp.limit = (sizeof(struct gdt_entry) * 6) - 1;
    gp.base = &gdt;

    /* Our NULL descriptor */
    encodeGdtEntry(0, 0, 0, 0, 0);

    // Code Segment.
    encodeGdtEntry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Data Segment.
    encodeGdtEntry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // User mode code segment.
    encodeGdtEntry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

    // User mode data segment.
    encodeGdtEntry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    encodeGdtEntry(5, &tss_entry, sizeof(tss_entry), 0x89, 0x0);

	// Ensure the TSS is initially zero'd.
	memset(&tss_entry, 0, sizeof tss_entry);
 
	tss_entry.ss0  = &_begin_data;  // Set the kernel stack segment.
	tss_entry.esp0 = &_begin_bss; // Set the kernel stack pointer.
	//note that CS is loaded from the IDT entry and should be the regular kernel code segment

    /* Reload the segment registers */
    gdt_flush(); 
	flush_tss();
}

void set_kernel_stack(uint32_t stack) { // Used when an interrupt occurs
	tss_entry.esp0 = stack;
}

#endif