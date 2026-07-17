/*
 * lcd.c
 *
 *  Created on: 17 июл. 2026 г.
 *      Author: kipin
 */


#include "device_drivers/lcd.h"

#define DATA_SIZE 8
#define GEN_PUSH_PULL_2MHZ 0x2
#define RST_MASK 0x0
#define PINS_SIZE 16
#define HALF_PINS_SIZE 8
#define CHUNK 8

enum lcd_pin_state {
	lcd_pin_state_low = 0,
	lcd_pin_state_high
};

struct lcd_pin _V0 = { GPIOB, lcd_pin_flag_15 };
struct lcd_pin _RS = { GPIOB, lcd_pin_flag_14 };
struct lcd_pin _RW = { GPIOB, lcd_pin_flag_13 };
struct lcd_pin _E  = { GPIOB, lcd_pin_flag_12 };
struct lcd_pin _D[DATA_SIZE] = {
		{ GPIOA, lcd_pin_flag_0 },
		{ GPIOA, lcd_pin_flag_1 },
		{ GPIOA, lcd_pin_flag_2 },
		{ GPIOA, lcd_pin_flag_3 },
		{ GPIOA, lcd_pin_flag_4 },
		{ GPIOA, lcd_pin_flag_5 },
		{ GPIOA, lcd_pin_flag_6 },
		{ GPIOA, lcd_pin_flag_7 }
};


#define SYSTICK_CTRL_ENABLE  1ul
#define SYSTICK_CTRL_CNTFLAG (1ul << 16)
#define SYSTICK_CTRL_CLKPROC (1ul << 2)
#define SYSTICK_LOAD_MICROS 16
#define SYSTICK_LOAD_MILLIS 16000


static void _waitMillis(const uint32_t millis) {

}


static void _waitMicros(const uint32_t micros) {
	// Writes Reload Value
	SysTick->LOAD = SYSTICK_LOAD_MICROS;
	// Dummy set for Initialization
	SysTick->VAL = 0;
	// Enable Timer
	SysTick->CTRL |= SYSTICK_CTRL_ENABLE | SYSTICK_CTRL_CLKPROC;

	for (int i = 0; i < micros; i++) {
		// Reload the counter
		SysTick->VAL = 0;
		// Wait for 1 microsecond
		while (!(SysTick->CTRL & SYSTICK_CTRL_CNTFLAG));
	}

	// Disable timer
	SysTick->CTRL &= ~SYSTICK_CTRL_ENABLE;
}


static void _initPin(struct lcd_pin * pin) {
	// Enable Port
	switch ((uint32_t)pin->port) {
	case (uint32_t)GPIOA:
			RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
			break;
	case (uint32_t)GPIOB:
			RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
			break;
	case (uint32_t)GPIOC:
			RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
			break;
	}

	// Configure Pin
	uint8_t pin_number = 0;
	uint16_t pin_mask = pin->pin_flag;
	while (pin_mask) {
		pin_mask >>= 1;
		pin_number++;
	}
	if (pin_number < HALF_PINS_SIZE) {
		pin->port->CRL &= ~(RST_MASK << CHUNK * pin_number);
		pin->port->CRL |= (GEN_PUSH_PULL_2MHZ << CHUNK * pin_number);
	} else {
		// We subtract HALF_PINS_SIZE to fit into CRH where order
		// starts from 8th pin
		pin_number -= HALF_PINS_SIZE;
		pin->port->CRH &= ~(RST_MASK << CHUNK * pin_number);
		pin->port->CRH |= (GEN_PUSH_PULL_2MHZ << CHUNK * pin_number);
	}
}

static void _initGPIO() {
	_initPin(&_V0);
	_initPin(&_RS);
	_initPin(&_RW);
	_initPin(&_E);
	for (int i = 0; i < 8; i++)
		_initPin(&_D[i]);
}


static void _writePin(struct lcd_pin * pin, enum lcd_pin_state state) {
	if (state)
		pin->port->BSRR |= pin->pin_flag;
	else
		pin->port->BRR |= pin->pin_flag;
}


void lcd_init(
		struct lcd_pin V0,
		struct lcd_pin RS,
		struct lcd_pin RW,
		struct lcd_pin E,
		struct lcd_pin D[8]) {
	_V0 = V0;
	_RS = RS;
	_RW = RW;
	_E = E;
	for (int i = 0; i < DATA_SIZE; i++)
		_D[i] = D[i];
	_initGPIO();
	_initTIM();
}




