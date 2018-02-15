/*
 * Driver_NVIC.h
 *
 *  Created on: 13.02.2018
 *      Author: Dawid Adamik
 */

#ifndef DRIVER_NVIC_H_
#define DRIVER_NVIC_H_

#include <stdint.h>

void NVIC_Config(void);

uint8_t CheckDispFlag(void);
void ResetDispFlag(void);
uint8_t CheckClkFlag(void);
void ResetClkFlag(void);

#endif /* DRIVER_NVIC_H_ */
