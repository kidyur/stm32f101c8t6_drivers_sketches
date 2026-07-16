/*
 * gpio.c
 *
 *  Created on: 10 июл. 2026 г.
 *      Author: kipin
 */

#include "device_drivers/gpio.h"

#define FULL_CHUNK 4
#define MODE_CHUNK 2
#define RST_FLAGS 0x3ul
#define CRL_PINS 8
#define PORTS_SIZE 5

const uint32_t _portsAddresses[PORTS_SIZE] = {
		(uint32_t)GPIOA,
		(uint32_t)GPIOB,
		(uint32_t)GPIOC,
		(uint32_t)GPIOD,
		(uint32_t)GPIOE
};

const uint32_t _portsEnMasks[PORTS_SIZE] = {
		0x04,
		0x08,
		0x10,
		0x20,
		0x40
};

void GPIO_EnablePort(GPIO_TypeDef * port) {
	for (int i = 0; i < PORTS_SIZE; i++) {
		if ((uint32_t)port == _portsAddresses[i]) {
			RCC->APB2ENR |= _portsEnMasks[i];
			return;
		}
	}
}


void GPIO_SetConfig(GPIO_TypeDef *port,
		const uint16_t pinsFlags,
		enum GPIO_Configuration config) {
	// For Input configurations.
	// We need this decrement to have both I and O configurations
	// under one enum.
	uint32_t cnf = config;
	if (cnf > 3)
		cnf -= 4;

	for (int i = 0; i < 16; i++) {
		if (pinsFlags & (1 << i)) {
			uint8_t pin = i;
			if (pin < CRL_PINS) {
				port->CRL &= ~(RST_FLAGS << (pin * FULL_CHUNK + MODE_CHUNK)); // Reset
				port->CRL |= (cnf << (pin * FULL_CHUNK + MODE_CHUNK)); // Set
			} else {
				pin -= CRL_PINS;
				port->CRH &= ~(RST_FLAGS << (pin * FULL_CHUNK + MODE_CHUNK));
				port->CRH |= (cnf << (pin * FULL_CHUNK + MODE_CHUNK));
			}
		}
	}
}


void GPIO_SetMode(GPIO_TypeDef * port,
		const uint16_t pinsFlags,
		enum GPIO_Mode mode) {
	for (int i = 0; i < 16; i++) {
		if (pinsFlags & (1 << i)) {
			uint8_t pin = i;
			if (pin < CRL_PINS) {
				port->CRL &= ~(RST_FLAGS << (pin * FULL_CHUNK)); // Reset
				port->CRL |= (mode << (pin * FULL_CHUNK)); // Set
			} else {
				pin -= CRL_PINS;
				port->CRH &= ~(RST_FLAGS << (pin * FULL_CHUNK));
				port->CRH |= (mode << (pin * FULL_CHUNK));
			}
		}
	}
}


void GPIO_Init(GPIO_TypeDef *port,
		const uint16_t pinsFlags,
		enum GPIO_Mode mode,
		enum GPIO_Configuration cnf)
{
	GPIO_EnablePort(port);
	GPIO_SetMode(port, pinsFlags, mode);
	GPIO_SetConfig(port, pinsFlags, cnf);
}


void GPIO_WritePins(GPIO_TypeDef *port,
		const uint16_t pinsFlags,
		enum GPIO_State state) {
	if (state)
		port->BSRR = pinsFlags;
	else
		port->BRR = pinsFlags;
}
















