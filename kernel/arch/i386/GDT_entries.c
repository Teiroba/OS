// Used for creating GDT segment descriptors in 64-bit integer form.
 
#include <stdio.h>
#include <stdint.h>
#include <kernel/GDT_entries.h>
 
// Each define here is for a specific flag in the descriptor.
// Refer to the intel documentation for a description of what each one does.
#define SEG_DESCTYPE(x)  ((x) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define SEG_PRES(x)      ((x) << 0x07) // Present, should be 1 except for null descriptor
#define SEG_SAVL(x)      ((x) << 0x0C) // Available for system use, should always be 0
#define SEG_LONG(x)      ((x) << 0x0D) // Long mode, this should always be 0
#define SEG_SIZE(x)      ((x) << 0x0E) // Size (0 for 16-bit, 1 for 32)
#define SEG_GRAN(x)      ((x) << 0x0F) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
#define SEG_PRIV(x)     (((x) &  0x03) << 0x05)   // Set privilege level (0 - 3) (0 = highest)
 
#define SEG_DATA_RD        0x00 // Read-Only
#define SEG_DATA_RDA       0x01 // Read-Only, accessed
#define SEG_DATA_RDWR      0x02 // Read/Write
#define SEG_DATA_RDWRA     0x03 // Read/Write, accessed
#define SEG_DATA_RDEXPD    0x04 // Read-Only, expand-down
#define SEG_DATA_RDEXPDA   0x05 // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD  0x06 // Read/Write, expand-down
#define SEG_DATA_RDWREXPDA 0x07 // Read/Write, expand-down, accessed
#define SEG_CODE_EX        0x08 // Execute-Only
#define SEG_CODE_EXA       0x09 // Execute-Only, accessed
#define SEG_CODE_EXRD      0x0A // Execute/Read
#define SEG_CODE_EXRDA     0x0B // Execute/Read, accessed
#define SEG_CODE_EXC       0x0C // Execute-Only, conforming
#define SEG_CODE_EXCA      0x0D // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC     0x0E // Execute/Read, conforming
#define SEG_CODE_EXRDCA    0x0F // Execute/Read, conforming, accessed
 
#define GDT_CODE_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_CODE_EXRD
 
#define GDT_DATA_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_DATA_RDWR
 
#define GDT_CODE_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_CODE_EXRD
 
#define GDT_DATA_PL3 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(3)     | SEG_DATA_RDWR
 
struct uint64_t gdt[5]
struct gdp


void
create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    uint64_t descriptor;
 
    // Create the high 32 bit segment
    descriptor  =  limit       & 0x000F0000;         // set limit bits 19:16
    descriptor |= (flag <<  8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
    descriptor |= (base >> 16) & 0x000000FF;         // set base bits 23:16
    descriptor |=  base        & 0xFF000000;         // set base bits 31:24
 
    // Shift by 32 to allow for low part of segment
    descriptor <<= 32;
 
    // Create the low 32 bit segment
    descriptor |= base  << 16;                       // set base bits 15:0
    descriptor |= limit  & 0x0000FFFF;               // set limit bits 15:0

    printf("0x%.16llX\n", descriptor);
}
 
void encode_gdt_entry(uint8_t *target, uint32_t base, uint32_t limit, uint16_t flag)
{
    // Check the limit to make sure that it can be encoded
    //if (limit > 0xFFFFF) {kerror("GDT cannot encode limits larger than 0xFFFFF");}
 
    // Encode the limit
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] = (limit >> 16) & 0x0F;
 
    // Encode the base
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;
 
    // Encode the access byte
    target[5] = flag & 0xFF;
 
    // Encode the flags
    target[6] |= (flag >> 8) & 0xF0;
}

void gdt_init(void)
{
    //Pointer to the GDT (GDT descriptor)
    gdp.limit = (sizeof(gdt) - 1);
    gdp.base = (uint32_t)&gdt;

    // target for encoding
    target = (uint8_t *)gdp.base;
    // Null descriptor
    encodeGdtEntry(target, 0, 0, 0);
    target += 8;
 
    // Kernel code segment
    encodeGdtEntry(target, 0, 0x000FFFFF, GDT_CODE_PL0);
    target += 8;
 
    // Kernel data segment
    encodeGdtEntry(target, 0, 0x000FFFFF, GDT_DATA_PL0);
    target += 8;
 
    // User code segment
    encodeGdtEntry(target, 0, 0x000FFFFF, GDT_CODE_PL3);
    target += 8;
 
    // User data segment
    encodeGdtEntry(target, 0, 0x000FFFFF, GDT_DATA_PL3);
    target += 8;

    // Load the new GDT
    asm volatile("lgdtl (%0)" : : "r" (&gdp));

    //This should load the full GDT in the register

    // Reload all the segment registers
    asm volatile("movw $0x10, %ax\n"
                 "movw %ax, %ds\n"
                 "movw %ax, %es\n"
                 "movw %ax, %fs\n"
                 "movw %ax, %gs\n"
                 "movw %ax, %ss\n"
                 "ljmp $0x08, $next\n"
                 "next:\n");

    
}


/*int
main(void)
{
    // Compute the null descriptor, kernel mode code/data segments, and user mode code/data segments
    create_descriptor(0, 0, 0);
    create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0));
    create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));
    create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3));
    create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3));
 
    return 0;
    // TODO : Actually create them instead of just computing them
}*/