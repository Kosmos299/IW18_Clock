//===========================================================================
// Project name:  MicorTIG - Master
//===========================================================================
//  Functional description:
/// \file   types.h
/// \brief  Typendefinition nach Lorch ECS
///         Type definition as in Lorch Embedded Coding Sstandard defined
//===========================================================================
//  $Author: wrn $
//  $LastChangedDate: 2016-04-13 14:57:58 +0200 (Mi, 13 Apr 2016) $
//  $Rev: 10 $
//===========================================================================
//
//  $Header: $
//  Version information:
//  $Log: $
//===========================================================================

#ifndef  __LCS_TYPES_H__
#define  __LCS_TYPES_H__

#include <stdint.h>

/** @defgroup Standard_types_boundary_values Standard types boundary values
* @{
*/
#define U8_MAX     ((uint8_t)255)
#define S8_MAX     ((int8_t)127)
#define S8_MIN     ((int8_t)-127)
#define U16_MAX    ((uint16_t)65535u)
#define S16_MAX    ((int16_t)32767)
#define S16_MIN    ((int16_t)-32767)
#define U32_MAX    ((uint32_t)4294967295uL)
#define S32_MAX    ((int32_t)2147483647)
#define S32_MIN    ((int32_t)-2147483647)
/**
  * @}
  */

/**
  * @brief  Used in terminal
  */
typedef unsigned char uchar; //8bit
typedef signed char schar;
typedef signed short sint;//16bit
typedef signed int slong;//32bit
typedef unsigned long long uextended;
typedef signed long long sextended;

/**
  * @brief  bool definition
  */
#define bool _Bool
enum
{
  FALSE = 0,
  TRUE
};

/**
  * @brief  system state machine states definition
  */
typedef enum   // Main machine state
{
  MSM_STARTUP = 0, 	//startup state
  MSM_SHUTDOWN,
  MSM_ERROR,		//error state

  MSM_CLK_SET,			//normal work - data input state
  MSM_CLK_DISP_TEST,	//display test
  MSM_CLK_DISP_HRS,		//normal work - show hours
  MSM_CLK_DISP_DATE,	//normal work - show date
  MSM_CLK_DISP_TEMP		//normal work - show temperature and humidity
} mState;

/**
  * @brief  boost converter state machine states definition
  */
typedef enum   // Main machine state
{
  BOOST_START = 0,
  BOOST_RUN,
  BOOST_SHUTDOWN
} boostState;

/**
  * @brief  PI parameters type definition
  */
typedef struct
{
  int16_t hKp_Gain;
  uint16_t hKp_Divisor;
  int16_t hKi_Gain;
  uint16_t hKi_Divisor;
  int16_t hLower_Limit_Output;     //Lower Limit for Output limitation
  int16_t hUpper_Limit_Output;     //Lower Limit for Output limitation
  int32_t wLower_Limit_Integral;   //Lower Limit for Integral term limitation
  int32_t wUpper_Limit_Integral;   //Lower Limit for Integral term limitation
  int32_t wIntegral;
  // Actually used only if DIFFERENTIAL_TERM_ENABLED is enabled
  int16_t hKd_Gain;
  uint16_t hKd_Divisor;
  int32_t wPreviousError;
  int32_t wError;//dk
} PID_Struct_t;

/**
* @brief  PI parameters extended type definition - for HRTIM control: the maximum number of timer ticks (U16) che be grater than a S16
 */
typedef struct
{
  int16_t hKp_Gain;
  uint16_t hKp_Divisor;
  int16_t hKi_Gain;
  uint16_t hKi_Divisor;
  int32_t wLower_Limit_Output;     //Lower Limit for Output limitation
  int32_t wUpper_Limit_Output;     //Lower Limit for Output limitation
  int32_t wLower_Limit_Integral;   //Lower Limit for Integral term limitation
  int32_t wUpper_Limit_Integral;   //Lower Limit for Integral term limitation
  int32_t wIntegral;
  //Actually used only if DIFFERENTIAL_TERM_ENABLED is enabled
  #ifdef DIFFERENTIAL_TERM_ENABLED_EX
  int16_t hKd_Gain;
  uint16_t hKd_Divisor;
  #endif
  int32_t wPreviousError;
  int32_t wError;//dk
} PID_Struct_ex_t;

#endif	/*__LCS_TYPES_H__*/
