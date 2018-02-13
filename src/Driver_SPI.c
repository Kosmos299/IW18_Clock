/*
 * Display_Driver.c
 *
 *  Created on: 03.11.2017
 *      Author: Dawid Adamik
 */

#include <Driver_SPI.h>
#include <BSP.h>
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

#include "stm32f10x.h"

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

/**
  * @brief  SPI1 Config
  * Configures SPI1 to work with MAX6269 Expander present on board.
  * Configures pins required, with manual (software) OE, LE custom to MAX6269
  * @param  None
  * @retval None
  */
void SPI1_Config (void)
{
	// Pin configuration
	GPIO_InitTypeDef gpio;

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = SPI1_SCK|SPI1_MOSI; // SCK, MOSI
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_PORT, &gpio);

	gpio.GPIO_Pin = SPI1_LOAD|SPI1_OE; //LOAD, OE
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_PORT, &gpio);

	//SPI engine configuration
	SPI_InitTypeDef spi;
	spi.SPI_Mode = SPI_Mode_Master;
	spi.SPI_Direction = SPI_Direction_1Line_Tx;
	spi.SPI_NSS = SPI_NSS_Soft;
	spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_Init(SPI1, &spi);
	SPI_Cmd(SPI1, ENABLE);

}

/**
  * @brief  SPI1 Send/Receive
  * Test send/receive spi routine.
  * @param  None
  * @retval None
  */

/*
uint8_t spi_sendrecv(uint8_t byte)
{
 // poczekaj az bufor nadawczy bedzie wolny
 while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
 SPI_I2S_SendData(SPI1, byte);

 // poczekaj na dane w buforze odbiorczym
 while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
 return SPI_I2S_ReceiveData(SPI1);
}
 */

