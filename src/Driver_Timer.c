/*
 * Driver_Boost.c
 *
 *  Created on: 05.11.2017
 *      Author: Dawid Adamik
 */

#include <Driver_Timer.h>

#include "stm32f10x.h"

void TIM2_Config (void)
{

	/* Boost converter timer timebase config */
	// 40kHz refresh timer interrupt => 25us period
	// 32Mhz Timer clock, 32 prescaler = 1us per count
	//
	// Interrupt on update

	TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
	TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBase_InitStructure.TIM_Period = 500;	//25
	TIM_TimeBase_InitStructure.TIM_Prescaler = 16000; //32
	TIM_TimeBaseInit(TIM2, &TIM_TimeBase_InitStructure);

	TIM_Cmd(TIM2, ENABLE);
}

void TIM4_Config (void)
{

	/* Display timer timebase config */
	// 55Hz refresh * 9 segments = 459Hz, rounded to 500Hz (2ms) timer interrupt
	// 32Mhz Timer clock, 3200 prescaler = 0,1ms per count
	// 2ms/0,1ms = 200 counts to reset
	// Interrupt on update

	TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
	TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBase_InitStructure.TIM_Period = 1000;	//200
	TIM_TimeBase_InitStructure.TIM_Prescaler = 16000; //3200
	TIM_TimeBaseInit(TIM4, &TIM_TimeBase_InitStructure);

	TIM_Cmd(TIM4, ENABLE);
}

