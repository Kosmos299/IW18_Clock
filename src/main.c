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
#include <Driver_Core.h>
#include <Driver_Terminal.h>

#include <Func_System.h>
#include <Func_Display.h>
#include <Func_Clock.h>
#include <Func_Boost.h> //delete after debug

#include "../build_number.h" //auto-incrementing build number

/*** Preprocessor definitions ************************************************/
#define DEBUG_BLINK

#define RELEASE_NUMBER_MAJOR 0
#define RELEASE_NUMBER_MINOR1 1
#define RELEASE_NUMBER_MINOR2 6
#define RELEASE_NUMBER_BUILD1 0
#define RELEASE_NUMBER_BUILD2 0

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

	#ifdef DEBUG_BLINK
	Debug_WelcomePrintout();
	#endif

/*** infinite loop ***********************************************************/
while (1)
{
	StateMachine_Handler();
	SystemIO_Handler();

	/* 2ms Display IRQ routines ************************/
	if (CheckDispFlag()==1)
	{
		Display_Update();
		ResetDispFlag();
	}
	/* 2ms Display IRQ routines end ********************/

	/* 1s RTC IRQ routines *****************************/
	if (CheckClkFlag()==1)
	{
		#ifdef DEBUG_BLINK
		Debug_Time_Display();
		//DBG_Test_SHT();
		DBG_Test_ADC();
		#endif

		Clock_Update();
		ResetClkFlag();
	}
	/* 1s IRQ routines end *****************************/

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
  TERMINAL(".%d\r\n", BUILD_NUMBER);
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
  TERMINAL("SYSCLK = %dMHz\r\n", stRCC_ClockFreq.SYSCLK_Frequency/1000000);
  TERMINAL("HCLK   = %dMHz\r\n", stRCC_ClockFreq.HCLK_Frequency/1000000);
  TERMINAL("PCLK1  = %dMHz\r\n", stRCC_ClockFreq.PCLK1_Frequency/1000000);
  TERMINAL("PCLK2  = %dMHz\r\n", stRCC_ClockFreq.PCLK2_Frequency/1000000);
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
