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

/******************************************************************************
**                      Private Macro Definitions                            **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

__STATIC_INLINE void Emo_lEstFlux(void);
__STATIC_INLINE void Emo_FluxAnglePll(void);
__STATIC_INLINE void Emo_lExeSvm(TEmo_Svm *pSvm);
__STATIC_INLINE void Emo_lExeMintime_Compare(void);
__STATIC_INLINE void Emo_lExeSvm_Compare(void);
__STATIC_INLINE void Emo_CalcT13Compare(void);
__STATIC_INLINE void Emo_CalcSvmTimes(TEmo_Svm *pSvm);
__STATIC_INLINE void Emo_CalcSvmDuties(void);
__STATIC_INLINE void Emo_SetSvmCompareValues(TEmo_Svm *pSvm);
__STATIC_INLINE void Emo_UpdateHardwareRegisters(void);


#if (EMO_DECOUPLING==1)
    __STATIC_INLINE TComplex Emo_CurrentDecoupling(void);
#endif

//#define EMO_IMESS  1

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

sint16 FluxAbsValueTemp;
uint16 Emo_AdcResult[4u];
TEmo_Ctrl Emo_Ctrl;
TEmo_Foc Emo_Foc;
TEmo_Svm Emo_Svm;
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
    Emo_AdcResult[0u] = (uint16)(ADC1->RES_OUT1.reg);
    /*disable ESM interrupt*/
    ADC1->IE.bit.ESM_IE = 0;
}

static void Emo_CurrAdc1(void)
{
    uint16 AdcResult0;
    uint16 AdcResult1;
    uint16 sector;
    sint16 R0mioffs;
    sint16 OffsmiR1;
    sint16 R1miR0;
    
    /*AdcResult[2] => 1st ADC Meas. **
    **AdcResult[1] => 2nd ADC Meas. */
    AdcResult0 = (Emo_AdcResult[2u] & 0x0FFFu);
    AdcResult1 = (Emo_AdcResult[1u] & 0x0FFFu);
    Emo_AdcResult[3u]= (AdcResult0+AdcResult1);
    /*result 0 minus CSA Offset*/
    R0mioffs = ((sint16)AdcResult0 - (sint16)Emo_Svm.CsaOffsetAdw);

    /*CSA Offset minus result 1*/
    OffsmiR1 = ((sint16)Emo_Svm.CsaOffsetAdw - (sint16)AdcResult1);
    /*Result 1 - Result 0*/
    R1miR0 = ((sint16)AdcResult1 - (sint16)AdcResult0);
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
        default: //case 5
        {
    		Emo_Svm.PhaseCurr.A = R0mioffs;
    		Emo_Svm.PhaseCurr.B = OffsmiR1;
    				  /* FIND */
    		Emo_Svm.PhaseCurr.C = R1miR0;
        }
        break;
    }
    //CheckOpenPhaseState(&Emo_Svm);

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
    CCU6->IEN.bit.ENT12PM = 0;
}
    
void Emo_HandleFoc(void)
{
    uint16 angle;
    //uint16 i;
    TComplex Vect1 = {0, 0};
    TComplex Vect2;
    sint16 Speed;
    sint32 jj;
		uint16 ampl;
	if(Emo_Svm.CounterOffsetAdw >= 128u){	
    Emo_AdcResult[2u] = Emo_AdcResult[0u];
    /* Enable ADC Interrupt */
    ADC1->ICLR.bit.ESM_ICLR = 1;
    ADC1->IE.bit.ESM_IE = 1;
    /*get 2nd ADC measurement from previous period*/
    Emo_AdcResult[1u] = (uint16)(ADC1->RES_OUT1.reg);
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
		Emo_Foc.StartAngle = Emo_Foc.StartAngle + (uint16)Emo_Foc.StartFrequencySlope;
		Emo_Foc.Angle = Emo_Foc.StartAngle;

		}
		Emo_Ctrl.PtrAngle = (Emo_Ctrl.PtrAngle + 1u) & 0xfu;

		Emo_Ctrl.AngleBuffer[Emo_Ctrl.PtrAngle] = Emo_Foc.FluxAngle;
		Emo_Ctrl.Speedest = (sint16)Emo_Foc.FluxAngle - (sint16)Emo_Ctrl.AngleBuffer[(Emo_Ctrl.PtrAngle - 8u) & 0xfu];


		Emo_Ctrl.Speedpll = Emo_Ctrl.Speedest / 8;
		/* jj => electrical rotation speed */
		//jj = Mat_FixMulScale(Emo_Ctrl.Speedest, 14062, 1);
		jj = ((sint32)Emo_Ctrl.Speedest * (sint32)14062) / (sint32)8192;
		/* calculate mech. speed out of electrical speed (jj), jj not further used */
		Speed = (sint16)(jj / 3); //Emo_Foc_PolePair
		/* Filter speed */
		Emo_Ctrl.ActSpeed = Mat_ExeLp_without_min_max(&Emo_Ctrl.SpeedLp, Speed);
		Emo_FluxAnglePll();

    if (Emo_Status.MotorState == EMO_MOTOR_STATE_START)
    {
        Emo_Ctrl.RefCurr = -Emo_Foc.StartCurrent;
    }
    else /* (Emo_Status.MotorState == EMO_MOTOR_STATE_RUN) */
    {
         /* assign PLL output angle to Emo_Foc.Angle */
        Emo_Foc.Angle = Emo_Ctrl.FluxAnglePll;
#if (EMO_DECOUPLING==0)

#else
        Emo_Foc.RotVoltCurrentcontrol.Real = Mat_ExePi(&Emo_Ctrl.RealCurrPi,  -(Emo_Foc.RotCurr.Real));
        Emo_Foc.RotVoltCurrentcontrol.Imag = Mat_ExePi(&Emo_Ctrl.ImagCurrPi, Emo_Ctrl.RefCurr - Emo_Foc.RotCurr.Imag);
        /* Calculate Decoupling */
        Emo_Foc.RotVolt = Emo_CurrentDecoupling();
#endif
    }
				
    /* Current Regulator: Execute PI algorithm for rotating voltage */
    /* id */ 
    Emo_Foc.RotVolt.Real = Mat_ExePi(&Emo_Ctrl.RealCurrPi, -(Emo_Foc.RotCurr.Real));

    /* iq */
    Emo_Foc.RotVolt.Imag = Mat_ExePi(&Emo_Ctrl.ImagCurrPi, Emo_Ctrl.RefCurr - Emo_Foc.RotCurr.Imag);

    /* DC-link voltage correction */
    Vect1.Real = (sint16)__ssat((((sint32)Emo_Foc.RotVolt.Real *  (sint32)Emo_Foc.Dcfactor1) / 16384), MAT_FIX_SAT);
    Vect1.Imag = (sint16)__ssat((((sint32)Emo_Foc.RotVolt.Imag * (sint32)Emo_Foc.Dcfactor1) / 16384), MAT_FIX_SAT);
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

        Emo_Svm.Amp = ampl;
    /* sum of regulated angle out of Current Regulation + PLL-Angle */
    Emo_Svm.Angle = angle + Emo_Foc.Angle;

    /* Perform Polar-2-Cartesian transformation      **
    ** preparation of input values for FluxEstimator */
    Emo_Foc.StatVoltAmpM = (uint16)__ssat((((sint32)Emo_Svm.Amp * (sint32)Emo_Foc.Dcfactor2) / 4096), MAT_FIX_SAT);
    Emo_Foc.StatVolt = Mat_PolarKartesisch(Emo_Foc.StatVoltAmpM, Emo_Svm.Angle);

    /* Perform space vector modulation */
    Emo_lExeSvm(&Emo_Svm);
    /* Filter for Iq  */
    //Emo_Ctrl.RotCurrImagdisplay = Mat_ExeLp_without_min_max(&Emo_Ctrl.RotCurrImagLpdisplay, Emo_Foc.RotCurr.Imag);
	}
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
   static uint32 ci_A;
   static uint32 ci_B;
   static uint32 ci_C;
   static uint32 T1;
   static uint32 T2;
   static uint16 Compare0up;
   static uint16 Compare1up;
   static uint16 Compare2up;
   static uint16 Compare0down;
   static uint16 Compare1down;
   static uint16 Compare2down;	 
   static uint32 Sector;
	 static uint16 T13ValueUp;
   static uint16 T13ValueDown;
	 
__STATIC_INLINE void Emo_lExeSvm(TEmo_Svm *pSvm)
{
    Emo_CalcSvmTimes(pSvm);
    Emo_CalcSvmDuties();
    Emo_SetSvmCompareValues(pSvm);
    Emo_UpdateHardwareRegisters();
}
/* 1. Function to calculate Angle, Sector, and Time (T1, T2) */
__STATIC_INLINE void Emo_CalcSvmTimes(TEmo_Svm *pSvm)
{
    uint32 Angle;
    uint32 Index;

    /* Calculate sector number 0..5 and table index 0..255 */
    Angle = ((uint32)pSvm->Angle) * 6u;
    Sector = (Angle >> 16u);
    pSvm->Sector = (uint16)Sector;
    Index = (Angle >> 8u) & 0xFFu;

    /* Calculate and limit times */
    T1 = ((((uint32)pSvm->Amp) * (uint32)Table_Sin60[255u - Index]) >> 16u);
    pSvm->T1 = (uint16)T1;

    /* RandVector1 = Amp * sin(gamma) */
    T2 = ((((uint32)pSvm->Amp) * (uint32)Table_Sin60[Index]) >> 16u);
    pSvm->T2 = (uint16)T2;
}

/* 2.Function to calculate intermediate compare values (ci_A, ci_B, ci_C) based on the sector */
__STATIC_INLINE void Emo_CalcSvmDuties(void)
{
    ci_A = 666u - T1 - T2;
    if (ci_A < EMO_SVM_DEADTIME) { ci_A = 0u; }

    if ((Sector == 0u) || (Sector == 2u) || (Sector == 4u))
    {
        ci_B = 666u + T1 - T2;
    }
    else
    {
        ci_B = 666u - T1 + T2;
    }
    
    if (ci_B < EMO_SVM_DEADTIME) { ci_B = 0u; }
    if (ci_B > (CCU6_T12PR - EMO_SVM_DEADTIME)) { ci_B = (CCU6_T12PR + 1u); }

    ci_C = 666u + T1 + T2;
    if (ci_C > (CCU6_T12PR - EMO_SVM_DEADTIME)) { ci_C = (CCU6_T12PR + 1u); }
}

/* 3. Function to update compare values and save them to the structure */
__STATIC_INLINE void Emo_SetSvmCompareValues(TEmo_Svm *pSvm)
{
	  static	uint16 temp;
    /* Set compare values according to sector number */
    Emo_lExeSvm_Compare();

    Compare0down = Compare0up; 
    Compare1down = Compare1up; 
    Compare2down = Compare2up;

    if ((Sector & 1u) != 0u)
    {
        temp = (uint16)T1;
        T1 = T2;
        T2 = (uint32)temp;
    }

    Emo_lExeMintime_Compare();
    Emo_CalcT13Compare();    

    /* Set T13 compare values based on CounterOffsetAdw */
    if (Emo_Svm.CounterOffsetAdw > 127u)
    {
        pSvm->CompT13ValueUp = T13ValueUp;
        pSvm->CompT13ValueDown = T13ValueDown;
    }
    else
    {
        pSvm->CompT13ValueUp = 444u;   // CCU6_T12PR / 3u
        pSvm->CompT13ValueDown = 888u; // CCU6_T12PR * 2 / 3u
    }    

    pSvm->comp60up = Compare0up; 
    pSvm->comp61up = Compare1up; 
    pSvm->comp62up = Compare2up;
    pSvm->comp60down = Compare0down; 
    pSvm->comp61down = Compare1down; 
    pSvm->comp62down = Compare2down;
}

/* 4. Function to apply values to hardware registers (CCU6) */
__STATIC_INLINE void Emo_UpdateHardwareRegisters(void)
{
    /* ensure loading of shadow registers only during T12 down_counting part */
    if (CCU6->TCTR0.bit.CDIR == 0u)
    {
        /* T12 still in up-counting part, enable T12 Period Match Interrupt **
        ** to load the shadow register inside extra ISR                     */
        CCU6->IEN.bit.ENT12PM = 1;
    }
    else
    {
        /* T12 in down-counting part, load shadow register */
        CCU6_LoadShadowRegister_CC60(Emo_Svm.comp60up);
        CCU6_LoadShadowRegister_CC61(Emo_Svm.comp61up);
        CCU6_LoadShadowRegister_CC62(Emo_Svm.comp62up);
        CCU6_EnableST_T12();
        
        /* Set Timer13 Trigger calculated previous period zero-match */
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
    Temp = (sint16)__ssat((sint32)Fluxrf.Real + (sint32)Emo_Foc.StatVolt.Real - Mat_FixMul_singed((sint32)Emo_Foc.StatCurr.Real, (sint32)Emo_Foc_PhaseRes), MAT_FIX_SAT);
    fluxh.Real = Mat_ExeLp_without_min_max(&Emo_Foc.RealFluxLp, Temp);
    Flux.Real = (sint16)__ssat(fluxh.Real - Mat_FixMul_singed((sint32)Emo_Foc.StatCurr.Real, (sint32)Emo_Foc_PhaseInd), 16u);
    /* Get stator flux in imaginary axis */
    Temp = (sint16)__ssat((sint32)Fluxrf.Imag + (sint32)Emo_Foc.StatVolt.Imag - Mat_FixMul_singed((sint32)Emo_Foc.StatCurr.Imag, (sint32)Emo_Foc_PhaseRes), 16u);

    fluxh.Imag = Mat_ExeLp_without_min_max(&Emo_Foc.ImagFluxLp, Temp);
    Flux.Imag = (sint16)__ssat(fluxh.Imag - Mat_FixMul_singed((sint32)Emo_Foc.StatCurr.Imag, (sint32)Emo_Foc_PhaseInd), 16u);
    /* Calculate flux angle */
    /* Tempu => FluxAmplitude */
    Emo_Foc.FluxAngle =  Mat_CalcAngleAmp(Flux, &Tempu);
	Tempu = (Tempu * 32000u) >> 15u;
    FluxAbsValue = Tempu;
    /*filtered FluxBetrag as reference for checkings below*/
    Temp = (sint16)Mat_ExeLp_without_min_max(&Emo_Ctrl.FluxbtrLp, (sint16)FluxAbsValue);
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
    deltaphi = ((sint16)Emo_Foc.FluxAngle - (sint16)Emo_Ctrl.FluxAnglePll);
    domega = (sint16)__ssat(Mat_FixMul_singed((sint32)deltaphi, (sint32)100) + Emo_Ctrl.Speedpll, MAT_FIX_SAT);
    Emo_Ctrl.FluxAnglePll = Emo_Ctrl.FluxAnglePll + (uint16)domega;
}


__STATIC_INLINE void Emo_lExeMintime_Compare(void)
{    	uint16 threshold = (uint16)(EMO_SVM_MINTIME >> 1);
	uint16 tempUp;
	static	uint16 i_compare;
	volatile uint16 *CompareUp = NULL_PTR;
  volatile uint16 *CompareDown = NULL_PTR;
    
    if ((T1 < threshold) || (T2 < threshold))
    {
        if (T1 < T2) { i_compare = (uint16)((uint32)EMO_SVM_MINTIME - ((uint32)(T1 << 1))); }
        else         { i_compare = (uint16)((uint32)EMO_SVM_MINTIME - ((uint32)(T2 << 1))); }
        
        switch (Sector) {
					  case 0: CompareUp = &Compare1up; CompareDown = &Compare1down; break;
            case 1: CompareUp = &Compare0up; CompareDown = &Compare0down; break;
            case 2: CompareUp = &Compare2up; CompareDown = &Compare2down; break;
					  case 3: CompareUp = &Compare1up; CompareDown = &Compare1down; break;
					  case 4: CompareUp = &Compare0up; CompareDown = &Compare0down; break;
					  case 5: CompareUp = &Compare2up; CompareDown = &Compare2down; break;
            default:
                    CompareUp = &Compare0up;
                    CompareDown = &Compare0down;
						    break;
        }

         tempUp = *CompareUp + i_compare;
        if (tempUp > CCU6_T12PR) {*CompareUp = (uint16)CCU6_T12PR;}
        else                     {*CompareUp = (uint16)tempUp;}

        if (i_compare < *CompareDown)  {*CompareDown = *CompareDown - i_compare;}
        else                    {*CompareDown = 1;}
    }
	}
__STATIC_INLINE void Emo_lExeSvm_Compare(void)
{
    switch (Sector)
    {
        case 0u: 
				{
		        Compare0up = (uint16)ci_A;
		        Compare1up = (uint16)ci_B;
		        Compare2up = (uint16)ci_C;
				}
					break;
        case 1u: 
				{
						Compare1up = (uint16)ci_A;
		        Compare0up = (uint16)ci_B;
		        Compare2up = (uint16)ci_C;
				}
				  break;
        case 2u: 
				{
          	Compare1up = (uint16)ci_A;
	          Compare2up = (uint16)ci_B;
	          Compare0up = (uint16)ci_C;
				}
   				break;
        case 3u: 
				{
		        Compare2up = (uint16)ci_A;
		        Compare1up = (uint16)ci_B;
		        Compare0up = (uint16)ci_C;
				}
  				break;
        case 4u: 
				{
		        Compare2up = (uint16)ci_A;
		        Compare0up = (uint16)ci_B;
		        Compare1up = (uint16)ci_C;
				} 
  				break;
        default: 
				{
		        Compare0up = (uint16)ci_A;
		        Compare2up = (uint16)ci_B;
		        Compare1up = (uint16)ci_C;
				}
  				break;
    }
}
__STATIC_INLINE void Emo_CalcT13Compare(void)
{
		switch (Sector)
    {
        case 0u: T13ValueUp = (Compare1up + Compare0up) >>1u; T13ValueDown = (CCU6_T12PR - ((Compare1down + Compare2down) >>1u)); break;
        case 1u: T13ValueUp = (Compare1up + Compare0up) >>1u; T13ValueDown = (CCU6_T12PR - ((Compare0down + Compare2down) >>1u)); break;
        case 2u: T13ValueUp = (Compare2up + Compare1up) >>1u; T13ValueDown = (CCU6_T12PR - ((Compare0down + Compare2down) >>1u)); break;
        case 3u: T13ValueUp = (Compare2up + Compare1up) >>1u; T13ValueDown = (CCU6_T12PR - ((Compare1down + Compare0down) >>1u)); break;
        case 4u: T13ValueUp = (Compare0up + Compare2up) >>1u; T13ValueDown = (CCU6_T12PR - ((Compare1down + Compare0down) >>1u)); break;
        default: T13ValueUp = (Compare0up + Compare2up) >>1u; T13ValueDown = (CCU6_T12PR - ((Compare1down + Compare2down) >>1u)); break;
    }
}
