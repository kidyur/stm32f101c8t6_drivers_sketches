/*
 * spi.h
 *
 *  Created on: 12 июл. 2026 г.
 *      Author: kipin
 */

#ifndef INC_DEVICE_DRIVERS_SPI_H_
#define INC_DEVICE_DRIVERS_SPI_H_

#include "stm32f101xb.h"


void SPI_Init();


void SPI_Enable();


void SPI_SendBytes_1(uint8_t data);


#endif /* INC_DEVICE_DRIVERS_SPI_H_ */
