/*
 * Func_Display.c
 *
 *  Created on: 12.02.2018
 *      Author: Dawid Adamik
 */

#include <Func_Display.h>
#include <Driver_SPI.h>

// output mask for coded symbols
const uint32_t Symbols [14] =
{
		0x003B0200,	// 0
		0x00120000,	// 1
		0x002E0200,	// 2
		0x00360200,	// 3
		0x00170000,	// 4
		0x00350200,	// 5
		0x003D0200,	// 6
		0x00120200,	// 7
		0x003F0200,	// 8
		0x00370200,	// 9
		0x00000000,	// blank
		0x00070200,	// deg
		0x00290200,	// C
		0x007C0200	// testchar
		//add next
};

// output mask for sequential anodes
const uint32_t Grids [9] =
{
		0x00000800,	// 0
		0x00800000,	// 1
		0x00001000,	// 2
		0x00000100,	// 3
		0x00002000,	// 4
		0x00000080,	// 5
		0x00000040,	// 6
		0x00000020,	// 7
		0x00000400	// 8
};

//buffer for drawing
VFDDigit DrawBuffer [9] =
{
		10, FALSE,	//0
		10, FALSE,	//1
		10, FALSE,	//2
		10, FALSE,	//3
		10, FALSE,	//4
		10, FALSE,	//5
		10, FALSE,	//6
		10, FALSE,	//7
		10, FALSE	//8
};

//display buffer
VFDDigit OpBuffer [9] =
{
		10, FALSE,	//0
		10, FALSE,	//1
		10, FALSE,	//2
		10, FALSE,	//3
		10, FALSE,	//4
		10, FALSE,	//5
		10, FALSE,	//6
		10, FALSE,	//7
		10, FALSE	//8
};

#define DECPOINT 0x00400000

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
 * @brief display refresh routine
 * cyclicaly refreshes eanch digit, claculates character mapping etc.
 * @param none
 * @retval none
 */
void Display_Update()
{
	uint32_t disp_content = 0;
	uint32_t dotstat = 0;
	uint8_t i = 0;

	/* wrap around display if needed */
	/* copy DrawBuffer once per display cycle to prevent change while in operation */
	if (SelectedDigit==9)
    {
    	SelectedDigit=0;
    	for(i = 0; i < 9; i++) {
    		OpBuffer[i].Symbol = DrawBuffer[i].Symbol;
    		OpBuffer[i].Dot = DrawBuffer[i].Dot;
    	}
    }

    /* prepare driver payload data */
	if (OpBuffer[SelectedDigit].Dot == TRUE)
	{
		dotstat = DECPOINT;
	}
	else
	{
		dotstat = 0x00000000;
	}

    disp_content = Symbols[OpBuffer[SelectedDigit].Symbol] + Grids[SelectedDigit] + dotstat;
    /* push data to controller */
    VFD_Set(disp_content);
    /* skip to next digit */
	SelectedDigit++;
}

/*
 * @brief Simple write to draw buffer
 * @param uint8_t position location to write - digit from rigt to left (0 to 8)
 * @param uint8_t symbol - symbol number form symbol map
 * @param bool dot - status of dot
 * @retval
 */
void Display_WriteBufferSimple(uint8_t position, uint8_t symbol, bool dot)
{
DrawBuffer[position].Symbol = symbol;
DrawBuffer[position].Dot = dot;
}


