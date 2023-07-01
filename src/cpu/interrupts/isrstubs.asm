[bits 64]
section .text

%macro ISR_NO_ERR 1
global isr%1
isr%1:
	cli
	push 0
	push %1
	jmp ISRCommon
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
	cli
	nop
	nop
	push %1
	jmp ISRCommon
%endmacro

global ISRTable
ISRTable:
	%macro ISRName 1
	dq isr%1
	%endmacro

	%assign x 0
	%rep 48
	ISRName x
	%assign x x+1
	%endrep

extern ISRHandler
ISRCommon:
	cld
	
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

	mov rdi, rsp

	call ISRHandler

	mov rsp, rax

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
	
	iretq

ISR_NO_ERR	0
ISR_NO_ERR	1
ISR_NO_ERR	2
ISR_NO_ERR	3
ISR_NO_ERR	4
ISR_NO_ERR	5
ISR_NO_ERR	6
ISR_NO_ERR	7
ISR_ERR		8
ISR_NO_ERR	9
ISR_ERR		10
ISR_ERR		11
ISR_ERR		12
ISR_ERR		13
ISR_ERR		14
ISR_NO_ERR	15
ISR_NO_ERR	16
ISR_ERR		17
ISR_NO_ERR	18
ISR_NO_ERR	19
ISR_NO_ERR	20
ISR_NO_ERR	21
ISR_NO_ERR	22
ISR_NO_ERR	23
ISR_NO_ERR	24
ISR_NO_ERR	25
ISR_NO_ERR	26
ISR_NO_ERR	27
ISR_NO_ERR	28
ISR_NO_ERR	29
ISR_ERR		30
ISR_NO_ERR	31
ISR_NO_ERR	32
ISR_NO_ERR	33
ISR_NO_ERR	34
ISR_NO_ERR	35
ISR_NO_ERR	36
ISR_NO_ERR	37
ISR_NO_ERR	38
ISR_NO_ERR	39
ISR_NO_ERR	40
ISR_NO_ERR	41
ISR_NO_ERR	42
ISR_NO_ERR	43
ISR_NO_ERR	44
ISR_NO_ERR	45
ISR_NO_ERR	46
ISR_NO_ERR	47