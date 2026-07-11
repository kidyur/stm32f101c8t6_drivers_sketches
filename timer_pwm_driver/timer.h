/*
 * timer.h
 *
 *  Created on: 11 июл. 2026 г.
 *      Author: kipin
 */

#ifndef INC_DEVICE_DRIVERS_TIMER_H_
#define INC_DEVICE_DRIVERS_TIMER_H_

#include "stm32f101xb.h"

// Timers on APB1
#define TIM_Timer2 0x0001
#define TIM_Timer3 0x0002
#define TIM_Timer4 0x0004
#define TIM_Timer5 0x0008
#define TIM_Timer6 0x0010
#define TIM_Timer7 0x0020
#define TIM_Timer12 0x0040
#define TIM_Timer13 0x0080
#define TIM_Timer14 0x0100

// Timers on APB2
#define TIM_Timer1 0x000800
#define TIM_Timer8 0x004000
#define TIM_Timer9 0x080000
#define TIM_Timer10 0x100000
#define TIM_Timer11 0x200000

void TIM_EnableTimer(uint32_t timer);


void TIM_InitPWM(TIM_TypeDef * timer);


void TIM_SetPeriodMillis(TIM_TypeDef * timer, uint32_t periodMillis);


void TIM_SetPeriodMicros(TIM_TypeDef * timer, uint32_t periodMicros);


void TIM_SetDutyCycle(TIM_TypeDef * timer, uint8_t percents);


void TIM_StartTimer(TIM_TypeDef * timer);


#endif /* INC_DEVICE_DRIVERS_TIMER_H_ */
