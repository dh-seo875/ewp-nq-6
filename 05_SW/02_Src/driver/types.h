/**
 * @cond
 ***********************************************************************************************************************
 *
 * Copyright (c) 2015, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **********************************************************************************************************************/

/*******************************************************************************
**                      Author(s) Identity                                    **
********************************************************************************
**                                                                            **
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** SS           Steffen Storandt                                              **
**                                                                            **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* 
 * V0.2.0: 2012-12-13, SS:   Formal changes
 * V0.1.0: 2012-11-12, SS:   Initial version
 */


#ifndef TYPES_H
#define TYPES_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

/******************************************************************************
**                      Global Macro Definitions                             **
*******************************************************************************/
/** \brief Standard real type definitions to ease the use of different types
 */
#define STD_REAL_FIX_POINT_16 (1)
#define STD_REAL_FIX_POINT_32 (2)
//#define STD_REAL_FLOAT_32     (3)

#ifndef STD_REAL_TYPE
#define STD_REAL_TYPE (STD_REAL_FIX_POINT_16)
#endif


/** \brief Boolean definitions
 */
#ifndef TRUE 
#define TRUE  (1U)
#endif
#ifndef FALSE
#define FALSE (0U)
#endif 




#define NULL_PTR ((void*)0)

/*******************************************************************************
**                      Global Type Definitions                               **
*******************************************************************************/
typedef unsigned char uint8;           /**< \brief 8 bit unsigned value  */
typedef unsigned short uint16;         /**< \brief 16 bit unsigned value */
typedef unsigned int uint32;           /**< \brief 32 bit unsigned value */
//typedef unsigned long long uint64;     < \brief 64 bit unsigned value 

//typedef signed char sint8;             *< \brief 8 bit signed value  
typedef signed short sint16;           /**< \brief 16 bit signed value */
typedef signed int sint32;             /**< \brief 32 bit signed value */
//typedef signed long long int64;        < \brief 64 bit signed value 

//typedef float   float32;               *< \brief 32 bit float value 
//typedef double  float64;               *< \brief 64 bit float value 

//typedef short FixPoint16;              *< \brief 16 bit fix point value 
//typedef long  FixPoint32;              *< \brief 32 bit fix point value 

//typedef unsigned char boolean;         *< \brief for use with TRUE/FALSE 


/** \brief Standard real type definitions to ease the use of different types
 */
#if (STD_REAL_TYPE == STD_REAL_FIX_POINT_16)
//typedef FixPoint16 TStdReal;
//typedef FixPoint32 TLongStdReal;
#elif (STD_REAL_TYPE == STD_REAL_FIX_POINT_32)
typedef FixPoint32 TStdReal;
typedef FixPoint32 TLongStdReal;
#elif (STD_REAL_TYPE == STD_REAL_FLOAT_32)
typedef float32 TStdReal;
typedef float32 TLongStdReal;
#endif


/** \brief Complex type definition based on TStdReal
 */
/*typedef struct _TStdRealComplex
{
  TStdReal imag;   *< \brief Imaginary part 
  TStdReal real;   *< \brief Real part 
} TStdRealComplex;*/

/** \brief Complex type definition
 */
typedef struct
{
  sint16 Real;  /**< \brief Real part */
  sint16 Imag;  /**< \brief Imaginary part */
}TComplex;

/** \brief 2 phase currents type definition
 */
typedef struct
{
  sint16 A;   /**< \brief Phase A current */
  sint16 B;   /**< \brief Phase B current */
	sint16 C;   /**< \brief Phase C current */
}TPhaseCurr;



//typedef enum 
//{ 
//	false = 0, 
//	true = 1 
//} bool;

#endif /* TYPES_H */

