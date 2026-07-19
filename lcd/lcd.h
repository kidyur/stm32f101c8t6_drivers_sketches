/*
 * lcd.h
 *
 *  Created on: 17 июл. 2026 г.
 *      Author: kipin
 */

#ifndef INC_DEVICE_DRIVERS_LCD_H_
#define INC_DEVICE_DRIVERS_LCD_H_


#include "stm32f101xb.h"


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


void lcd_write(const uint8_t data);


void lcd_switchDisplay(
		const uint8_t displayOn,
		const uint8_t cursorOn,
		const uint8_t blinkOn);


void lcd_clearScreen();


void lcd_inputSet(
		const uint8_t isIncrement,
		const uint8_t isShift);


void waitMicros(const uint32_t micros);


void waitMillis(const uint16_t millis);




#endif /* INC_DEVICE_DRIVERS_LCD_H_ */
