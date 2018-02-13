/*
 * Hasiok.c
 *
 *  Created on: 29.10.2017
 *      Author: Dawid Adamik
 */

#include "stm32f10x.h"
#include "system_stm32f10x.h"

//void dummy(){
//
//
//
//}


//void Configuration_Interrupts(void)
//{
//	//zdefiniowanie struktury wykorzystywanej do konfiguracji przerwani
//	NVIC_InitTypeDef NVIC_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;
//	RCC_ClocksTypeDef RCC_Clocks;
//
//	//skonfigurowanie miejsca przechowywania tablicy wektorow przerwan
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
//
//	//konfiguracja podzialu priorytetow przerwan: 4 grupy, 4 podgrupy
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//
//	//w³¹czenie obs³ugi alarmu
//	if(alarm_state == 1)
//	{
//		//konfiguracja przerwan pochodzacych od RTC Alarm
//		EXTI_ClearITPendingBit(EXTI_Line17);
//		EXTI_InitStructure.EXTI_Line = EXTI_Line17;
//		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//		EXTI_Init(&EXTI_InitStructure);
//		//generowanie przerwania przez RTC Alarm
//		RTC_ITConfig(RTC_IT_ALR, ENABLE);
//		//odczekanie na zakoñczenie operacji
//		RTC_WaitForLastTask();
//	}
//	//wy³¹czenie obs³ugi alarmu
//	else
//	{
//		//konfiguracja przerwan pochodzacych od RTC Alarm
//		EXTI_ClearITPendingBit(EXTI_Line17);
//		EXTI_InitStructure.EXTI_Line = EXTI_Line17;
//		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
//		EXTI_Init(&EXTI_InitStructure);
//		//generowanie przerwania przez RTC Alarm
//		RTC_ITConfig(RTC_IT_ALR, ENABLE);
//		//odczekanie na zakoñczenie operacji
//		RTC_WaitForLastTask();
//	}
//
//	//konfiguracja przerwan pochodzacych od RTC
//	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	//generowanie przerwania przez RTC co 1s
//	RTC_ITConfig(RTC_IT_SEC, ENABLE);
//	//odczekanie na zakoñczenie operacji
//	RTC_WaitForLastTask();
//
//	//konfiguracja przerwan pochodz¹cych od RTC Alarm
//	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//
//	//ustwienie dzialania GPIO jako linii EXTI
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);
//  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);
//
//	//ustawienie sposobu generowania przerwania
//  	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1 | EXTI_Line2 | EXTI_Line3 | EXTI_Line5;
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);
//
//	//konfiguracja NVIC dla przerwania EXTI0_IRQn
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//
//	//konfiguracja NVIC dla przerwania EXTI1_IRQn
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//
//	//konfiguracja NVIC dla przerwania EXTI2_IRQn
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//
//	//konfiguracja NVIC dla przerwania EXTI3_IRQn
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//
//	//konfiguracja NVIC dla przerwania EXTI9_5_IRQn
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//
//	//konfiguracja przerwan pochodzacych od SysTick
//	NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//
//	//uzyskanie czestotliwosci poszczegolnych linii zegara
//	RCC_GetClocksFreq(&RCC_Clocks);
//
//	//ustawienie przerwania SysTick co 100ms
//	SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 10000);
//
//	freq = RCC_Clocks.SYSCLK_Frequency;
//}
//

