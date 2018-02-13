/*
 * IW18_BSP.h
 *
 *  Created on: 29.10.2017
 *      Author: Dawid Adamik
 */

#ifndef BSP_H_
#define BSP_H_

/**********************/
#include <Driver_ADC.h>
#include <Driver_I2C.h>
#include "stm32f10x.h"
#include "system_stm32f10x.h"
#include <Utility.h>

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

#include "stm32f10x.h"

/***** PORT A *****/
#define AN_PORT 		GPIOA
#define EXT_USART_PORT 	GPIOA
#define SPI1_PORT 		GPIOA

#define EXT_WKUP 		GPIO_Pin_0
#define AN_LIGHT 		GPIO_Pin_1
#define AN_VBUS			GPIO_Pin_2
#define EXT_PA3 		GPIO_Pin_3
#define SPI1_LOAD 		GPIO_Pin_4
#define SPI1_SCK 		GPIO_Pin_5
#define SPI1_OE 		GPIO_Pin_6
#define SPI1_MOSI 		GPIO_Pin_7

#define EXT_PA8 		GPIO_Pin_8
#define EXT_USART_TX 	GPIO_Pin_9
#define EXT_USART_RX 	GPIO_Pin_10
#define A_BTN 			GPIO_Pin_11
#define B_BTN 			GPIO_Pin_12
#define JTMS 			GPIO_Pin_13
#define JTCK 			GPIO_Pin_14
#define JTDI 			GPIO_Pin_15

/***** PORT B *****/
#define LED_PORT GPIOB
#define ENC_PORT GPIOB
#define EXT_SPI_PORT GPIOB
#define I2C_PORT GPIOB

#define EXT_PB0 		GPIO_Pin_0
#define ERR_LED 		GPIO_Pin_1
#define STAT_LED		GPIO_Pin_2
#define JTDO 			GPIO_Pin_3
#define ENC_A 			GPIO_Pin_4
#define ENC_B 			GPIO_Pin_5
#define I2C_SCL 		GPIO_Pin_6
#define I2C_SDA 		GPIO_Pin_7

#define BUZZ 			GPIO_Pin_8
#define ENC_BTN 		GPIO_Pin_9
#define PWM 			GPIO_Pin_10
#define EXT_PB11 		GPIO_Pin_11
#define EXT_CS 			GPIO_Pin_12
#define EXT_SCK 		GPIO_Pin_13
#define EXT_MISO 		GPIO_Pin_14
#define EXT_MOSI 		GPIO_Pin_15

/***** PORT 3 *****/
#define EXT_PC13 		GPIO_Pin_13


void GPIO_Config(void);

void BSP_GetVbus (void);
void BSP_GetVlux (void);
void BSP_GetTemp(void);
void BSP_GetHumid(void);


#endif /* BSP_H_ */
