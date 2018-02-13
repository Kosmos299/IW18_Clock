/*
 * BSP.c
 *
 *  Created on: 29.10.2017
 *      Author: Dawid Adamik
 */
#include <BSP.h>  	//Board support package, pin definitions
#include <Driver_ADC.h>
#include <Driver_I2C.h>
#include "stm32f10x.h"
#include "system_stm32f10x.h"
#include <Utility.h>

#include "stdio.h"
#include "stdlib.h"
#include <stdint.h>

#include "stm32f10x.h"


void GPIO_Config(void)
{
	GPIO_InitTypeDef gpio;

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = ERR_LED|STAT_LED;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LED_PORT, &gpio);
}

void BSP_GetTemp (void)
{
uint16_t tempval = 0;
uint32_t converted = 0;


//I2C1_Byte_Write(TRIG_T_NHM, SHT_ADDR);
delay_ms(100);
tempval = I2C1_Byte_Read(SHT_ADDR);

converted = tempval * 100 * 175.720 / 65536;
converted = converted - 4685;

printf("Temperature: REG = %d, Temperature = %d\n\r", tempval, converted); // measure in mK
}

void BSP_GetHumid (void)
{
uint16_t tempval = 0;
uint32_t converted = 0;


I2C1_Byte_Write(TRIG_H_NHM, SHT_ADDR);
delay_ms(250);
tempval = I2C1_Byte_Read(SHT_ADDR);

converted = tempval * 100 * 125 / 65536;
converted = converted - 600;

printf("Humidity: REG = %d, Relative Humidity = %d\n\r", tempval, converted); // measure in mK
}

void BSP_GetVbus (void)
{
	uint16_t tempval = 0;
	tempval = ADC1_Read(AN_CHN_VBUS);
	printf("Vbus: ADC = %d, Voltage = %dmV\n\r", tempval, tempval * 3300*11 / 4096); // hardware divided by 11, measure in mV

}
void BSP_GetVlux (void)
{
	uint16_t tempval = 0;
	tempval = ADC1_Read(AN_CHN_LIGHT);
	printf("Vlux: ADC = %d, Voltage = %dmV\n\r", tempval, tempval * 3300 / 4096); // direct measure, in mV
}
