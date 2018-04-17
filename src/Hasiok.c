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
//}
