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

#endif /* BSP_H_ */
