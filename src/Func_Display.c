/*
 * Func_Display.c
 *
 *  Created on: 12.02.2018
 *      Author: Dawid Adamik
 */

#include <Func_Display.h>
#include <Driver_SPI.h>

// output mask for coded digits
const uint32_t Symbols [13] =
{
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
		0x00000000,	// blank
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
		0x00, //1
		0x00, //2
		0x00, //3
		0x00, //4
		0x00, //5
		0x00, //6
		0x00, //7
		0x00, //8
		0x00  //9
};

//#define DEBUG_SPEW

#define DECPOINT 0x00040000

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
	SPI1_Config();
}

/*
 * @brief
 * @param
 * @retval
 */
void Display_Update()
{
	uint32_t disp_content = 0;
	uint8_t OpBuffer [9] =
	{
			0x00, //1
			0x00, //2
			0x00, //3
			0x00, //4
			0x00, //5
			0x00, //6
			0x00, //7
			0x00, //8
			0x00  //9
	};

	uint8_t i = 0;
	/* copy DrawBuffer to prevent change in operation */
	for(i = 0; i < 10; i++) {
		OpBuffer[i] = DrawBuffer[i];
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
    //TODO: debug sending dataframe, maybe problem is here: "+" instead of "&"
    disp_content = Symbols[OpBuffer[SelectedDigit]] + Grids[SelectedDigit];

    /* push data to controller */
    VFD_Set(disp_content);
}

/*
 * @brief
 * @param
 * @retval
 */
void Display_WriteBufferSimple(uint8_t position, uint8_t symbol)
{
DrawBuffer[position]=Symbols[symbol];
}


