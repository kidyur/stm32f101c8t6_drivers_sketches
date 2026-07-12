/*
 * spi.c
 *
 *  Created on: 12 июл. 2026 г.
 *      Author: kipin
 */

#include "device_drivers/spi.h"


void SPI_Enable() {
	SPI1->CR1 |= SPI_CR1_SPE;
}


void SPI_Init() {
	// Stop the SPI communication
	SPI1->CR1 &= ~SPI_CR1_SPE;

	// Setting it
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR | SPI_CR1_BR_0 | SPI_CR1_BR_2;

	// Receive Buffer is not Empty
	SPI1->CR2 |= SPI_CR2_RXNEIE;
}


void SPI_SendBytes_1(uint8_t data) {
	// While buffer is not empty we wait
	while (!(SPI1->SR & SPI_SR_TXE));

	*((volatile uint8_t *)&SPI1->DR) = data;
}
