#include "device_drivers/lcd.h"

void lcd_createCharExample() 
{
    uint8_t heart[8] = {
        0b00000,
        0b01010,
        0b11111,
        0b11111,
        0b01110,
        0b00100,
        0b00000,
        0b00000
    };

    lcd_createChar(0, heart);
    lcd_write(0);
}


void init(void)
{
    struct lcd_pin dummy = {0, 0};
    struct lcd_pin dummy_row[8] = {{0, 0}}; 
	lcd_init(&dummy, &dummy, &dummy, &dummy, dummy_row);
	lcd_writeLine("Feel home!");
    while (1);
}

void lcd_makeBlinkingFace() {
    struct lcd_pin dummy = {0, 0};
    struct lcd_pin dummy_row[8] = {{0, 0}};
    lcd_init(&dummy, &dummy, &dummy, &dummy, dummy_row);

	uint8_t left_eye[8] =  {
		0b01111,
		0b10000,
		0b00110,
		0b01001,
		0b10111,
		0b10111,
		0b01111,
	};

	uint8_t right_eye[8] =  {
		0b11110,
		0b00001,
		0b01100,
		0b10010,
		0b11101,
		0b11101,
		0b11110,
	};

	uint8_t right_eye_closed[8] =  {
		0b00000,
		0b11110,
		0b00001,
		0b00000,
		0b11100,
		0b11110,
		0b11110,
	};

	uint8_t mouth[8] =  {
		0b01110,
		0b00000,
		0b00001,
		0b11111,
		0b01110,
		0b00000,
		0b00000,
	};

	lcd_createChar(0, left_eye);
	lcd_createChar(1, right_eye);
	lcd_createChar(3, right_eye_closed);
	lcd_createChar(2, mouth);
	lcd_write(0);
	lcd_write(' ');
	lcd_write(1);
	lcd_setCursorPosition(1, 1);
	lcd_write(2);

  while (1)
  {
	  waitMillis(2000);
	  lcd_setCursorPosition(0, 2);
	  lcd_write(3);
	  waitMillis(300);
	  lcd_setCursorPosition(0, 2);
	  lcd_write(1);
  }
}