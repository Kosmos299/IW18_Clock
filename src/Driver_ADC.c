/*
 * ADC.c
 *
 *  Created on: 12.02.2018
 *      Author: Dawid Adamik
 */

#include <Driver_ADC.h>

#include <stm32f10x.h>
#include <system_stm32f10x.h>

/**
  * @brief ADC Config
  * Configures Analog-to-Digital converter, input channel pins.
  * @param  None
  * @retval None
  */
void ADC1_Config(void)
{

	ADC_InitTypeDef adc;
	GPIO_InitTypeDef gpio;

	gpio.GPIO_Pin = AN_LIGHT|AN_VBUS;
	gpio.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(AN_PORT, &gpio);

	ADC_StructInit(&adc);
	adc.ADC_ContinuousConvMode = DISABLE;
	adc.ADC_NbrOfChannel = 1;
	adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_Init(ADC1, &adc);
	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1));

	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1));


}

/**
  * @brief ADC channele read
  * Configures Analog-to-Digital converter, input channel pins.
  * @param  int channel - channel to be read
  * @retval int value - measurement result
  */
int ADC1_Read(int channel)
{
    ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_71Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

    return ADC_GetConversionValue(ADC1);
}

//////////////////////
//HW DEBUG
/////////////////////
#include <Driver_Terminal.h>

/**
  * @brief  test-polls SHT21, sends converted content via uart.
  * WARNING: THIS IS DEBUG FUNCTION
  * @param  None
  * @retval None
  */
void DBG_Test_ADC(void)
{
	uint16_t tempval = 0;
	tempval = ADC1_Read(AN_CHN_VBUS);
	TERMINAL("Vbus: ADC = %d,", tempval);
	TERMINAL(" Voltage = %d [mV]\n\r", tempval * 3300*11 / 4096); // hardware divided by 11, measure in mV

	tempval = 0;
	tempval = ADC1_Read(AN_CHN_LIGHT);
	TERMINAL("Vlux: ADC = %d,", tempval);
	TERMINAL(" Voltage = %d [mV]\n\r", tempval * 3300 / 4096); // direct measure, in mV
}
