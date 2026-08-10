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
#include "Mat.h"


/******************************************************************************
**                      Private Macro Definitions                            **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/


/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/


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
TComplex Mat_Clarke(TPhaseCurr PhaseCurr)
{
  TComplex StatCurr = {0,0};
  
  /* Real current = saturate(4 * Ia) */
  StatCurr.Real = (sint16)__ssat(4 * (sint32)PhaseCurr.A, 16u);

  /* Imag. current = saturate(1 / sqrt(3)) * 4 * (Ia + 2 * Ib) */
  StatCurr.Imag = (sint16)__ssat(
    Mat_FixMul_singed((sint32)MAT_ONE_OVER_SQRT_3, ((sint32)PhaseCurr.A) + (((sint32)2) * (sint32)PhaseCurr.B)) 
    * 4, MAT_FIX_SAT
  );

  
  return StatCurr;

} /* End of Mat_Clarke */


sint16 Mat_ExePi(TMat_Pi *pPi, sint16 Error)
{
  sint32 IOut;
  sint32 PiOut;
  sint32 Min;
  sint32 Max;
  sint32 Temp;
  sint32 sTemp;
  uint32 uTemp;

  /* I output = old output + error * I parameter */
  IOut = pPi->IOut + ((sint32)Error * (sint32)pPi->Ki);
  
  /* Limit I output */ 
  Min = (sint32)(pPi->IMin * 32768);
  if (IOut < Min)
  {
    IOut = Min;
  }  
  else
  {
    Max = (sint32)(pPi->IMax * 32768);
    if (IOut > Max)
    {
      IOut = Max;
    }  
  }    
  /* Store I output */
  pPi->IOut = IOut;

  /* PI output = upper half of (I output + saturate(error * P parameter) * 64) */
  Temp = __ssat(Error * ((sint32)pPi->Kp), 31u - 6u);
  sTemp = (IOut + (Temp * 64));
  if((sTemp) > 0)
  {
	  uTemp = (uint32)sTemp >> 15u;
	  PiOut = (sint32)uTemp;
  }
  else
  {
          sTemp = sTemp * -1;
          uTemp = (uint32)(sTemp);
  	  uTemp = uTemp >> 15u;
	  PiOut = (sint32)uTemp * -1;
  }

  
  /* Limit PI output */
  Min = (sint32)(pPi->PiMin);
  if (PiOut < Min)
  {
    PiOut = Min;
  }  
  else
  {
    Max = (sint32)(pPi->PiMax);
    if (PiOut > Max)
    {
      PiOut = Max;
    }  
  }    
  return (sint16)PiOut;

} /* End of Mat_ExePi_Windup */

/** \brief Performs PI control algorithm.
 *
 * \param[inout] pPi Pointer to PI status
 * \param[in] Error Difference between reference and actual value
 *
 * \return PI output
 * \ingroup math_api
 */



/** \brief Performs low-pass filter algorithm.
 *
 * \param[inout] pLp Pointer to low-pass filter status
 * \param[in] Input Input in fixed-point format
 *
 * \return Low-pass filter output in fixed-point format
 * \ingroup math_api
 */



/** \brief Performs low-pass filter algorithm.
 *
 * \param[inout] pLp Pointer to low-pass filter status
 * \param[in] Input Input in fixed-point format
 *
 * \return Low-pass filter output in fixed-point format without min and max
 * \ingroup math_api
 */
sint16 Mat_ExeLp_without_min_max(TMat_Lp_Simple *pLp, sint16 Input)
{
  sint32 Out;

  /* New output = saturate(old output + coefficient A * input - coefficient B * old output/2^15 */
  Out = pLp->Out;
  Out = __ssat((Out + ((sint32)pLp->CoefA * (sint32)Input)) -
               ((sint32)pLp->CoefB * Mat_ArithmeticShiftRight15(Out)), 31u);
  /* Store new output */
  pLp->Out = Out;

  /* return upper part */
  return (sint16)Mat_ArithmeticShiftRight15(Out);
  
} /* End of Mat_ExeLp */


/** \brief Performs the Clarke transformation.
 *
 * \param[in] PhaseCurr 3-phase current structure
 * \return 2-phase stationary current structure
 *
 * \ingroup math_api
 */


/** \brief Performs the Park transformation.
 *
 * \param[in] StatCurr Stationary 2-phase current structure
 * \param[in] Angle Angle [0..65535 = 0..2Pi]
 * \return Rotating 2-phase current structure
 *
 * \ingroup math_api
 */
TComplex Mat_Park(TComplex StatCurr, uint16 Angle)
{
  TComplex RotCurrent = {0,0};
  sint32 Cos;
  sint32 Sin;
  uint16 UAngle;
  
  /* Take upper 10 bits of 16-bit angle */
  UAngle = (Angle >> 6u);

  /* Get angle functions */
  Cos = Table_Sin[UAngle + 256u];
  Sin = Table_Sin[UAngle];

  /* Real output = saturate(real input * cos + imag. input * sin) */
  RotCurrent.Real = (sint16)__ssat(Mat_FixMul_singed((sint32)StatCurr.Real, (sint32)Cos) + Mat_FixMul_singed((sint32)StatCurr.Imag, (sint32)Sin), MAT_FIX_SAT);

  /* Imag. output = saturate(imag. input * cos - real input * sin) */
  RotCurrent.Imag = (sint16)__ssat(Mat_FixMul_singed((sint32)StatCurr.Imag, (sint32)Cos) - Mat_FixMul_singed((sint32)StatCurr.Real, (sint32)Sin), MAT_FIX_SAT);
  
  return RotCurrent;

} /* End of Mat_Park */


/** \brief Performs the inverse Park transformation.
 *
 * \param[in] RotVolt Rotating 2-phase voltage structure
 * \param[in] Angle Angle [0..65535 = 0..2Pi]
 * \return Stationary 2-phase voltage structure
 *
 * \ingroup math_api
 */

sint32 Mat_FixMulScale_17(sint32 a, sint32 b)
{
    sint32 Temp;
    sint32 sTemp;
    uint32 uTemp;
    sint32 returnval;
	Temp = a * b;
	if(Temp > 0)
    {
        uTemp = ((uint32)Temp >> 17u);
		returnval = (sint32)uTemp;
    }
    else
    {
        sTemp = (Temp * -1);
	    uTemp = (uint32)sTemp;
        uTemp = uTemp >> 17u;
	    sTemp = (sint32)uTemp * -1;
		returnval = sTemp;
    }
    return (sint32)returnval;
}
TComplex Mat_InvPark(TComplex RotVolt, uint16 Angle)
{
  TComplex StatVolt = {0,0};
  
  sint32 Cos;
  sint32 Sin;
  uint16 UAngle;
  
  /* Take upper 10 bits of 16-bit angle */
  UAngle = Angle >> 6u;

  /* Get angle functions */
  Cos = Table_Sin[UAngle + 256u];
  Sin = Table_Sin[UAngle];

  /* Real output = saturate(real input * cos / 4 - imag. input * sin / 4) */
  StatVolt.Real = (sint16)(__ssat(Mat_FixMulScale_17(RotVolt.Real, Cos) - Mat_FixMulScale_17(RotVolt.Imag, Sin), MAT_FIX_SAT));

  /* Imaginary output = saturate(real input * sin / 4 + imag. input * cos / 4) */
  StatVolt.Imag = (sint16)(__ssat(Mat_FixMulScale_17(RotVolt.Real, Sin) + Mat_FixMulScale_17(RotVolt.Imag, Cos), MAT_FIX_SAT));

  return StatVolt;

} /* End of Mat_InvPark */

/** \brief Performs the Polar in Kartesisch.
 *
 * \param[in] Amp 
 * \param[in] Angle Angle [0..65535 = 0..2Pi]
 * \return Stationary 2-phase voltage structure
 *
 * \ingroup math_api
 */
TComplex Mat_PolarKartesisch(uint16 Amp, uint16 Angle)
{
  TComplex StatOut = {0,0};
  
  sint32 Cos;
  sint32 Sin;
  uint16 UAngle;
  
  /* Take upper 10 bits of 16-bit angle */
  UAngle = Angle >> 6u;

  /* Get angle functions */
  Cos = Table_Sin[UAngle + 256u];  //pTable_Cos[UAngle];
  Sin = Table_Sin[UAngle];

  /* Real output = saturate(real input * cos / 4 - imag. input * sin / 4) */
  StatOut.Real = (sint16)(__ssat(Mat_FixMul_singed((sint32)Amp, (sint32)Cos), MAT_FIX_SAT));

  /* Imaginary output = saturate(real input * sin / 4 + imag. input * cos / 4) */
  StatOut.Imag = (sint16)(__ssat(Mat_FixMul_singed((sint32)Amp, (sint32)Sin), MAT_FIX_SAT));

  return StatOut;

} /* End of Mat_PolarKartesisch */

/** \brief Calculates angle and amplitude from stationary coordinates.
 *
 * \param[in] Stat Stationary coordinates in fixed-point format
 * \param[out] Pointer to amplitude
 * \return Angle [0..65535 = 0..2Pi]
 *
 * \ingroup math_api
 */
uint16 Mat_CalcAngleAmp(TComplex Stat, uint16 *pAmp)
{
  sint32 AbsReal;
  sint32 AbsImag;
  uint32 Index;
  uint32 Angle;
  uint32 TableValue;

  /* Get absolute values */
  AbsReal = Stat.Real;
  if(AbsReal < 0)
  {
    AbsReal = -AbsReal;
  }
  AbsImag = Stat.Imag;
  if(AbsImag < 0)
  {
    AbsImag = -AbsImag;
  }

  if(AbsImag <= AbsReal)
  {
    /* Get basic angle from table */
    Index = (((uint32)AbsImag) * 1024u) / ((uint32)AbsReal);
    TableValue = Table_ArcTan[Index];

    /* Get amplitude from table */
    *pAmp = (uint16)((((uint32)AbsReal) * Table_Amp[Index]) >> 15u);
    
    /* Get final angle depending on quadrant */
    if(Stat.Real > 0)
    {
      if(Stat.Imag >= 0)
      {
        Angle = TableValue;
      }
      else /* (Stat.Imag < 0) */
      {
        Angle = 0x10000u - TableValue;
      }
    }
    else /* (Stat.Real < 0) */
    {
      if(Stat.Imag >= 0)
      {
        Angle = 0x8000u - TableValue;
      }
      else /* (Stat.Imag < 0) */
      {
        Angle = 0x8000u + TableValue;
      }
    }
  }
  else /* (AbsReal < AbsImag) */
  {
    /* Get basic angle from table */
    Index = (((uint32)AbsReal) * 1024u) / ((uint32)AbsImag);

    TableValue = Table_ArcTan[Index];

    /* Get amplitude from table */
    *pAmp = (uint16)((((uint32)AbsImag) * Table_Amp[Index]) >> 15u);
    
    /* Get final angle depending on quadrant */
    if(Stat.Real >= 0)
    {
      if(Stat.Imag > 0)
      {
        Angle = 0x4000u - TableValue;
      }
      else /* (Stat.Imag < 0) */
      {
        Angle = 0xC000u + TableValue;
      }
    }
    else /* (Stat.Real < 0) */
    {
      if(Stat.Imag > 0)
      {
        Angle = 0x4000u + TableValue;
      }
      else /* (Stat.Imag < 0) */
      {
        Angle = 0xC000u - TableValue;
      }
    }
  }
  return (uint16)Angle;
  
} /* End of Mat_CalcAngleAmp */


/** \brief Calculates angle from stationary coordinates.
 *
 * \param[in] Stat Stationary coordinates in fixed-point format
 * \return Angle [0..65535 = 0..2Pi]
 *
 * \ingroup math_api
 */

/** \brief Calculates amplitude from stationary coordinates.
 *
 * \param[in] Stat Stationary coordinates in fixed-point format
 * \param[out] Pointer to amplitude
 * \return Angle [0..65535 = 0..2Pi]
 *
 * \ingroup math_api
 */


/** \brief Calculates rampe.
 *
 * \param[in] Slewrate Input in fixed-point format
 * \param[out] output32
 * \return output [-32767..+32767]
 *
 * \ingroup math_api
 */
sint16 Mat_Ramp(sint16 Input, sint32 Slewrate, sint32 *Output)
{
  sint32 inp;
  sint32 outp;
  uint32 uTemp;
  sint32 sTemp;
  sint32 returnval;
  outp =* Output;
  if(Input > 0)
  {
      uTemp = (uint32)Input;
      uTemp = (uTemp << 16u);
	  inp= (sint32)uTemp;
  }
  else
  {
     sTemp = (sint32)Input * -1;
	   uTemp = (uint32)sTemp;
	   uTemp = (uTemp << 16u);
	   inp = (sint32)uTemp * -1;	   
  }
  if (inp > outp)
  {
    outp = outp + Slewrate;
    if (outp > inp) {outp = inp;}
  }
  else
  {
    outp=outp-Slewrate;
    if (outp < inp){outp = inp;}
  }
  *Output = outp;
	if(outp > 0)
    {
        uTemp = ((uint32)outp >> 16u);
		returnval = (sint32)uTemp;
    }
    else
    {
        sTemp = (outp * -1);
	    uTemp = (uint32)sTemp;
        uTemp = uTemp >> 16u;
	    sTemp = (sint32)uTemp * -1;
		returnval = sTemp;
    }
	return (sint16)returnval;
}
 /* End of Mat_Ramp */

sint32 Mat_FixMul_singed(sint32 Factor1,sint32 Factor2)
{
	sint32 result;
	sint32 s32_res;
	uint32 u32_res;
	
	s32_res = (Factor1 * Factor2)  + 1073741824;
	u32_res = (uint32)s32_res >> 15u;
	result = (sint32)u32_res - 32768;

	return result;
}


