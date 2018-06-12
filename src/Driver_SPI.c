/*
 * Display_Driver.c
 *
 *  Created on: 03.11.2017
 *      Author: Dawid Adamik
 */

#include <Driver_SPI.h>
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

#include "stm32f10x.h"
#include <stm32f10x_spi.h>

#define VFD_LOAD_HIGH GPIO_SetBits(SPI1_PORT, SPI1_LOAD);
#define VFD_LOAD_LOW GPIO_ResetBits(SPI1_PORT, SPI1_LOAD);
#define VFD_BLANK GPIO_SetBits(SPI1_PORT, SPI1_OE);
#define VFD_SHOW GPIO_ResetBits(SPI1_PORT, SPI1_OE);

/**
  * @brief  SPI1 Config
  * Configures SPI1 to work with MAX6269 Expander present on board.
  * Configures pins required, with manual (software) OE, LE custom to MAX6269
  * @param  None
  * @retval None
  */
void SPI1_Config (void)
{
	/*
	MAX6921AUI+ used in project is basically HV SPI expander with latch and blank function.

	The  MAX6921/MAX6931  are  normally  written  using  the following sequence:
	1)   Take CLK low.
	2)   Clock  20  bits  of  data  in  order  D19  first  to  D0  last into DIN, observing the data setup and hold times.
	3)   Load  the  20  output  latches  with  a  falling  edge on LOAD.

	The  output  latch  is transparent  to  the  shift  register  outputs  when  LOAD  is high,
	and  latches  the  current  state  on  the  falling  edge of LOAD.

	Timings required:
	+----+----------------------------+--------+-----+-----+-----+------+
	| No | Timing                     | Symbol | MIN | TYP | MAX | UNIT |
	+----+----------------------------+--------+-----+-----+-----+------+
	| 1  | CLK Clock Period           | Tcp    | 200 |     |     | ns   |
	+----+----------------------------+--------+-----+-----+-----+------+
	| 2  | CLK Pulse-Width High       | Tch    | 90  |     |     | ns   |
	+----+----------------------------+--------+-----+-----+-----+------+
	| 3  | CLK Pulse-Width Low        | Tcl    | 90  |     |     | ns   |
	+----+----------------------------+--------+-----+-----+-----+------+
	| 4  | CLK Rise to LOAD Rise Hold | Tcsh   | 100 |     |     | ns   |
	+----+----------------------------+--------+-----+-----+-----+------+
	| 5  | LOAD Pulse High            | Tcsw   | 55  |     |     | ns   |
	+----+----------------------------+--------+-----+-----+-----+------+

	From this we can derive, that maximum SPI frequency is 5MHz,
	and after sending data we have to wait 100ns and then put 55ns wide LOAD pulse

	*/


	// Pin configuration
	GPIO_InitTypeDef gpio_struct;

	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = SPI1_SCK|SPI1_MOSI; // SCK, MOSI
	gpio_struct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_PORT, &gpio_struct);

	gpio_struct.GPIO_Pin = SPI1_LOAD|SPI1_OE; //LOAD, OE
	gpio_struct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_PORT, &gpio_struct);

	//SPI engine configuration
	SPI_InitTypeDef spi_struct;
	SPI_I2S_DeInit(SPI1);

	spi_struct.SPI_Direction = SPI_Direction_1Line_Tx;
	spi_struct.SPI_Mode = SPI_Mode_Master;
	spi_struct.SPI_DataSize = SPI_DataSize_16b;
	spi_struct.SPI_CPOL = SPI_CPOL_Low; 		// clock is low while idling
	spi_struct.SPI_CPHA = SPI_CPHA_1Edge; 		// data is recognized on first edge of clk
	spi_struct.SPI_NSS = SPI_NSS_Soft;
	spi_struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	spi_struct.SPI_FirstBit = SPI_FirstBit_MSB; // data is sent MSB first
	spi_struct.SPI_CRCPolynomial = 7;

	SPI_Init(SPI1, &spi_struct);

	// Enable SPI
	SPI_Cmd(SPI1, ENABLE);
}

/**
  * @brief  SPI2 Config
  * Configures SPI2, interface routed onto external connector
  * Configures pins required, with hardware NSS
  * @param  None
  * @retval None
  */
void SPI2_Config (void)
{
	// Pin configuration
	GPIO_InitTypeDef gpio_struct;

	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = EXT_SCK|EXT_MOSI; // SCK, MOSI
	gpio_struct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(EXT_SPI_PORT, &gpio_struct);

	gpio_struct.GPIO_Pin = EXT_MISO|EXT_CS; //MISO, NSS
	gpio_struct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(EXT_SPI_PORT, &gpio_struct);

	//SPI engine configuration
	SPI_InitTypeDef spi_struct;
	SPI_I2S_DeInit(SPI2);

	spi_struct.SPI_Direction = SPI_Direction_1Line_Tx;	//SPI_Direction_2Lines_FullDuplex;
	spi_struct.SPI_Mode = SPI_Mode_Master;
	spi_struct.SPI_DataSize = SPI_DataSize_16b;
	spi_struct.SPI_CPOL = SPI_CPOL_Low; // clock is low while idling
	spi_struct.SPI_CPHA = SPI_CPHA_1Edge; // data is recognized on first edge of clk
	spi_struct.SPI_NSS = SPI_NSS_Hard;
	spi_struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	spi_struct.SPI_FirstBit = SPI_FirstBit_MSB; //SPI_FirstBit_LSB;
	spi_struct.SPI_CRCPolynomial = 7;

	SPI_Init(SPI2, &spi_struct);

	// Initialize the FIFO threshold
	//SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);

	// Init DMA
	//SPI_InitDMA();

	// Enable SPI
	SPI_Cmd(SPI2, ENABLE);
    //SPI_CalculateCRC(SPI1, DISABLE);
    //SPI_SSOutputCmd(SPI1, DISABLE);

    //SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
   // SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);

}


/**
  * @brief  SPI1 Send
  * Test send 16bits over SPI1 routine.
  * @param  None
  * @retval None
  */
void SPI_Send16(SPI_TypeDef* SPIx, uint16_t data_written)
{
  uint32_t timeout = 0xFFFFFF;

  timeout = 0xFFFFFF;
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE ) == RESET)
  {
    timeout--;
    if (!timeout)
      break;
  }
  SPI_I2S_SendData(SPIx, data_written);
}

/*
 * @brief Send prepared register data to VFD Driver
 * @param uint32_t data_written - register data - 20bits payload, 12bits offset
 * @retval None
 */
void VFD_Set(uint32_t data_written)
{
	// divide data into segments
	uint16_t data_l, data_h;

	data_l = data_written & 0x0000FFFF;
	data_h = (data_written >> 16) & 0xFFFF;

	//send data in two parts, MSB first
	VFD_BLANK

	SPI_Send16(SPI1, data_h);
	SPI_Send16(SPI1, data_l);
	//wait 100ns - at 32MHz, 1 nop = ~32ns
	asm ("nop");
	asm ("nop");
	asm ("nop");
	VFD_LOAD_HIGH
	// LOAD at least 66ns wide
	asm ("nop");
	asm ("nop");
	VFD_LOAD_LOW
	VFD_SHOW
}


