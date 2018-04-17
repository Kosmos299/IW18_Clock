/*
 * Clock.c
 *
 *  Created on: 11.02.2018
 *      Author: Dawid Adamik
 */

#include <Func_Clock.h>
#include <Driver_Core.h>

/*
The backup registers are ten 16-bit registers used to store 20 bytes of user application data when VDD power is not present.
*/

uint8_t Month_Len[12] =
  {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
  };

bool Minute_Flag = 0;

/**
  * @brief  Starts clock functionality. If Vbat has been lost, fully configures LSE.
  * 		If POR occurred, only re-enables APB1 interface.
  * @param  None
  * @retval None
  */
void Clock_Init(void)
{
	  if (BKP_ReadBackupRegister(BKP_DR1) != LIVE_REG)
	  {
	    /* Backup data register value is not correct or not yet programmed (when
	       the first time the program is executed) */
	    RTC_Config();
	    Time_Set(INIT_TIME);
	    Date_Set(INIT_DATE);
	    BKP_WriteBackupRegister(BKP_DR1, LIVE_REG);
	  }
	  else
	  {
	    /* Wait for RTC registers synchronization */
	    RTC_WaitForSynchro();
	    RTC_WaitForLastTask();
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
void Time_Set(uint32_t Tmp_HH, uint32_t Tmp_MM, uint32_t Tmp_SS)
{
  RTC_WaitForLastTask();

  RTC_SetCounter(Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS);	// Change the current time
  RTC_WaitForLastTask();							// Wait until last write operation on RTC registers has finished
}

/**
  * @brief  Returns actual time.
  * @param  TmpHH: Hours to return.
  * @param  TmpMM: Minutes to return.
  * @param  TmpSS: Seconds to retunr.
  *
  * note: uses 3 pointers, in which data is allocated.
  *
  * @retval None
  */
void Time_Get(uint8_t* Tmp_HH, uint8_t* Tmp_MM, uint8_t* Tmp_SS)
{
	  uint32_t TimeVar = 0;
	  RTC_WaitForLastTask();// Wait until last write operation on RTC registers has finished

	  TimeVar = RTC_GetCounter();

	  *Tmp_HH = TimeVar / 3600; 		// Compute  hours
	  *Tmp_MM = (TimeVar % 3600) / 60;	// Compute  minutes
	  *Tmp_SS = (TimeVar % 3600) % 60;	// Compute  seconds
}

/**
  * @brief  Sets new date.
  * @param  TmpDD: Days to set.
  * @param  TmpMM: Months to set.
  * @param  TmpYY: Years to set.
 *
  * @retval None
  */
void Date_Set(uint8_t Tmp_DD, uint8_t Tmp_MM, uint16_t Tmp_YY)
{
	// 5 bits for day [0-31], 4 bits for month [0-15], 12 bits for year [0-4095]
	// day and month combined to save one reg, !lazy! implementation
	uint16_t Tmp_Reg = 0;
	Tmp_Reg = (Tmp_DD << 8) | Tmp_MM;
	BKP_WriteBackupRegister(BKP_DR2, Tmp_Reg);
	BKP_WriteBackupRegister(BKP_DR3, Tmp_YY);

}

/**
  * @brief  Returns actual date.
  * @param  TmpDD: Days to return.
  * @param  TmpMM: Months to return.
  * @param  TmpYY: Years to retunr.
  *
  * note: uses 3 pointer, in which data is allocated.
  *
  * @retval None
  */
void Date_Get(uint8_t* Tmp_DD, uint8_t* Tmp_MM, uint16_t* Tmp_YY)
{
	// 5 bits for day [0-31], 4 bits for month [0-15], 12 bits for year [0-4095]
	// day and month combined to save one reg, !lazy! implementation
	uint16_t Tmp_Reg = 0;
	Tmp_Reg = BKP_ReadBackupRegister(BKP_DR2);
	*Tmp_DD = (Tmp_Reg >> 8) & 0x00FF;
	*Tmp_MM = Tmp_Reg & 0x00FF;
	*Tmp_YY = BKP_ReadBackupRegister(BKP_DR3);
}

/**
  * @brief  Checks if provided Year is Leap Year.
  * @param  uint32_t TYY - year to be checked
  * @retval uint8_t - 1: Leap 0: Non-Leap
  */
bool IsLeap(uint16_t TYY)
{
	if (TYY % 400 == 0)
	{
		return TRUE;
	}
	if (TYY % 400 == 0)
	{
		return FALSE;
	}
	if (TYY % 4 == 0)
	{
		return TRUE;
	}
	return 0;
}

/**
  * @brief  Updates date registers.
  * When called, pushes calendar to next day - taking into consideration current day and month, and leap year.
  * @param  none
  * @retval none
  */
void Date_Increment()
{
	uint8_t TDD = 0, TMM = 0;
	uint16_t TYY = 0;

	Date_Get(&TDD,&TMM,&TYY);

	/* find if leap year, check February length, modify if needed - probably called once a year */
	if(IsLeap(TYY)&&Month_Len[2]!=29)
	{
		Month_Len[2]=29;
	}
	if(!IsLeap(TYY)&&Month_Len[2]!=28)
	{
		Month_Len[2]=28;
	}

	/* increment date registers */
	if(TDD < Month_Len[TMM-1])
	{
		Date_Set(TDD+1,TMM,TYY); // not last day, -> increment day
	}
	else
	{
		if(TMM == 12) 			//last day of December -> new year
		{
			Date_Set(1,1,TYY+1);
		}
		else					//last day of month other than December -> new month
		{
			Date_Set(1,TMM+1,TYY);
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
	  uint8_t TimeFlag = 0;

	  /* Update date */
	  RTC_WaitForLastTask();// Wait until last write operation on RTC registers has finished
	  TimeVar = RTC_GetCounter();

	  while (TimeVar > SECONDS_PER_DAY)
	  {
		  //Full day(s) has elapsed, change date
		  Date_Increment();
		  TimeVar -= SECONDS_PER_DAY;
		  TimeFlag = 1;
	  }

	  if(TimeFlag)
	  {
	  RTC_SetCounter(TimeVar);
	  RTC_WaitForLastTask();
	  }

	  if (TimeVar%3600 == 0)
	  {
		  Minute_Flag = TRUE;
	  }
	  else
	  {
		  Minute_Flag = FALSE;
	  }

}

////////////////////////////
// UNUSED, UNTESTED FUNCTIONS
////////////////////////////

/**
  * @brief  Sets new time.
  * @param  TmpHH: Hours to set.
  * @param  TmpMM: Minutes to set.
  * @param  TmpSS: Seconds to set.
  * @retval None
  */
void Alarm_Set(uint32_t Tmp_HH, uint32_t Tmp_MM, uint32_t Tmp_SS)
{
	// may not work without following code (in init maybe?)
	/*
    RTC_ClearFlag(RTC_FLAG_SEC);						//Reset SECF flag, RTC->CRL, required to write into protected alarm register
    while(RTC_GetFlagStatus(RTC_FLAG_SEC) == RESET){};
    RTC_WaitForLastTask();
    */

  RTC_WaitForLastTask();

  RTC_ClearFlag(RTC_FLAG_SEC);						//Reset SECF flag, RTC->CRL, required to write into protected alarm register
  while(RTC_GetFlagStatus(RTC_FLAG_SEC) == RESET){};
  RTC_WaitForLastTask();

  RTC_SetAlarm(Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS);	// Set alarm time
  RTC_WaitForLastTask();							// Wait until last write operation on RTC registers has finished
}

////////////////////////////
// DEBUG FUNCTIONS
////////////////////////////
#include <Driver_Terminal.h>

/**
  * @brief  Sends the current time and date via terminal.
  * WARNING: THIS IS DEBUG FUNCTION
  * @param  None
  * @retval None
  */
void Debug_Time_Display()
{
  uint32_t THH = 0, TMM = 0, TSS = 0, TimeVar = 0;
  RTC_WaitForLastTask();// Wait until last write operation on RTC registers has finished

  TimeVar = RTC_GetCounter();

  THH = TimeVar / 3600; 		// Compute  hours
  TMM = (TimeVar % 3600) / 60;	// Compute  minutes
  TSS = (TimeVar % 3600) % 60;	// Compute  seconds

  TERMINAL("Time: %d:", THH);
  TERMINAL("%d:", TMM);
  TERMINAL("%d ", TSS);

  uint8_t TDD = 0, TMM2 = 0;
  uint16_t TYY = 0;

  Date_Get(&TDD,&TMM2,&TYY);

  TERMINAL("Date: %d-", TDD);
  TERMINAL("%d-", TMM2);
  TERMINAL("%d\r\n", TYY);

}
