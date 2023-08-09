#include "cpu.h"

#include <types.h>

uint8_t inb(uint16_t port){
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

uint16_t inw(uint16_t port){
	uint16_t ret;
	asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

uint32_t inl(uint16_t port){
	uint32_t ret;
	asm volatile("inl %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

void outb(uint16_t port, uint8_t data){
	asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

void outw(uint16_t port, uint16_t data){
	asm volatile("outw %0, %1" : : "a"(data), "Nd"(port));
}

void outl(uint16_t port, uint32_t data){
	asm volatile("outl %0, %1" : : "a"(data), "Nd"(port));
}

// List of useful ports to know

uint16_t PM1a_CNT;
uint16_t PM1b_CNT;
uint16_t PM1a_EVT;
uint16_t PM1b_EVT;

// List of useful values for ports to know

uint16_t SLP_TYPa;
uint16_t SLP_TYPb;
uint16_t SLP_EN;
uint16_t SCI_EN;