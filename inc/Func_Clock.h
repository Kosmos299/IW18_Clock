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
#include <Types.h>

#define SECONDS_PER_DAY 86400
#define	LIVE_REG 0xA5A5
#define	INIT_TIME 12,00,00
#define	INIT_DATE 01,01,2018

void Clock_Init(void);

void Time_Set(uint32_t Tmp_HH, uint32_t Tmp_MM, uint32_t Tmp_SS);
void Time_Get(uint8_t* Tmp_HH, uint8_t* Tmp_MM, uint8_t* Tmp_SS);
void Date_Set(uint8_t Tmp_DD, uint8_t Tmp_MM, uint16_t Tmp_YY);
void Date_Get(uint8_t* Tmp_DD, uint8_t* Tmp_MM, uint16_t* Tmp_YY);

bool IsLeap(uint16_t TYY);

void Date_Increment(void);
void Clock_Update(void);
bool GetSecondsFlag(void);
bool GetMinutesFlag(void);

// untested/unused
void Alarm_Set(uint32_t Tmp_HH, uint32_t Tmp_MM, uint32_t Tmp_SS);

//debug functions
void Debug_Time_Display(void);

#endif /* CLOCK_H_ */
