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
void Set_Time(uint32_t Tmp_HH, uint32_t Tmp_MM, uint32_t Tmp_SS);
void Set_Date(uint32_t Tmp_DD, uint32_t Tmp_MM, uint32_t Tmp_YY);
void Set_Alarm(uint32_t Tmp_HH, uint32_t Tmp_MM, uint32_t Tmp_SS);

void Next_Day(uint32_t TDD);
void Next_Month(uint32_t TMM);
void Next_Year(uint32_t TYY);
uint8_t IsLeap(uint32_t TYY);

void Update_Date(void);
void Clock_Update(void);

void BSP_Time_Display(void);

#endif /* CLOCK_H_ */
