/*
 * spi.c
 *
 *  Created on: 12 июл. 2026 г.
 *      Author: kipin
 */

#include "device_drivers/spi.h"


volatile uint8_t __gpioLayout = 0;


void kp_SPI1_Resume() {
	SPI1->CR1 |= SPI_CR1_SPE;
}


void kp_SPI1_Stop() {
	SPI1->CR1 &= ~(SPI_CR1_SPE);
}


static void _Init_GPIO(const enum kp_SPI1_GPIO_Speed speed) {
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	switch (__gpioLayout) {
	/* [Primary layout]
	 * A7 - MOSI (Data Out)    - Alternate output push\pull
	 * A6 - MISO (Data In)     - Floating input
	 * A5 - CLK  (Clock)       - Alternate output push\pull
	 * A4 - NSS  (Chip Select) - General output PULL-UP
	 */
	case kp_SPI1_GPIO_LAYOUT_PRIM:
		// Reset pins
		GPIOA->CRL &= ~(0xFFFFul << 16);
		GPIOA->ODR &= ~(0xFul << 4);
		// PULL-UP for A4
		GPIOA->ODR |= (1ul << 4);
		if (speed == kp_SPI1_GPIO_Speed_10MHz)
			GPIOA->CRL |= (0x9491ul << 16);
		else if (speed == kp_SPI1_GPIO_Speed_2MHz)
			GPIOA->CRL |= (0xA4A2ul << 16);
		else if (speed == kp_SPI1_GPIO_Speed_50MHz)
			GPIOA->CRL |= (0xB4B3ul << 16);
		break;
	/* [Secondary layout]
	 * B5  - MOSI (Data Out)    - Alternate output push\pull
	 * B4  - MISO (Data In)     - Floating input
	 * B3  - CLK  (Clock)       - Alternate output push\pull
	 * A15 - NSS  (Chip Select) - General output PULL-UP
	 */
	case kp_SPI1_GPIO_LAYOUT_SCND:
		// Change AF layout
		AFIO->MAPR |= AFIO_MAPR_SPI1_REMAP;
		// Reset pins
		GPIOB->CRL &= ~(0x38ul);
		GPIOA->CRH &= ~(0xFul << 28);
		// General output PULL-UP for A15
		GPIOA->ODR |= (1ul << 15);
		if (speed == kp_SPI1_GPIO_Speed_10MHz) {
			 // A15
			GPIOA->CRH |= (0x1ul << 28);
			// B5..3
			GPIOB->CRL |= (0x949ul << 8);
		} else if (speed == kp_SPI1_GPIO_Speed_2MHz) {
			GPIOA->CRL |= (0x2ul << 28);
			GPIOB->CRL |= (0xA4Aul << 8);
		} else if (speed == kp_SPI1_GPIO_Speed_50MHz) {
			GPIOA->CRL |= (0x3ul << 28);
			GPIOB->CRL |= (0xB4Bul << 8);
		}
		break;
	}
}


void kp_SPI1_Init(const uint8_t gpioLayout,
			  const enum kp_SPI1_GPIO_Speed gpioSpeed,
			  const uint8_t endianness,
			  const enum kp_SPI1_BAUDRATE_SCALER BRScaler) {
	// To read latch pin later in SPI_SendBytes
	__gpioLayout = gpioLayout;
	// Enable clocking for SPI1
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	_Init_GPIO(gpioSpeed);
	// To prevent troubles with multiple SPI_Init() calls
	kp_SPI1_Stop();
	// Setup
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR;
	SPI1->CR1 |= endianness;
	SPI1->CR1 |= (BRScaler << BR_CHUNK_BEG);
	// Call interrupt (Receive buffer is not empty)
	SPI1->CR2 |= SPI_CR2_RXNEIE;
}


void kp_SPI1_SendBytes_1(const uint8_t data) {
	  // CS to LOW
	if (__gpioLayout == kp_SPI1_GPIO_LAYOUT_PRIM)
		GPIOA->BRR = (1ul << 4);
	else if (__gpioLayout == kp_SPI1_GPIO_LAYOUT_SCND)
		GPIOA->BRR = (1ul << 15);
	// wait until last operation isn't completed
	while (!(SPI1->SR & SPI_SR_TXE));
	// put data
	*((volatile uint8_t *)&SPI1->DR) = data;
	// wait until our transmission is going
	while (SPI1->SR & SPI_SR_BSY);
	// CS to HIGH
	if (__gpioLayout == kp_SPI1_GPIO_LAYOUT_PRIM)
		GPIOA->BSRR = (1ul << 4);
	else if (__gpioLayout == kp_SPI1_GPIO_LAYOUT_SCND)
		GPIOA->BSRR = (1ul << 15);
}
