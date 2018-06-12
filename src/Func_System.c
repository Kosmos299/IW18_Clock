/*
 * Func_System.c
 *
 *  Created on: 29.10.2017
 *      Author: Dawid Adamik
 */
#include <Func_System.h>

#include <Driver_Core.h>
#include <Driver_I2C.h>
#include <Driver_Terminal.h>

#include <Func_Clock.h>
#include <Func_Display.h>
#include <Func_System.h>
#include <Func_Boost.h>

#include <Types.h>

#include "stm32f10x.h"
#include "system_stm32f10x.h"

#include <Driver_SPI.h>

volatile uint32_t timer_ms = 0;

mState SystemState = MSM_STARTUP;
sSystimer stTimers;

bool DispIRQFlag = FALSE;
uint16_t TimeBaseCnt = 0;

bool TEMPFLAG = FALSE;

void SysTick_Handler()
{
    if (timer_ms) {
        timer_ms--;
    }
}

void delay_ms(int time)
{
    timer_ms = 32000*time;
    while(timer_ms > 0){};
}

/**
  * @brief  Initializes core system functions - quartz clock, status leds, uart terminal.
  * Starts up state machine.
  * Configures TIM4 as timebase generator. generates 200us interrupt
  * @param  None
  * @retval None
  */
void System_Init()
{
	RCC_Config();				// Clock path config
	TERMINAL_Init();			// Debug terminal 56000 baud

	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = ERR_LED|STAT_LED;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LED_PORT, &gpio);

	/* reset all timers*/
	uint8_t i = 0;

	for (i = 0; i < CNT_100MS; i++)
	{
	  stTimers.T100[i] = 0;
	}
	for (i = 0; i < CNT_10MS; i++)
	{
	  stTimers.T10[i] = 0;
	}
	for (i = 0; i < CNT_1MS; i++)
	{
	  stTimers.T1[i] = 0;
	}
	stTimers.T02MS = 5;

	TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBase_InitStructure.TIM_Prescaler = 32-1; 	// t = 1us per count
	TIM_TimeBase_InitStructure.TIM_Period = 200-1;		// T = 200us
	TIM_TimeBaseInit(TIM4, &TIM_TimeBase_InitStructure);

	TIM_Cmd(TIM4, ENABLE);

	/* TIM4 Update Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	SystemState = MSM_STARTUP;
}

/**
  * @brief  TIM4 Update IRQ Handler.
  * TIM4 is used as timebase for process timers
  * @param  None
  * @retval None
  */
void TIM4_IRQHandler()
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) {
		/* interrupt code begin */

		/* MECHANISM 1 - flag for display and PID */
		TimeBaseCnt++;
		if (TimeBaseCnt == 10) //10 counts = 2ms
		{
			TimeBaseCnt = 0;
			DispIRQFlag = TRUE; //2ms [500Hz] display refresh flag
		}

		//VBUS_ExecuteVoltageLoop(); 	//Execute PID, full speed, 5kHz
		/* MECHANISM 1 END */

		/* MECHANISM 2 - composite timers for system process */
		uint8_t i = 0;
		stTimers.T02MS--;

		if (!(stTimers.T02MS)) //timebase divisor, 200us =>1ms
		{
			stTimers.T02MS = 5;

			// decrement 1 ms timers
			for (i = 0; i < CNT_1MS; i++)
			{
				if (stTimers.T1[i])
				{
					stTimers.T1[i]--;
				}
			}

			//Check if 10ms already has passed
			if (!(stTimers.T1[T1_10MS]))
			{
				stTimers.T1[T1_10MS] = 10;

				// decrement 10 ms timers
				for (i = 0; i < CNT_10MS; i++)
				{
					if (stTimers.T10[i])
					{
						stTimers.T10[i]--;
					}
				}

				//Check if 10 * 10ms = 100ms already has passed
				if (!(stTimers.T10[T10_100MS]))
				{
					stTimers.T10[T10_100MS] = 10;

					// decrement 100 ms timers
					for (i = 0; i < CNT_100MS; i++)
					{
						if (stTimers.T100[i])
						{
							stTimers.T100[i]--;
						}
					}

					//Check if 10 * 10 * 10ms = 1s already has passed
					if (!(stTimers.T100[T100_1S]))
					{

						stTimers.T100[T100_1S] = 10;
						// decrement 1s timers
						for (i = 0; i < CNT_1000MS; i++)
						{
							if (stTimers.T1000[i])
							{
								stTimers.T1000[i]--;
							}

						}
					} //End Update 1s
				} //End Update 100ms
			} //End Update 10ms
			//
		}
		/* MECHANISM 2 END */

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

/**
  * @brief  Toggles state of selected led.
  * @param  None
  * @retval None
  */
void LED_Toggle(uint16_t led)
{
if (GPIO_ReadOutputDataBit(LED_PORT, led))
	GPIO_ResetBits(LED_PORT, led);
else
	GPIO_SetBits(LED_PORT, led);
}
////////////////////////////////
void SystemIO_Handler()
{
	//Status LEDs
	if (SystemState == MSM_ERROR)
	{
    	GPIO_ResetBits(LED_PORT, STAT_LED);
        if(!stTimers.T100[T100_ERRORLED])
        {
      	  stTimers.T100[T100_ERRORLED] = 5;
      	  LED_Toggle(ERR_LED);
        }
	}
	else
	{
		GPIO_ResetBits(LED_PORT, ERR_LED);
        if(!stTimers.T100[T100_STATUSLED])
        {
      	  stTimers.T100[T100_STATUSLED] = 10;
      	  LED_Toggle(STAT_LED);
        }
	}
	//Keypad

	//Alarm
}
////////////////////////////////
void StateMachine_Handler()
{
uint8_t i = 0;
  switch (SystemState)
  {
    case MSM_STARTUP:
    	/* clock startup */
    	Display_Init();
    	VBUS_BoostInit();

    	for (i=0;i<9;i++)
    	{
    	Display_WriteBufferSimple(i, 9);
    	}

    	Clock_Init();
    	SystemState = MSM_CLK_DISP_HRS;
    	TERMINAL("Startup complete.\n\r");

    break;

    case MSM_SHUTDOWN:
    	// Run shutdown procedure
    	//SystemState = MSM_STANDBY;
    break;

    case MSM_ERROR:

    break;

    case MSM_CLK_DISP_TEST:

    break;

    case MSM_CLK_DISP_HRS:

        if(!stTimers.T1000[T1000_TESTTIMER])
        {
      	  stTimers.T1000[T1000_TESTTIMER] = 2;

      	  if (TEMPFLAG == TRUE)
      	  {
          	  VFD_Set(0b1111111111110000);
          	  TEMPFLAG = FALSE;
      	  }
      	  else
       	  {
          	  VFD_Set(0b1111111111111111);
          	  TEMPFLAG = TRUE;
      	  }

        }


    break;

    case MSM_CLK_DISP_DATE:

    break;

    case MSM_CLK_DISP_TEMP:

    break;

    case MSM_CLK_SET:

    break;
  }
}
