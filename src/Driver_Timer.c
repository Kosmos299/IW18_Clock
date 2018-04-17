/*
 * Driver_Boost.c
 *
 *  Created on: 05.11.2017
 *      Author: Dawid Adamik
 */

#include <Driver_Timer.h>
#include <Driver_Terminal.h>

#include "stm32f10x.h"

bool DispIRQFlag = FALSE;
bool PIDIRQFlag = FALSE;
uint16_t TimeBaseCnt = 0;

/**
  * @brief  Boost converter PWM config
  * Configures TIM2 ans PWM generator.
  * @param  None
  * @retval None
  */
void TIM2_Config (void)
{
	// 40kHz refresh timer interrupt => 25us period
	TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
	GPIO_InitTypeDef gpio;
	TIM_OCInitTypeDef  channel;

	TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBase_InitStructure.TIM_Prescaler = 0; 	// t = 1/32us
	TIM_TimeBase_InitStructure.TIM_Period = 800-1;	// T = 25us => F = 40kHz
	TIM_TimeBaseInit(TIM2, &TIM_TimeBase_InitStructure);

	TIM_OCStructInit(&channel);
	channel.TIM_OCMode = TIM_OCMode_PWM1;
	channel.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC3Init(TIM4, &channel);

	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4, ENABLE); //only one compare?

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = BOOST_PWM;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(BOOST_PORT, &gpio);
}

//void TIM2_IRQHandler()
//{
///* interrupt code begin ***********************
// * */
//	/* overflow interrupt */
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//    {
//        GPIO_SetBits(BOOST_PORT, BOOST_PWM);
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//    }
//
//	/* compare 1 interrupt */
//	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET)
//	{
//		GPIO_ResetBits(BOOST_PORT, BOOST_PWM);
//	    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
//	}
///* interrupt code end ***************************/
//}

void TIM2_SetCompare(uint16_t CMP)
{
//TODO: maybe percent? dunno
TIM_SetCompare3(TIM2, CMP);
}

/**
  * @brief  PID/Disp timebase timer
  * Configures TIM4 as timebase generator. generates 100us interrupt
  * @param  None
  * @retval None
  */
void TIM4_Config (void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBase_InitStructure.TIM_Prescaler = 32; 	// t = 1us per count
	TIM_TimeBase_InitStructure.TIM_Period = 100;	// T = 100us
	TIM_TimeBaseInit(TIM4, &TIM_TimeBase_InitStructure);

	TIM_Cmd(TIM4, ENABLE);

	/* TIM4 Update Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}

/**
  * @brief  TIM4 Update IRQ Handler.
  * TIM4 is used as timebase for multiplexed display
  * @param  None
  * @retval None
  */
void TIM4_IRQHandler()
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
    {
        /* interrupt code begin */
        TimeBaseCnt++;
        if (TimeBaseCnt == 10000)
		{
        	TimeBaseCnt = 0;
		}

        /*2ms [500Hz] display refresh flag*/
        if (!TimeBaseCnt%20)
        {
        	DispIRQFlag = TRUE;
        }

        /*5ms [2kHz] PID refresh flag*/
        if (!TimeBaseCnt%5)
        {
        	PIDIRQFlag = TRUE;
        }

        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        /* interrupt code end */
    }
}

bool CheckDispFlag()
{
	return DispIRQFlag;
}

void ResetDispFlag()
{
	DispIRQFlag = FALSE;
}

