global irq_0
global irq_1
global irq_2
global irq_3
global irq_4
global irq_5
global irq_6
global irq_7
global irq_8
global irq_9
global irq_10
global irq_11
global irq_12
global irq_13
global irq_14
global irq_15

; Remapping the PIC to interrupts 32-47

irq_0:
    push byte 0 ; Dummy error code
    push byte 32
    jmp irq_common_stub

irq_1:
    push byte 0 ; Dummy error code
    push byte 33
    jmp irq_common_stub

irq_2:
    push byte 0 ; Dummy error code
    push byte 34
    jmp irq_common_stub

irq_3: 
    push byte 0 ; Dummy error code
    push byte 35
    jmp irq_common_stub

irq_4:
    push byte 0 ; Dummy error code
    push byte 36
    jmp irq_common_stub

irq_5:
    push byte 0 ; Dummy error code
    push byte 37
    jmp irq_common_stub

irq_6:
    push byte 0 ; Dummy error code
    push byte 38
    jmp irq_common_stub

irq_7:
    push byte 0 ; Dummy error code
    push byte 39
    jmp irq_common_stub

irq_8:
    push byte 0 ; Dummy error code
    push byte 40
    jmp irq_common_stub

irq_9:
    push byte 0 ; Dummy error code
    push byte 41
    jmp irq_common_stub

irq_10:
    push byte 0 ; Dummy error code
    push byte 42
    jmp irq_common_stub

irq_11:
    push byte 0 ; Dummy error code
    push byte 43
    jmp irq_common_stub

irq_12:
    push byte 0 ; Dummy error code
    push byte 44
    jmp irq_common_stub

irq_13:
    push byte 0 ; Dummy error code
    push byte 45
    jmp irq_common_stub

irq_14:
    push byte 0 ; Dummy error code
    push byte 46
    jmp irq_common_stub

irq_15:
    push byte 0 ; Dummy error code
    push byte 47
    jmp irq_common_stub



extern irq_handler

irq_common_stub:
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
    mov eax, irq_handler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops CS, EIP, EFLAGS, SS, and ESP