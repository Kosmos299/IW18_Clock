/*
 * Func_Display.c
 *
 *  Created on: 12.02.2018
 *      Author: Dawid Adamik
 */

#include <Func_Display.h>
#include <BSP.h>

void Disp_Update(uint32_t data_written)
{
	// divide data into segments
	uint16_t data_l, data_h;
	data_l = (0x0000FFFF & data_written);
	data_h = (data_written >> 16);

	//send data in two parts, MSB first

//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//	SPI_I2S_SendData(SPI1, data_h);
//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//	SPI_I2S_SendData(SPI1, data_l);

//    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)!=SET) {
//    SPI_I2S_SendData(SPI1, *send++);
//  }


	//at 32MHz, 1 nop = ~32ns
	//wait 100ns
	asm ("nop");
	asm ("nop");
	asm ("nop");
	GPIO_SetBits(SPI1_PORT, SPI1_LOAD);
	// LOAD at least 66ns wide
	asm ("nop");
	asm ("nop");
	GPIO_ResetBits(SPI1_PORT, SPI1_LOAD);

}
