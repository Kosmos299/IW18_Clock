/*
 * Driver_SPI.h
 *
 * Contains SPI initializes, and functions required to send data via SPI
 *
 *  Created on: 03.11.2017
 *      Author: Dawid Adamik
 */

#ifndef DRIVER_SPI_H_
#define DRIVER_SPI_H_

#include <stdint.h>
#include <stm32f10x_spi.h>

#define SPI1_PORT 		GPIOA
#define SPI1_LOAD 		GPIO_Pin_4
#define SPI1_SCK 		GPIO_Pin_5
#define SPI1_OE 		GPIO_Pin_6
#define SPI1_MOSI 		GPIO_Pin_7

#define EXT_SPI_PORT GPIOB
#define EXT_CS 			GPIO_Pin_12
#define EXT_SCK 		GPIO_Pin_13
#define EXT_MISO 		GPIO_Pin_14
#define EXT_MOSI 		GPIO_Pin_15

void SPI1_Config (void);
void SPI2_Config (void);
void SPI_Send16(SPI_TypeDef* SPIx, uint16_t data_written);

void VFD_Set(uint32_t data_written);

#endif /* DRIVER_SPI_H_ */
