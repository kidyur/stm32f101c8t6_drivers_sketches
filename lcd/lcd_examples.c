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