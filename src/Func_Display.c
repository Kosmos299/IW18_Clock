/*
 * Func_Display.c
 *
 *  Created on: 12.02.2018
 *      Author: Dawid Adamik
 */

#include <Func_Display.h>
#include <Func_Clock.h>
#include <BSP.h>

uint32_t Symbols [12] =
{
		0x040BC000,	// 0
		0x00028000,	// 1
		0x04054000,	// 2
		0x0404C000,	// 3
		0x000C8000,	// 4
		0x040AC000,	// 5
		0x040BC000,	// 6
		0x04028000,	// 7
		0x040DC000,	// 8
		0x040CC000,	// 9
		0x040C0000,	// deg
		0x04094000	// C
		//add next
};


uint32_t Grids [9] =
{
		0x01000000,	// 1
		0x00001000,	// 2
		0x00800000,	// 3
		0x08000000,	// 4
		0x00400000,	// 5
		0x10000000,	// 6
		0x20000000,	// 7
		0x40000000,	// 8
		0x02000000,	// 9
};

#define DECPOINT 0x00002000
#define DEBUG_SPEW

uint8_t SelectedDigit = 0;

/*
 * @brief Send prepared register data to VFD Driver
 * @param uint32_t data_written - register data - 20bits payload, 12bits offset
 * @retval None
 */
void VFD_Set(uint32_t data_written)
{
	//TODO: function incomplete, untested
	// divide data into segments
	uint16_t data_l, data_h;
	data_l = (0x0000FFFF & data_written);
	data_h = (data_written >> 16);

	//send data in two parts, MSB first
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, data_h);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, data_l);
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

/*
 * @brief
 * @param
 * @retval
 */
void Display_Update()
{
    if (SelectedDigit==8)
    {
    	SelectedDigit=0;
    }
    else
    {
    	SelectedDigit++;
    }

#ifdef DEBUG_SPEW
    printf("Display Digit = %d\n\r", SelectedDigit);
#endif

}
