[bits 64]
section .text

%macro IRQ 1
global irq%1
irq%1:
	cli
	push byte 0
	%assign second %1
	%assign second second+32
	push byte second
	jmp IRQCommon
%endmacro

extern IRQHandlerMaster
IRQCommon:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	mov rdi, [rsp + 120]
	mov rsi, rsp

	call IRQHandlerMaster

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax

	add rsp, 16
	sti
	iretq

%assign x 0
%rep 16
IRQ x
%assign x x+1
%endrep