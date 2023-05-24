#include <stdio.h>
#include <stdint.h>
#include <kernel/idt.h>
#include <stdbool.h>
#include <string.h>
#include <kernel/irq.h>
#include <kernel/isr.h>
#include <kernel/syscalls.h>
#include <errno.h>

#undef errno
extern int errno;

extern void syscallsinterrupt();

void syscall_handler(struct regs *r)
{
    if (r -> err_code == 0)
    {
        //exit
    }
    if (r -> err_code == 1)
    {
        //close
    }
    if (r -> err_code == 2)
    {
        //envirom
    }
    if (r -> err_code == 3)
    {
        //execve
    }
    if (r -> err_code == 4)
    {
        //fork
    }
    if (r -> err_code == 5)
    {
        //fstat
    }
    if (r -> err_code == 6)
    {
        //getpid
    }
    if (r -> err_code == 7)
    {
        //isatty
    }
    if (r -> err_code == 8)
    {
        //kill
    }
    if (r -> err_code == 9)
    {
        //link
    }
    if (r -> err_code == 10)
    {
        //lseek
    }
    if (r -> err_code == 11)
    {
        //open
    }
    if (r -> err_code == 12)
    {
        //read
    }
    if (r -> err_code == 13)
    {
        //sbrk
    }
    if (r -> err_code == 14)
    {
        //stat
    }
    if (r -> err_code == 15)
    {
        //times
    }
    if (r -> err_code == 16)
    {
        //unlink
    }
    if (r -> err_code == 17)
    {
        //wait
    }
    if (r -> err_code == 18)
    {
        //write
    }
}

void syscalls_install()
{
    idt_set_descriptor(0x80, (uint32_t)syscallsinterrupt, 0x08, 0x8E);
}

int close(int file) {
  return -1;
}

char *__env[1] = { 0 };
char **environ = __env;

int execve(char *name, char **argv, char **env) {
  errno = ENOMEM;
  return -1;
}

int fork() {
  errno = EAGAIN;
  return -1;
}

int fstat(int file, struct stat *st) {
  st->st_mode = S_IFCHR;
  return 0;
}

int getpid() {
  return 1;
}

int isatty(int file) {
  return 1;
}

int kill(int pid, int sig) {
  errno = EINVAL;
  return (-1);
}

int link(char *old, char *new) {
  errno = EMLINK;
  return -1;
}

int lseek(int file, int ptr, int dir) {
  return 0;
}

int open(const char *name, int flags, int mode) {
  return -1;
}

int read(int file, char *ptr, int len) {
  return 0;
}

int stat (const char *file, struct stat *st) {
  st->st_mode = S_IFCHR;
  return 0;
}

int times(struct tms *buf) {
  return -1;
}

int unlink(char *name) {
  errno = ENOENT;
  return -1;
}

int wait(int *status) {
  errno = ECHILD;
  return -1;
}

int write(int file, char *ptr, int len) {
  return 0;
}

