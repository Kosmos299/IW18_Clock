/*
 * Func_Display.h
 *
 *  Created on: 12.02.2018
 *      Author: Dawid Adamik
 */

#ifndef FUNC_DISPLAY_H_
#define FUNC_DISPLAY_H_

#include <stdint.h>
#include <Types.h>

typedef struct
{
	bool Dot;
	uint8_t Symbol;
} VFDDigit;

void Display_Init(void);
void Display_Update(void);
void Display_WriteBufferSimple(uint8_t position, uint8_t symbol, bool dot);

#endif /* FUNC_DISPLAY_H_ */
