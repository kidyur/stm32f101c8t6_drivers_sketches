/*
 * kp_GPIO.h
 *
 *  Created on: 10 июл. 2026 г.
 *      Author: kipin
 */

#ifndef INC_DEVICE_DRIVERS_GPIO_KP_H_
#define INC_DEVICE_DRIVERS_GPIO_KP_H_

#include "stm32f101xb.h"

enum kp_GPIO_Pin {
	kp_GPIO_Pin_0  = (0x0001ul << 0),
	kp_GPIO_Pin_1  = (0x0001ul << 1),
	kp_GPIO_Pin_2  = (0x0001ul << 2),
	kp_GPIO_Pin_3  = (0x0001ul << 3),
	kp_GPIO_Pin_4  = (0x0001ul << 4),
	kp_GPIO_Pin_5  = (0x0001ul << 5),
	kp_GPIO_Pin_6  = (0x0001ul << 6),
	kp_GPIO_Pin_7  = (0x0001ul << 7),
	kp_GPIO_Pin_8  = (0x0001ul << 8),
	kp_GPIO_Pin_9  = (0x0001ul << 9),
	kp_GPIO_Pin_10 = (0x0001ul << 10),
	kp_GPIO_Pin_11 = (0x0001ul << 11),
	kp_GPIO_Pin_12 = (0x0001ul << 12),
	kp_GPIO_Pin_13 = (0x0001ul << 13),
	kp_GPIO_Pin_14 = (0x0001ul << 14),
	kp_GPIO_Pin_15 = (0x0001ul << 15)
};


enum kp_GPIO_Configuration {
	kp_GPIO_Configuration_O_GeneralPushPull = 0x0,
	kp_GPIO_Configuration_O_GeneralOpenDrain,
	kp_GPIO_Configuration_O_AltPushPull,
	kp_GPIO_Configuration_O_AltOpenDrain,
	kp_GPIO_Configuration_I_Analog,
	kp_GPIO_Configuration_I_Floating,
	kp_GPIO_Configuration_I_PullUpDown,
	kp_GPIO_Configuration_I_Reserved
};


enum kp_GPIO_Mode {
	kp_GPIO_Mode_Input = 0x00,
	kp_GPIO_Mode_Output_10MHz,
	kp_GPIO_Mode_Output_2MHz,
	kp_GPIO_Mode_Output_50MHz
};


enum kp_GPIO_State {
	kp_GPIO_State_Low = 0,
	kp_GPIO_State_High
};


void kp_GPIO_EnablePort(GPIO_TypeDef * port);


void kp_GPIO_SetConfig(GPIO_TypeDef *port,
		const uint16_t pinsFlags,
		enum kp_GPIO_Configuration config);


void kp_GPIO_SetMode(GPIO_TypeDef * port,
		const uint16_t pinsFlags,
		enum kp_GPIO_Mode mode);


void kp_GPIO_Init(GPIO_TypeDef *port,
		const uint16_t pinsFlags,
		enum kp_GPIO_Mode mode,
		enum kp_GPIO_Configuration cnf);


void kp_GPIO_WritePins(GPIO_TypeDef *port,
		const uint16_t pinsFlags,
		enum kp_GPIO_State state);


#endif /* INC_DEVICE_DRIVERS_kp_GPIO_H_ */
