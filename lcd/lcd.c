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
#define CHUNK 4
#define HIGH 1
#define LOW  0

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
#define SYSTICK_LOAD_MICROS 8ul
#define SYSTICK_LOAD_MILLIS 8000ul


void waitMillis(const uint16_t millis) {
	// Writes Reload Value
	SysTick->LOAD = SYSTICK_LOAD_MILLIS * millis - 1ul;
	// Dummy set for Initialization
	// Enable Timer
	SysTick->CTRL |= SYSTICK_CTRL_ENABLE | SYSTICK_CTRL_CLKPROC;
	SysTick->VAL = 0;

	while (!(SysTick->CTRL & SYSTICK_CTRL_CNTFLAG));

	// Disable timer
	SysTick->CTRL &= ~SYSTICK_CTRL_ENABLE;
}


void waitMicros(const uint32_t micros) {
	// Writes Reload Value
	SysTick->LOAD = SYSTICK_LOAD_MICROS * micros - 1;
	// Enable Timer
	SysTick->CTRL |= SYSTICK_CTRL_ENABLE | SYSTICK_CTRL_CLKPROC;

	// Reload the counter
	SysTick->VAL = 0;
	// Wait for 1 microsecond
	while (!(SysTick->CTRL & SYSTICK_CTRL_CNTFLAG));

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
	while (pin_mask != 1) {
		pin_mask >>= 1;
		pin_number++;
	}
	if (pin_number < HALF_PINS_SIZE) {
		pin->port->CRL &= ~(0xFul << CHUNK * pin_number);
		pin->port->CRL |= (GEN_PUSH_PULL_2MHZ << CHUNK * pin_number);
	} else {
		// We subtract HALF_PINS_SIZE to fit into CRH where order
		// starts from 8th pin
		pin_number -= HALF_PINS_SIZE;
		pin->port->CRH &= ~(0xFul << CHUNK * pin_number);
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


static void _setBus(const uint8_t flags) {
	for (int i = 0; i < DATA_SIZE; i++) {
		const uint8_t state = (flags & (1 << i)) ? 1 : 0;
		_writePin(&_D[i], state);
	}
}

static void _pulseLatch()
{
	_writePin(&_E, HIGH);
	waitMicros(1000);
	_writePin(&_E, LOW);
}



static void _sendCmd(
		const uint8_t RSFlag,
		const uint8_t RWFlag,
		const uint8_t dataFlags)
{
	_setBus(dataFlags);
	_writePin(&_RS, RSFlag);
	_writePin(&_RW, RWFlag);
	_pulseLatch();
}


void lcd_write(const uint8_t data)
{
	_sendCmd(HIGH, LOW, data);
}


void lcd_switchDisplay(
		const uint8_t displayOn,
		const uint8_t cursorOn,
		const uint8_t blinkOn)
{
	uint8_t flags = 0x08ul;
	if (displayOn) flags |= 0x04ul;
	if (cursorOn)  flags |= 0x02ul;
	if (blinkOn)   flags |= 0x01ul;
	_sendCmd(LOW, LOW, flags);
}

void lcd_init(
		struct lcd_pin * V0,
		struct lcd_pin * RS,
		struct lcd_pin * RW,
		struct lcd_pin * E,
		struct lcd_pin D[8])
{
	const uint32_t IS_NOT_STD_LAYOUT = (uint32_t)V0->port;
	if (IS_NOT_STD_LAYOUT) {
		_V0 = *V0;
		_RS = *RS;
		_RW = *RW;
		_E = *E;
		for (int i = 0; i < DATA_SIZE; i++)
			_D[i] = D[i];
	}

	_initGPIO();

	// Wait time > 15ms after Power On
	waitMillis(20);

	_sendCmd(0, 0, 0x30u);
	// Wait time > 4.1ms after previous instruction
	waitMillis(10);

	_sendCmd(0, 0, 0x30u);
	// Wait time > 100us after previous instruction
	waitMicros(200);

	_sendCmd(0, 0, 0x30u);

	_sendCmd(0, 0, 0x20u);

	_sendCmd(0, 0, 0x20u);
	_sendCmd(0, 0, 0xC0u);

	_sendCmd(0, 0, 0x00u);
	_sendCmd(0, 0, 0x80u);

	_sendCmd(0, 0, 0x00u);
	_sendCmd(0, 0, 0x10u);

	_sendCmd(0, 0, 0x00u);
	_sendCmd(0, 0, 0x70u);
}


void lcd_clearScreen()
{
	_sendCmd(LOW, LOW, 0x01u);
}

void lcd_cursorReturn()
{
	_sendCmd(LOW, LOW, 0x02u);
}

void lcd_inputSet(const uint8_t isIncrement, const uint8_t isShift)
{
	uint8_t flags = 0x04u;
	if (isIncrement) flags |= 0x02u;
	if (isShift)     flags |= 0x01u;
	_sendCmd(LOW, LOW, flags);
}


