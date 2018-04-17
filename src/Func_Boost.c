/*
 * Func_Boost.c
 *
 *  Created on: 17.03.2018
 *      Author: Dawid Adamik
 */

#include <Func_Boost.h>

#include <Driver_ADC.h>
#include <Driver_Timer.h>

void Boost_Init()
{
	TIM2_Config();
	ADC1_Config();				// ADC config
}

