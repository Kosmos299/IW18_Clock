/*
 * Func_Display.c
 *
 *  Created on: 12.02.2018
 *      Author: Dawid Adamik
 */

#include <Func_Display.h>
#include <Driver_SPI.h>
#include <Driver_Timer.h>
#include <stm32f10x.h>

// output mask for coded digits
const uint32_t Symbols [13] =
{
		0x00000000,	// blank
		0x0003B020,	// 0
		0x00012000,	// 1
		0x0002E020,	// 2
		0x00036020,	// 3
		0x00017000,	// 4
		0x00035020,	// 5
		0x0003D020,	// 6
		0x00012020,	// 7
		0x0003F020,	// 8
		0x00037020,	// 9
		0x00007020,	// deg
		0x00029020	// C
		//add next
};

// output mask for sequential anodes
const uint32_t Grids [9] =
{
		0x00000080,	// 1
		0x00080000,	// 2
		0x00000100,	// 3
		0x00000010,	// 4
		0x00000200,	// 5
		0x00000008,	// 6
		0x00000004,	// 7
		0x00000002,	// 8
		0x00000040	// 9
};

//
uint8_t DrawBuffer [9] =
{
		0x09,	// 1
		0x09,	// 2
		0x09,	// 3
		0x09,	// 4
		0x09,	// 5
		0x09,	// 6
		0x09,	// 7
		0x09,	// 8
		0x09	// 9
};

//#define DEBUG_SPEW

#define DECPOINT 0x00040000

#define VFD_LOAD_HIGH GPIO_SetBits(SPI1_PORT, SPI1_LOAD);
#define VFD_LOAD_LOW GPIO_ResetBits(SPI1_PORT, SPI1_LOAD);
#define VFD_BLANK GPIO_SetBits(SPI1_PORT, SPI1_OE);
#define VFD_SHOW GPIO_ResetBits(SPI1_PORT, SPI1_OE);

uint8_t SelectedDigit = 0;

/**
 * @brief Initialize display
 * Initializes all HW functions required, prepares data buffers
 * @param none
 * @retval none
 */
void Display_Init()
{
	//display routine startup
	TIM4_Config();
	SPI1_Config();
	VFD_SHOW
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
	//VFD_BLANK //so far no blanking

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

/*
 * @brief
 * @param
 * @retval
 */
void Display_Update()
{
	uint32_t disp_content = 0;
	uint8_t OpBuffer [9] = {0,0,0,0,0,0,0,0,0};

	uint8_t i = 8;
	/* copy DrawBuffer to prevent change in operation */
	while (i)
	{
		OpBuffer[i] = DrawBuffer[i];
		i--;
	}

	/* skip to next digit */
    if (SelectedDigit==9)
    {
    	SelectedDigit=0;
    }
    else
    {
    	SelectedDigit++;
    }

    /* prepare data */
    disp_content = Symbols[OpBuffer[SelectedDigit]] + Grids[SelectedDigit];

    /* push data to controller */
    VFD_Set(disp_content);

}
