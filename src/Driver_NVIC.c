/*
 * Driver_NVIC.c
 *
 *  Created on: 13.02.2018
 *      Author: Dawid Adamik
 */

#include <Driver_NVIC.h>
#include <Func_Clock.h>
#include <BSP.h>
#include <stm32f10x_conf.h>

#include "stm32f10x.h"

uint8_t DispIRQFlag = 0, ClkIRQFlag = 0;

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Config(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;

    /* RTC Second Interrupt */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	RTC_ITConfig(RTC_IT_SEC, ENABLE);

	/* TIM2 Update Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	/* TIM4 Update Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);




//	NVIC_InitTypeDef NVIC_InitStruct;
//	/*  receive interrupt configuration */
//    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
//    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStruct);
//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//NVIC_EnableIRQ(USART1_IRQn);

}

/**
  * @brief  RTC Seconds IRQ Handler.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
		RTC_ClearITPendingBit(RTC_IT_SEC);
		/* interrupt code begin */

		RTC_WaitForLastTask();	// Wait until last write operation on RTC registers has finished
		/* interrupt code end */
	}
}

/**
  * @brief  TIM2 Update IRQ Handler.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler()
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        /* interrupt code begin */

        /* dummy routine */
        if (GPIO_ReadOutputDataBit(LED_PORT, ERR_LED))
            GPIO_ResetBits(LED_PORT, ERR_LED);
        else
            GPIO_SetBits(LED_PORT, ERR_LED);
        /* dummy routine end */

        /* interrupt code end */
    }
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
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        /* interrupt code begin */

        DispIRQFlag = 1;

        /* interrupt code end */
    }
}

uint8_t CheckDispFlag()
{
	return DispIRQFlag;
}

void ResetDispFlag()
{
	DispIRQFlag = 0;
}

uint8_t CheckClkFlag()
{
	return ClkIRQFlag;
}

void ResetClkFlag()
{
	ClkIRQFlag = 0;
}

/*
void USART3_IRQHandler(void)
{
    int i = 0;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
            static uint8_t cnt = 0;
        char t = USART_ReceiveData(USART1);
        USART1_putch(t);
        if((t!='n')&&(cnt < MAX_STRLEN))
        {
            received_string[cnt] = t;
            cnt++;
        }
        else
        {
            cnt = 0;
            USART3_puts(received_string);
            for (i = 0; i <= MAX_STRLEN+1; i++)         // flush buffer
            received_string[i] = '\0';
        }
    }
}*/

