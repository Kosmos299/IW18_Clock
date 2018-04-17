/*
 * Driver_Boost.h
 *
 *  Created on: 05.11.2017
 *      Author: Dawid Adamik
 */

#ifndef DRIVER_TIMER_H_
#define DRIVER_TIMER_H_

#include <stdint.h>
#include <Utilities.h>

#include "stm32f10x.h"

#define BOOST_PORT 		GPIOB
#define BOOST_PWM 		GPIO_Pin_10

void TIM2_Config (void);
void TIM2_SetCompare(uint16_t CMP);

void TIM4_Config (void);

bool CheckDispFlag(void);
void ResetDispFlag(void);


#endif /* DRIVER_TIMER_H_ */
