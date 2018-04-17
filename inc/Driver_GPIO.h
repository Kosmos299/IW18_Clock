/*
 * Driver_GPIO.h
 *
 *  Created on: 11.03.2018
 *      Author: Dawid Adamik
 */

#ifndef DRIVER_GPIO_H_
#define DRIVER_GPIO_H_

#define LED_PORT GPIOB
#define ERR_LED 		GPIO_Pin_1
#define STAT_LED		GPIO_Pin_2

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

void GPIO_Config(void);

void DBG_LED_Toggle(uint16_t led);

#endif /* DRIVER_GPIO_H_ */
