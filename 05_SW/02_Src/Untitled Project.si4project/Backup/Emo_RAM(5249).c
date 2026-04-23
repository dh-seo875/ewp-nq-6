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
 * V0.4.0: 2013-06-03, SS:   DC-Link current evaluation added, FOC SL functionality added
 * V0.3.0: 2013-02-25, SS:   PI control for speed changed
 * V0.2.0: 2012-12-13, SS:   CtrlSpeed added
 * V0.1.0: 2012-11-12, SS:   Initial version
 */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Emo_RAM.h"
#include "Usr.h"

/******************************************************************************
**                      Private Macro Definitions                            **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
static void Emo_CurrAdc1(void);
void Emo_HandleCCU6ShadowTrans(void);
 void Emo_HandleFoc(void);


__STATIC_INLINE void Emo_lEstFlux(void);
__STATIC_INLINE void Emo_FluxAnglePll(void);
__STATIC_INLINE void Emo_lExeSvm(TEmo_Svm *pSvm);

#if (EMO_DECOUPLING==1)
    __STATIC_INLINE TComplex Emo_CurrentDecoupling(void);
#endif

#define EMO_IMESS  1

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/
//extern TEmo_Status Emo_Status;
sint16 FluxAbsValueTemp;
static uint32 Emo_AdcResult[4u];
TEmo_Ctrl Emo_Ctrl;
TEmo_Foc Emo_Foc;
TEmo_Svm Emo_Svm;


const uint16 Table_Sin60[256] =
{
    0, 16, 32, 47
    , 63, 79, 95, 110
    , 126, 142, 158, 173
    , 189, 205, 220, 236
    , 252, 268, 283, 299
    , 315, 330, 346, 362
    , 377, 393, 409, 424
    , 440, 456, 472, 487
    , 503, 518, 534, 550
    , 565, 581, 596, 612
    , 627, 643, 659, 674
    , 690, 705, 720, 736
    , 751, 767, 782, 798
    , 813, 829, 844, 859
    , 875, 890, 905, 921
    , 936, 951, 966, 982
    , 997, 1012, 1027 ,1042
    , 1058, 1073, 1088, 1103
    , 1118, 1133, 1148, 1163
    , 1178, 1193, 1208, 1223 //20
    , 1238, 1253, 1268, 1283
    , 1298, 1312, 1327, 1342
    , 1357, 1371, 1386, 1401
    , 1416, 1430, 1445, 1459
    , 1474, 1489, 1503, 1518
    , 1532, 1547, 1561, 1575
    , 1590, 1604, 1618, 1633
    , 1647, 1661, 1675, 1689
    , 1704, 1718, 1732, 1746
    , 1760, 1774, 1788, 1802
    , 1816, 1830, 1843, 1857
    , 1871, 1885, 1899, 1912
    , 1926, 1940, 1953, 1967
    , 1980, 1994, 2007, 2021
    , 2034, 2047, 2061, 2074
    , 2087, 2101, 2114, 2127
    , 2140, 2153, 2166, 2179
    , 2192, 2205, 2218, 2231
    , 2244, 2256, 2269, 2282
    , 2295, 2307, 2320, 2332 //40
    , 2345, 2357, 2370, 2382
    , 2395, 2407, 2419, 2431
    , 2444, 2456, 2468, 2480
    , 2492, 2504, 2516, 2528
    , 2540, 2552, 2563, 2575
    , 2587, 2598, 2610, 2622
    , 2633, 2645, 2656, 2667
    , 2679, 2690, 2701, 2713
    , 2724, 2735, 2746, 2757
    , 2768, 2779, 2790, 2801
    , 2811, 2822, 2833, 2844
    , 2854, 2865, 2875, 2886
    , 2896, 2906, 2917, 2927
    , 2937, 2947, 2958, 2968
    , 2978, 2988, 2998, 3007
    , 3017, 3027, 3037, 3046
    , 3056, 3065, 3075, 3084
    , 3094, 3103, 3113, 3122
    , 3131, 3140, 3149, 3158
    , 3167, 3176, 3185, 3194 //60
    , 3203, 3211, 3220, 3229
    , 3237, 3246, 3254, 3263
    , 3271, 3279, 3288, 3296
    , 3304, 3312, 3320, 3328 //64
};

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

/* FOC functions *************************************************************/

/** \brief Handles the ADC1.
 *
 * \param none
 *
 * \return None
 * \ingroup emo_api
 */
void Emo_HandleAdc1(void)
{
    /*prepare Timer13 for 2nd ADC measurement, **
    **values calculated in previous period     */
    CCU6_SetT13Compare(Emo_Svm.CompT13ValueDown);
    /* Set Timer13 Trigger calculated previous period pm*/
    CCU6_SetT13Trigger(0x76);
    /*read ADC digital value and store in array*/
    Emo_AdcResult[0u] = (uint32)ADC1->RES_OUT1.reg;
    /*disable ESM interrupt*/
    ADC1->IE.bit.ESM_IE = 0u;
}


static void Emo_CurrAdc1(void)
{
	uint16 u16_AdcResult0;
	uint16 u16_AdcResult1;
    sint16 AdcResult0;
    sint16 AdcResult1;
    uint16 sector;
    sint16 R0mioffs;
    sint16 OffsmiR1;
    sint16 R1miR0;
    
    /*AdcResult[2] => 1st ADC Meas. **
    **AdcResult[1] => 2nd ADC Meas. */
    u16_AdcResult0 = (uint16)(Emo_AdcResult[2u] & 0x0FFFu);
    AdcResult0 = (sint16)u16_AdcResult0;
	u16_AdcResult1 = (uint16)(Emo_AdcResult[1u] & 0x0FFFu);
    AdcResult1 = (sint16)u16_AdcResult1;
    Emo_AdcResult[3u]=(uint32)AdcResult0+(uint32)AdcResult1;
    /*result 0 minus CSA Offset*/
    R0mioffs = AdcResult0 - (sint16)Emo_Svm.CsaOffsetAdw;

    /*CSA Offset minus result 1*/
    OffsmiR1 = (sint16)Emo_Svm.CsaOffsetAdw - AdcResult1;
    /*Result 1 - Result 0*/
    R1miR0 = AdcResult1 - AdcResult0;
    /* Calculate currents according to sector number */
    sector = Emo_Svm.StoredSector1;

    switch (sector)
    {
        case 0:
        {
            Emo_Svm.PhaseCurr.A = R0mioffs;
            Emo_Svm.PhaseCurr.B = R1miR0;
					  /* FIND */
            Emo_Svm.PhaseCurr.C = OffsmiR1;
        }
        break;

        case 1:
        {
            Emo_Svm.PhaseCurr.A = R1miR0;
            Emo_Svm.PhaseCurr.B = R0mioffs;
					  /* FIND */
            Emo_Svm.PhaseCurr.C = OffsmiR1;
        }
        break;

        case 2:
        {
            Emo_Svm.PhaseCurr.A = OffsmiR1;
            Emo_Svm.PhaseCurr.B = R0mioffs;
					  /* FIND */
            Emo_Svm.PhaseCurr.C = R1miR0;
        }
        break;

        case 3:
        {
            Emo_Svm.PhaseCurr.A = OffsmiR1;
            Emo_Svm.PhaseCurr.B = R1miR0;
					  /* FIND */
            Emo_Svm.PhaseCurr.C = R0mioffs;
        }
        break;

        case 4:
        {
            Emo_Svm.PhaseCurr.A = R1miR0;
            Emo_Svm.PhaseCurr.B = OffsmiR1;
					  /* FIND */
            Emo_Svm.PhaseCurr.C = R0mioffs;
        }
        break;

        case 5:
        {
            Emo_Svm.PhaseCurr.A = R0mioffs;
            Emo_Svm.PhaseCurr.B = OffsmiR1;
					  /* FIND */
            Emo_Svm.PhaseCurr.C = R1miR0;
        }
        break;
        default:
        {
            /* unexpected error => halt motor, reset device */
            (void)Emo_StopMotor();
        }
        break;
    }
    CheckOpenPhaseState(&Emo_Svm);

    /*prepare sector number for next period*/
    Emo_Svm.StoredSector1 = Emo_Svm.Sector;
} /* End of Emo_HandleAdc1 */


void Emo_HandleCCU6ShadowTrans(void)
{
    CCU6_LoadShadowRegister_CC60(Emo_Svm.comp60up);
    CCU6_LoadShadowRegister_CC61(Emo_Svm.comp61up);
    CCU6_LoadShadowRegister_CC62(Emo_Svm.comp62up);
    CCU6_EnableST_T12();
    /* Set Timer13 Trigger calculated previous period zero-match*/
    CCU6_SetT13Trigger(0x7a);
    /* Set T13 Compare Value */
    CCU6_SetT13Compare(Emo_Svm.CompT13ValueUp);
    /*disable Timer12 PM Interrupt*/
    CCU6->IEN.bit.ENT12PM = 0u;
}

void Emo_HandleFoc(void)
{
    uint16 angle;
    uint16 ampl;
    uint16 i;
    TComplex Vect1 = {0, 0};
    TComplex Vect2;
    sint32 Speed;
    sint32 jj;
		
    Emo_AdcResult[2u] = Emo_AdcResult[0u];
    /* Enable ADC Interrupt */
    ADC1->ICLR.bit.ESM_ICLR = 1u;
    ADC1->IE.bit.ESM_IE = 1u;
    /*get 2nd ADC measurement from previous period*/
    Emo_AdcResult[1u] = (uint32_t)ADC1->RES_OUT1.reg;
    /*set PWM compare values for T12 down-counting part*/
    CCU6_LoadShadowRegister_CC60(Emo_Svm.comp60down);
    CCU6_LoadShadowRegister_CC61(Emo_Svm.comp61down);
    CCU6_LoadShadowRegister_CC62(Emo_Svm.comp62down);
    CCU6_EnableST_T12();
    Emo_CurrAdc1();
    /* Perform Clarke transformation to stationary 2-phase system */
    Emo_Foc.StatCurr = Mat_Clarke(Emo_Svm.PhaseCurr);
    
    /* Perform Park transformation to rotating 2-phase system */
    Emo_Foc.RotCurr = Mat_Park(Emo_Foc.StatCurr, Emo_Foc.Angle);
		
    /* Estimate flux and calculate rotor angle */
    Emo_lEstFlux();

    if (Emo_Status.MotorState == EMO_MOTOR_STATE_START)
    {
        /* Open loop: */
        /* Increment angle */
        Emo_Foc.StartAngle += (uint16)Emo_Foc.StartFrequencySlope;
        Emo_Foc.Angle = Emo_Foc.StartAngle;
        Emo_Ctrl.PtrAngle = (Emo_Ctrl.PtrAngle + 1u) & 0x1fu;

        if (Emo_Ctrl.Anglersptr == 32u)
        {
            angle = Emo_Ctrl.AngleBuffer[Emo_Ctrl.PtrAngle];
            Emo_Ctrl.AngleBuffer[Emo_Ctrl.PtrAngle] = Emo_Foc.FluxAngle;
            Emo_Ctrl.Speedest = (sint16)Emo_Foc.FluxAngle - (sint16)angle;
        }
        else
        {
            Emo_Ctrl.AngleBuffer[Emo_Ctrl.PtrAngle] = Emo_Foc.FluxAngle;
            Emo_Ctrl.Speedest = (sint16)Emo_Foc.FluxAngle - (sint16)Emo_Ctrl.AngleBuffer[(Emo_Ctrl.PtrAngle - Emo_Ctrl.Anglersptr) & 0x1fu];
        }

        Emo_Ctrl.Speedpll = (Emo_Ctrl.Speedest / (sint16)Emo_Ctrl.Exppllhigh);

        /* jj => electrical rotation speed */
        jj = ((((sint32)Emo_Ctrl.Speedest * (sint32)Emo_Ctrl.Factorspeed)) / ((sint32)32768 / (sint32)Emo_Ctrl.Expspeedhigh));

        /* calculate mech. speed out of electrical speed (jj), jj not further used */
        Speed = jj / (sint32)Emo_Foc.PolePair;
        /* Filter speed */
        Emo_Ctrl.ActSpeed = Mat_ExeLp_without_min_max(&Emo_Ctrl.SpeedLp, (sint16)Speed);
        Emo_FluxAnglePll();

        if (Emo_Ctrl.RefSpeed > 0)
        {
            Emo_Ctrl.RefCurr = Emo_Foc.StartCurrent;
        }
        else
        {
            Emo_Ctrl.RefCurr = -Emo_Foc.StartCurrent;
        }

        /* Current Regulator: Execute PI algorithm for rotating voltage */
        /* id */
        Emo_Foc.RotVolt.Real = Mat_ExePi(&Emo_Ctrl.RealCurrPi, Emo_Ctrl.RefCurr - Emo_Foc.RotCurr.Real);
        /* iq */
        Emo_Foc.RotVolt.Imag = Mat_ExePi(&Emo_Ctrl.ImagCurrPi,  (-Emo_Foc.RotCurr.Imag));
    }
    else /* (Emo_Status.MotorState == EMO_MOTOR_STATE_RUN) */
    {
        /* Closed loop: */
        /* Speed calculation */
        Emo_Ctrl.PtrAngle = (Emo_Ctrl.PtrAngle + 1u) & 0x1fu;

        if (Emo_Ctrl.Anglersptr == 32u)
        {
            angle = Emo_Ctrl.AngleBuffer[Emo_Ctrl.PtrAngle];
            Emo_Ctrl.AngleBuffer[Emo_Ctrl.PtrAngle] = Emo_Foc.FluxAngle;
            Emo_Ctrl.Speedest = (sint16)Emo_Foc.FluxAngle - (sint16)angle;
        }
        else
        {
            Emo_Ctrl.AngleBuffer[Emo_Ctrl.PtrAngle] = Emo_Foc.FluxAngle;
            Emo_Ctrl.Speedest = (sint16)Emo_Foc.FluxAngle - (sint16)Emo_Ctrl.AngleBuffer[(Emo_Ctrl.PtrAngle - Emo_Ctrl.Anglersptr) & 0x1fu];
        }

        Emo_Ctrl.Speedpll = (Emo_Ctrl.Speedest / (sint16)Emo_Ctrl.Exppllhigh);

        /* jj => electrical rotation speed */
        jj = ((((sint32)Emo_Ctrl.Speedest * (sint32)Emo_Ctrl.Factorspeed)) / ((sint32)32768 / (sint32)Emo_Ctrl.Expspeedhigh));

        /* calculate mech. speed out of electrical speed (jj), jj not further used */
        Speed = jj / (sint32)Emo_Foc.PolePair;
        /* Filter speed */
        Emo_Ctrl.ActSpeed = Mat_ExeLp_without_min_max(&Emo_Ctrl.SpeedLp, (sint16)Speed);
        Emo_FluxAnglePll();
        /* assign PLL output angle to Emo_Foc.Angle */
        Emo_Foc.Angle = Emo_Ctrl.FluxAnglePll;
#if (EMO_DECOUPLING==0)
        /* Current Regulator: Execute PI algorithm for rotating voltage */
        /* id */
        Emo_Foc.RotVolt.Real = Mat_ExePi(&Emo_Ctrl.RealCurrPi, (-Emo_Foc.RotCurr.Real));

        /* iq */
        Emo_Foc.RotVolt.Imag = Mat_ExePi(&Emo_Ctrl.ImagCurrPi, Emo_Ctrl.RefCurr - Emo_Foc.RotCurr.Imag);
#else
        Emo_Foc.RotVoltCurrentcontrol.Real = Mat_ExePi(&Emo_Ctrl.RealCurrPi, (-Emo_Foc.RotCurr.Real));
        Emo_Foc.RotVoltCurrentcontrol.Imag = Mat_ExePi(&Emo_Ctrl.ImagCurrPi, Emo_Ctrl.RefCurr - Emo_Foc.RotCurr.Imag);
        /* Calculate Decoupling */
        Emo_Foc.RotVolt = Emo_CurrentDecoupling();
#endif
    }

    /* DC-link voltage correction */
    Vect1.Real = (sint16)__ssat(((sint32)Emo_Foc.RotVolt.Real * (sint32)Emo_Foc.Dcfactor1 / 16384), MAT_FIX_SAT);
    Vect1.Imag = (sint16)__ssat(((sint32)Emo_Foc.RotVolt.Imag * (sint32)Emo_Foc.Dcfactor1 / 16384), MAT_FIX_SAT);
    /* Limitation Algorithm */
    Vect2 = Limitsvektor(&Vect1, &Emo_Svm);
    /* Cartesian to Polar Transformation **
    ** outputs; angle, ampl              */
    angle = Mat_CalcAngleAmp(Vect2, &ampl);
    
    /* limitation of max. amplitude */
    if (ampl > Emo_Svm.MaxAmp)
    {
        ampl = Emo_Svm.MaxAmp;
    }

    if (Emo_Svm.CounterOffsetAdw > 127u)
    {
        Emo_Svm.Amp = ampl;
    }
    else
    {
        Emo_Svm.Amp    = 0u;
        Emo_Svm.CsaOffsetAdwSumme = Emo_Svm.CsaOffsetAdwSumme+Emo_AdcResult[3u];
        Emo_Svm.CounterOffsetAdw++;
        if (Emo_Svm.CounterOffsetAdw == 128u)
        {
            i = (uint16)(Emo_Svm.CsaOffsetAdwSumme >> 8u);
            if (i < Emo_Svm.CsaOffset)
            {
                i = Emo_Svm.CsaOffset;
            }
            else 
            {
                if (i > (Emo_Svm.CsaOffset+100u))
                {
                    i = Emo_Svm.CsaOffset + 100u;
                }
            }
            Emo_Svm.CsaOffsetAdw = i;
        }
    }

    /* sum of regulated angle out of Current Regulation + PLL-Angle */
    Emo_Svm.Angle = angle + Emo_Foc.Angle;

    /* Perform Polar-2-Cartesian transformation      **
    ** preparation of input values for FluxEstimator */
    Emo_Foc.StatVoltAmpM = (uint16)__ssat((((sint32)Emo_Svm.Amp * (sint32)Emo_Foc.Dcfactor2)/4096), MAT_FIX_SAT);
    Emo_Foc.StatVolt = Mat_PolarKartesisch(Emo_Foc.StatVoltAmpM, Emo_Svm.Angle);

    /* Perform space vector modulation */
    Emo_lExeSvm(&Emo_Svm);
    /* Filter for Iq  */
    Emo_Ctrl.RotCurrImagdisplay = Mat_ExeLp_without_min_max(&Emo_Ctrl.RotCurrImagLpdisplay, Emo_Foc.RotCurr.Imag);
} /* End of Emo_HandleFoc */


/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
/** \brief Performs space vector modulation.
 *
 * \param none
 *
 * \return None
 * \ingroup emo_api
 */

__STATIC_INLINE void Emo_lExeSvm(TEmo_Svm *pSvm)
{
    uint32 u_T1;
    uint32 u_T2;
    sint32 T1;
    sint32 T2;
    uint32 Sector;
    uint32 Angle;
    uint32 Index;
    uint32 Compare0up;
    uint32 Compare1up;
    uint32 Compare2up;
    uint32 Compare0down;
    uint32 Compare1down;
    uint32 Compare2down;
    uint16 T13ValueUp;
    uint16 T13ValueDown;
    uint16 i;
    uint16 per;
    sint32 ci;
    /* Calculate sector number 0..5 and table index 0..255 */
    Angle = ((uint32)pSvm->Angle) * 6u;
    Sector = (Angle >> 16u);
    pSvm->Sector = (uint16)Sector;
    Index = (Angle >> 8u) & 0xFFu;
    /* Calculate and limit times */
    u_T1 = (((uint32)pSvm->Amp) * Table_Sin60[255u - Index]) >> (MAT_FIX_SHIFT + 1u);
	T1 = (sint32)u_T1;
    pSvm->T1 = (uint16)T1;
    /* RandVector1 = Amp * sin(gamma) */
    u_T2 = (((uint32)pSvm->Amp) * Table_Sin60[Index]) >> (MAT_FIX_SHIFT + 1u);
	T2 = (sint32)u_T2;
    pSvm->T2 = (uint16)T2;

    /* in case of sector borders this defines the min. Null Vector duration **
    ** 10 ticks => 250ns@40MHz                                              **/
    per = CCU6_T12PR;

    /* Set compare values according to sector number */
    switch (Sector)
    {
        case 0u:
        {
            ci = ((sint32)CCU6_T12PR / 2) - T1 - T2;
            if (ci < EMO_SVM_DEADTIME) {ci = 0;};
            Compare0up = (uint32)ci;
            ci = ((sint32)CCU6_T12PR / 2) + T1 - T2;
            if (ci < EMO_SVM_DEADTIME) {ci = 0;};
            if (ci > ((sint32)CCU6_T12PR - EMO_SVM_DEADTIME)) {ci = (sint32)CCU6_T12PR + 1;};
            Compare1up = (uint32)ci;
            ci = ((sint32)CCU6_T12PR / 2) + T1 + T2;
            if (ci > ((sint32)CCU6_T12PR - EMO_SVM_DEADTIME)) {ci = (sint32)CCU6_T12PR + 1;};
            Compare2up = (uint32)ci;

            Compare0down = Compare0up;
            Compare1down = Compare1up;
            Compare2down = Compare2up;

#if (EMO_IMESS==1)
            if (T1 < (EMO_SVM_MINTIME / 2))
            {
                /* pattern has to be applied asymetric because T1 too short */
                i = (uint16)EMO_SVM_MINTIME - (uint16)T1 - (uint16)T1;
                Compare1up = Compare1up + i;

                if (Compare1up > per)
                {
                    Compare1up = per;
                }
                
                if (i < Compare1down)
                {
                    Compare1down = Compare1down - i;
                }
                else
                {
                    Compare1down = 1u;
                }
            }
            /* at least T1 has enough room for ADC measurement **
            ** now check also T2 against min. SVM Time         */
            if (T2 < (EMO_SVM_MINTIME / 2))
            {
                /* pattern has to be applied asymetric because of T2 too short */
                i = (uint16)EMO_SVM_MINTIME - (uint16)T2 - (uint16)T2;
                Compare1up = Compare1up + i;
                if (Compare1up > per)
                {
                    Compare1up = per;
                }
                
                if (i < Compare1down)
                {
                    Compare1down = Compare1down - i;
                }
                else
                {
                    Compare1down = 1u;
                }
            }
#endif /* (EMO_IMESS==1) */

            T13ValueUp = ((uint16)Compare1up + (uint16)Compare0up) / 2u;
            T13ValueDown = CCU6_T12PR - (((uint16)Compare1down + (uint16)Compare2down) / 2u);
            break;
        }

        case 1u:
        {
            ci = ((sint32)CCU6_T12PR / 2) - T1 - T2;
            if (ci < EMO_SVM_DEADTIME) {ci = 0;};
            Compare1up = (uint32)ci;
            ci = ((sint32)CCU6_T12PR / 2) - T1 + T2;
            if (ci < EMO_SVM_DEADTIME) {ci = 0;};
            if (ci > ((sint32)CCU6_T12PR - EMO_SVM_DEADTIME)) {ci = (sint32)CCU6_T12PR + 1;};
            Compare0up = (uint32)ci;
            ci = ((sint32)CCU6_T12PR / 2) + T1 + T2;
            if (ci > ((sint32)CCU6_T12PR - EMO_SVM_DEADTIME)) {ci = (sint32)CCU6_T12PR + 1;};
            Compare2up = (uint32)ci;

            Compare0down = Compare0up;
            Compare1down = Compare1up;
            Compare2down = Compare2up;
            
#if (EMO_IMESS==1)
            i = (uint16)T1;
            T1 = T2;
            T2 = (sint32)i;

            if (T1 < (EMO_SVM_MINTIME / 2))
            {
                /* pattern has to be applied asymetric because T1 too short */
                i = (uint16)EMO_SVM_MINTIME - (uint16)T1 - (uint16)T1;
                Compare0up = Compare0up + i;

                if (Compare0up > per)
                {
                    Compare0up = per;
                }
                
                if (i < Compare0down)
                {
                    Compare0down = Compare0down - i;
                }
                else
                {
                    Compare0down = 1u;
                }
            }
            /* at least T1 has enough room for ADC measurement **
            ** now check also T2 against min. SVM Time         */
            if (T2 < (EMO_SVM_MINTIME / 2))
            {
                /* pattern has to be applied asymetric because of T2 too short */
                i = (uint16)EMO_SVM_MINTIME - (uint16)T2 - (uint16)T2;
                Compare0up = Compare0up + i;
                if (Compare0up > per)
                {
                    Compare0up = per;
                }
                
                if (i < Compare0down)
                {
                    Compare0down = Compare0down - i;
                }
                else
                {
                    Compare0down = 1u;
                }
            }
#endif /* (EMO_IMESS==1) */

            T13ValueUp = ((uint16)Compare0up + (uint16)Compare1up) / 2u;
            T13ValueDown = CCU6_T12PR - (((uint16)Compare0down + (uint16)Compare2down) / 2u);
            break;
        }

        case 2u:
        {
            ci = ((sint32)CCU6_T12PR / 2) - T1 - T2;
            if (ci < EMO_SVM_DEADTIME) {ci = 0;};
            Compare1up = (uint32)ci;
            ci = ((sint32)CCU6_T12PR / 2) + T1 - T2;
            if (ci < EMO_SVM_DEADTIME) {ci = 0;};
            if (ci > ((sint32)CCU6_T12PR - EMO_SVM_DEADTIME)) {ci = (sint32)CCU6_T12PR + 1;};
            Compare2up = (uint32)ci;
            ci = ((sint32)CCU6_T12PR / 2) + T1 + T2;
            if (ci > ((sint32)CCU6_T12PR - EMO_SVM_DEADTIME)) {ci = (sint32)CCU6_T12PR + 1;};
            Compare0up = (uint32)ci;

            Compare0down = Compare0up;
            Compare1down = Compare1up;
            Compare2down = Compare2up;

#if (EMO_IMESS==1)
            if (T1 < (EMO_SVM_MINTIME / 2))
            {
                /* pattern has to be applied asymetric because T1 too short */
                i = (uint16)EMO_SVM_MINTIME - (uint16)T1 - (uint16)T1;
                Compare2up = Compare2up + i;

                if (Compare2up > per)
                {
                    Compare2up = per;
                }
                
                if (i < Compare2down)
                {
                    Compare2down = Compare2down - i;
                }
                else
                {
                    Compare2down = 1u;
                }
            }
                /* at least T1 has enough room for ADC measurement **
                ** now check also T2 against min. SVM Time         */
            if (T2 < (EMO_SVM_MINTIME / 2))
            {
                /* pattern has to be applied asymetric because of T2 too short */
                i = (uint16)EMO_SVM_MINTIME - (uint16)T2 - (uint16)T2;
                Compare2up = Compare2up + i;
                if (Compare2up > per)
                {
                    Compare2up = per;
                }
                
                if (i < Compare2down)
                {
                    Compare2down = Compare2down - i;
                }
                else
                {
                    Compare2down = 1u;
                }
            }
#endif /* (EMO_IMESS==1) */

            T13ValueUp = ((uint16)Compare2up + (uint16)Compare1up) / 2u;
            T13ValueDown = CCU6_T12PR - (((uint16)Compare2down + (uint16)Compare0down) / 2u);
            break;
        }

        case 3u:
        {
            ci = ((sint32)CCU6_T12PR / 2) - T1 - T2;
            if (ci < EMO_SVM_DEADTIME) {ci = 0;};
            Compare2up = (uint32)ci;
            ci = ((sint32)CCU6_T12PR / 2) - T1 + T2;
            if (ci < EMO_SVM_DEADTIME) {ci = 0;};
            if (ci > ((sint32)CCU6_T12PR - EMO_SVM_DEADTIME)) {ci = (sint32)CCU6_T12PR + 1;};
            Compare1up = (uint32)ci;
            ci = ((sint32)CCU6_T12PR / 2) + T1 + T2;
            if (ci > ((sint32)CCU6_T12PR - EMO_SVM_DEADTIME)) {ci = (sint32)CCU6_T12PR + 1;};
            Compare0up = (uint32)ci;

            Compare0down = Compare0up;
            Compare1down = Compare1up;
            Compare2down = Compare2up;

#if (EMO_IMESS==1)
            i = (uint16)T1;
            T1 = T2;
            T2 = (sint32)i;

            if (T1 < (EMO_SVM_MINTIME / 2))
            {
                /* pattern has to be applied asymetric because T1 too short */
                i = (uint16)EMO_SVM_MINTIME - (uint16)T1 - (uint16)T1;
                Compare1up = Compare1up + i;

                if (Compare1up > per)
                {
                    Compare1up = per;
                }
                if (i < Compare1down)
                {
                    Compare1down = Compare1down - i;
                }
                else
                {
                    Compare1down = 1u;
                }
            }
            /* at least T1 has enough room for ADC measurement **
            ** now check also T2 against min. SVM Time         */
            if (T2 < (EMO_SVM_MINTIME / 2))
            {
                /* pattern has to be applied asymetric because of T2 too short */
                i = (uint16)EMO_SVM_MINTIME - (uint16)T2 - (uint16)T2;
                Compare1up = Compare1up + i;
                
                if (Compare1up > per)
                {
                    Compare1up = per;
                }
                
                if (i < Compare1down)
                {
                    Compare1down = Compare1down - i;
                }
                else
                {
                    Compare1down = 1u;
                }
            }
#endif
            T13ValueUp = ((uint16)Compare1up + (uint16)Compare2up) / 2u;
            T13ValueDown = CCU6_T12PR - (((uint16)Compare1down + (uint16)Compare0down) / 2u);
            break;
        }

        case 4u:
        {
            ci = ((sint32)CCU6_T12PR / 2) - T1 - T2;
            if (ci < EMO_SVM_DEADTIME) {ci = 0;};
            Compare2up = (uint32)ci;
            ci = ((sint32)CCU6_T12PR / 2) + T1 - T2;
            if (ci < EMO_SVM_DEADTIME) {ci = 0;};
            if (ci > ((sint32)CCU6_T12PR - EMO_SVM_DEADTIME)) {ci = (sint32)CCU6_T12PR + 1;};
            Compare0up = (uint32)ci;
            ci = ((sint32)CCU6_T12PR / 2) + T1 + T2;
            if (ci > ((sint32)CCU6_T12PR - EMO_SVM_DEADTIME)) {ci = (sint32)CCU6_T12PR + 1;};
            Compare1up = (uint32)ci;

            Compare0down = Compare0up;
            Compare1down = Compare1up;
            Compare2down = Compare2up;

#if (EMO_IMESS==1)
            if (T1 < (EMO_SVM_MINTIME / 2))
            {
                /* pattern has to be applied asymetric because T1 too short */
                i = (uint16)EMO_SVM_MINTIME - (uint16)T1 - (uint16)T1;
                Compare0up = Compare0up + i;

                if (Compare0up > per)
                {
                    Compare0up = per;
                }
                
                if (i < Compare0down)
                {
                    Compare0down = Compare0down - i;
                }
                else
                {
                    Compare0down = 1u;
                }
            }
            /* at least T1 has enough room for ADC measurement **
            ** now check also T2 against min. SVM Time         */
            if (T2 < (EMO_SVM_MINTIME / 2))
            {
                /* pattern has to be applied asymetric because of T2 too short */
                i = (uint16)EMO_SVM_MINTIME - (uint16)T2 - (uint16)T2;
                Compare0up = Compare0up + i;
                
                if (Compare0up > per)
                {
                    Compare0up = per;
                }
                
                if (i < Compare0down)
                {
                    Compare0down = Compare0down - i;
                }
                else
                {
                    Compare0down = 1u;
                }
            }
#endif /* (EMO_IMESS==1) */
            T13ValueUp = ((uint16)Compare0up + (uint16)Compare2up) / 2u;
            T13ValueDown = CCU6_T12PR - (((uint16)Compare0down + (uint16)Compare1down) / 2u);
            break;
        }

        default: /* case 5u: */
        {
            ci = ((sint32)CCU6_T12PR / 2) - T1 - T2;
            if (ci < EMO_SVM_DEADTIME) {ci = 0;};
            Compare0up = (uint32)ci;
            ci = ((sint32)CCU6_T12PR / 2) - T1 + T2;
            if (ci < EMO_SVM_DEADTIME) {ci = 0;};
            if (ci > ((sint32)CCU6_T12PR - EMO_SVM_DEADTIME)) {ci = (sint32)CCU6_T12PR + 1;};
            Compare2up = (uint32)ci;
            ci = ((sint32)CCU6_T12PR / 2) + T1 + T2;
            if (ci > ((sint32)CCU6_T12PR - EMO_SVM_DEADTIME)) {ci = (sint32)CCU6_T12PR + 1;};
            Compare1up = (uint32)ci;

            Compare0down = Compare0up;
            Compare1down = Compare1up;
            Compare2down = Compare2up;

#if (EMO_IMESS==1)
            i = (uint16)T1;
            T1 = T2;
            T2 = (sint32)i;

            if (T1 < (EMO_SVM_MINTIME / 2))
            {
                /* pattern has to be applied asymetric because T1 too short */
                i = (uint16)EMO_SVM_MINTIME - (uint16)T1 - (uint16)T1;
                Compare2up = Compare2up + i;

                if (Compare2up > per)
                {
                    Compare2up = per;
                }
                
                if (i < Compare2down)
                {
                    Compare2down = Compare2down - i;
                }
                else
                {
                    Compare2down = 1u;
                }
            }
            /* at least T1 has enough room for ADC measurement **
            ** now check also T2 against min. SVM Time         */
            if (T2 < (EMO_SVM_MINTIME / 2))
            {
                /* pattern has to be applied asymetric because of T2 too short */
                i = (uint16)EMO_SVM_MINTIME - (uint16)T2 - (uint16)T2;
                Compare2up = Compare2up + i;
                
                if (Compare2up > per)
                {
                    Compare2up = per;
                }
                
                if (i < Compare2down)
                {
                    Compare2down = Compare2down - i;
                }
                else
                {
                    Compare2down = 1u;
                }
            }
#endif /* (EMO_IMESS==1) */
            T13ValueUp = ((uint16)Compare2up + (uint16)Compare0up) / 2u;
            T13ValueDown = CCU6_T12PR - (((uint16)Compare2down + (uint16)Compare1down) / 2u);
            break;
        }
    }

    /* Set compare values */
    if (Emo_Svm.CounterOffsetAdw > 127u)
    {
        pSvm->CompT13ValueUp = T13ValueUp;
        pSvm->CompT13ValueDown = T13ValueDown;
    }
    else
    {
        pSvm->CompT13ValueUp = CCU6_T12PR / 3u ;
        pSvm->CompT13ValueDown = CCU6_T12PR * 2u / 3u;
    }    
    pSvm->comp60up = (uint16)Compare0up;
    pSvm->comp61up = (uint16)Compare1up;
    pSvm->comp62up = (uint16)Compare2up;
    pSvm->comp60down = (uint16)Compare0down;
    pSvm->comp61down = (uint16)Compare1down;
    pSvm->comp62down = (uint16)Compare2down;

    /*ensure loading of shadow registers only during T12 down_counting part*/
    if (CCU6->TCTR0.bit.CDIR == 0u)
    {
        /* T12 still in up-counting part, enable T12 Period Match Interrupt **
        ** to load the shadow register inside extra ISR                    */
        CCU6->IEN.bit.ENT12PM = 1u;
    }
    else
    {
        /* T12 in down-counting part, load shadow register */
        CCU6_LoadShadowRegister_CC60(Emo_Svm.comp60up);
        CCU6_LoadShadowRegister_CC61(Emo_Svm.comp61up);
        CCU6_LoadShadowRegister_CC62(Emo_Svm.comp62up);
        CCU6_EnableST_T12();
        /* Set Timer13 Trigger calculated previous period zero-match*/
        CCU6_SetT13Trigger(0x7a);
        /* Set T13 Compare Value */
        CCU6_SetT13Compare(Emo_Svm.CompT13ValueUp);
    }
} /* End of Emo_lExeSvm */
__STATIC_INLINE void Emo_lEstFlux(void)
{
    sint16 Temp;
    uint16 Tempu;
    uint16 FluxAbsValue;
    TComplex fluxh = {0, 0};
    static TComplex Fluxrf;
    static TComplex Flux;
    /* Get stator flux in real axis */
    Temp = (sint16)__ssat((sint32)Fluxrf.Real + (sint32)Emo_Foc.StatVolt.Real - ((sint32)Emo_Foc.StatCurr.Real * (sint32)Emo_Foc.PhaseRes / 32768), MAT_FIX_SAT);
    fluxh.Real = Mat_ExeLp_without_min_max(&Emo_Foc.RealFluxLp, Temp);
    Flux.Real = (sint16)__ssat((sint32)fluxh.Real - ((sint32)Emo_Foc.StatCurr.Real * (sint32)Emo_Foc.PhaseInd / 32768), MAT_FIX_SAT);
    /* Get stator flux in imaginary axis */
    Temp = (sint16)__ssat((sint32)Fluxrf.Imag + (sint32)Emo_Foc.StatVolt.Imag - ((sint32)Emo_Foc.StatCurr.Imag * (sint32)Emo_Foc.PhaseRes / 32768), MAT_FIX_SAT);

    fluxh.Imag = Mat_ExeLp_without_min_max(&Emo_Foc.ImagFluxLp, Temp);
    Flux.Imag = (sint16)__ssat((sint32)fluxh.Imag - ((sint32)Emo_Foc.StatCurr.Imag * (sint32)Emo_Foc.PhaseInd / 32768), MAT_FIX_SAT);
    /* Calculate flux angle */
    /* Tempu => FluxAmplitude */
    Emo_Foc.FluxAngle =  Mat_CalcAngleAmp(Flux, &Tempu);
    /*Tempu = (Tempu * 32000) / 32768 => ensure that FluxAmplitude will not clamp*/
    FluxAbsValue = (uint16)__ssat((((sint32)Tempu * (sint32)32000) / (sint32)32768), MAT_FIX_SAT + 1u);
    /*filtered FluxBetrag as reference for checkings below*/
    Temp = Mat_ExeLp_without_min_max(&Emo_Ctrl.FluxbtrLp, (sint16)FluxAbsValue);
	FluxAbsValueTemp = Temp;

    if (Flux.Real > Temp)
    {
        Fluxrf.Real = -400;
    }
    else
    {
        if (Flux.Real < -Temp)
        {
            Fluxrf.Real = 400;
        }
        else
        {
            Fluxrf.Real = 0;
        }
    }

    if  (Flux.Imag > Temp)
    {
        Fluxrf.Imag = -400;
    }
    else
    {
        if (Flux.Imag < -Temp)
        {
            Fluxrf.Imag = 400;
        }
        else
        {
            Fluxrf.Imag = 0;
        }
    }
} /* End of Emo_lEstFlux */

__STATIC_INLINE void Emo_FluxAnglePll(void)
{
    sint16 deltaphi;
    sint16 domega;
    deltaphi = (sint16)Emo_Foc.FluxAngle - (sint16)Emo_Ctrl.FluxAnglePll;
    domega = (sint16)__ssat(((sint32)deltaphi * (sint32)Emo_Ctrl.Pllkp / 32768) + Emo_Ctrl.Speedpll, MAT_FIX_SAT);
    Emo_Ctrl.FluxAnglePll = Emo_Ctrl.FluxAnglePll + (uint16)domega;
}

#if (EMO_DECOUPLING==1)
__STATIC_INLINE TComplex Emo_CurrentDecoupling(void)
{
    TComplex StatOut = {0, 0};
    sint16 Kentk = 10547;
    sint16 Oml;
    Oml = (Kentk * Emo_Ctrl.SpeedLpdisplay.Out / 16384);
    StatOut.Real = __ssat((sint32)Emo_Foc.RotVoltCurrentcontrol.Real - ((sint32)Emo_Foc.RotCurr.Imag * (sint32)Oml /16384), MAT_FIX_SAT);
    StatOut.Imag = __ssat((sint32)Emo_Foc.RotVoltCurrentcontrol.Imag + ((sint32)Emo_Foc.RotCurr.Real * (sint32)Oml /16384), MAT_FIX_SAT);
    return StatOut;
}
#endif
