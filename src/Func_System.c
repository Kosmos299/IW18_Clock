/*
 * Utility.c
 *
 *  Created on: 29.10.2017
 *      Author: Dawid Adamik
 */
#include <BSP.h>  	//Board support package, pin definitions
#include <Driver_GPIO.h>
#include <Driver_Core.h>
#include <Driver_I2C.h>
#include <Driver_Terminal.h>
#include "stm32f10x.h"
#include "system_stm32f10x.h"

//===========================================================================
//  Sub structure  "mainState"
//  description:  data for/of Main machine state
//===========================================================================
typedef enum   // Main machine state
{
  MSM_STARTUP = 0,
  MSM_NORMAL,
  MSM_SHUTDOWN,
  MSM_ERROR,
  MSM_TEST,
  MSM_STANDBY
} mState;

volatile uint32_t timer_ms = 0;
mState SystemState = MSM_STARTUP;

void System_Init()
{
	RCC_Config();				// Clock path config
	GPIO_Config();				// GPIO/BSP config
	TERMINAL_Init();			// Debug terminal 56000 baud
	//I2C1_Config();			//Temperature and humidity sensor on I2C bus
}

void SysTick_Handler()
{
    if (timer_ms) {
        timer_ms--;
    }
}

void delay_ms(int time)
{
    timer_ms = time;
    while(timer_ms > 0){};
}

////////////////////////////////
void State_Machine()
{
  switch (SystemState)
  {
    case MSM_STARTUP:
      /*if(!stTimers.T1S[T1S_DisplayTest])
      {
        Display_SetLEDand7Seg(FALSE);
        SystemState = MSM_NORMAL;
        Menu_Init();
        MSM_StartUpDone = TRUE;
      }
      else
      {
        Display_SetLEDand7Seg(TRUE);
        //OS-Versionen der Komponenten anfragen
        if (aucData[0] == 0)
        {
          aucData[0] = GID_MASTER;
          aucData[1] = CO_GET_OSVERSION;
          CAN_SendStdMsg(CAN_MSG_0x400, 8, aucData, REGULAR);
          aucData[0] = GID_PROZESS;
          aucData[1] = CO_GET_OSVERSION;
          CAN_SendStdMsg(CAN_MSG_0x400, 8, aucData, REGULAR);
          aucData[0] = GID_PRIMARY;
          aucData[1] = CO_GET_OSVERSION;
          CAN_SendStdMsg(CAN_MSG_0x400, 8, aucData, REGULAR);
        }
      }*/
    break;

    case MSM_NORMAL:
    /*if(!stTimers.T10[T10_REFRESHDISPLAY])
    {
      stTimers.T10[T10_REFRESHDISPLAY] = 4;
      //Eingabe prüfen & MenuPoint setzen
      Menu_Steuerung();
      //MenuPoint prüfen / ProcessBits setzen
      Menu_BitControl();
      //Darstellung der Nebenparameter + HauptSollStrom
      Menu_Show_Parameter();
      //Werte für CAN zur Verfügung stellen
      Menu_to_CAN();
    }*/
    break;

    case MSM_SHUTDOWN:
//        startup_Shutdown();  // Run shutdown procedure
//  //        if(GData.SystemStat.ulMainStat & MSM_STANDBY)
//  //        {
//  //          MStatus = eMSTANDBY;
//  //        }
    	SystemState = MSM_STANDBY; //Vorerst mal so KLA
    break;

    case MSM_ERROR:
     /* if (GData.ProcessData.ulProcessBits & WC_Standby) //CheckSysStatus(MS_SHUTDOWN)
      {
    	  SystemState = MSM_SHUTDOWN;
      }
      // Return to normal operation if there are no errors anymore / valid error
      else if (!GData.CANRegister.uiErrorBits)
      {
        if (MSM_StartUpDone)
        {
        	SystemState = MSM_NORMAL;
        }
        else
        {
        	SystemState = MSM_STARTUP; // Startup was incomplete
        }
        ErrorSwitchT1S = TRUE; //Init for next error / hint
      }
      else
      {
        #define SubError   ((GData.CANRegister.uiErrorBits & 0x7F00) >>  8)
        #define MainError  ((GData.CANRegister.uiErrorBits & 0x00FF) >>  0)

        //Status-Error LED Handling
        if (GData.CANRegister.uiErrorBits & 0x8000) //Error
        {
          if (GData.BFRegister.ulKeyWord & (E_Menu | E_Takt2_4 | E_Pulsen))
          {
            ErrorSwitchT1S = TRUE;
            stTimers.T1S[T1S_SwitchMenu] = 2; //2 Sek
            //Wechsel zu SubError //10 = '-'
            Display_WriteText((10 << 10) + ((SubError / 10) << 5) + (SubError % 10), FALSE);
          }
          else if (!stTimers.T1S[T1S_SwitchMenu])
          {
            //Zeige MainError
            if (ErrorSwitchT1S) //13 = 'E'
            {
              LED_Set(LED_Temp, ON, NORM);
              Display_WriteText((13 << 10) + ((MainError / 10) << 5) + (MainError % 10), FALSE);
            }
            ErrorSwitchT1S = FALSE;
          }
        }
        else //Hint
        {
          if (GData.BFRegister.ulKeyWord == E_Enc_OK) //Exit Hint
          {
            GData.ProcessData.ulProcessBits |= UI_Hinweis;
          }

          if (GData.BFRegister.ulKeyWord & (E_Menu | E_Takt2_4 | E_Pulsen))
          {
            ErrorSwitchT1S = TRUE;
            stTimers.T1S[T1S_SwitchMenu] = 2; //2 Sek
            //Wechsel zu SubHint //10 = '-'
            Display_WriteText((10 << 10) + ((SubError / 10) << 5) + (SubError % 10), FALSE);
          }
          else if (!stTimers.T1S[T1S_SwitchMenu])
          {
            //Zeige MainHint
            if (ErrorSwitchT1S) //24 = 'H'
            {
              LED_Set(LED_Temp, OFF, NORM);
              Display_WriteText((24 << 10) + ((MainError / 10) << 5) + (MainError % 10), FALSE);
            }
            ErrorSwitchT1S = FALSE;
          }
        }
        GData.BFRegister.ulKeyWord = 0;
      }*/
    break;

    case MSM_TEST:
    /*  if(!stTimers.T10[T10_REFRESHDISPLAY])
      {
        stTimers.T10[T10_REFRESHDISPLAY] = 4;
        if (BfPoint < 10)
          BfPoint = BedienfeldTest(BfPoint);
        else
        {
          BfPoint = 0;
          CANOutputBfTest(TRUE);
          Display_SetLEDand7Seg(FALSE);
          SystemState = MSM_NORMAL;
          Menu_Init();
        }
      }*/
    break;

    case MSM_STANDBY:
     /* if(!stTimers.T10[T10_REFRESHDISPLAY])
      {
        stTimers.T10[T10_REFRESHDISPLAY] = 4;
        if (GData.BFRegister.ulKeyWord == E_Standby)
        {
          GData.ProcessData.ulProcessBits &= ~WC_Standby;
          SystemState = MSM_STARTUP;
          //stTimers.T1S[T1S_DisplayTest] = 20; //DisplayTest Zeit
          stTimers.T1S[T1S_DisplayTest] = 2; //2 Sek 170921 GTC nach Korrektur Systimer
        }
        GData.BFRegister.ulKeyWord = 0;
      }
      else
      {
        Display_SetLEDand7Seg(FALSE);
        GData.ProcessData.ulProcessBits |= WC_Standby;
      }*/
    break;
  }
}
////////////////////////////////
