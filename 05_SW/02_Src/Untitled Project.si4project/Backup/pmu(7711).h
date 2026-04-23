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
/* See pmu.c */

#ifndef PMU_H
#define PMU_H

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include <tle_device.h>
#include <Types.h>
#include "Variant_cfg.h"

#if((EWP_TYPE == PEWP_TYPE2_0X37) || (EWP_TYPE == PEWP_TYPE2_0X38) || (EWP_TYPE == IEWP_TYPE2_0X37) || (EWP_TYPE == AEWP_TYPE2_0X37) || (EWP_TYPE == AEWP_TYPE2_0X37_WITHOUTHOTSPOT))
#include "../driver/Type2/pmu_defines.h"

#elif((EWP_TYPE == PEWP_TYPE3_0X37) || (EWP_TYPE == PEWP_TYPE3_0X38) || (EWP_TYPE == IEWP_TYPE3_0X37) || (EWP_TYPE == BEWP_TYPE3_0X37) || (EWP_TYPE == BEWP_TYPE3_0X38))
#include "../driver/Type3/pmu_defines.h"

#else
/* No Action */
#endif

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
/* Typedef Reset Status */
#define PMU_RESET_STS_POR      (0x80u)
#define PMU_RESET_STS_PIN      (0x40u)
#define PMU_RESET_STS_WDT1     (0x20u)
#define PMU_RESET_STS_ClkWDT   (0x10u)
#define PMU_RESET_STS_LPR      (0x08u)
#define PMU_RESET_STS_SLEEP    (0x04u)
#define PMU_RESET_STS_WAKE     (0x02u)
#define PMU_RESET_STS_SYS_FAIL (0x01u)

/* VDDEXT Flags */
#define PMU_VDDEXT_STABLE       (0x80u)
#define PMU_VDDEXT_OK           (0x40u)
#define PMU_VDDEXT_OVERLOAD     (0x20u)
#define PMU_VDDEXT_OVERVOLT     (0x10u)
#define PMU_VDDEXT_SHORT        (0x08u)
#define PMU_VDDEXT_IE           (0x04u)
#define PMU_VDDEXT_CYC_EN       (0x02u)
#define PMU_VDDEXT_ENABLE       (0x01u)

/* PMU Interrupt Enable/Disable Macros */
#define PMU_VDDEXT_Int_En()     (PMU->VDDEXT_CTRL.bit.FAIL_EN = 1u)
#define PMU_VDDEXT_Int_Dis()    (PMU->VDDEXT_CTRL.bit.FAIL_EN = 0u)
#define PMU_VDDC_Int_En()       (PMU->PMU_SUPPLY_STS.bit.PMU_1V5_FAIL_EN = 1u)
#define PMU_VDDC_Int_Dis()      (PMU->PMU_SUPPLY_STS.bit.PMU_1V5_FAIL_EN = 0u)
#define PMU_VDDP_Int_En()       (PMU->PMU_SUPPLY_STS.bit.PMU_5V_FAIL_EN = 1u)
#define PMU_VDDP_Int_Dis()      (PMU->PMU_SUPPLY_STS.bit.PMU_5V_FAIL_EN = 0u)

/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/
void PMU_Init(void);
/* reset function */
__STATIC_INLINE uint8 PMU_Get_Reset_Status(void);
__STATIC_INLINE void PMU_Clear_Reset_Status(void);
/* VDDEXT functions */
bool PMU_VDDEXT_On(void);
__STATIC_INLINE uint8 PMU_VDDEXT_Off(void);
__STATIC_INLINE void PMU_VDDEXT_Error_Clear(uint8 ErrorFlagMask);
__STATIC_INLINE void PMU_VDDEXT_Set(uint8 FlagMask);

/*******************************************************************************
**                      Global INLINE Function Definitions                    **
*******************************************************************************/
/** \brief Reads out the Reset Status register
 *
 * \param[in] None
 * \return or'ed combination of reset status bits
 *
 * \ingroup pmu_api
 */
__STATIC_INLINE uint8 PMU_Get_Reset_Status(void)
{
  return ((uint8) PMU->PMU_RESET_STS1.reg);
}

/** \brief Clears the Reset Status register
 *
 * \param[in] None
 * \return None
 *
 * \ingroup pmu_api
 */
__STATIC_INLINE void PMU_Clear_Reset_Status(void)
{
  PMU->PMU_RESET_STS1.reg = (uint8) 0;
}

/** \brief Switches VDDEXT off
 *
 * \param[in] None
 * \return status of VDDEXT
 *
 * \ingroup pmu_api
 */
__STATIC_INLINE uint8 PMU_VDDEXT_Off(void)
{
  PMU->VDDEXT_CTRL.bit.ENABLE = 0u;
  return ((uint8) PMU->VDDEXT_CTRL.bit.STABLE);
}

/** \brief Clear VDDEXT Error Flags
 *
 * \param[in] VDDEXT Error Flags Mask
 * \return None
 *
 * \ingroup pmu_api
 */
__STATIC_INLINE void PMU_VDDEXT_Error_Clear(uint8 ErrorFlagMask)
{
  PMU->VDDEXT_CTRL.reg &= (uint8) ~ ErrorFlagMask;
}

/** \brief Set VDDEXT Bit
 *
 * \param[in] VDDEXT Flags Mask
 * \return None
 *
 * \ingroup pmu_api
 */
__STATIC_INLINE void PMU_VDDEXT_Set(uint8 FlagMask)
{
  PMU->VDDEXT_CTRL.reg |= (uint8) FlagMask;
}
#endif
