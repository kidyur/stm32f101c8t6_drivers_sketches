/*
 * timer.c
 *
 *  Created on: 11 июл. 2026 г.
 *      Author: kipin
 */

#include "device_drivers/gpio.h"




void TIM_EnableTimer(uint32_t timer) {
	RCC->APB1ENR |= timer;
}

void TIM_SetPeriodMillis(TIM_TypeDef * timer, uint32_t periodMillis) {
	timer->ARR = periodMillis * 1000;
}

void TIM_SetPeriodMicros(TIM_TypeDef * timer, uint32_t periodMicros) {
	timer->ARR = periodMicros;
}

void TIM_SetDutyCycle(TIM_TypeDef * timer, uint8_t percents) {
	timer->CCR1 = (timer->ARR / 100) * percents;
}

void TIM_StartTimer(TIM_TypeDef * timer) {
	timer->CR1 |= 0x01U;
}

void TIM_InitPWM(TIM_TypeDef * timer) {
	timer->CR1 |= 0x80U;
	timer->CCMR1 |= 0x68U;
	timer->CCER |= 0x01U;

	// scaling timer to 1MHz
	timer->PSC = 7U;
}
