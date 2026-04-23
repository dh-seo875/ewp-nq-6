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


/******************************************************************************
**                      Revision Control History                             **
******************************************************************************/
/* See ccu6.c */

#ifndef CCU6_H
#define CCU6_H

/******************************************************************************
**                      Includes                                             **
******************************************************************************/
#include <tle_device.h>
#include <Types.h>
#include "Variant_cfg.h"

#if((EWP_TYPE == PEWP_TYPE2_0X37) || (EWP_TYPE == PEWP_TYPE2_0X38) || (EWP_TYPE == IEWP_TYPE2_0X37) || (EWP_TYPE == AEWP_TYPE2_0X37) || (EWP_TYPE == AEWP_TYPE2_0X37_WITHOUTHOTSPOT))
#include "../driver/Type2/ccu6_defines.h"

#elif((EWP_TYPE == PEWP_TYPE3_0X37) || (EWP_TYPE == PEWP_TYPE3_0X38) || (EWP_TYPE == IEWP_TYPE3_0X37) || (EWP_TYPE == BEWP_TYPE3_0X37) || (EWP_TYPE == BEWP_TYPE3_0X38))
#include "../driver/Type3/ccu6_defines.h"

#else
/* No Action */
#endif

/******************************************************************************
**                      Global Macro Definitions                             **
******************************************************************************/
/* Constants for TCTR4 register */
#define CCU6_MASK_TCTR4_STOP_T12       (0x0001u)
#define CCU6_MASK_TCTR4_START_T12      (0x0002u)
//#define CCU6_MASK_TCTR4_RESET_T12      (0x0004u)
#define CCU6_MASK_TCTR4_SHADOW_T12     (0x0040u)
#define CCU6_MASK_TCTR4_STOP_T13       (0x0100u)
#define CCU6_MASK_TCTR4_START_T13      (0x0200u)
//#define CCU6_MASK_TCTR4_RESET_T13      (0x0400u)
#define CCU6_MASK_TCTR4_SHADOW_T13     (0x4000u)

#define CCU6_T12_OM_Int_Clr()        (CCU6->ISR.reg = (uint16)1u << 6u)
#define CCU6_T12_PM_Int_Clr()        (CCU6->ISR.reg = (uint16)1u << 7u)


/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/
void CCU6_Init(void);
__STATIC_INLINE void CCU6_StartTmr_T12(void);
__STATIC_INLINE void CCU6_StartTmr_T13(void);
__STATIC_INLINE void CCU6_StopTmr_T12(void);
__STATIC_INLINE void CCU6_StopTmr_T13(void);
__STATIC_INLINE void CCU6_EnableST_T12(void);
__STATIC_INLINE void CCU6_EnableST_T13(void);
__STATIC_INLINE void CCU6_LoadShadowRegister_CC60(uint16 tick);
__STATIC_INLINE void CCU6_LoadShadowRegister_CC61(uint16 tick);
__STATIC_INLINE void CCU6_LoadShadowRegister_CC62(uint16 tick);
__STATIC_INLINE void CCU6_LoadShadowRegister_CC63(uint16 tick);
__STATIC_INLINE void CCU6_LoadPeriodRegister_T13_Time(uint16 us);
__STATIC_INLINE void CCU6_LoadPeriodRegister_T13_Tick(uint16 tick);
//__STATIC_INLINE bool CCU6_IsT13Running(void);
__STATIC_INLINE void CCU6_SetT13Trigger(uint16 Mask);
__STATIC_INLINE void CCU6_SetT13Compare(uint16 Compare);
__STATIC_INLINE void CCU6_SetT12T13ControlBits(uint16 Mask);
__STATIC_INLINE uint32 CCU6_ReadHallReg(void);
__STATIC_INLINE uint16 CCU6_ReadMultichannelPatterns(void);
__STATIC_INLINE void CCU6_WriteMultichannelPatterns(uint16 Patterns);
__STATIC_INLINE void CCU6_ConfigureMultichannelModulation(uint16 Mode);
__STATIC_INLINE void CCU6_ConfigureGlobalModulation(uint16 Mode);
__STATIC_INLINE void CCU6_EnableInt(uint16 Mask);
__STATIC_INLINE void CCU6_ClearIntStatus(uint16 Mask);

/******************************************************************************
**                      Global Inline Function Definitions                   **
******************************************************************************/
/** \brief Start CCU6 Timer
 *
 * \param[in] none
 * \return void
 * \ingroup ccu_api
 */
__STATIC_INLINE void CCU6_StartTmr_T12(void)
{
  CCU6->TCTR4.reg = CCU6_MASK_TCTR4_START_T12;
}

__STATIC_INLINE void CCU6_StartTmr_T13(void)
{
  CCU6->TCTR4.reg = CCU6_MASK_TCTR4_START_T13;
}


/** \brief Stop CCU6 Timer
 *
 * \param[in] TimerNr-T12, T13
 * \return void
 * \ingroup ccu_api
 */
__STATIC_INLINE void CCU6_StopTmr_T12(void)
{
  CCU6->TCTR4.reg = CCU6_MASK_TCTR4_STOP_T12;
}

__STATIC_INLINE void CCU6_StopTmr_T13(void)
{
  CCU6->TCTR4.reg = CCU6_MASK_TCTR4_STOP_T13;
}

/** \brief Enable Shadow Transfer
 *
 * \param[in] TimerNr-T12, T13
 * \return void
 * \ingroup ccu_api
 */
__STATIC_INLINE void CCU6_EnableST_T12(void)
{
  CCU6->TCTR4.reg = CCU6_MASK_TCTR4_SHADOW_T12;
}

__STATIC_INLINE void CCU6_EnableST_T13(void)
{
  CCU6->TCTR4.reg = CCU6_MASK_TCTR4_SHADOW_T13;
}

/** \brief Load compare value to the shadow register
 *
 * \param[in] compare value in clock ticks
 * \return void
 * \ingroup ccu_api
 */
__STATIC_INLINE void CCU6_LoadShadowRegister_CC60(uint16 tick)
{
  CCU6->CC60SR.reg = tick;
}

__STATIC_INLINE void CCU6_LoadShadowRegister_CC61(uint16 tick)
{
  CCU6->CC61SR.reg = tick;
}

__STATIC_INLINE void CCU6_LoadShadowRegister_CC62(uint16 tick)
{
  CCU6->CC62SR.reg = tick;
}

__STATIC_INLINE void CCU6_LoadShadowRegister_CC63(uint16 tick)
{
  CCU6->CC63SR.reg = tick;
}

/** \brief Load Timer13 Period Register as Time Value
 *
 * \param[in] Period in Time [us]
 * \return void
 * \ingroup ccu_api
 */
__STATIC_INLINE void CCU6_LoadPeriodRegister_T13_Time(uint16 us)
{
  CCU6->T13PR.reg = (uint16) ((uint16)CCU6_T13_CLK * us);
}

/** \brief Load Timer13 Period Register as Time Value
 *
 * \param[in] Period in clock ticks
 * \return void
 * \ingroup ccu_api
 */
__STATIC_INLINE void CCU6_LoadPeriodRegister_T13_Tick(uint16 tick)
{
  CCU6->T13PR.reg = tick;
}

/** \brief Load Timer13 Period Register as Time Value
 *
 * \param[in] Period in Time [us]
 * \return void
 * \ingroup ccu_api
 */
//__STATIC_INLINE bool CCU6_IsT13Running(void)
//{
//  return ((boolean_t) (CCU6->TCTR0.bit.T13R == 1u));
//}

/** \brief Sets trigger event for the T13 timer.
 *
 * \param[in] Mask Trigger mask
 * \return None
 *
 * \ingroup ccu6_api
 */
__STATIC_INLINE void CCU6_SetT13Trigger(uint16 Mask)
{
  /* Set trigger mask */
  CCU6->TCTR2.reg = Mask;
}

/** \brief Sets compare value for the T13 timer.
 *
 * \param[in] Compare Compare value
 * \return None
 *
 * \ingroup ccu6_api
 */
__STATIC_INLINE void CCU6_SetT13Compare(uint16 Compare)
{
  /* Set compare value */
  CCU6->CC63SR.reg = Compare;
  
  /* Set period value */
  CCU6->T13PR.reg = Compare + 10u;
  
  /* Enable shadow transfer */
  CCU6->TCTR4.reg = CCU6_MASK_TCTR4_SHADOW_T13;
}

/** \brief Sets write-only control bits for T12 and/or T13 timer.
 *
 * \param[in] Mask Control bit mask
 * \return None
 *
 * \ingroup ccu6_api
 */
__STATIC_INLINE void CCU6_SetT12T13ControlBits(uint16 Mask)
{
  CCU6->TCTR4.reg = Mask;
}

/** \brief Reads sampled Hall pattern from CCU6 CMPSTAT register.
 *
 * \return Sampled Hall pattern
 *
 * \ingroup ccu6_api
 */
__STATIC_INLINE uint32 CCU6_ReadHallReg(void)
{
  return ((uint32)(CCU6->CMPSTAT.reg) >> 3u) & 0x7u;
}

/** \brief Reads actual Hall and PWM patterns for Multi-Channel Mode.
 *
 * \return Hall and PWM patterns
 *
 * \ingroup ccu6_api
 */
__STATIC_INLINE uint16 CCU6_ReadMultichannelPatterns(void)
{
  return CCU6->MCMOUT.reg;
}

/** \brief Writes Hall and/or PWM patterns for Multi-Channel Mode to shadow register.
 *
 * \param[in] Patterns Hall and/or PWM patterns
 * \return None
 *
 * \ingroup ccu6_api
 */
__STATIC_INLINE void CCU6_WriteMultichannelPatterns(uint16 Patterns)
{
  CCU6->MCMOUTS.reg = Patterns;
}

/** \brief Sets Multi-Channel Mode control register.
 *
 * \param[in] Mode Multi-Channel mode control bits
 * \return None
 *
 * \ingroup ccu6_api
 */
__STATIC_INLINE void CCU6_ConfigureMultichannelModulation(uint16 Mode)
{
  CCU6->MCMCTR.reg = Mode;
}

/** \brief Sets global modulation control register.
 *
 * \param[in] Mode Global modulation control bits
 * \return None
 *
 * \ingroup ccu6_api
 */
__STATIC_INLINE void CCU6_ConfigureGlobalModulation(uint16 Mode)
{
  CCU6->MODCTR.reg = Mode;
}

/** \brief Enables/disables interrupt(s).
 *
 * \param[in] Mask for interrupt(s) to be enabled.
 * \return None
 *
 * \ingroup ccu6_api
 */
__STATIC_INLINE void CCU6_EnableInt(uint16 Mask)
{
  CCU6->IEN.reg = Mask;
}

/** \brief Clears interrupt status bit(s).
 *
 * \param[in] Mask for interrupt status bit(s) to be cleared.
 * \return None
 *
 * \ingroup ccu6_api
 */
__STATIC_INLINE void CCU6_ClearIntStatus(uint16 Mask)
{
  CCU6->ISR.reg = Mask;
}


#endif
