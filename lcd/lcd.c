/*
 * lcd.c
 *
 *  Created on: 17 июл. 2026 г.
 *      Author: kipin
 */


#include "device_drivers/lcd.h"

#define DATA_SIZE 8
#define GEN_PUSH_PULL_2MHZ 0x2
#define RESET_MASK 0xFul
#define PINS_SIZE 16
#define HALF_PINS_SIZE 8
#define CHUNK 4
#define HIGH 1
#define LOW  0

#define SYSTICK_CTRL_ENABLE  1ul
#define SYSTICK_CTRL_CNTFLAG (1ul << 16)
#define SYSTICK_CTRL_CLKPROC (1ul << 2)
#define SYSTICK_LOAD_MICROS 8ul
#define SYSTICK_LOAD_MILLIS 8000ul

#define ECYCLE_MIN_40US 50
#define ECYCLE_MIN_1p53MS 2


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

static void _waitMillis(const uint16_t millis)
{
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


static void _waitMicros(const uint32_t micros)
{
	// Writes Reload Value
	SysTick->LOAD = SYSTICK_LOAD_MICROS * micros - 1;
	// Enable Timer
	SysTick->CTRL |= SYSTICK_CTRL_ENABLE | SYSTICK_CTRL_CLKPROC;

	// Reload the counter
	SysTick->VAL = 0;
	// _wait for 1 microsecond
	while (!(SysTick->CTRL & SYSTICK_CTRL_CNTFLAG));

	// Disable timer
	SysTick->CTRL &= ~SYSTICK_CTRL_ENABLE;
}


static void _initPin(struct lcd_pin * pin)
{
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
		pin->port->CRL &= ~(RESET_MASK << CHUNK * pin_number);
		pin->port->CRL |= (GEN_PUSH_PULL_2MHZ << CHUNK * pin_number);
	} else {
		// We subtract HALF_PINS_SIZE to fit into CRH where order
		// starts from 8th pin
		pin_number -= HALF_PINS_SIZE;
		pin->port->CRH &= ~(RESET_MASK << CHUNK * pin_number);
		pin->port->CRH |= (GEN_PUSH_PULL_2MHZ << CHUNK * pin_number);
	}
}

static void _initGPIO(void)
{
	_initPin(&_V0);
	_initPin(&_RS);
	_initPin(&_RW);
	_initPin(&_E);
	for (int i = 0; i < DATA_SIZE; i++)
		_initPin(&_D[i]);
}


static void _writePin(struct lcd_pin * pin, enum lcd_pin_state state)
{
	if (state)
		pin->port->BSRR = pin->pin_flag;
	else
		pin->port->BRR = pin->pin_flag;
}


static void _setBus(const uint8_t flags)
{
	for (int i = 0; i < DATA_SIZE; i++) {
		const uint8_t state = (flags & (1 << i)) ? HIGH : LOW;
		_writePin(&_D[i], state);
	}
}

static void _pulseLatch(void)
{
	_writePin(&_E, HIGH);
	_waitMicros(600);
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
	_waitMicros(ECYCLE_MIN_40US);
}


void lcd_switchDisplay(
		const bool displayOn,
		const bool cursorOn,
		const bool blinkOn)
{
	uint8_t flags = 0x08ul;
	if (displayOn) flags |= 0x04ul;
	if (cursorOn)  flags |= 0x02ul;
	if (blinkOn)   flags |= 0x01ul;
	_sendCmd(LOW, LOW, flags);
	_waitMicros(ECYCLE_MIN_40US);
}

static void _runInitSeq(void)
{
	_waitMillis(20);

	_sendCmd(0, 0, 0x30u);
	_waitMillis(10);

	_sendCmd(0, 0, 0x30u);
	_waitMicros(200);

	_sendCmd(0, 0, 0x30u);
	_waitMicros(100);

	lcd_functionSet(1, 2, 0);

	lcd_switchDisplay(0, 0, 0);

	lcd_clearScreen();

	lcd_inputSet(1, 0);

	lcd_switchDisplay(1, 0, 0);
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
	_runInitSeq();
	lcd_cursorReturn();
}


void lcd_clearScreen(void)
{
	_sendCmd(LOW, LOW, 0x01u);
	_waitMillis(ECYCLE_MIN_1p53MS);
}


void lcd_cursorReturn(void)
{
	_sendCmd(LOW, LOW, 0x02u);
	_waitMillis(ECYCLE_MIN_1p53MS);
}


void lcd_inputSet(const bool decrementOn, const bool shiftOn)
{
	uint8_t flags = 0x04u;
	if (decrementOn) flags |= 0x02u;
	if (shiftOn)     flags |= 0x01u;
	_sendCmd(LOW, LOW, flags);
	_waitMicros(ECYCLE_MIN_40US);
}


void lcd_shift(const bool right, const bool fullDisplay)
{
	uint8_t flags = 0x10u;
	if (right)       flags |= 0x04u;
	if (fullDisplay) flags |= 0x08u;
	_sendCmd(LOW, LOW, flags);
	_waitMicros(ECYCLE_MIN_40US);
}


int lcd_functionSet(
		const bool fullBus,
		const int8_t rows,
		const bool extendedGlyphFormat)
{
	if (rows <= 0 || rows > 2)
		return LCD_ERROR_INVALID_ARG;
	uint8_t flags = 0x20u;
	if (fullBus)
		flags |= 0x10u;
	if (rows == 2)
		flags |= 0x08u;
	if (extendedGlyphFormat)
		flags |= 0x04u;
	_sendCmd(LOW, LOW, flags);
	_waitMicros(ECYCLE_MIN_40US);
	return LCD_SUCCESS;
}


static void lcd_read(const uint8_t flags)
{
	_sendCmd(HIGH, HIGH, flags);
	_waitMicros(ECYCLE_MIN_40US);
}


static void _DDRAM_setAddress(const uint8_t addr)
{
	_sendCmd(LOW, LOW, (1ul << 7) | addr);
	_waitMicros(ECYCLE_MIN_40US);
}


static void _CGRAM_setAddress(const uint8_t addr)
{
	_sendCmd(LOW, LOW, (1ul << 6) | addr);
	_waitMicros(ECYCLE_MIN_40US);
}


int lcd_createChar(const uint8_t addr, const uint8_t * matrix)
{
	if (addr > 15)
		return LCD_ERROR_INVALID_ARG;
	_CGRAM_setAddress(addr << 3);
	for (uint8_t i = 0; i < 8; i++)
		lcd_write(matrix[i]);
	lcd_setCursorPosition(0, 0);
	return LCD_SUCCESS;
}


int lcd_setCursorPosition(const int8_t row, const int8_t col)
{
	if (row < 0 || row > 1 || col < 0 || col > 15)
		return LCD_ERROR_INVALID_ARG;
	uint8_t ddramAddress = col;
	if (row == 1)
		ddramAddress |= 0x40;
	_DDRAM_setAddress(ddramAddress);
	return LCD_SUCCESS;
}


void lcd_writeLine(const char * str)
{
	for (int i = 0; str[i] != '\0'; i++)
		lcd_write(str[i]);
}
