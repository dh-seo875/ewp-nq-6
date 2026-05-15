/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : Emo_interface.h
* Project         : 
* Created         : YYYY/MM/DD
* Description     : 
* 
* 
************************************************************************************************************************
* Revison History:
* YYYY/MM/DD             Version              Description                      Author
* 2018/02/09             v1.0                 Draft first version              Alex.kim
*
************************************************************************************************************************
*/


#ifndef EMO_INTERFACE_H
#define EMO_INTERFACE_H

/*
************************************************************************************************************************
*                                                  Include header files
************************************************************************************************************************
*/
#include "Std_Types.h"
#include "Emo_RAM.h"
#include "bdrv.h"


/*
************************************************************************************************************************
*                                                   Defines and macros
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                                         Typedefs
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                                    Exported variables
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                               Exported function prototypes
************************************************************************************************************************
*/
__STATIC_INLINE sint16_t Emo_GetIdFixedPoint(void)
{
    return (Emo_Foc.RotCurr.Real * (-1));
}


__STATIC_INLINE sint16_t Emo_GetIqFixedPoint(void)
{
    return (Emo_Foc.RotCurr.Imag * (-1));
}


__STATIC_INLINE sint16_t Emo_GetVdFixedPoint(void)
{
    return (Emo_Foc.RotVolt.Real * (-1));
}


__STATIC_INLINE sint16_t Emo_GetVqFixedPoint(void)
{
    return (Emo_Foc.RotVolt.Imag * (-1));
}

__STATIC_INLINE sint32_t Emo_GetIdcDecimalPoint(void)
{
    sint32 Idc = 0;
    sint32 Iq_imag_float = 0;
    sint32 Iq_real_float = 0;
    sint32 Idc_imag = 0;
    sint32 Idc_real = 0;
    
	    /* 166.666 A = 166666 / multifly 1000 */
    Iq_imag_float = ((sint32)Emo_Foc.RotCurr.Imag *166666) / 32767;
    Iq_real_float = ((sint32)Emo_Foc.RotCurr.Real *166666) / 32767;        
    Idc_imag = (Iq_imag_float * (sint32)Emo_Foc.RotVolt.Imag) / Emo_Ctrl.ImagCurrPi.PiMax;
    Idc_real = (Iq_real_float * (sint32)Emo_Foc.RotVolt.Real) / Emo_Ctrl.RealCurrPi.PiMax;
    
    /* Idc = ((Iq * Vq) + (Id * Vd)) * (efficient * coffcient) */
    Idc = (((Idc_imag) + (Idc_real)) * 85) / 100;	
    
    return ((sint16_t)Idc);
}


__STATIC_INLINE sint16_t Emo_GetActSpeedDecimalPoint(void)
{
    return (Emo_Ctrl.ActSpeed * (-1));
}


__STATIC_INLINE sint16_t Emo_GetRefSpeedDecimalPoint(void)
{
    return (Emo_speedRef);
}


__STATIC_INLINE void Emo_SetSpeedDecimalPoint(sint16 refSpeed)
{
    Emo_speedRef = refSpeed;
}


__STATIC_INLINE sint16_t Emo_GetSpeedErrorDecimalPoint(void)
{
    return ((Emo_Ctrl.RefSpeed - Emo_Ctrl.ActSpeed) * (-1));
}


__STATIC_INLINE sint16_t Emo_GetFluxAbsValue(void)
{
    return (FluxAbsValueTemp);
}

__STATIC_INLINE uint8_t Emo_GetPowereLimitStatePoint(void)
{
    return (Power_Limit_Flag);
}


__STATIC_INLINE uint8_t Emo_GetMotorState(void)
{
    return (uint8_t)Emo_Status.MotorState;
}

__STATIC_INLINE sint16_t Emo_GetPhaseCurrA(void)
{
    return (sint16_t)Emo_Svm.PhaseCurr.A;
}

__STATIC_INLINE sint16_t Emo_GetPhaseCurrB(void)
{
    return (sint16_t)Emo_Svm.PhaseCurr.B;
}

__STATIC_INLINE sint16_t Emo_GetPhaseCurrC(void)
{
    return (sint16_t)Emo_Svm.PhaseCurr.C;
}

#endif    /* EMO_INTERFACE_H */

