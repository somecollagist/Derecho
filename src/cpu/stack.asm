[bits 64]
[global SetupStack]

[section .text]
SetupStack:
	mov rsp, StackEnd
	ret

[section .bss]
StackBegin:
	resb 0x4000
StackEnd:
