global idt_load
extern idtp
idt_load:
    lidt [_idtp]
    ret