/*
 * kp_SPI1.h
 *
 *  Created on: 12 июл. 2026 г.
 *      Author: kipin
 */

#ifndef INC_DEVICE_DRIVERS_KP_SPI1_H
#define INC_DEVICE_DRIVERS_KP_SPI1_H

#include "stm32f101xb.h"
#include "stdint.h"

#define kp_SPI1_LAYOUT_PRIMARY 0x00
#define kp_SPI1_LAYOUT_SECONDARY 0x01

#define BR_CHUNK_BEG 3

#define kp_SPI1_GPIO_LAYOUT_PRIM 0x00
#define kp_SPI1_GPIO_LAYOUT_SCND 0x01

#define kp_SPI1_ENDIANNESS_MSB_FIRST   0x00
#define kp_SPI1_ENDIANNESS_LSB_FIRST   kp_SPI1_CR1_LSBFIRST


enum kp_SPI1_GPIO_Speed {
	kp_SPI1_GPIO_Speed_10MHz = 0,
	kp_SPI1_GPIO_Speed_2MHz,
	kp_SPI1_GPIO_Speed_50MHz
};

enum kp_SPI1_BAUDRATE_SCALER {
	kp_SPI1_BAUDRATE_SCALER_2 = 0,
	kp_SPI1_BAUDRATE_SCALER_4,
	kp_SPI1_BAUDRATE_SCALER_8,
	kp_SPI1_BAUDRATE_SCALER_16,
	kp_SPI1_BAUDRATE_SCALER_32,
	kp_SPI1_BAUDRATE_SCALER_64,
	kp_SPI1_BAUDRATE_SCALER_128,
	kp_SPI1_BAUDRATE_SCALER_256,
};


void kp_SPI1_Init(const uint8_t gpioLayout,
			  const enum kp_SPI1_GPIO_Speed gpioSpeed,
			  const uint8_t endianness,
			  const enum kp_SPI1_BAUDRATE_SCALER BRScaler);


void kp_SPI1_Stop();


void kp_SPI1_Resume();


void kp_SPI1_SendBytes_1(const uint8_t data);


#endif /* INC_DEVICE_DRIVERS_KP_SPI1 */
