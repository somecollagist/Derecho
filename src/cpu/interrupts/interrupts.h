#pragma once

#include <types.h>

typedef struct {
	uint16_t	OffsetLow;
	uint16_t	SegmentSelector;
	uint8_t		IST;
	uint8_t		Attributes;
	uint16_t	OffsetMid;
	uint32_t	OffsetHigh;
	uint32_t	Reserved;
} __attribute__((packed)) IDTEntry;

typedef struct {
	uint16_t	Size;
	uint64_t	Offset;
} __attribute__((packed)) IDTDescriptor;

typedef struct {
	uint64_t	r15;
	uint64_t	r14;
	uint64_t	r13;
	uint64_t	r12;
	uint64_t	r11;
	uint64_t	r10;
	uint64_t	r9;
	uint64_t	r8;
	uint64_t	rbp;
	uint64_t	rdi;
	uint64_t	rsi;
	uint64_t	rdx;
	uint64_t	rcx;
	uint64_t	rbx;
	uint64_t	rax;

	uint64_t	int_num;
	uint64_t	err;

	uint64_t	rip;
	uint64_t	cs;
	uint64_t	rflags;
	uint64_t	rsp;
	uint64_t	ss;
} __attribute__((packed)) InterruptStackFrame;

typedef struct {
	InterruptStackFrame*	rbp;
	uint64_t				rip;
} __attribute__((packed)) StackFrame;

#define IDT_MAX_ENTRIES 256

void InitInterrupts();

void RemapPIC();
void AcknowledgePIC(uint8_t irq);
void InitIDT();
void SetIDTDescriptor(uint8_t vector, void* isr, uint8_t attrs);
void InitIRQ();