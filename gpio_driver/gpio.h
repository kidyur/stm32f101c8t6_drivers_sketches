/*
 * gpio.h
 *
 *  Created on: 10 июл. 2026 г.
 *      Author: kipin
 */

#ifndef INC_DEVICE_DRIVERS_GPIO_H_
#define INC_DEVICE_DRIVERS_GPIO_H_

#include "device_drivers/gpio.h"
#include "stm32f101xb.h"


#define GPIO_PortA (uint32_t)0x04
#define GPIO_PortB (uint32_t)0x08
#define GPIO_PortC (uint32_t)0x10

void GPIO_PortEnable(uint32_t port);


// GPIO Output and Input Configurations
#define GPIO_In_AnalogMode (uint32_t)0x00
#define GPIO_In_FloatingInputMode (uint32_t)0x01
#define GPIO_In_PullUpDown (uint32_t)0x02
#define GPIO_In_Reserved (uint32_t)0x03

#define GPIO_Out_GenPushPull (uint32_t)0x00
#define GPIO_Out_GenOpenDrain (uint32_t)0x01
#define GPIO_Out_AltPushPull (uint32_t)0x02
#define GPIO_Out_AltOpenDrain (uint32_t)0x03

void GPIO_SetConfig(GPIO_TypeDef *port, uint32_t pin, uint32_t config);


// GPIO Speed Mode
#define GPIO_InputMode (uint32_t)0x00
#define GPIO_OutputMode_10MHz (uint32_t)0x01
#define GPIO_OutputMode_2MHz (uint32_t)0x02
#define GPIO_OutputMode_50MHz (uint32_t)0x03

void GPIO_SetMode(GPIO_TypeDef * port, uint32_t pin, uint32_t mode);


void GPIO_Init(GPIO_TypeDef *port,
		uint32_t pin,
		uint32_t mode,
		uint32_t configuration);


void GPIO_WritePin(GPIO_TypeDef *port, uint32_t pin, uint32_t state);


void GPIO_AtomicSetPins(GPIO_TypeDef * port, uint16_t flags);


void GPIO_AtomicResetPins(GPIO_TypeDef * port, uint16_t flags);


















#endif /* INC_DEVICE_DRIVERS_GPIO_H_ */
