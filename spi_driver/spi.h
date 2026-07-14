/*
 * spi.h
 *
 *  Created on: 12 июл. 2026 г.
 *      Author: kipin
 */

#ifndef INC_DEVICE_DRIVERS_SPI_H_
#define INC_DEVICE_DRIVERS_SPI_H_

#include "stm32f101xb.h"
#include "stdint.h"

#define SPI_LAYOUT_PRIMARY 0x00
#define SPI_LAYOUT_SECONDARY 0x01

#define BR_CHUNK_BEG 3

#define SPI_GPIO_LAYOUT_PRIM 0x00
#define SPI_GPIO_LAYOUT_SCND 0x01

#define SPI_ENDIANNESS_MSB_FIRST   0x00
#define SPI_ENDIANNESS_LSB_FIRST   SPI_CR1_LSBFIRST


enum SPI_GPIO_Speed {
	SPI_GPIO_Speed_10MHz = 0,
	SPI_GPIO_Speed_2MHz,
	SPI_GPIO_Speed_50MHz
};

enum SPI_BAUDRATE_SCALER {
	SPI_BAUDRATE_SCALER_2 = 0,
	SPI_BAUDRATE_SCALER_4,
	SPI_BAUDRATE_SCALER_8,
	SPI_BAUDRATE_SCALER_16,
	SPI_BAUDRATE_SCALER_32,
	SPI_BAUDRATE_SCALER_64,
	SPI_BAUDRATE_SCALER_128,
	SPI_BAUDRATE_SCALER_256,
};


void SPI_Init(const uint8_t gpioLayout,
			  const enum SPI_GPIO_Speed gpioSpeed,
			  const uint8_t endianness,
			  const enum SPI_BAUDRATE_SCALER BRScaler);


void SPI_Stop();


void SPI_Resume();


void SPI_SendBytes_1(const uint8_t data);


#endif /* INC_DEVICE_DRIVERS_SPI_H_ */
