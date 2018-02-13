/*
 * Utility.c
 *
 *  Created on: 29.10.2017
 *      Author: Dawid Adamik
 */
#include <BSP.h>  	//Board support package, pin definitions
#include "stm32f10x.h"
#include "system_stm32f10x.h"


volatile uint32_t timer_ms = 0; //deklaracja zmiennej 32 bitowej bez znaku

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

void delay(int time)
{
    int i;
    for (i = 0; i < time * 4000; i++) {}
}
