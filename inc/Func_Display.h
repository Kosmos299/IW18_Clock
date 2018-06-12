/*
 * Func_Display.h
 *
 *  Created on: 12.02.2018
 *      Author: Dawid Adamik
 */

#ifndef FUNC_DISPLAY_H_
#define FUNC_DISPLAY_H_

#include <stdint.h>

void Display_Init(void);
void Display_Update(void);
void Display_WriteBufferSimple(uint8_t position, uint8_t symbol);

#endif /* FUNC_DISPLAY_H_ */
