/*
 * Func_Boost.h
 *
 *  Created on: 17.03.2018
 *      Author: Dawid Adamik
 */

#ifndef FUNC_BOOST_H_
#define FUNC_BOOST_H_

#include <Types.h>
#include <stm32f10x.h>
#include <system_stm32f10x.h>

#define AN_CHN_LIGHT 	ADC_Channel_1
#define AN_CHN_VBUS 	ADC_Channel_2

#define AN_PORT 		GPIOA
#define AN_LIGHT 		GPIO_Pin_1
#define AN_VBUS			GPIO_Pin_2

#define BOOST_ON

int DBG_ADC1_Read(int channel);
void DBG_Test_ADC(void);


void VBUS_BoostInit(void);
void VBUS_PIDInit(PID_Struct_t *pID_VBUS, bool updateKpKi);
void VBUS_ExecuteVoltageLoop(void);

/* HW Defines  ---------------------------------------------------------------*/
#define VBUS_REFERENCE 35 //output voltage
#define VBUS_DIVIDER 19	//existing physical resistor divider

#define BOOST_PORT 		GPIOB
#define BOOST_PWM 		GPIO_Pin_10

/*-- MIN/MAX values ----------------------------------------------------------*/
#define DUTY_CYCLE_MIN_PERCENT 24 /* [0.1%] */
#define DUTY_CYCLE_MIN_VAL DUTY_CYCLE_MIN_PERCENT*800/1000 /* [0-800] */
#define DUTY_CYCLE_MAX_PERCENT 900 /* [0.1%] */
#define DUTY_CYCLE_MAX_VAL DUTY_CYCLE_MAX_PERCENT*800/1000 /* [0-800] */

/************** VBUS PID-CONTROLLER INIT VALUES **************/
#define PID_VBUS_KP_DEFAULT             ((int16_t)7500)                /**< default Kp value of bus voltage PI regulator */
#define PID_VBUS_KI_DEFAULT             ((int16_t) 220)                 /**< default Ki value of bus voltage PI regulator */
#define PID_VBUS_KD_DEFAULT             ((int16_t)   1)                 /**< default Kd value of bus voltage PI regulator */

#define PID_VBUS_INTEGRAL_LOWER_LIMIT				(PID_VBUS_LOWER_LIMIT * VBUS_KIDIV2)  /**< integral lower limit of bus voltage PI regulator */
#define PID_VBUS_LOWER_LIMIT               -50                     /**< output lower limit of bus voltage PI regulator */
// Vbus PID  parameter dividers
#define VBUS_KPDIV2                             ((uint16_t) (8192))             /**< proportional term divider of bus voltage PI regulator */
#define VBUS_KIDIV2                             ((uint16_t) (32768))            /**< integral term divider of bus voltagePI regulator */
#define VBUS_KDDIV2                             ((uint16_t) (8192))   		/**< derivative term divider of bus voltage PI regulator */

/**************************************************************/
//#define DIFFERENTIAL_TERM_ENABLED             // if defined enables derivative term in PID regulators
//#define DIFFERENTIAL_TERM_ENABLED_EX          // if defined enables derivative term in PID extended regulators


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* General PID functions -----------*/
void PID_Set_ProportionalGain(PID_Struct_t *PID_Struct, int16_t hProportionalGain);
void PID_Set_IntegralGain(PID_Struct_t *PID_Struct, int16_t hIntegralGain);
void PID_Reset_integralTerms(PID_Struct_t *PID_Struct);
void PID_Set_OutputUpperLimit(PID_Struct_t *PID_Struct, int16_t hOutputUpperLimit);
void PID_Set_OutputLowerLimit(PID_Struct_t *PID_Struct, int16_t hOutputLowerLimit);

/* General PID functions -----------*/

int16_t PID_Regulator(int16_t hReference, int16_t hPresentFeedback, PID_Struct_t *PID_Struct);      // MC traditional PID

/* General extended PID functions -----------*/
void PID_Set_ProportionalGain_ex(PID_Struct_ex_t *PID_Struct, int16_t hProportionalGain);
void PID_Set_IntegralGain_ex(PID_Struct_ex_t *PID_Struct, int16_t hIntegralGain);
void PID_Reset_integralTerms_ex(PID_Struct_ex_t *PID_Struct);
void PID_Set_OutputUpperLimit_ex(PID_Struct_ex_t *PID_Struct, int32_t wOutputUpperLimit);
void PID_Set_OutputLowerLimit_ex(PID_Struct_ex_t *PID_Struct, int32_t wOutputLowerLimit);

int32_t PID_Regulator_ex(int16_t hReference, int16_t hPresentFeedback, PID_Struct_ex_t *PID_Struct);      // MC traditional PID for extended PID
int32_t PID_Regulator_ex2(int16_t hReference, int16_t hPresentFeedback, PID_Struct_ex_t *PID_Struct);

/* Exported variables ------------------------------------------------------- */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#endif /* FUNC_BOOST_H_ */
