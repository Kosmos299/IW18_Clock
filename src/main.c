/**
 ******************************************************************************
 * @file    main.c
 * @author  Dawid.Adamik
 * @version V1.0
 * @date    17-03-2018
 * @brief   Main function.
 ******************************************************************************
 */
/*** Includes ****************************************************************/
#include <BSP.h>
//#include <Driver_I2C.h>
#include <Driver_ADC.h>

#include <Driver_GPIO.h>
#include <Driver_Timer.h>
#include <Driver_Core.h>
#include <Driver_Terminal.h>

#include <Func_Clock.h>
#include <Func_Display.h>
#include <Func_System.h>
#include <Func_Boost.h>

#include <stm32f10x_gpio.h>

/*** Preprocessor definitions ************************************************/
#define DEBUG_BLINK

#define RELEASE_NUMBER_MAJOR 0
#define RELEASE_NUMBER_MINOR1 1
#define RELEASE_NUMBER_MINOR2 2
#define RELEASE_NUMBER_BUILD1 0
#define RELEASE_NUMBER_BUILD2 0
#define BUILD_ID 20

#ifdef DEBUG_BLINK
#warning DEBUG ENABLED
#endif


/*** Definition of variables *************************************************/
/* NO DEFINITIONS */

/*** Prototypes of functions *************************************************/
void Debug_WelcomePrintout(void);
void ShowClocks(void);
void ShowResetSource(void);

/*** Definitions of functions ************************************************/
int main(void)
{
	//system configuration
	System_Init();

	//clock startup
	Boost_Init();
	Display_Init();
	Clock_Init();

	#ifdef DEBUG_BLINK
	Debug_WelcomePrintout();
	#endif

/*** infinite loop ***********************************************************/
while (1)
{
	State_Machine(); //??

	/* 100us IRQ routines ********/
	if (CheckDispFlag()==1)
	{
		#ifdef DEBUG_BLINK
		DBG_LED_Toggle(STAT_LED);
		#endif

		Display_Update();
		/* clear flag */
		ResetDispFlag();
	}
	/* 2ms IRQ routines end ******/

	/* 1s RTC IRQ routines *******/
	if (CheckClkFlag()==1)
	{
		#ifdef DEBUG_BLINK
		Debug_Time_Display();
		//DBG_Test_SHT();
		//DBG_Test_ADC();
		#endif

		Clock_Update();
		/* clear flag */
		ResetClkFlag();
	}
	/* 1s IRQ routines end *******/

/*** infinite loop end********************************************************/
}
/*** main end*****************************************************************/
}

/******************************************************
 ** Name            : Debug_WelcomePrintout
 **
 ** Created from/on : AD / 27.11.2015
 **
 ** Description     : Welcome Printout. Just for debug.
 **
 ** Calling         : not important
 **
 ** InputValues     : none
 ** OutputValues    : none
 ******************************************************/
void Debug_WelcomePrintout(void)
{
  TERMINAL("\r\n================ KOSMOS ===============\r\n");
  TERMINAL("        IW-18 VFD Clock       \r\n");
  TERMINAL("        ver. %d", RELEASE_NUMBER_MAJOR);
  TERMINAL(".%d", RELEASE_NUMBER_MINOR1);
  TERMINAL("%d", RELEASE_NUMBER_MINOR2);
  TERMINAL(".%d", RELEASE_NUMBER_BUILD1);
  TERMINAL("%d", RELEASE_NUMBER_BUILD2);
  TERMINAL(".%d\r\n", BUILD_ID);
  TERMINAL("        build: %s %s\r\n", __DATE__, __TIME__);
  TERMINAL("========================================\r\n");
  ShowClocks();
  ShowResetSource();
}

/******************************************************
 ** Name            : ShowClocks
 **
 ** Created from/on : RM / 13.11.2015
 **
 ** Description     : This function displays clocks on usart debug interface
 **                   just to check if RCC is configured correctly
 **
 ** Calling         : not important
 **
 ** InputValues     : none
 ** OutputValues    : none
 ******************************************************/
void ShowClocks(void)
{
  RCC_ClocksTypeDef stRCC_ClockFreq;
  RCC_GetClocksFreq(&stRCC_ClockFreq);

  TERMINAL("------ System clocks ------\r\n");
  TERMINAL("SYSCLK = %d Hz\r\n", stRCC_ClockFreq.SYSCLK_Frequency);
  TERMINAL("HCLK   = %d Hz\r\n", stRCC_ClockFreq.HCLK_Frequency);
  TERMINAL("PCLK1  = %d Hz\r\n", stRCC_ClockFreq.PCLK1_Frequency);
  TERMINAL("PCLK2  = %d Hz\r\n", stRCC_ClockFreq.PCLK2_Frequency);
  TERMINAL("========================================\r\n");
}

/******************************************************
 ** Name            : ShowResetSource
 **
 ** Created from/on : RM / 13.11.2015
 **
 ** Description     : This function displays reset reason, sometimes
 **                   important to diagnose WDT problem
 **
 ** Calling         : not important
 **
 ** InputValues     : none
 ** OutputValues    : none
 ******************************************************/
void ShowResetSource(void)
{
  if (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) != RESET)
    TERMINAL("RST: HSI oscillator clock ready\r\n");

  if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET)
    TERMINAL("RST: HSE oscillator clock ready\r\n");

  if (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET)
    TERMINAL("RST: PLL clock ready\r\n");

  if (RCC_GetFlagStatus(RCC_FLAG_LSERDY) != RESET)
    TERMINAL("RST: LSE oscillator clock ready\r\n");

  if (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) != RESET)
    TERMINAL("RST: LSI oscillator clock ready\r\n");

  if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    TERMINAL("RST: Pin reset\r\n");

  if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    TERMINAL("RST: POR/PDR reset\r\n");

  if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET)
    TERMINAL("RST: Software reset\r\n");

  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
    TERMINAL("RST: Independent Watchdog reset\r\n");

  if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
    TERMINAL("RST: Window Watchdog reset\r\n");

  if (RCC_GetFlagStatus(RCC_FLAG_LPWRRST) != RESET)
    TERMINAL("RST: Low Power reset\r\n");
  TERMINAL("========================================\r\n\r\n");
}
