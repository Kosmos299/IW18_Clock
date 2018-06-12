/*
 * IW18_BSP.h
 *
 *  Created on: 29.10.2017
 *      Author: Dawid Adamik
 */

#ifndef BSP_H_
#define BSP_H_

#include "stm32f10x.h"

/***** PORT A *****/

#define JTMS_A 			GPIO_Pin_13
#define JTCK_A 			GPIO_Pin_14
#define JTDI_A 			GPIO_Pin_15
#define JTDO_B 			GPIO_Pin_3
/***** PORT B *****/

#define BUZZ_PORT 			GPIOB
#define BUZZ_PIN 			GPIO_Pin_8

#define BTN_PORT GPIOA
#define A_BTN 			GPIO_Pin_11
#define B_BTN 			GPIO_Pin_12

#define ENC_PORT GPIOB
#define ENC_BTN 		GPIO_Pin_9
#define ENC_A 			GPIO_Pin_4
#define ENC_B 			GPIO_Pin_5

#define EXTA_A 			GPIOA
#define EXTA_B 			GPIOB
#define EXTA_C 			GPIOC
#define EXT_PA0_WKUP 	GPIO_Pin_0
#define EXT_PA3 		GPIO_Pin_3
#define EXT_PA8 		GPIO_Pin_8
#define EXT_PB0 		GPIO_Pin_0
#define EXT_PB11 		GPIO_Pin_11
#define EXT_PC13 		GPIO_Pin_13


void delay(int time);

#endif /* BSP_H_ */
