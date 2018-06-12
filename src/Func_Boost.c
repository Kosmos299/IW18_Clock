/*
 * Func_Boost.c
 *
 *  Created on: 17.03.2018
 *      Author: Dawid Adamik
 */

#include <Func_Boost.h>

PID_Struct_t VBUS_PID;
boostState PFC_State;


/**
  * @brief  Boost converter config
  * Configures TIM2 ans PWM generator, sets up PID
  * Configures Analog-to-Digital converter, input channel pins.
  * @param  None
  * @retval None
  */
void VBUS_BoostInit()
{
	// Init VBUS pid structure
	VBUS_PIDInit(&VBUS_PID,TRUE);

	// init PWM
	TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
	GPIO_InitTypeDef gpio;
	TIM_OCInitTypeDef  channel;
	ADC_InitTypeDef adc;

	TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBase_InitStructure.TIM_Prescaler = 0; 	// t = 1/32us
	TIM_TimeBase_InitStructure.TIM_Period = 800-1;	// T = 25us => F = 40kHz
	TIM_TimeBaseInit(TIM2, &TIM_TimeBase_InitStructure);

	TIM_OCStructInit(&channel);
	channel.TIM_OCMode = TIM_OCMode_PWM1;
	channel.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC3Init(TIM2, &channel);

	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = BOOST_PWM;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(BOOST_PORT, &gpio);

	// init ADC
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

	PFC_State = BOOST_START;
	//TODO: see if expansion needed
	//DBG:
	#ifdef BOOST_ON
	TIM_SetCompare3(TIM2, 500); //Debug: 50%
	#endif
}

void VBUS_ExecuteVoltageLoop()
{
	uint16_t CMP =0;
	int32_t HDutyCycleValue_tmp1 = 0;

    ADC_RegularChannelConfig(ADC1, AN_CHN_VBUS, 1, ADC_SampleTime_71Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    //cmp to v?
    CMP = ADC_GetConversionValue(ADC1);

    //note: so far boost_state is mostly unused
	if(PFC_State == BOOST_START)
	{
		//softstart here, for now placeholder
		PFC_State = BOOST_RUN;
    }
	if(PFC_State == BOOST_RUN)
	{
        /* PI */
	    HDutyCycleValue_tmp1 = PID_Regulator(VBUS_REFERENCE, CMP, &VBUS_PID); //_ex?
	    //range 0-800
		//TIM_SetCompare3(TIM2, HDutyCycleValue_tmp1);
    }
	if(PFC_State == BOOST_SHUTDOWN)
	{
		TIM_SetCompare3(TIM2, 0);
	}

}

//void VBUS_Set_State()


void VBUS_PIDInit(PID_Struct_t *pID_VBUS, bool updateKpKi)
{
	  /* Vbus PID init */
	  pID_VBUS->hKp_Gain     = PID_VBUS_KP_DEFAULT;
	  pID_VBUS->hKp_Divisor  = VBUS_KPDIV2;
	  pID_VBUS->hKi_Gain     = PID_VBUS_KI_DEFAULT;
	  pID_VBUS->hKi_Divisor  = VBUS_KIDIV2;
	  pID_VBUS->hKd_Gain     = PID_VBUS_KD_DEFAULT;
	  pID_VBUS->hKd_Divisor  = VBUS_KDDIV2;
	  pID_VBUS->wPreviousError = 0;

	  pID_VBUS->hLower_Limit_Output = PID_VBUS_LOWER_LIMIT;
	  /*
	    pID_VBUS->hUpper_Limit_Output = DC_PID_VBUS_UPPER_LIMIT(MaxInputCurrentAmps);
	    pID_VBUS->wUpper_Limit_Integral = DC_PID_VBUS_INTEGRAL_UPPER_LIMIT(MaxInputCurrentAmps);
	  */
	  pID_VBUS->wLower_Limit_Integral = PID_VBUS_INTEGRAL_LOWER_LIMIT;

	  pID_VBUS->wIntegral = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////
/**
  * @brief  Set PID proportional gain
  * @param  PID_Struct: pointer to PID structure
  * @retval None
  *
  */
void PID_Set_ProportionalGain(PID_Struct_t *pID_Struct, int16_t hProportionalGain)
{
  pID_Struct->hKp_Gain = hProportionalGain;
}

/**
  * @brief  Set PID integral gain
  * @param  PID_Struct: pointer to PID structure
  * @retval None
  *
  */
void PID_Set_IntegralGain(PID_Struct_t *pID_Struct, int16_t hIntegralGain)
{
  pID_Struct->hKi_Gain = hIntegralGain;
}

/**
  * @brief  Reset PFC PID integral terms for Passed PI object
  * @param  PID_Struct to PI structure to reset
  * @retval None
  *
  */
void PID_Reset_integralTerms(PID_Struct_t *pID_Struct)
{
  pID_Struct->wIntegral = 0;
}

/**
  * @brief  Set PID output upper limit
  * @param  PID_Struct: pointer to PID structure
  * @retval None
  *
  */
void PID_Set_OutputUpperLimit(PID_Struct_t *pID_Struct, int16_t hOutputUpperLimit)
{
  pID_Struct->hUpper_Limit_Output = hOutputUpperLimit;
}

/*
  * @brief  Set PID output lower limit
  * @param  PID_Struct: pointer to PID structure
  * @retval None
  *
  */
void PID_Set_OutputLowerLimit(PID_Struct_t *pID_Struct, int16_t hOutputLowerLimit)
{
  pID_Struct->hLower_Limit_Output = hOutputLowerLimit;
}


/**
  * @brief  Compute the PI(D) output for a PI(D) regulation.
  *
  *     - Kp_Gain: proportional coeffcient
  *     - Ki_Gain: integral coeffcient
  *     - Kd_Gain: differential coeffcient
  *
  * @param  *PID_Struct to Pointer to the PID settings
  * @param  hReference reference value for control loop
  * @param  hPresentFeedback reference value for control loop
  * @retval int16_t output of PID regulator
  *
  */
int16_t PID_Regulator(int16_t hReference, int16_t hPresentFeedback, PID_Struct_t *pID_Struct)
{
  //int32_t /*wError,*/ houtput_32;
  //int64_t dwAux;
  #ifdef DIFFERENTIAL_TERM_ENABLED
  //int32_t wDifferential_Term;
  #endif
  // error computation
  pID_Struct->wError = (int32_t)(hReference - hPresentFeedback);
  // Proportional term computation
  int32_t wProportional_Term = pID_Struct->hKp_Gain * pID_Struct->wError;

  // Integral term computation
  if(pID_Struct->hKi_Gain == 0)
  {
    pID_Struct->wIntegral = 0;
  }
  else
  {
    int32_t wIntegral_Term = pID_Struct->hKi_Gain * pID_Struct->wError;
    int64_t dwAux = pID_Struct->wIntegral + (int64_t)(wIntegral_Term);

    if(dwAux > pID_Struct->wUpper_Limit_Integral)
    {
      pID_Struct->wIntegral = pID_Struct->wUpper_Limit_Integral;
    }
    else if(dwAux < pID_Struct->wLower_Limit_Integral)
    {
      pID_Struct->wIntegral = pID_Struct->wLower_Limit_Integral;
    }
    else
    {
      pID_Struct->wIntegral = (int32_t)(dwAux);
    }
  }

  // Differential term computation
  #ifdef DIFFERENTIAL_TERM_ENABLED
  int32_t wtemp = pID_Struct->wError - pID_Struct->wPreviousError;
  int32_t wDifferential_Term = pID_Struct->hKd_Gain * wtemp;
  pID_Struct->wPreviousError = pID_Struct->wError;    // store value

  int32_t houtput_32 = (wProportional_Term / pID_Struct->hKp_Divisor +
                        pID_Struct->wIntegral / pID_Struct->hKi_Divisor +
                        wDifferential_Term / pID_Struct->hKd_Divisor);
  #else
  int32_t houtput_32 = (wProportional_Term / pID_Struct->hKp_Divisor +
                pID_Struct->wIntegral / pID_Struct->hKi_Divisor);
  #endif

  if(houtput_32 >= pID_Struct->hUpper_Limit_Output)
  {
    return(pID_Struct->hUpper_Limit_Output);
  }
  else if(houtput_32 < pID_Struct->hLower_Limit_Output)
  {
    return(pID_Struct->hLower_Limit_Output);
  }
  else
  {
    return((int16_t)(houtput_32));
  }
}

////////////////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  Set PID proportional gain for extended PID
  * @param  PID_Struct: pointer to extended PID structure
  * @retval None
  *
  */
void PID_Set_ProportionalGain_ex(PID_Struct_ex_t *pID_Struct, int16_t hProportionalGain)
{
  pID_Struct->hKp_Gain = hProportionalGain;
}

/**
  * @brief  Set PID integral gain for extended PID
  * @param  PID_Struct: pointer to extended PID structure
  * @retval None
  *
  */
void PID_Set_IntegralGain_ex(PID_Struct_ex_t *pID_Struct, int16_t hIntegralGain)
{
  pID_Struct->hKi_Gain = hIntegralGain;
}

/**
  * @brief  Reset PFC PID integral terms for Passed PI object
  * @param  PID_Struct: pointer to extended PI structure to reset
  * @retval None
  *
  */
void PID_Reset_integralTerms_ex(PID_Struct_ex_t *pID_Struct)
{
  pID_Struct->wIntegral = 0;
}

/**
  * @brief  Set PID output upper limit
  * @param  PID_Struct: pointer to extended PID structure
  * @retval None
  *
  */
void PID_Set_OutputUpperLimit_ex(PID_Struct_ex_t *pID_Struct, int32_t wOutputUpperLimit)
{
  pID_Struct->wUpper_Limit_Output = wOutputUpperLimit;
}

/**
  * @brief  Set PID output lower limit
  * @param  PID_Struct: pointer to extended PID structure
  * @retval None
  *
  */
void PID_Set_OutputLowerLimit_ex(PID_Struct_ex_t *pID_Struct, int32_t wOutputLowerLimit)
{
  pID_Struct->wLower_Limit_Output = wOutputLowerLimit;
}


/**
  * @brief  Compute the PI(D) output for a PI(D) regulation with an int32_t output.
  *
  *     - Kp_Gain: proportional coeffcient
  *     - Ki_Gain: integral coeffcient
  *     - Kd_Gain: differential coeffcient
  *
  * @param  *PID_Struct to Pointer to the extended PID settings
  * @param  hReference reference value for control loop
  * @param  hPresentFeedback reference value for control loop
  * @retval int32_t output of PID regulator
  *
  */
int32_t PID_Regulator_ex(int16_t hReference, int16_t hPresentFeedback, PID_Struct_ex_t *pID_Struct)
{
  // error computation
  pID_Struct->wError = (int32_t)(hReference - hPresentFeedback);

  // Proportional term computation
  int32_t wProportional_Term = pID_Struct->hKp_Gain * pID_Struct->wError;

  // Integral term computation
  if(pID_Struct->hKi_Gain == 0)
  {
    pID_Struct->wIntegral = 0;
  }
  else
  {
    int32_t wIntegral_Term = pID_Struct->hKi_Gain * pID_Struct->wError;

    int64_t dwAux = pID_Struct->wIntegral + (int64_t)(wIntegral_Term);

    if(dwAux > pID_Struct->wUpper_Limit_Integral)
    {
      pID_Struct->wIntegral = pID_Struct->wUpper_Limit_Integral;

    }
    else if(dwAux < pID_Struct->wLower_Limit_Integral)
    {
      pID_Struct->wIntegral = pID_Struct->wLower_Limit_Integral;
    }
    else
    {
      pID_Struct->wIntegral = (int32_t)(dwAux);

    }
  }

  // Differential term computation
  #ifdef DIFFERENTIAL_TERM_ENABLED_EX
  int32_t wtemp;
  wtemp = wError - pID_Struct->wPreviousError;
  int32_t wDifferential_Term = pID_Struct->hKd_Gain * wtemp;
  pID_Struct->wPreviousError = wError;    // store value

  houtput_32 = (wProportional_Term / pID_Struct->hKp_Divisor +
                pID_Struct->wIntegral / pID_Struct->hKi_Divisor +
                wDifferential_Term / pID_Struct->hKd_Divisor);
  #else
  int32_t houtput_32 = (wProportional_Term / pID_Struct->hKp_Divisor +
                        pID_Struct->wIntegral / pID_Struct->hKi_Divisor);
  #endif

  if(houtput_32 >= pID_Struct->wUpper_Limit_Output)
  {
    return(pID_Struct->wUpper_Limit_Output);
  }
  else if(houtput_32 < pID_Struct->wLower_Limit_Output)
  {
    return(pID_Struct->wLower_Limit_Output);
  }
  else
  {
    return(houtput_32);
  }
}

/**
  * @brief  Compute the PI(D) output for a PI(D) regulation with an int32_t output.
  *
  *     - Kp_Gain: proportional coeffcient
  *     - Ki_Gain: integral coeffcient
  *     - Kd_Gain: differential coeffcient
  *
  * @param  *PID_Struct to Pointer to the extended PID settings
  * @param  hReference reference value for control loop
  * @param  hPresentFeedback reference value for control loop
  * @retval int32_t output of PID regulator
  *
  */
int32_t PID_Regulator_ex2(int16_t hReference, int16_t hPresentFeedback, PID_Struct_ex_t *pID_Struct)
{
  int32_t houtput_32 = pID_Struct->wIntegral;/* In order to remember previous value */

  pID_Struct->wError = (int32_t)(hReference - hPresentFeedback);

  int32_t wProportional_Term = pID_Struct->hKp_Gain * pID_Struct->wError;

  int32_t wIntegral_Term = pID_Struct->hKi_Gain * pID_Struct->wError;

  houtput_32 = houtput_32 + (wProportional_Term - (pID_Struct->hKp_Gain * pID_Struct->wPreviousError)) /
               pID_Struct->hKp_Divisor + wIntegral_Term / pID_Struct->hKi_Divisor;

  pID_Struct->wPreviousError = pID_Struct->wError;/* In order to remember previous value */

  if(houtput_32 >= pID_Struct->wUpper_Limit_Output)
  {
    houtput_32 = pID_Struct->wUpper_Limit_Output;
  }
  else if(houtput_32 < pID_Struct->wLower_Limit_Output)
  {
    houtput_32 = pID_Struct->wLower_Limit_Output;
  }

  return(pID_Struct->wIntegral = houtput_32);
}


///////////////////////////////////////////////

//////////////////////
//HW DEBUG
/////////////////////
#include <Driver_Terminal.h>

/**
  * @brief  test-polls ADC, sends converted content via uart.
  * WARNING: THIS IS DEBUG FUNCTION
  * @param  None
  * @retval None
  */
void DBG_Test_ADC(void)
{
	uint16_t tempval = 0;
	tempval = DBG_ADC1_Read(AN_CHN_VBUS);
	TERMINAL("Vbus: ADC = %d,", tempval);
	TERMINAL(" Voltage = %d [mV]\n\r", tempval * 3300 * VBUS_DIVIDER / 4096); // hardware divided by 19, measure in mV

	//tempval = 0;
	//tempval = DBG_ADC1_Read(AN_CHN_LIGHT);
	//TERMINAL("Vlux: ADC = %d,", tempval);
	//TERMINAL(" Voltage = %d [mV]\n\r", tempval * 3300 / 4096); // direct measure, in mV
}

/**
  * @brief ADC channel read
  * Configures Analog-to-Digital converter, input channel pins.
  * @param  int channel - channel to be read
  * @retval int value - measurement result
  */
int DBG_ADC1_Read(int channel)
{
    ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_71Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

    return ADC_GetConversionValue(ADC1);
}
