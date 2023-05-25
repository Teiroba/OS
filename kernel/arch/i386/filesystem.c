#include <stdio.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/GDT_entries.h>
#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/irq.h>
#include <kernel/timer.h>
#include <kernel/keyboard.h>
#include <limits.h>
#include <kernel/malloc.h>

int current_inode = 0;
char* current_folder = "/";
void* fs;

void init_filesystem()
{
    fs = malloc(4096*128); // We will use 128 blocks of 4KB for the filesystem
    init_superblock(fs);
    create_child(fs+0x1000, "H", 1, 1);
    // We will use the first 4KB block for the superblock
    // The next 4KB block will be used for the inode table
    // The next 4KB block will be used for the data block bitmap
    // The next 4KB block will be used for the inode bitmap
    // The next 4KB block will be used for the root directory
    // The next blocks will be used for the data blocks

    // The superblock will contain the following information:
    // 1. The number of blocks in the filesystem
    // 2. The number of inodes in the filesystem
    // 3. The number of data blocks in the filesystem
    // 4. The number of free inodes in the filesystem
    // 5. The number of free data blocks in the filesystem
    // 6. The block number of the inode table
    // 7. The block number of the data block bitmap
    // 8. The block number of the inode bitmap
    // 9. The block number of the root directory
    // 10. The block number of the first data block
    
    // The inode table will contain the following information:
    // 1. The inode number
    // 2. The size of the file (in datablocks)
    // 3. The inode number of the parent directory
    // 4. The inode numbers of the children directories/files
    // 5. The block numbers of the data blocks



}

void init_superblock()
{
    // The superblock will contain the following information:
    // 1. The number of blocks in the filesystem
    // 2. The number of inodes in the filesystem
    // 3. The number of data blocks in the filesystem
    // 4. The number of free inodes in the filesystem
    // 5. The number of free data blocks in the filesystem
    // 6. The block number of the inode table
    // 7. The block number of the data block bitmap
    // 8. The block number of the inode bitmap
    // 9. The block number of the root directory
    // 10. The block number of the first data block
    fs = fs + 0x0000;
    *((uint32_t*)fs) = 128; // The number of blocks in the filesystem
    *((uint32_t*)fs + 1) = 128; // The number of inodes in the filesystem
    *((uint32_t*)fs + 2) = 128; // The number of data blocks in the filesystem
    *((uint32_t*)fs + 3) = 128; // The number of free inodes in the filesystem
    *((uint32_t*)fs + 4) = 128; // The number of free data blocks in the filesystem
    *((uint32_t*)fs + 5) = 1; // The block number of the inode table
    *((uint32_t*)fs + 6) = 2; // The block number of the data block bitmap
    *((uint32_t*)fs + 7) = 3; // The block number of the inode bitmap
    *((uint32_t*)fs + 8) = 4; // The block number of the root directory
    *((uint32_t*)fs + 9) = 5; // The block number of the first data block
}

int get_free_block()
{
    fs = fs + 0x2000;
    for (int i = 6; i < 128; i++) {
        if (*((uint32_t*)fs + i) == 0) {
            *((uint32_t*)fs + i) = 1;
            fs = fs - 0x2000;
            return i;
        }
    }
    fs = fs - 0x2000;
}

int get_free_inode()
{
    fs = fs + 0x3000;
    for (int i = 1; i < 128; i++) {
        if (*((uint32_t*)fs + 3 + i) == 0) {
            *((uint32_t*)fs + 3 + i) = 1;
            fs = fs - 0x3000;
            return i;
        }
    }
    fs = fs - 0x3000;
}

void create_child(void* parent, char* name, uint32_t type, uint32_t size)
{
    // The inode table will contain the following information:
    // 0. The type of the file (0 = directory, 1 = file)
    // 1. The inode number
    // 2. The name of the file
    // 3. The size of the file (in datablocks)
    // 4. The inode number of the parent directory
    // 5. The number of children directories/files
    // 6. The inode numbers of the children directories/files
    // 7. The block numbers of the data blocks
    fs = fs + 0x1000;
    if (*((uint32_t*)parent) != 0) {
        printf("Parent is not a directory\n");
        return;
    }
    if (size > 10) {
        printf("File is too big\n");
        return;
    }
    if (*((uint32_t*)parent + 5) == 10) {
        printf("Directory is full\n");
        return;
    }
    uint32_t parent_inode_number = *((uint32_t*)parent + 1);
    uint32_t child_inode_number = get_free_inode(fs - 0x1000);
    *((uint32_t*)fs + child_inode_number*32 ) = type;
    *((uint32_t*)fs + child_inode_number*32 + 1) = child_inode_number;
    *((uint32_t*)fs + child_inode_number*32 + 2) = name;
    *((uint32_t*)fs + child_inode_number*32+ 3) = size;
    *((uint32_t*)fs + child_inode_number*32 + 4) = parent_inode_number;
    *((uint32_t*)parent + 5) = *((uint32_t*)parent + 5) + 1;
    for (int i = 0; i < *((uint32_t*)parent + 5); i++) {
        if (*((uint32_t*)parent + 6 + i) == 0) {
            *((uint32_t*)parent + 6 + i) = child_inode_number;
            break;
        }
    }
    for (int i = 0; i < size; i++) {
        *((uint32_t*)fs + 6 + i) = get_free_block(fs - 0x1000);
    }
    fs = fs - 0x1000;
}

void delete_file(void* file)
{
    // The inode table will contain the following information:
    // 0. The type of the file (0 = directory, 1 = file)
    // 1. The inode number
    // 2. The name of the file
    // 3. The size of the file (in datablocks)
    // 4. The inode number of the parent directory
    // 5. The number of children directories/files
    // 6. The inode numbers of the children directories/files
    // 7. The block numbers of the data blocks
    fs = fs + 0x1000;
    uint32_t inode_number = *((uint32_t*)file + 1);
    uint32_t parent_inode_number = *((uint32_t*)file + 4);
    for (int i = 0; i < *((uint32_t*)fs + parent_inode_number*32 + 5); i++) {
        if (*((uint32_t*)fs + parent_inode_number*32 + 6 + i) == inode_number) {
            *((uint32_t*)fs + parent_inode_number*32 + 6 + i) = 0;
            break;
        }
    }
    for (int i = 0; i < *((uint32_t*)fs + inode_number*32 + 3); i++) {
        *((uint32_t*)fs + 0x1000 + *((uint32_t*)fs + 7 + i)) = 0;
        *((uint32_t*)fs + 7 + i) = 0;
    }
    *((uint32_t*)fs + inode_number*32) = 0;
    *((uint32_t*)fs + inode_number*32 + 1) = 0;
    *((uint32_t*)fs + inode_number*32 + 2) = 0;
    *((uint32_t*)fs + inode_number*32 + 3) = 0;
    *((uint32_t*)fs + inode_number*32 + 4) = 0;
    for (int i = 0; i < *((uint32_t*)fs + inode_number*32 + 5); i++) {
        fs = fs - 0x1000;
        delete_file(fs + 0x1000 + *((uint32_t*)fs + 0x1000 + inode_number*32 + 6 + i)*32);
        fs = fs + 0x1000;
    }
    *((uint32_t*)fs + inode_number*32 + 5) = 0;


    *((uint32_t*)fs + 0x2000 + 3 + inode_number) = 0;
    fs = fs - 0x1000;
}

void go_to_child(int inode_number)
{
    fs = fs + 0x1000;
    for (int i = 0; i < *((uint32_t*)fs + *((uint32_t*)fs + inode_number*32 + 4)*32 + 5); i++) {
        if (*((uint32_t*)fs + *((uint32_t*)fs + inode_number*32 + 4)*32 + 6 + i) == inode_number) {
            current_inode = inode_number;
            current_folder = *((uint32_t*)fs + inode_number*32 + 2);
            fs = fs - 0x1000;
            return;
        }
    }
    fs = fs - 0x1000;
}

void go_to_parent(int inode_number)
{
    fs = fs + 0x1000;
    current_inode = *((uint32_t*)fs + inode_number*32 + 4);
    current_folder = *((uint32_t*)fs + current_inode*32 + 2);
    fs = fs - 0x1000;
}

void go_to_root()
{
    fs = fs + 0x1000;
    current_inode = 0;
    current_folder = *((uint32_t*)fs + current_inode*32 + 2);
    fs = fs - 0x1000;
}

void list_children()
{
    fs = fs + 0x1000;
    for (int i = 0; i < *((uint32_t*)fs + current_inode*32 + 5); i++) {
        if (*((uint32_t*)fs + current_inode*32 + 6 + i) != 0) {
            printf("%c \n", (*((uint32_t*)fs + ((uint32_t*)fs + current_inode*32 + 6 + i)[0]*32 + 2)));
        }
    }
    fs = fs - 0x1000;
}

void print_file(void* file)
{
    fs = fs + 0x1000;
    uint32_t inode_number = *((uint32_t*)file + 1);
    for (int i = 0; i < *((uint32_t*)fs + inode_number*32 + 3); i++) {
        int *p = ((int *) *((uint32_t*)fs + inode_number*32 + 7 + i))[0]*0x1000;

        printf("%s", *p);
    }
    printf("\n");
    fs = fs - 0x1000;
}