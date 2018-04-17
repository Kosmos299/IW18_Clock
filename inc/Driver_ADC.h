/*
 * ADC.h
 *
 *  Created on: 12.02.2018
 *      Author: Dawid Adamik
 */

#ifndef DRIVER_ADC_H_
#define DRIVER_ADC_H_

#define AN_CHN_LIGHT 	ADC_Channel_1
#define AN_CHN_VBUS 	ADC_Channel_2

#define AN_PORT 		GPIOA
#define AN_LIGHT 		GPIO_Pin_1
#define AN_VBUS			GPIO_Pin_2

int ADC1_Read(int channel);
void ADC1_Config(void);

void DBG_Test_ADC(void);

#endif /* DRIVER_ADC_H_ */
