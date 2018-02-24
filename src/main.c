/**
 ******************************************************************************
 * @file    main.c
 * @author  Dawid.Adamik
 * @version V1.0
 * @date    02-11-2017
 * @brief   Main function.
 ******************************************************************************
 */

#include <BSP.h>  				//Board support package, pin definitions
#include <Driver_ADC.h>
#include <Driver_Core.h>		//Core clock configuration
#include <Driver_NVIC.h>
#include <Driver_Timer.h>
#include <Driver_UART.h>
#include <Func_Clock.h>
#include <Func_Display.h>
#include <stm32f10x_gpio.h>

//#define DEBUG_SPEW


int main(void)
{
	/* HW initialization */
	//basic configuration
	RCC_Config();				// Clock path config
	GPIO_Config();				// GPIO/BSP config
	NVIC_Config();

	//display routine startup
	TIM4_Config();
	//SPI1_Config();

	//boost converter startup
	//TIM2_Config();
	ADC1_Config();				// ADC config

	//I2C1_Config();				// Temperature and humidity sensor on I2C bus
	USART1_Config();			// Debug terminal 56000


	//clock functionalities
	Clock_Init();

	/* pre-code */


#ifdef DEBUG_SPEW
	printf("Initialization...  DONE\n\r");
	//BSP_GetVbus();
	//BSP_GetVlux();
	//BSP_GetTemp();
	//BSP_GetHumid();

#endif

	/////////////////////////////////////////////////////
	while (1)
	{


#ifdef DEBUG_SPEW

		//BSP_GetVbus();
		//BSP_GetVlux();
		//BSP_GetTemp();
		//BSP_GetHumid();
		//BSP_Time_Display();
#endif
		/* main loop led blink */

		/* check display refresh flag triggered in IRQ */
		if (CheckDispFlag()==1)
		{
			ResetDispFlag();
			Display_Update();

			/* dummy routine */
			if (GPIO_ReadOutputDataBit(LED_PORT, STAT_LED))
				GPIO_ResetBits(LED_PORT, STAT_LED);
			else
				GPIO_SetBits(LED_PORT, STAT_LED);
			/* dummy routine end*/
		}

		/* check clock refresh flag triggered in IRQ */
		if (CheckClkFlag()==1)
		{
			ResetClkFlag();
			Clock_Update();

			/* dummy routine */
			if (GPIO_ReadOutputDataBit(LED_PORT, STAT_LED))
				GPIO_ResetBits(LED_PORT, ERR_LED);
			else
				GPIO_SetBits(LED_PORT, ERR_LED);
			/* dummy routine end*/

			BSP_Time_Display();		// Enable time update
		}



	}
}

