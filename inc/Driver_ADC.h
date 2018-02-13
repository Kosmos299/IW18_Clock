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

int ADC1_Read(int channel);
void ADC1_Config(void);

#endif /* DRIVER_ADC_H_ */
