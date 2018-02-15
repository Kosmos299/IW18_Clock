/*
 * Clock.c
 *
 *  Created on: 11.02.2018
 *      Author: Dawid Adamik
 */

#include <Func_Clock.h>
#include <Driver_Core.h>

//#define DEBUG_SPEW
#define SECONDS_PER_DAY 86400

uint8_t Month_Len[12] =
  {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
  };

uint8_t HRS = 0, MIN = 0, SEC = 0;


/**
  * @brief  Starts clock functionality. If Vbat has been lost, fully configures LSE.
  * 		If POR occurred, only re-enables APB1 interface.
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
	    Set_Time(12,0,0);
	    Set_Date(15,2,2018);
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
	    /* Update calendar */
	    RTC_Resume();
	}
}

/**
  * @brief  Sets new time.
  * @param  TmpHH: Hours to set.
  * @param  TmpMM: Minutes to set.
  * @param  TmpSS: Seconds to set.
  * @retval None
  */
void Set_Time(uint32_t Tmp_HH, uint32_t Tmp_MM, uint32_t Tmp_SS)
{
  RTC_WaitForLastTask();

  RTC_SetCounter(Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS);	// Change the current time
  RTC_WaitForLastTask();							// Wait until last write operation on RTC registers has finished
}

/**
  * @brief  Sets new time.
  * @param  TmpHH: Hours to set.
  * @param  TmpMM: Minutes to set.
  * @param  TmpSS: Seconds to set.
  * @retval None
  */
void Set_Alarm(uint32_t Tmp_HH, uint32_t Tmp_MM, uint32_t Tmp_SS)
{
  RTC_WaitForLastTask();

  RTC_ClearFlag(RTC_FLAG_SEC);						//Reset SECF flag, RTC->CRL, required to write into protected alarm register
  while(RTC_GetFlagStatus(RTC_FLAG_SEC) == RESET){};
  RTC_WaitForLastTask();

  RTC_SetAlarm(Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS);	// Set alarm time
  RTC_WaitForLastTask();							// Wait until last write operation on RTC registers has finished
}

/**
  * @brief  Sets new date.
  * @param  TmpDD: Days to set.
  * @param  TmpMM: Months to set.
  * @param  TmpYY: Years to set.
  *
  * note: uses 3 registers, while could be coded on one. If required, reimplement this.
  *
  * @retval None
  */
void Set_Date(uint32_t Tmp_DD, uint32_t Tmp_MM, uint32_t Tmp_YY)
{
	BKP_WriteBackupRegister(BKP_DR2, Tmp_DD);
	BKP_WriteBackupRegister(BKP_DR3, Tmp_MM);
	BKP_WriteBackupRegister(BKP_DR4, Tmp_YY);
}

/**
  * @brief  Increases days counter.
  * @param  uint32_t TDD - CURRENT day, to be increased
  * @retval None
  */
void Next_Day(uint32_t TDD)
{
	TDD++;
	BKP_WriteBackupRegister(BKP_DR2, TDD);
}

/**
  * @brief  Increases months counter.
  * Sets "day" to first of month
  * @param  uint32_t TMM - CURRENT month, to be increased
  * @retval None
  */
void Next_Month(uint32_t TMM)
{
	TMM++;
	BKP_WriteBackupRegister(BKP_DR2, 1);
	BKP_WriteBackupRegister(BKP_DR3, TMM);
}

/**
  * @brief  Increases years counter.
  * Sets date to New Year
  * @param  uint32_t TYY - CURRENT year, to be increased!
  * @retval None
  */
void Next_Year(uint32_t TYY)
{
	TYY++;
	BKP_WriteBackupRegister(BKP_DR2, 1);
	BKP_WriteBackupRegister(BKP_DR3, 1);
	BKP_WriteBackupRegister(BKP_DR4, TYY);
}

/**
  * @brief  Checks if provided Year is Leap Year.
  * @param  uint32_t TYY - year to be checked
  * @retval uint8_t - 1: Leap 0: Non-Leap
  */
uint8_t IsLeap(uint32_t TYY)
{
	if (TYY % 400 == 0)
	{
		return 1;
	}
	if (TYY % 400 == 0)
	{
		return 0;
	}
	if (TYY % 4 == 0)
	{
		return 1;
	}
	return 0;
}

/**
  * @brief  Updates date registers.
  * When called, pushes colandar to next day - taking into consideration current day and month, and leap year.
  * @param  none
  * @retval none
  */
void Increment_Date()
{
	uint32_t TDD = 0, TMM = 0, TYY = 0;

	TDD = BKP_ReadBackupRegister(BKP_DR2);
	TMM = BKP_ReadBackupRegister(BKP_DR3);
	TYY = BKP_ReadBackupRegister(BKP_DR4);

	/* find if leap year, update February length - ! to be improved */
	if(IsLeap(TYY))
	{
		Month_Len[2]=29;
	}
	else
	{
		Month_Len[2]=28;
	}

	/* increment date registers */
	if(TDD < Month_Len[TMM-1])
	{
		Next_Day(TDD); 			// not last day, -> increment day
	}
	else
	{
		if(TMM == 12) 			//last day of December -> new year
		{
			Next_Year(TYY);
		}
		else					//last day of month other than December -> new month
		{
			Next_Month(TMM);
		}
	}
}

/**
  * @brief  Updates Clock State.
  * When called, updates date depending to seconds passed.
  * If required adds multiple days. Also updates display data feed registers
  * @param  none
  * @retval none
  */
void Clock_Update()
{
	  uint32_t TimeVar = 0;

	  /* Update date */
	  RTC_WaitForLastTask();// Wait until last write operation on RTC registers has finished
	  TimeVar = RTC_GetCounter();

	  while (TimeVar > SECONDS_PER_DAY)
	  {
		  //Full day(s) has elapsed, change date
		  Increment_Date();
		  TimeVar -= SECONDS_PER_DAY;
		  RTC_SetCounter(TimeVar);
		  RTC_WaitForLastTask();
	  }

	  /* Update Display Indexes */
	  //TODO Display indexing
	  HRS = TimeVar / 3600; 		// Compute  hours
	  MIN = (TimeVar % 3600) / 60;	// Compute  minutes
	  SEC = (TimeVar % 3600) % 60;	// Compute  seconds

}

/**
  * @brief  Sends the current time and date via terminal.
  * WARNING: THIS IS DEBUG FUNCTION
  * @param  None
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

  printf("Time: %d:%d:%d ", THH, TMM, TSS);

  THH = BKP_ReadBackupRegister(BKP_DR2);
  TMM = BKP_ReadBackupRegister(BKP_DR3);
  TSS = BKP_ReadBackupRegister(BKP_DR4);

  printf("Date: %d:%d:%d\n\r", THH, TMM, TSS);
}
