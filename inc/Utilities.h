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

typedef unsigned char uchar; //8bit
typedef signed char schar;
typedef signed short sint;//16bit
typedef signed int slong;//32bit
typedef unsigned long long uextended;
typedef signed long long sextended;

#define bool _Bool
enum
{
  FALSE = 0,
  TRUE
};

void delay(int time);

#endif	/*__LCS_TYPES_H__*/
