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
**                      Revision Control History                              **
*******************************************************************************/
/* See Emo.c */

#ifndef EMO_H
#define EMO_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "tle_device.h"
#include "Emo.h"
#include "Mat.h"
#include "Table.h"
#include "Variant_cfg.h"
/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
/* Mon16 enable
 * Range: 0=disabled, 1=enabled */
//#define EMO_CFG_MON16_ENABLED (1)

/* DC-Link current not in the allowed range */
//#define EMO_E_DCLINK_CURRENT_FAILURE (0u)

/* Temperature is above the MaxTemperature */
//#define EMO_E_OVER_TEMP_FAILURE      (2u)

/* Over-voltage shutdown */
//#define EMO_E_MOTOR_CURRENT_FAILURE  (3u)

/* Motor states */
#define EMO_MOTOR_STATE_UNINIT (0u)
#define EMO_MOTOR_STATE_STOP   (1u)
#define EMO_MOTOR_STATE_START  (2u)
#define EMO_MOTOR_STATE_RUN    (3u)
#define EMO_MOTOR_STATE_FAULT  (4u)

#define EMO_ERROR_NONE              (0u)
#define EMO_ERROR_MOTOR_INIT        (1u)
#define EMO_ERROR_MOTOR_NOT_STOPPED (2u)
#define EMO_ERROR_MOTOR_NOT_STARTED (3u)

/* Motor Start error */
//#define EMO_ERROR_VALUE_CU_KI           (0x0001)
//#define EMO_ERROR_VALUE_CU_KP           (0x0002)
//#define EMO_ERROR_VALUE_CU_ADCC         (0x0004)
//#define EMO_ERROR_LIMITS_REFCURRENT     (0x0008)
//#define EMO_ERROR_REFCURRENT            (0x0010)
//#define EMO_ERROR_SPEED_POINTS          (0x0020)
//#define EMO_ERROR_T_SPEED_LP            (0x0040)
//#define EMO_ERROR_STARTCURRENT          (0x0080)
//#define EMO_ERROR_STARTTIME             (0x0100)
//#define EMO_ERROR_SPEEDSLEWRATE         (0x0200)
//#define EMO_ERROR_MINTIME               (0x0400)
//#define EMO_ERROR_POLPAIR               (0x0800)
#define EMO_ERROR_CSAOFFSET             (0x1000u)

/* Svm Parameter */
/* EMO_SVM_MINTIME defines the minimum time slot required to place **
** the ADC measurement inside the 3phase PWM pattern               **
** 80 * CCU6_CLK = 80 * 25ns = 2us                                 */
//#define EMO_SVM_MINTIME           (80)
#define EMO_SVM_DEADTIME          (30u)
#define EMO_DECOUPLING            (0u)

/* FluxEstimator filter time */
//#define FOC_ESTFLUX_FILT_TIME FOC_FLUX_ADJUST

/*0 = stays in open-loop operation, 1 = switch into closed-loop operation */
#define EMO_RUN                                   (1)


//#define Emo_Foc_PolePair     3
#define Emo_Foc_PhaseRes 28937 // 32768.0 * KI * Emo_Focpar_Cfg.PhaseRes / KU;

#define Emo_Foc_PhaseInd 10256	 // 32768.0 * KI * Emo_Focpar_Cfg.PhaseInd / KPSIE;

/*******************************************************************************
**                      Global Type Definitions                              **
*******************************************************************************/
/** \brief PI configuration */
//typedef struct
//{
//    sint16 Kp;                  < \brief Proportional parameter 
//    sint16 Ki;                  < \brief Integral parameter
//    sint16 IMin;                < \brief Minimum of I output
//    sint16 IMax;                < \brief Maximum of I output 
//    sint16 PiMin;               < \brief Minimum of PI output 
//    sint16 PiMax;               < \brief Maximum of PI output 
//} TEmo_Pi_Cfg;

/** \brief Low pass configuration */
//typedef struct
//{
//    sint16 CoefA;                   < \brief Coefficient A 
//    sint16 CoefB;                   < \brief Coefficient B 
//    sint16 Min;                    < \brief Minimum 
//    sint16 Max;                    < \brief Maximum 
//} TEmo_Lp_Cfg;

/** \brief FOC status */
typedef struct
{
    TComplex StatCurr;              /**< \brief Stationary current 0*/
    TComplex RotCurr;               /**< \brief Rotating current 2*/
    TComplex StatVolt;              /**< \brief Stationary voltage 4*/
    TComplex RotVolt;               /**< \brief Rotating voltage 6*/
    uint16 Angle;                   /**< \brief Angle 8*/
    uint16 FluxAngle;               /**< \brief Angle calculated by flux estimator 9*/
    //uint16 StoredAngle;             < \brief Stored Angle 10

    TMat_Lp_Simple RealFluxLp;      /**< \brief Real flux low pass 16*/
    TMat_Lp_Simple ImagFluxLp;      /**< \brief Imaginary flux low pass 20*/
    uint8 CountStart;              /**< \brief Counter for Start with angle=0 24*/
    sint16 StartCurrent;            /**< \brief Start Current 26*/

    //uint16 CsaGain;                 < \brief CsaGain Faktor 32
   
    uint16 Dcfactor1;               /**< \brief 327670/Dc Link Voltage 34*/
    sint16 StartSpeedSlope;         /**< \brief Start Speed Slope 35*/
    sint16 StartFrequencySlope;     /**< \brief Start Frequency Slope 36*/
    sint32 StartSpeedSlopeMem;      
    TComplex StartVoltage;          /**< \brief Startvoltage for Startmode 1*/
    uint16 StartAngle;              /**< \brief Start angle for Mode 0 and 1 */
    TComplex RotVoltCurrentcontrol;
    //uint16 StartVoltAmpDivUz;       < \brief Voltage offset for start with mode 1 14
    uint16 Dcfactor2;               /**< \brief Kfactor2*Dc Link Voltage 34*/

    //uint16 Kdcfactor2;              < \brief Factor for K*Uz 34

    uint16 StatVoltAmpM;            /**< \brief Amplitude for Stat. Voltage FM 34*/

} TEmo_Foc;


/** \brief FOC configuration */
//typedef struct
//{
//    float Rshunt;
//    float NominalCurrent;      
//    float PWM_Frequency;
//    float PhaseRes;             
//    float PhaseInd;          
//    uint16 SpeedPi_Kp;           
//    uint16 SpeedPi_Ki;        
//    float MaxRefCurr;           
//    float MinRefCurr;         
//    float MaxRefStartCurr;      
//    float MinRefStartCurr;     
//    float SpeedLevelPos;         
//    float SpeedLevelNeg;        
//    float TimeConstantSpeedFilter;  
//    float TimeConstantEstFluxFilter;
//    uint16 CsaOffset;          
//    uint16 PolePair;      
//    float StartCurrent;         
//    float TimeSpeedzero;         
//    float StartSpeedEnd;       
//    float StartSpeedSlewRate;  
//    uint16 EnableFrZero;        
//    float SpeedLevelSwitchOn; 
//    float AdjustmCurrentControl; 
//    float MaxSpeed;               
//} TEmo_Focpar_Cfg;



/** \brief Control status */
typedef struct
{
    sint16 RefSpeed;                /**< \brief Reference speed 0*/
    sint16 ActSpeed;                /**< \brief Actual speed 1*/
    sint16 RefCurr;                 /**< \brief Reference current (imaginary) 2*/
    sint16 ActSpeeddisplay;         /**< \brief Actuel speed display3*/
      
    TMat_Pi SpeedPi;                /**< \brief Speed PI control */
    TMat_Pi RealCurrPi;             /**< \brief Real current PI control */
    TMat_Pi ImagCurrPi;             /**< \brief Imaginary current PI control */
    TMat_Lp_Simple SpeedLp;         /**< \brief Speed low pass */
    uint16 AngleBuffer[16];         /**< \brief buffer for angle */
    uint16 PtrAngle;                /**< \brief pointer for anglebuffer */
    sint16 MaxRefCurrent;           /*  69  */
    sint16 MinRefCurrent;           /*  70  */
    sint16 MaxRefStartCurrent;      /*  71  */
    sint16 MinRefStartCurrent;      /*  72  */

    TMat_Lp_Simple SpeedLpdisplay;  /**< \brief Speed low pass 76*/
    TMat_Lp_Simple FluxbtrLp;       /* 82 */
    sint16 Speedest;                /* 88 */
    sint16 Speedpll;                /* 89 */
    uint16 FluxAnglePll;            /* 90 */
    sint16 RefCurrD;                /**< \brief D-axis current reference */
    sint16 RefCurrQ;                /**< \brief Q-axis current reference */

} TEmo_Ctrl;


/** \brief Space vector modulation status */
typedef struct
{
    uint16 Angle;                   /**< \brief Angle 0*/
    uint16 Amp;                     /**< \brief Amplitude 1*/
    uint16 Sector;                  /**< \brief Sector number 2*/
    uint16 comp60up;                /**< \brief Caomparecc60_up 3*/
    uint16 T1;                      /**< \brief Time T1 4*/
    uint16 comp61up;                /**< \brief Caomparecc61_up T1 5*/
    uint16 T2;                      /**< \brief Time T2 6*/
    uint16 comp62up;                /**< \brief Caomparecc62_up 7*/
    TPhaseCurr PhaseCurr;           /**< \brief Phase current 8*/
    uint16 CsaOffset;               /**< \brief Offset of current sense amplifier 11*/
    uint16 MaxAmp;                  /**< \brief Maximum amplitude 12*/
    uint16 MaxAmp9091pr;            /**< \brief Maximum amplitude 90 prozent13*/
    uint16 MaxAmp4164pr;            /**< \brief Maximum amplitude 41 prozent14*/
    uint16 Kfact256;                /**< \brief Faktor Maximum Amp auf 256 Inc 15*/
    uint32 MaxAmpQuadrat;           /**< \brief Maximum amplitude Quadrat 16*/
    uint16 CompT13ValueUp;
    uint16 CompT13ValueDown;
    //uint16 T13Trigger;
    uint16 comp60down;               /**< \brief Caomparecc60_down 21*/
    uint16 comp61down;               /**< \brief Caomparecc61_down 22*/
    uint16 comp62down;               /**< \brief Caomparecc62_down 23*/
    uint16 StoredSector1;            /**< \brief Stored Sector nr 24*/
    uint16 CounterOffsetAdw;         /**< \brief Counter for Adw Offsetestimation*/
    uint32 CsaOffsetAdwSumme;        /**< \brief OffsetValue Adw*/	
    uint16 CsaOffsetAdw;             /**< \brief OffsetValue Adw*/	
} TEmo_Svm;



/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/
//extern TEmo_Status Emo_Status;


extern  TEmo_Ctrl Emo_Ctrl;

//extern const TEmo_Focpar_Cfg Emo_Focpar_Cfg;
extern TEmo_Foc Emo_Foc;


extern TEmo_Svm Emo_Svm;
extern uint16 Emo_AdcResult[4u];
extern sint16 FluxAbsValueTemp;
/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/

extern uint16 speeduserreferenz;
extern void Emo_HandleCCU6ShadowTrans(void);
extern void Emo_HandleAdc1(void);
extern void Emo_HandleFoc(void);

extern void Emo_InitFoc(void);

extern void Emo_ExeSvmTest(TEmo_Svm *pSvm);
extern void Emo_EstFluxTest(void);
extern uint16 Emo_CalcAngleAmpTest(TComplex Stat, uint16 *pAmp);
extern void Emo_CalcAngleAmpSvmTest(void);
extern void Emo_setspeedreferenz(uint16 speedreferenz);

__STATIC_INLINE TComplex Limitsvektor(const TComplex *inp, const TEmo_Svm *par);
//__STATIC_INLINE TComplex Limitsvektorphase(TComplex *inp, TEmo_Svm *par);

/* INLINE functions ***********************************************************/

//extern sint16 abs(sint16 inp);

/*******************************************************************************
**                      Global Inline Function Definitions                    **
*******************************************************************************/


/** \brief Performs Limits Raumvektor algorithm.
 *
 * \param[inout]
 * \param[in] Input TComplex in fixed-point format
 *
 * \return  output TComplex in fixed-point format
 * \ingroup math_api
 */
__STATIC_INLINE TComplex Limitsvektor(const TComplex *inp, const TEmo_Svm *par)
{
    TComplex outp;
    sint32 re = (sint32)inp->Real;
    sint32 im = (sint32)inp->Imag;
    uint32 btrqu;
	  sint32 abs_re;
	  sint32 max_90;
	  uint32 inpa;
	  uint32 lim_im;

    btrqu = (((uint32)re * (uint32)re) + ((uint32)im * (uint32)im));

    outp.Real = (sint16)re;
    outp.Imag = (sint16)im;

    if (btrqu > par->MaxAmpQuadrat)
    {
        abs_re = (re < 0) ? -re : re;
        max_90 = (sint32)par->MaxAmp9091pr;

        if (abs_re > max_90)
        {
            outp.Real = (re < 0) ? -(sint16)max_90 : (sint16)max_90;
            outp.Imag = (im < 0) ? -(sint16)par->MaxAmp4164pr : (sint16)par->MaxAmp4164pr;
        }
        else
        {
            inpa = ((uint32)abs_re * (uint32)par->Kfact256) >> MAT_FIX_SHIFT;
            
            if (inpa > 256u) { inpa = 256u; }

            lim_im = ((uint32)Table_sqrtmqu[inpa] * (uint32)par->MaxAmp) >> 8u;

            outp.Imag = (im < 0) ? -(sint16)lim_im : (sint16)lim_im;
        }
    }

    return outp;
} /* End of Limitsvektor */


/** \brief Performs Limits Raumvektor algorithm.
 *
 * \param[inout]
 * \param[in] Input TComplex in fixed-point format
 *
 * \return  output TComplex in fixed-point format
 * \ingroup math_api
 */
//__STATIC_INLINE TComplex Limitsvektorphase(TComplex *inp, TEmo_Svm *par)
//{
//    uint32 btrqu;
//    uint32 btrfactor;
//    TComplex outp = {0, 0};
//    btrqu = ((uint32)inp->Real * (uint32)inp->Real) + ((uint32)inp->Imag * (uint32)inp->Imag);

//    if (btrqu > par->MaxAmpQuadrat)
//    {
//        btrfactor = par->MaxAmpQuadrat / (btrqu >> 15);
//        btrfactor = 32500u - ((32767u - btrfactor) >> 1u);
//        outp.Real = (sint16)__ssat(((sint32)inp->Real * (sint32)btrfactor / 32768), 16u);
//        outp.Imag = (sint16)__ssat(((sint32)inp->Imag * (sint32)btrfactor / 32768), 16u);
//    }
//    else
//    {
//        outp.Real = inp->Real;
//        outp.Imag = inp->Imag;
//    }

//    return outp;
//}  End of Limitsvektorphase 


#endif /* EMO_H */

