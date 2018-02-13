/*
 * Clock.c
 *
 *  Created on: 11.02.2018
 *      Author: Dawid Adamik
 */

#include <Func_Clock.h>
#include <Driver_Core.h>

//#define DEBUG_SPEW

/**
  * @brief  Starts clock functionality. If Vbat has been lost, fully configures LSE.
  * 		If only reset occured, re-enables APB1 interface.
  * @param  None
  * @retval None
  */
void Clock_Init(void)
{
	  if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	  {
	    /* Backup data register value is not correct or not yet programmed (when
	       the first time the program is executed) */
#ifdef DEBUG_SPEW
	    printf("\r\n\n RTC not yet configured....");
#endif
	    /* RTC Configuration */
	    RTC_Config();
	    Time_Set(12,0,0);
#ifdef DEBUG_SPEW
	    printf("\r\n RTC configured....");
#endif
	    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	  }
	  else
	  {
#ifdef DEBUG_SPEW

	    /* Check if the Power On Reset flag is set */
	    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
	    {
	      printf("\r\n\n Power On Reset occurred....");
	    }
	    /* Check if the Pin Reset flag is set */
	    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
	    {
	      printf("\r\n\n External Reset occurred....");
	    }

	    printf("\r\n No need to configure RTC....");
#endif
	    /* Wait for RTC registers synchronization */
	    RTC_WaitForSynchro();
	    RTC_WaitForLastTask();

	}
}


/**
  * @brief  Sets new time.
  * @param  TmpHH: Hours to set.
  * @param  TmpMM: Minutes to set.
  * @param  TmpSS: Seconds to set.
  * @retval None
  */
void Time_Set(uint32_t Tmp_HH, uint32_t Tmp_MM, uint32_t Tmp_SS)
{
  RTC_WaitForLastTask();

  RTC_SetCounter(Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS);	// Change the current time
  RTC_WaitForLastTask();							// Wait until last write operation on RTC registers has finished
}

/**
  * @brief  Sends the current time via terminal.
  * @param  TimeVar: RTC counter value.
  * @retval None
  */
void BSP_Time_Display()
{
  uint32_t THH = 0, TMM = 0, TSS = 0, TimeVar = 0;
  RTC_WaitForLastTask();// Wait until last write operation on RTC registers has finished

  TimeVar = RTC_GetCounter();

  THH = TimeVar / 3600; 		// Compute  hours
  TMM = (TimeVar % 3600) / 60;	// Compute  minutes
  TSS = (TimeVar % 3600) % 60;	// Compute  seconds

  printf("Time: %d:%d:%d\n\r", THH, TMM, TSS);
}




//	//zresetowanie flagi SECF rejestru RTC->CRL, wymagane do poprawnego zapisu rejestru alarmu
//	RTC_ClearFlag(RTC_FLAG_SEC);
//	while(RTC_GetFlagStatus(RTC_FLAG_SEC) == RESET){};
//
//	//odczekanie na zakoñczenie operacji na RTC
//	RTC_WaitForLastTask();
//
//	//ustawienie rejestru alarmu RTC na 12h00m00s
//	RTC_SetAlarm(43200);
//	//odczekanie na zakoñczenie operacji na RTC
//	RTC_WaitForLastTask();
