/*
 * lcd.h
 *
 *  Created on: 17 июл. 2026 г.
 *      Author: kipin
 */

#ifndef INC_DEVICE_DRIVERS_LCD_H_
#define INC_DEVICE_DRIVERS_LCD_H_


#include "stm32f101xb.h"
#include <stdbool.h>


enum lcd_pin_flag {
	lcd_pin_flag_0  = (0x0001ul << 0),
	lcd_pin_flag_1  = (0x0001ul << 1),
	lcd_pin_flag_2  = (0x0001ul << 2),
	lcd_pin_flag_3  = (0x0001ul << 3),
	lcd_pin_flag_4  = (0x0001ul << 4),
	lcd_pin_flag_5  = (0x0001ul << 5),
	lcd_pin_flag_6  = (0x0001ul << 6),
	lcd_pin_flag_7  = (0x0001ul << 7),
	lcd_pin_flag_8  = (0x0001ul << 8),
	lcd_pin_flag_9  = (0x0001ul << 9),
	lcd_pin_flag_10 = (0x0001ul << 10),
	lcd_pin_flag_11 = (0x0001ul << 11),
	lcd_pin_flag_12 = (0x0001ul << 12),
	lcd_pin_flag_13 = (0x0001ul << 13),
	lcd_pin_flag_14 = (0x0001ul << 14),
	lcd_pin_flag_15 = (0x0001ul << 15)
};

enum LCD_ERROR {
	LCD_SUCCESS = 0,
	LCD_ERROR_INVALID_ARG
};


struct lcd_pin {
	GPIO_TypeDef * port;
	enum lcd_pin_flag pin_flag;
};


/**
 * Pass {0, 0} as V0 argument to agree with default layout
 *
 * [Default layout]
 * A0 - A7 -> D0 - D7
 * B15     -> V0
 * B14     -> RS
 * B13     -> RW
 * B12     -> E
 */
void lcd_init(
		struct lcd_pin * V0,
		struct lcd_pin * RS,
		struct lcd_pin * RW,
		struct lcd_pin * E,
		struct lcd_pin D[8]);



void lcd_switchDisplay(
		const bool displayOn,
		const bool cursorOn,
		const bool blinkOn);


void lcd_clearScreen();


void lcd_inputSet(
		const bool decrementOn,
		const bool shiftOn);


void lcd_cursorReturn();


void lcd_shift(const bool right, const bool fullDisplay);

/**
 * \param  rows Amount of visible rows on the display, 1 <= rows <= 2
 * \throw  LCD_ERROR_INVALID_ARG if 'rows' does not satisfy the constraints
 * \return LCD_SUCCESS           if there is no error
 */

int lcd_functionSet(
		const bool fullBus,
		const int8_t rows,
		const bool extendedGlyphFormat);

/**
 * \brief Use it to create your own character
 * \param addr   Address in CGRAM, 0 <= addr < 16
 * \param matrix Representation of the character,
 *		  set bit - LED on, reset bit - LED off.
 *		  format: 3 higher bits are not used
 *		  		  5 lower bits represent character
 *		  		  7 bytes represent your character
 *		  		  8th (last) byte is for caret
 * \throw  LCD_ERROR_INVALID_ARG if addr doesn't satisfy the constraints
 * \return LCD_SUCCESS           if there is no error
 * \example lcd_examples.c, see lcd_createCharExample();
 */
int lcd_createChar(const uint8_t addr, const uint8_t * matrix);

/**
 * \brief To set cursor position on the display
 * \param row Cursors row, 0 <= row <= 1
 * \param col Cursors col, 0 <= col <= 15
 * \throw  LCD_ERROR_INVALID_ARG if position doesn't satisfy the constraints
 * \return LCD_SUCCESS           if there is no error
 */
int lcd_setCursorPosition(const int8_t row, const int8_t col);

/**
 * \brief You can use it to write a character on the display.
 * 	      It writes a symbol and moves caret, shifts side depends on
 * 	      input mode (see lcd_inputSet())
 * \param data Address of character in CGRAM
 * 		  (see lcd-datasheet.pdf, p.12, Character Generator ROM Pattern)
 * 		  addresses 0x00 - 0x0F for users patterns (see lcd_createChar())
 */
void lcd_write(const uint8_t data);

// \brief Writes a string until first '\0'
void lcd_writeLine(const char * str);


#endif /* INC_DEVICE_DRIVERS_LCD_H_ */
