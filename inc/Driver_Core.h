/*
 * SysClk.h
 *
 *  Created on: 29.10.2017
 *      Author: Dawid Adamik
 */

#ifndef DRIVER_CORE_H_
#define DRIVER_CORE_H_

#include <stdint.h>
#include <Types.h>

void RCC_Config(void);
void RTC_Config (void);
void RTC_Resume(void);

bool CheckClkFlag(void);
void ResetClkFlag(void);

#endif /* DRIVER_CORE_H_ */
