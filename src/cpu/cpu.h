#pragma once

#include <types.h>

uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);

void outb(uint16_t port, uint8_t data);
void outw(uint16_t port, uint16_t data);
void outl(uint16_t port, uint32_t data);

void ACPIPowerOff();

// List of useful ports to know

extern uint16_t PM1a_CNT;
extern uint16_t PM1b_CNT;
extern uint16_t PM1a_EVT;
extern uint16_t PM1b_EVT;

// List of useful values for ports to know

extern uint16_t SLP_TYPa;
extern uint16_t SLP_TYPb;
extern uint16_t SLP_EN;	
extern uint16_t SCI_EN;