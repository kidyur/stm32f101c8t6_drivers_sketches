/*
 * gpio.h
 *
 *  Created on: 10 июл. 2026 г.
 *      Author: kipin
 */

#ifndef INC_DEVICE_DRIVERS_GPIO_H_
#define INC_DEVICE_DRIVERS_GPIO_H_

#include "stm32f101xb.h"

enum GPIO_Pin {
	GPIO_Pin_0  = (0x0001ul << 0),
	GPIO_Pin_1  = (0x0001ul << 1),
	GPIO_Pin_2  = (0x0001ul << 2),
	GPIO_Pin_3  = (0x0001ul << 3),
	GPIO_Pin_4  = (0x0001ul << 4),
	GPIO_Pin_5  = (0x0001ul << 5),
	GPIO_Pin_6  = (0x0001ul << 6),
	GPIO_Pin_7  = (0x0001ul << 7),
	GPIO_Pin_8  = (0x0001ul << 8),
	GPIO_Pin_9  = (0x0001ul << 9),
	GPIO_Pin_10 = (0x0001ul << 10),
	GPIO_Pin_11 = (0x0001ul << 11),
	GPIO_Pin_12 = (0x0001ul << 12),
	GPIO_Pin_13 = (0x0001ul << 13),
	GPIO_Pin_14 = (0x0001ul << 14),
	GPIO_Pin_15 = (0x0001ul << 15)
};


enum GPIO_Configuration {
	GPIO_Configuration_O_GeneralPushPull = 0x0,
	GPIO_Configuration_O_GeneralOpenDrain,
	GPIO_Configuration_O_AltPushPull,
	GPIO_Configuration_O_AltOpenDrain,
	GPIO_Configuration_I_Analog,
	GPIO_Configuration_I_Floating,
	GPIO_Configuration_I_PullUpDown,
	GPIO_Configuration_I_Reserved
};


enum GPIO_Mode {
	GPIO_Mode_Input = 0x00,
	GPIO_Mode_Output_10MHz,
	GPIO_Mode_Output_2MHz,
	GPIO_Mode_Output_50MHz
};


enum GPIO_State {
	GPIO_State_Low = 0,
	GPIO_State_High
};


void GPIO_EnablePort(GPIO_TypeDef * port);


void GPIO_SetConfig(GPIO_TypeDef *port,
		const uint16_t pinsFlags,
		enum GPIO_Configuration config);


void GPIO_SetMode(GPIO_TypeDef * port,
		const uint16_t pinsFlags,
		enum GPIO_Mode mode);


void GPIO_Init(GPIO_TypeDef *port,
		const uint16_t pinsFlags,
		enum GPIO_Mode mode,
		enum GPIO_Configuration cnf);


void GPIO_WritePins(GPIO_TypeDef *port,
		const uint16_t pinsFlags,
		enum GPIO_State state);


#endif /* INC_DEVICE_DRIVERS_GPIO_H_ */
