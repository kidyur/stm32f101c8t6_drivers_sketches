/*
 * gpio.c
 *
 *  Created on: 10 июл. 2026 г.
 *      Author: kipin
 */

#include "device_drivers/gpio.h"


void GPIO_PortEnable(uint32_t port) {
	  RCC->APB2ENR |= port;
}

void GPIO_SetConfig(GPIO_TypeDef *port, uint32_t pin, uint32_t config) {
	if (pin < 8) {
		uint32_t rst_mask = ~((uint32_t)0x3 << (pin * 4 + 2));
		port->CRL &= rst_mask;
		uint32_t mode_mask = config << (pin * 4 + 2);
		port->CRL |= mode_mask;
	} else {
		uint32_t rst_mask = ~((uint32_t)0x3 << ((pin - 8) * 4 + 2));
		port->CRH &= rst_mask;
		uint32_t mode_mask = config << ((pin - 8) * 4 + 2);
		port->CRH |= mode_mask;
	}
}


void GPIO_SetMode(GPIO_TypeDef * port, uint32_t pin, uint32_t mode) {
	if (pin < 8) {
		uint32_t rst_mask = ~((uint32_t)0x3 << (pin * 4));
		port->CRL &= rst_mask;
		uint32_t mode_mask = mode << (pin * 4);
		port->CRL |= mode_mask;
	} else {
		uint32_t rst_mask = ~((uint32_t)0x3 << ((pin - 8) * 4));
		port->CRH &= rst_mask;
		uint32_t mode_mask = mode << ((pin - 8) * 4);
		port->CRH |= mode_mask;
	}
}

void GPIO_Init(GPIO_TypeDef *port,
		uint32_t pin,
		uint32_t mode,
		uint32_t configuration)
{
	GPIO_SetMode(port, pin, mode);
	GPIO_SetConfig(port, pin, configuration);
}

void GPIO_WritePin(GPIO_TypeDef *port, uint32_t pin, uint32_t state) {
	uint32_t rst_mask = ~(1 << pin);
	port->ODR &= rst_mask;
	uint32_t make_mask = state << pin;
	port->ODR |= make_mask;
}

void GPIO_AtomicSetPins(GPIO_TypeDef * port, uint16_t flags) {
	port->BSRR = flags;
}

void GPIO_AtomicResetPins(GPIO_TypeDef * port, uint16_t flags) {
	port->BRR = flags;
}
















