/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : Std_Types.h
* Project         : 
* Created         : YYYY/MM/DD
* Description     : 
* 
* 
************************************************************************************************************************
* Revison History:
* YYYY/MM/DD             Version              Description                      Author
* 2018/01/09             v1.0                 Draft first version
*
************************************************************************************************************************
*/


#ifndef STD_TYPES_H
#define STD_TYPES_H
/*
************************************************************************************************************************
*                                                   Defines and macros
************************************************************************************************************************
*/
#ifndef TRUE                      /* conditional check */
#define TRUE  1u
#endif
 
#ifndef FALSE                     /* conditional check */
#define FALSE 0u
#endif
 
#ifndef CONST
#define CONST     const           /* configuration constants are stored in ROM */
#endif
 
#ifndef EXTERN
#define EXTERN    extern
#endif


/*
************************************************************************************************************************
*                                                   Platform Typedef
************************************************************************************************************************
*/
typedef signed char    sint8_t;               /*        -128 .. +127            */
typedef unsigned char  uint8_t;               /*           0 .. 255             */
typedef signed short   sint16_t;              /*      -32768 .. +32767          */
typedef unsigned short uint16_t;              /*           0 .. 65535           */
typedef signed long    sint32_t;              /* -2147483648 .. +2147483647     */
#ifndef __stdint_h
typedef unsigned long  uint32_t;              /*           0 .. 4294967295      */
#endif
typedef unsigned char  boolean_t;             /* for use with TRUE/FALSE        */

typedef uint32_t           uintptr_t;


typedef uint8_t Std_ReturnType;

#define E_OK 					   (Std_ReturnType)0
#define E_NOT_OK 				(Std_ReturnType)1


extern sint16_t iirVdc(sint16_t NewSample);
extern sint16_t iirIdc(sint16_t NewSample);
extern sint16_t iirIq(sint16_t NewSample);
extern sint16_t iirSpeed(sint16_t NewSample);
extern sint16_t iirTemperature(sint16_t NewSample);
extern sint16_t iirIgn(sint16_t NewSample);
//extern sint16_t firFluxAbsValue(sint16_t NewSample);


#endif    /* STD_TYPES_H */

