# LCD1602 driver for STM32F10xxx
Driver to control the display. It provides useful functions to
draw symbols on display, make your own symbols and much more what 
you can find in the datasheet (appendix 1, p12). 

You can find examples of its usage in `lcd_examples.c` file.

> **Optional:** You can find waitMicros() and waitMillis() in `lcd.c` file.

# Compatibility
This is Bare Metal driver to connect STM32 to LCD1602 with HD44780 bus.
It works with:
* STM32F101xx
* STM32F102xx
* STM32F103xx
* STM32F105xx
* STM32F107xx

# Installation
> **IMPORTANT:** This driver is only for full bus connecting (8 pins)

Library has only one .h and one .c files. Download them, put into your 
projects folder and change the `#include` directories: 
1. change `#include "stm32f101xb.h"` in `lcd.h` to match your platform
2. change `#include "device_drivers/lcd.h"` in `lcd.c` to match your file tree
Then you'll be able to include the library to `main.c`

# References
1. LCD1602 Datasheet: https://www.vishay.com/docs/37484/lcd016n002bcfhet.pdf
2. STM32F10xxx Reference Manual: https://clck.ru/3UtKgo
3. STM32F10xxx Datasheet: https://www.st.com/resource/en/datasheet/cd00161566.pdf

