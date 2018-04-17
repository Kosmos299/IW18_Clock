/*
 * Driver_GPIO.c
 *
 *  Created on: 29.10.2017
 *      Author: Dawid Adamik
 */
#include "stm32f10x.h"
#include <Driver_GPIO.h>

/**
  * @brief  Configures GPIOs.
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{
	GPIO_InitTypeDef gpio;

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = ERR_LED|STAT_LED;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LED_PORT, &gpio);
}

/**
  * @brief  Toggles state of selected led.
  * WARNING: THIS IS DEBUG FUNCTION
  * @param  None
  * @retval None
  */
void DBG_LED_Toggle(uint16_t led)
{
/* WARNING ASSUMES LED ON FIXED PORT */
if (GPIO_ReadOutputDataBit(LED_PORT, led))
	GPIO_ResetBits(LED_PORT, led);
else
	GPIO_SetBits(LED_PORT, led);
}

