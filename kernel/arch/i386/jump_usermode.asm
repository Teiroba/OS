global jump_usermode
extern test_user_function
jump_usermode:
	mov ax, (4 * 8) | 3 ; ring 3 data with bottom 2 bits set for ring 3
	mov ds, ax
	mov es, ax 
	mov fs, ax 
	mov gs, ax ; SS is handled by iret
 
	; set up the stack frame iret expects
	mov eax, esp
	push (4 * 8) | 3 ; data selector
	push eax ; current esp
	pushf ; eflags
	push (3 * 8) | 3 ; code selector (ring 3 code with bottom 2 bits set for ring 3)
	push test_user_function ; instruction address to return to
	iret

;global jump_usermode
;extern test_user_function
;jump_usermode:
;	mov ax, (4 * 8) | 3 ; user data segment with RPL 3
;	mov ds, ax
;	mov es, ax
;	mov fs, ax
;	mov gs, ax ; sysexit sets SS
 
;	; setup wrmsr inputs
;	xor edx, edx ; not necessary; set to 0
;	mov eax, 0x8 ; the segments are computed as follows: CS=MSR+0x10 (0x8+0x10=0x18), SS=MSR+0x18 (0x8+0x18=0x20).
;	mov ecx, 0x174 ; MSR specifier: IA32_SYSENTER_CS
;	wrmsr ; set sysexit segments
 
;	; setup sysexit inputs
;	mov edx, test_user_function ; to be loaded into EIP
;	mov ecx, esp ; to be loaded into ESP
;	sysexit

