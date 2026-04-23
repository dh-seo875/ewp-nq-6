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
**                      Includes                                              **
*******************************************************************************/
#include "Emo_RAM.h"
#include "Variant_cfg.h"
#include "Manage.h"
#include "RteStd.h"



#if((EWP_TYPE == PEWP_TYPE2_0X37) || (EWP_TYPE == PEWP_TYPE2_0X38) || (EWP_TYPE == IEWP_TYPE2_0X37) || (EWP_TYPE == AEWP_TYPE2_0X37) || (EWP_TYPE == AEWP_TYPE2_0X37_WITHOUTHOTSPOT))
#include "../driver/Type2/foc_defines.h"

#elif((EWP_TYPE == PEWP_TYPE3_0X37) || (EWP_TYPE == PEWP_TYPE3_0X38) || (EWP_TYPE == IEWP_TYPE3_0X37) || (EWP_TYPE == BEWP_TYPE3_0X37) || (EWP_TYPE == BEWP_TYPE3_0X38))
#include "../driver/Type3/foc_defines.h"

#else
/* No Action */
#endif

/*******************************************************************************
**                      Global Constant Definitions to be changed             **
*******************************************************************************/
/* Constants for MCMOUTS register */
#define CCU6_MASK_MCMOUTS_ENABLE_MCMOUTS  (0x00BFu)
TEmo_Status Emo_Status;

sint16 Emo_speedRef = 0;
uint16 EMO_SVM_MINTIME;
/** \brief Initializes E-Motor HW and SW.
 *
 * \param None
 * \return Error or EMO_ERROR_NONE
 *
 * \ingroup emo_api
 */
static void Emo_HandleT2Overflow(void);
static void Emo_HandleT2Overflow_Start(void);
static void Emo_HandleT2Overflow_Run(void);
static void Emo_lInitFocVar(void);
static void Emo_lInitFocPar(void);
static uint32 Emo_StartMotor(void);


void Task_Emo_MotorControl(void) 
{
    sint16_t main_speedRef = 0;
	(void)GetSpeedRef(&main_speedRef);
	
	if (main_speedRef >= 1000)
	{
		/* set speed as reference speed for the BLDC motor */
    	Emo_Ctrl.RefSpeed = (sint16)main_speedRef * (-1);
		/* start the motor if not running yet */
		(void)Emo_StartMotor();
	}
	else
	{			 
		(void)SetDiagnosisCurrentSource();
		(void)ClearVsUnderVoltage();
		(void)ClearShortCircuit();
		(void)ClearGateDrvError();
		(void)ClearOverCurrent();
		(void)ClearVcpWarningStatus();
		(void)ClearVddExtShortCircuitStatus();

    	Emo_Ctrl.RefSpeed = (sint16)main_speedRef * (-1);		
		
		/* if speed is below Speed Min, stop the motor */
		(void)Emo_StopMotor();
	}
}



uint32 Emo_Init(void)
{
    uint32 retval = 0u;
    if (Emo_Status.MotorState != EMO_MOTOR_STATE_UNINIT)
    {
        retval = EMO_ERROR_MOTOR_INIT;
    }
	else
	{
	    GPT12E_T2_Start();
        /* Initialize FOC parameters */
        Emo_lInitFocPar();
        /* Initialize motor state */
        Emo_Status.MotorState = EMO_MOTOR_STATE_STOP;
		retval = EMO_ERROR_NONE;
	}
    return retval;
} /* End of Emo_Init */


/** \brief Sets the reference speed of the motor.
 *
 * \param Reference speed [rpm]
 * \return None
 *
 * \ingroup emo_api
 */


/** \brief Gets motor speed.
 *
 * \param None
 * \return None
 *
 * \ingroup emo_api
 */
//uint32 Emo_GetSpeed(void)
//{
//    uint32 Speed;


//    if (Emo_Ctrl.ActSpeed >= 0)
//    {
//        Speed = (uint16)Emo_Ctrl.ActSpeed;
//    }
//    else
//    {
//        Speed = (uint16)(-Emo_Ctrl.ActSpeed);
//    }

//    return Speed;
//}

/** \brief Starts the motor.
 *
 * \param None
 * \return Error or EMO_ERROR_NONE
 *
 * \note Service should only be called when motor is stopped.
 *
 * \ingroup emo_api
 */
static uint32 Emo_StartMotor(void)
{
    uint32 retval = 0u;
    if (Emo_Status.MotorState != EMO_MOTOR_STATE_STOP)
    {
	    retval = EMO_ERROR_MOTOR_NOT_STOPPED;
    }
    else
    {
        CCU6_Init();
        /* Initialize variables */
        Emo_lInitFocVar();
        /* Set start state */
        Emo_Status.MotorState = EMO_MOTOR_STATE_START;
        /* Start PWM */
        CCU6_StartTmr_T12();
        /* Enable outputs immediately */
        CCU6->MCMOUTS.reg = CCU6_MASK_MCMOUTS_ENABLE_MCMOUTS;
        /* Enable bridge (after starting PWM according to UM) */
        BDRV_Set_Bridge(Ch_PWM, Ch_PWM, Ch_PWM, Ch_PWM, Ch_PWM, Ch_PWM);
		retval = EMO_ERROR_NONE;
    }    

    return retval;
} /* End of Emo_StartMotor */


/** \brief Stops the motor.
 *
 * \param None
 * \return Error code
 *
 * \note Service should only be called when motor was started before.
 *
 * \ingroup emo_api
 */
uint32 Emo_StopMotor(void)
{
    uint32 retval = 0u;

    if ((Emo_Status.MotorState != EMO_MOTOR_STATE_RUN) &&
        ((Emo_Status.MotorState != EMO_MOTOR_STATE_START) &&
         (Emo_Status.MotorState != EMO_MOTOR_STATE_FAULT)))
    {
        /* Error detected: return with error */
        retval = EMO_ERROR_MOTOR_NOT_STARTED;
    }
    else
    {
        /* Disable bridge */
        BDRV_Set_Bridge(Ch_Off, Ch_Off, Ch_Off, Ch_Off, Ch_Off, Ch_Off);
        /* Stop PWM */
        CCU6_StopTmr_T12();

        /* Initialize variables */
        Emo_lInitFocVar();    
        /* Set stop state */
        Emo_Status.MotorState = EMO_MOTOR_STATE_STOP;
        retval = EMO_ERROR_NONE;
	}

    return retval;
} /* End of Emo_StopMotor */

static void Emo_lInitFocPar(void)
{
    /* set to 15V */
//    float KU = 15.0;
   // float KI;
    /* set to 0.1V/s */
//    float KPSIE = 0.1;
   // float KUZ;
//    static float x;
//    float CoAFlux = 200.0;
//    float OpGain;
	uint16 CSA_Offset;
    uint16 i;
	uint16 timer = 10000u;
    Emo_Status.MotorStartError = 0;
    Emo_Svm.MaxAmp = 11330; // (CCU6_T12PR / EMO_CFG_FOC_TABLE_SCALE);
    /*vectorial voltage limitation, for block limitation algorithm*/
    /*0.9091 * MaxAmp*/
    Emo_Svm.MaxAmp9091pr = 10299; // (29789 * Emo_Svm.MaxAmp) >> MAT_FIX_SHIFT;
    /*0.4164 * MaxAmp*/
    Emo_Svm.MaxAmp4164pr = 4717; // (13643 * Emo_Svm.MaxAmp) >> MAT_FIX_SHIFT;
    Emo_Svm.MaxAmpQuadrat = 128368900; // (uint32)Emo_Svm.MaxAmp * Emo_Svm.MaxAmp;
    Emo_Svm.Kfact256 = 740; // 8388608 / Emo_Svm.MaxAmp;  exp23/MaxAmp
    //OpGain = 10.0;
    //CSA_Set_Gain(0u);
    
    /* 1.25 => 125mV, secured OpAmp differential input voltage range */
	//x = Emo_Focpar_Cfg.NominalCurrent * Emo_Focpar_Cfg.Rshunt * 40.0;
//    OpGain = 40.0;
    CSA_Set_Gain(2u);

    /* KI current regulator parameter          **
    ** 5.0 => 5V ADC referenc voltage          */
   // KI = 83.33f; // 5.0 * 2.0 / (Emo_Focpar_Cfg.Rshunt * OpGain);
    /* KUZ Battery Voltage parameter */
   // KUZ = 243.93f; // 32768.0 * 12.0 / 1612.0;  12V = 1612Ink ADW 
    /* Measuring the CSA Offset */
    CSA->CTRL.bit.VZERO = 0;
    ADC1_SetMode(SW_MODE);

    while ((ADC1_Busy() == true) && (timer > 0u)) { timer = timer - 1u;}
    timer = 10000u;
	
    ADC1_SetSocSwMode(ADC1_CH1);

    while ((ADC1_GetEocSwMode() == false) && (timer > 0u)) { timer = timer - 1u; }
    timer = 10000u;

    while ((ADC1->RES_OUT1.bit.VF1 == 0u) && (timer > 0u)) { timer = timer - 1u;}
    timer = 10000u;

    CSA_Offset = ADC1->RES_OUT1.bit.OUT_CH1;
    ADC1_SetMode(SEQ_MODE);

    while ((ADC1_Busy() == false) && (timer > 0u)) { timer = timer - 1u;}

	
    i = CSA_Offset;
    if (i > 1800u)
    {
        i = 1800u;
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | EMO_ERROR_CSAOFFSET;
    }
    if (i < 1500u)
    {
        i = 1500u;
        Emo_Status.MotorStartError = Emo_Status.MotorStartError | EMO_ERROR_CSAOFFSET;
    }

    Emo_Svm.CsaOffset = i;

    /* Initialize parameters for FOC */
    //Emo_Foc.PhaseRes = 14468; // 32768.0 * KI * Emo_Focpar_Cfg.PhaseRes / KU;
	
    //Emo_Foc.PhaseInd = 5128; // 32768.0 * KI * Emo_Focpar_Cfg.PhaseInd / KPSIE;
	
    //Emo_Foc.Kdcdivident1 = 19771316; // KU * 1.7320508 * Emo_Svm.MaxAmp / KUZ * 32768 / 2.0;
    //Emo_Foc.Kdcfactor2 = 13902; // KUZ * 32768.0 * 32768.0 / (KU * 1.7320508 * Emo_Svm.MaxAmp * 64);
    //Emo_Foc.Kdcfactoriqc = 9614; // 32768.0 * KUZ / (1.7320508 * KU * 32.0);
    //Emo_Foc.PolePair = 3; // Emo_Focpar_Cfg.PolePair = 3

    Emo_Foc.StartCurrent = 3145; // Emo_Focpar_Cfg.StartCurrent / KI * 32768.0;	

    //Emo_Foc.TimeSpeedzero = 27;	// Emo_Focpar_Cfg.TimeSpeedzero * SCU_FSYS / ((GPT12E_T2) * 4.0);
    //Emo_Foc.StartEndSpeed = 300; // Emo_Focpar_Cfg.StartSpeedEnd;	

    //Emo_Foc.StartSpeedSlewRate = 360008; // ((GPT12E_T2) * 4.0) / SCU_FSYS * Emo_Focpar_Cfg.StartSpeedSlewRate * 65536.0;
	
    //Emo_Foc.SpeedtoFrequency = 7158; // (32768.0 * (1.0 / CCU6_T12_FREQ) * 32768.0 / 30.0) * FOC_POLE_PAIRS;
	
    Emo_Foc.RealFluxLp.CoefA = 327; //32768.0 * KU / (KPSIE * CCU6_T12_FREQ);
    Emo_Foc.ImagFluxLp.CoefA = 327;	//32768.0 * KU / (KPSIE * CCU6_T12_FREQ);
//    Emo_Foc.LpCoefb1 = 109; // 32768.0 / (Emo_Focpar_Cfg.TimeConstantEstFluxFilter * CCU6_T12_FREQ); //Time const = 0.10s
//    Emo_Foc.LpCoefb2 = 218;   32768.0 / (0.01 * CCU6_T12_FREQ); Time const = 0.010s
    Emo_Foc.RealFluxLp.CoefB = 109; // Emo_Foc.LpCoefb1;
    Emo_Foc.ImagFluxLp.CoefB = 109; // Emo_Foc.LpCoefb1;
    Emo_Ctrl.SpeedPi.Kp = 1200; // Emo_Focpar_Cfg.SpeedPi_Kp;
    Emo_Ctrl.SpeedPi.Ki = 600; // Emo_Focpar_Cfg.SpeedPi_Ki;
    Emo_Ctrl.SpeedPi.PiMin = -3933; // 32767.0 * Emo_Focpar_Cfg.MinRefStartCurr / KI;
    Emo_Ctrl.SpeedPi.PiMax = 3932; // 32767.0 * Emo_Focpar_Cfg.MaxRefStartCurr / KI;

    
    //Emo_Ctrl.MaxRefCurrent = 5898; // 32767.0 * Emo_Focpar_Cfg.MaxRefCurr / KI;
    //Emo_Ctrl.MinRefCurrent = -5899; // 32767.0 * Emo_Focpar_Cfg.MinRefCurr / KI;	
    
    //Emo_Ctrl.MaxRefStartCurrent = 3932; // 32767.0 * Emo_Focpar_Cfg.MaxRefStartCurr / KI;
    //Emo_Ctrl.MinRefStartCurrent = -3933; // 32767.0 * Emo_Focpar_Cfg.MinRefStartCurr / KI;
	
    //Emo_Ctrl.Speedlevelmaxstart = 1000; // Emo_Focpar_Cfg.SpeedLevelPos;
    //Emo_Ctrl.Speedlevelminstart = -1000; // Emo_Focpar_Cfg.SpeedLevelNeg;
    //Emo_Ctrl.SpeedLevelSwitchOn = 100; // Emo_Focpar_Cfg.SpeedLevelSwitchOn;


    // x = 0.2f;  Emo_Focpar_Cfg.AdjustmCurrentControl;
    
    Emo_Ctrl.RealCurrPi.Kp = 149; // Emo_Focpar_Cfg.AdjustmCurrentControl * KI * Emo_Focpar_Cfg.PhaseInd / (256.0 * (1.0 / CCU6_T12_FREQ) * KU) * 32767.0;        
    Emo_Ctrl.RealCurrPi.Ki = 309; // Emo_Focpar_Cfg.AdjustmCurrentControl * KI * Emo_Focpar_Cfg.PhaseRes / (4.0 * KU) * 32767.0;
	
    /* id = PI regulator limits */
    Emo_Ctrl.RealCurrPi.IMin = -28272;
    Emo_Ctrl.RealCurrPi.IMax = 28272;
    Emo_Ctrl.RealCurrPi.PiMin = -28272;
    Emo_Ctrl.RealCurrPi.PiMax = 28272;
    Emo_Ctrl.ImagCurrPi.Kp =  Emo_Ctrl.RealCurrPi.Kp;
    Emo_Ctrl.ImagCurrPi.Ki =  Emo_Ctrl.RealCurrPi.Ki;
    /* iq = PI regulator limits, iq needs higher range than id */
    Emo_Ctrl.ImagCurrPi.IMin = -16580;
    Emo_Ctrl.ImagCurrPi.IMax = 16580;
    Emo_Ctrl.ImagCurrPi.PiMin = -16580;
    Emo_Ctrl.ImagCurrPi.PiMax = 16580;
	
    Emo_Ctrl.SpeedLp.CoefA = 218; // (1.0 / CCU6_T12_FREQ) / (Emo_Focpar_Cfg.TimeConstantSpeedFilter) * 32768.0;
    Emo_Ctrl.SpeedLp.CoefB = 218; // (1.0 / CCU6_T12_FREQ) / (Emo_Focpar_Cfg.TimeConstantSpeedFilter) * 32768.0;

    //Emo_Ctrl.RotCurrImagLpdisplay.CoefA=10;
    //Emo_Ctrl.RotCurrImagLpdisplay.CoefB=10;
    Emo_Ctrl.FluxbtrLp.CoefA = 1000;
    Emo_Ctrl.FluxbtrLp.CoefB = 1000;
    Emo_Ctrl.SpeedLpdisplay.CoefA = 1000;
    Emo_Ctrl.SpeedLpdisplay.CoefB = 1000;
//    Emo_Ctrl.Pllkp = 100;
	
//    Emo_Ctrl.Factorspeed = 14062;     60.0 * Emo_Focpar_Cfg.PWM_Frequency / 64.0;
	
    /* Speed PLL adjustment based on max. mech. speed             **
    ** 60.0 => conversion frequency into rpm (seconds to minutes) **
    ** 4.0 => 1/4 electrical rotation => 90°                      */
    // 60.0 * Emo_Focpar_Cfg.PWM_Frequency / (Emo_Focpar_Cfg.MaxSpeed * Emo_Focpar_Cfg.PolePair * 4.0);
    
//	Emo_Ctrl.Exppllhigh = 4;
//	Emo_Ctrl.Anglersptr = 16;
//	Emo_Ctrl.Expspeedhigh = 1;

//    if (x >= 32.0)
//    {
//        Emo_Ctrl.Exppllhigh = 5;
//        Emo_Ctrl.Anglersptr = 32;
//        Emo_Ctrl.Expspeedhigh = 0;
//    }
//    else
//    {
//        if (x >= 16.0)
//        {
//            Emo_Ctrl.Exppllhigh = 4;
//            Emo_Ctrl.Anglersptr = 16;
//            Emo_Ctrl.Expspeedhigh = 1;
//        }
//        else
//        {
//            if (x >= 8.0)
//            {
//                Emo_Ctrl.Exppllhigh = 3;
//                Emo_Ctrl.Anglersptr = 8;
//                Emo_Ctrl.Expspeedhigh = 2;
//            }
//            else
//            {
//                if (x >= 4.0)
//                {
//                    Emo_Ctrl.Exppllhigh = 2;
//                    Emo_Ctrl.Anglersptr = 4;
//                    Emo_Ctrl.Expspeedhigh = 3;
//                }
//                else
//                {
//                    Emo_Ctrl.Exppllhigh = 1;
//                    Emo_Ctrl.Anglersptr = 2;
//                    Emo_Ctrl.Expspeedhigh = 4;
//                }
//            }
//        }
//    }

} /* End of Emo_lInitFocPar */

static void Emo_lInitFocVar(void)
{
    uint16 i;
    /* Initialize variables for SVM */
    Emo_Svm.Angle = 0u;
    Emo_Svm.Amp = 0u;
    Emo_Svm.Sector = 0u;
    Emo_Svm.T1 = 0u;
    Emo_Svm.T2 = 0u;
    Emo_Svm.PhaseCurr.A = 0;
    Emo_Svm.PhaseCurr.B = 0;
    Emo_Svm.comp60up = 0;
    Emo_Svm.comp61up = 0;
    Emo_Svm.comp62up = 0;
    Emo_Svm.comp60down = 0;
    Emo_Svm.comp61down = 0;
    Emo_Svm.comp62down = 0;
    /* Initialize variables for FOC */
    Emo_Foc.StatCurr.Real = 0;
    Emo_Foc.StatCurr.Imag = 0;
    Emo_Foc.RotCurr.Real = 0;
    Emo_Foc.RotCurr.Imag = 0;
    Emo_Foc.StatVolt.Real = 0;
    Emo_Foc.StatVolt.Imag = 0;
    Emo_Foc.RotVolt.Real = 0;
    Emo_Foc.RotVolt.Imag = 0;
    Emo_Foc.Angle = 0u;
    Emo_Foc.FluxAngle = 0u;
    Emo_Foc.StoredAngle = 0u;
    Emo_Foc.RealFluxLp.Out = 0;
    Emo_Foc.ImagFluxLp.Out = 0;
    Emo_Ctrl.ActSpeed = 0;
    Emo_Ctrl.RefCurr = 0;
    Emo_Ctrl.RealCurrPi.IOut = 0;
    Emo_Ctrl.ImagCurrPi.IOut = 0;
    Emo_Foc.StartSpeedSlopeMem = 0;
    Emo_Foc.CountStart = 54;//Emo_Foc.TimeSpeedzero;
    /* define 30° as start angle */
    Emo_Foc.StartAngle = (uint16)(30.0 / 360.0 * 65536.0);

    for (i = 0u; i < 32u; i++)
    {
        Emo_Ctrl.AngleBuffer[i] = 0;
    }

    Emo_Ctrl.SpeedLpdisplay.Out = 0;
    Emo_Ctrl.SpeedLp.Out = 0;
    //Emo_Svm.CounterOffsetAdw=0;
    //Emo_Svm.CsaOffsetAdwSumme=0;
		Emo_Foc.StartCurrent = 600;
		EMO_SVM_MINTIME = 60;
} /* End of Emo_lInitFocVar */


/** \brief Handles T2 overflow.
 *
 * \param None
 * \return None
 *
 * \ingroup emo_api
 */

static void Emo_HandleT2Overflow_Start(void)
{
		 sint16_t Emo_Foc_StartEndSpeed = 800; // Emo_Focpar_Cfg.StartSpeedEnd;	 
		 sint32_t Emo_Foc_StartSpeedSlewRate = 180004; 
		 sint16_t Emo_Foc_SpeedtoFrequency = 7158; // (32768.0 * (1.0 / CCU6_T12_FREQ) * 32768.0 / 30.0) * FOC_POLE_PAIRS;

		 /* Open loop: */
		 /* Perform ramp to start end speed */
		 if (Emo_Foc.CountStart == 0u)
		 {
			 /* I/F Start-up */
			 if (Emo_Ctrl.RefSpeed > 0)
			 {
				 /* positive rotation */
				 /* apply start up ramp */
				 Emo_Foc.StartSpeedSlope = Mat_Ramp(Emo_Foc_StartEndSpeed, Emo_Foc_StartSpeedSlewRate, &Emo_Foc.StartSpeedSlopeMem);
				 /* calculate frequency out of ramp */
				 Emo_Foc.StartFrequencySlope = (sint16)__ssat(Mat_FixMul_singed((sint32)Emo_Foc.StartSpeedSlope, (sint32)Emo_Foc_SpeedtoFrequency), MAT_FIX_SAT);
				 
				 if (Emo_Foc.StartSpeedSlope == Emo_Foc_StartEndSpeed)
				 {
					 /* end speed reached */
#if (EMO_RUN==1)
					 Emo_Status.MotorState = EMO_MOTOR_STATE_RUN;						/*Start Foc */
#endif
					 /* preload speed regulator */
					 Emo_Ctrl.SpeedPi.IOut = (sint32)(Emo_Ctrl.RefCurr * (sint32)16384);
				 }
			 }
			 else
			 {
				 /* negative rotation */
				 /* apply start up ramp */
				 Emo_Foc.StartSpeedSlope = Mat_Ramp(-Emo_Foc_StartEndSpeed, Emo_Foc_StartSpeedSlewRate, &Emo_Foc.StartSpeedSlopeMem);
				 /* calculate frequency out of ramp */
				 Emo_Foc.StartFrequencySlope = (sint16)__ssat(Mat_FixMul_singed((sint32)Emo_Foc.StartSpeedSlope, (sint32)Emo_Foc_SpeedtoFrequency), MAT_FIX_SAT);
				 
				 if (Emo_Foc.StartSpeedSlope == -Emo_Foc_StartEndSpeed)
				 {
#if (EMO_RUN==1)
					 Emo_Status.MotorState = EMO_MOTOR_STATE_RUN;						/*Start Foc */
					 EMO_SVM_MINTIME = 100;
#endif
					 Emo_Ctrl.SpeedPi.IOut = (sint32)(Emo_Ctrl.RefCurr * (sint32)16384);
				 }
			 }
		 }
		 else
		 {
			 Emo_Foc.CountStart--;
			 Emo_Foc.StartFrequencySlope = 0;
			 Emo_Foc.StartCurrent += 80;
			 if(Emo_Foc.StartCurrent < 600)
			 {
				 Emo_Foc.StartCurrent = 600;
			 }
			 if(Emo_Foc.StartCurrent > 3145)
			 {Emo_Foc.StartCurrent = 3145;}
			 
		 }
				 
		 //Emo_Ctrl.ActSpeeddisplay = Mat_ExeLp_without_min_max(&Emo_Ctrl.SpeedLpdisplay, Emo_Ctrl.ActSpeed);
		 //Emo_Ctrl.EnableStartVoltage = 1;
}

static void Emo_HandleT2Overflow_Run(void)
{
	/* switch speed regulator current limits based on speed */
	if (Emo_Ctrl.ActSpeed > Emo_Ctrl_Speedlevelmaxstart)
	{
		Emo_Ctrl.SpeedPi.PiMax = 7864;
		Emo_Ctrl.SpeedPi.IMax = 7864;
	}
	else
	{
		if (Emo_Ctrl.ActSpeed < Emo_Ctrl_Speedlevelminstart)
		{
			Emo_Ctrl.SpeedPi.PiMin = -7865;
			Emo_Ctrl.SpeedPi.IMin = -7865;
		}
		else
		{
			Emo_Ctrl.SpeedPi.PiMax = 7864;
			Emo_Ctrl.SpeedPi.IMax = 7864;
			Emo_Ctrl.SpeedPi.PiMin = -7865;
			Emo_Ctrl.SpeedPi.IMin = -7865;
		}
	}
	
	/* Speed Regulator: Execute PI algorithm for (imaginary) reference current */
	Emo_Ctrl.RefCurr = Mat_ExePi(&Emo_Ctrl.SpeedPi, Emo_Ctrl.RefSpeed - Emo_Ctrl.ActSpeed);
	/* Actual Speed Filter */
	//Emo_Ctrl.ActSpeeddisplay = Mat_ExeLp_without_min_max(&Emo_Ctrl.SpeedLpdisplay, Emo_Ctrl.ActSpeed);
}

static void Emo_HandleT2Overflow(void)
{
	uint16 Emo_Foc_DcLinkVoltage = 0;
	uint32 Emo_Foc_Kdcdivident1 = 19771316;
	uint16 Emo_Foc_Kdcfactor2 = 13902; // KUZ * 32768.0 * 32768.0 / (KU * 1.7320508 * Emo_Svm.MaxAmp * 64);
	uint16 Emo_Foc_Kdcfactoriqc = 9614; // 32768.0 * KUZ / (1.7320508 * KU * 32.0);
			
   
	switch (Emo_Status.MotorState)
	{			 
		 case EMO_MOTOR_STATE_START:
			 Emo_HandleT2Overflow_Start();
			 break;
	
		 case EMO_MOTOR_STATE_RUN:
			 Emo_HandleT2Overflow_Run();
			 break;
			 
		 default: 
			 (void)Emo_StopMotor();
			 break;
	 }

    if (Emo_Status.MotorState == EMO_MOTOR_STATE_STOP)
    {
        Emo_Ctrl.ActSpeed = Mat_ExeLp_without_min_max(&Emo_Ctrl.SpeedLp, 0);
        //Emo_Ctrl.ActSpeeddisplay = Mat_ExeLp_without_min_max(&Emo_Ctrl.SpeedLpdisplay, Emo_Ctrl.ActSpeed);
    }

    /* read DC-Link-Voltage */
    Emo_Foc_DcLinkVoltage = (uint16)ADC1->RES_OUT6.reg;
    Emo_Foc.Dcfactor1 = (uint16)(Emo_Foc_Kdcdivident1 / Emo_Foc_DcLinkVoltage);
    Emo_Foc.Dcfactor2 = (uint16)__ssat((((sint32)Emo_Foc_DcLinkVoltage * (sint32)Emo_Foc_Kdcfactor2) / (sint32)4096), MAT_FIX_SAT);
    Emo_Ctrl.ImagCurrPi.IMax = (sint16)__ssat((((sint32)Emo_Foc_DcLinkVoltage * (sint32)Emo_Foc_Kdcfactoriqc) / (sint32)1024), MAT_FIX_SAT);

    Emo_Ctrl.ImagCurrPi.PiMax = Emo_Ctrl.ImagCurrPi.IMax;
    Emo_Ctrl.ImagCurrPi.IMin = -Emo_Ctrl.ImagCurrPi.IMax;
    Emo_Ctrl.ImagCurrPi.PiMin = Emo_Ctrl.ImagCurrPi.IMin;

    //if (Emo_Ctrl.EnableStartVoltage == 1)
    //{
        /* correct Start Voltage based on currently measured DC-Link voltage */
        //Emo_Foc.StartVoltAmpDivUz = __ssat(Mat_FixMulScale(Emo_Foc.StartVoltAmp, Emo_Foc.Dcfactor1, 1), MAT_FIX_SAT);
    //}

    Emo_Ctrl.SpeedPi.IMin = Emo_Ctrl.SpeedPi.PiMin;
    Emo_Ctrl.SpeedPi.IMax = Emo_Ctrl.SpeedPi.PiMax;

    if (Emo_Svm.Amp < Emo_Svm.MaxAmp9091pr)
    {
        Emo_Foc.RealFluxLp.CoefB = 109;
        Emo_Foc.ImagFluxLp.CoefB = 109;
    }
    else
    {
        Emo_Foc.RealFluxLp.CoefB = 218;
        Emo_Foc.ImagFluxLp.CoefB = 218;
    }

} /* End of Emo_HandleT2Overflow */

void GPT1_T2_Handler(void)
{
    /* T2 overflow: */
    /* Set T2 to reload value */
    GPT12E->T2.reg += GPT12E_T2_TICK;
    /* Handle T2 overflow */
    Emo_HandleT2Overflow();
} /* End of GPT1_IRQHandler */

