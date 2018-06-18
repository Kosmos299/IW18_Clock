/*
 * Utility.h
 *
 *  Created on: 29.10.2017
 *      Author: Dawid Adamik
 */

#ifndef FUNC_SYSTEM_H_
#define FUNC_SYSTEM_H_

#include <stdint.h>
#include <Types.h>

#define LED_PORT GPIOB
#define ERR_LED 		GPIO_Pin_1
#define STAT_LED		GPIO_Pin_2

// Vector of timers used by the process
#define CNT_1000MS              4  	//1s Timers - total number of 1s timers
#define T1S_NEWSECTIMER         0
#define T1000_TESTTIMER         1
#define T1000_ERRORLED          2
#define T1000_STATUSLED         3

#define CNT_100MS               9  	//100ms Timers - total number of 100ms timers
#define T100_1S                 0  	//Timer for updating 1s periods (used internally by the mechanism)
#define T100_OVLOAD             1
#define T100_OVLOAD_HIGH        2
#define T100_OVLOAD_SUPER_HIGH  3
#define T100_UPSLOPE            4
#define T100_DATE_DISP_TIME     5	//Display Date length
#define T100_STARTDELAY         6	//Starting display delay
#define T100_ERRORLED  			7 	//Error LED blink period
#define T100_STATUSLED       	8  	//Status LED blink period

#define CNT_10MS                7  	//10ms Timers - total number of 10ms timers
#define T10_100MS               0  	//Timer for updating 100ms periods (used internally by the mechanism)
#define T10_GREENLED            1
#define T10_ERRORLED            2
#define T10_PREGASFLOW          3
#define T10_POSTGASFLOW         4
#define T10_REFRESHDISPLAY      5
#define T10_ValueToggle         6

#define CNT_1MS                10   //1ms Timers - total number of 1ms timers
#define T1_10MS                 0   //Timer for updating 10ms periods (used internally by the mechanism)
#define T1_Drehg1L              1
#define T1_DIG_INPUT            2
#define T1_Drehg1R              3
#define T1_REFRESHKEYS          4
#define T1_CANSOLL              5
#define T1_CANSOLL2             6
#define T1_WAITFORLLGEN         7
#define T1_REFRESHDISPLAY       8
#define T1_PM_INPUT             9

typedef struct
{//do i really need uint_32?
  uint32_t T1000[CNT_1000MS];
  uint32_t T100[CNT_100MS];
  uint32_t T10[CNT_10MS];
  uint32_t T1[CNT_1MS];
  uint8_t T02MS;
} sSystimer;
extern sSystimer stTimers;

void delay_ms(int time);
void LED_Toggle(uint16_t led);

bool CheckDispFlag(void);
void ResetDispFlag(void);

void System_Init(void);
void StateMachine_Handler(void);
void SystemIO_Handler(void);

#endif /* FUNC_SYSTEM_H_ */
