global syscallinterrupt

extern syscall_handler
syscallinterrupt:
    push byte 128 ; 128 is the syscall interrupt number
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor into DS, ES, FS, and GS
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   ; Pushes the stack pointer (esp) into the eax register
    push eax
    mov eax, syscall_handler ; Pushes the address of the syscall_handler function into eax
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops CS, EIP, EFLAGS, SS, and ESP    



