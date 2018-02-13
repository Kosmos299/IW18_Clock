/*
 * Clock.h
 *
 *  Created on: 11.02.2018
 *      Author: Dawid Adamik
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "stm32f10x_conf.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"
#include <stdio.h>

void Clock_Init(void);
void BSP_Time_Display(void);
void Time_Set(uint32_t Tmp_HH, uint32_t Tmp_MM, uint32_t Tmp_SS);

#endif /* CLOCK_H_ */
