#ifndef FILESYSTEM_H
#define FILESYSTEM_H

void init_filesystem();
void init_superblock();
int get_free_block();
int get_free_inode();
void create_child(void* parent, char* name, uint32_t type, uint32_t size);
void delete_file(void* file);
void go_to_child(int inode_number);
void go_to_parent();
void go_to_root();
void list_children();
void print_file(void* file);

#endif
