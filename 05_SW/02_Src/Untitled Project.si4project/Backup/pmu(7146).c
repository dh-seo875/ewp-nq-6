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
** DM           Daniel Mysliwitz                                              **
** TA           Thomas Albersinger                                            **
**                                                                            **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* 
 * V0.1.3: 2017-07-20, DM:   PMU_VDDEXT_On() workaround reworked to work
 *                           for fSYS=40MHz as well
 * V0.1.2: 2017-07-06, DM:   PMU_VDDEXT_On() function updated to satisfy
 *                           the EPN for BE-Step
 * V0.1.1: 2015-02-10, DM:   individual header file added
 * V0.1.0: 2014-05-12, DM:   Initial version
 */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include <pmu.h>

/*******************************************************************************
**                      External CallBacks                                    **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/** \brief Initializes the PMU module.
 *
 * \param None
 * \return None
 *
 * \ingroup drv_api
 */
void PMU_Init(void)
{
#ifndef RTE_DEVICE_BF_STEP
  #if ((PMU_VDDEXT_CTRL & 1u) == 1u)
    /* turn VDDEXT on using the workaround, for BE-Step and earlier */
    PMU_VDDEXT_On();
    /* in addition write all the other control bits of VDDEXT       */
    PMU->VDDEXT_CTRL.reg = (uint8) PMU_VDDEXT_CTRL;
  #else
    /* VDDEXT is not supposed to be enabled now, so write the other **
    ** control bits only                                            */
    PMU->VDDEXT_CTRL.reg = (uint8) PMU_VDDEXT_CTRL;
  #endif /* ((PMU_VDDEXT_CTRL & 1u) == 1u) */
#else /* BF-Step */
  /* for BF-Step the register can be written directly             */
  PMU->VDDEXT_CTRL.reg = (uint8) PMU_VDDEXT_CTRL;
#endif /* RTE_DEVICE_BF_STEP */
  PMU->CNF_CYC_SENSE.reg = (uint8) PMU_CNF_CYC_SENSE;
  PMU->CNF_CYC_SAMPLE_DEL.reg = (uint8) PMU_CNF_CYC_SAMPLE_DEL;
  PMU->CNF_CYC_WAKE.reg = (uint8) PMU_CNF_CYC_WAKE;
  PMU->CNF_PMU_SETTINGS.reg = (uint8) PMU_CNF_PMU_SETTING;
  PMU->PMU_SUPPLY_STS.reg = (uint8) PMU_PMU_SUPPLY_STS;
  PMU->LIN_WAKE_EN.reg = (uint8) PMU_LIN_WAKE_EN;
  PMU->WAKE_CONF_GPIO0_CYC.reg = (uint8) PMU_WAKE_CONF_GPIO0_CYC;
  PMU->WAKE_CONF_GPIO0_FALL.reg = (uint8) PMU_WAKE_CONF_GPIO0_FALL;
  PMU->WAKE_CONF_GPIO0_RISE.reg = (uint8) PMU_WAKE_CONF_GPIO0_RISE;
  PMU->WAKE_CONF_GPIO1_CYC.reg = (uint8) PMU_WAKE_CONF_GPIO1_CYC;
  PMU->WAKE_CONF_GPIO1_FALL.reg = (uint8) PMU_WAKE_CONF_GPIO1_FALL;
  PMU->WAKE_CONF_GPIO1_RISE.reg = (uint8) PMU_WAKE_CONF_GPIO1_RISE;
  PMU->CNF_RST_TFB.reg = (uint8) PMU_CNF_RST_TFB;
  PMU->CNF_WAKE_FILTER.reg = (uint8) PMU_CNF_WAKE_FILTER;
}

#ifdef RTE_DEVICE_BF_STEP
/** \brief Turns VDDEXT on.
 * \brief BF-Step implementation
 *
 * \param None
 * \return True = VDDEXT on, False if VDDEXT is off
 *
 * \ingroup drv_api
 */
bool PMU_VDDEXT_On(void)
{
  PMU->VDDEXT_CTRL.reg = 1u;
  /* delay 200us, to give VDDEXT some time to ramp up */
  Delay_us(200);
  /* return the status of VDDEXT_CTRL.STABLE */
  return((bool)((PMU->VDDEXT_CTRL.reg & PMU_VDDEXT_STABLE) == PMU_VDDEXT_STABLE));
}
#else /* BE-Step or earlier */
/** \brief Turns VDDEXT on.
 * \brief to satify the EPN for BE-Step.
 *
 * \param None
 * \return True = VDDEXT on, False if VDDEXT is off
 *
 * \ingroup drv_api
 */
bool PMU_VDDEXT_On(void)
{
  /* disable all interrupts to ensure toggle timing 0.1us < on/off-time < 2.5us */
  __disable_irq();
  /* toggle VDDEXT.CTRL.ENABLE eight times */
  /* VDDEXT Enable #1 */
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 1u;
  /* VDDEXT Disable #1 */
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 0u;
  /* VDDEXT Enable #2 */
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 1u;
  /* VDDEXT Disable #2 */
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 0u;
  /* VDDEXT Enable #3 */
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 1u;
  /* VDDEXT Disable #3 */
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 0u;
  /* VDDEXT Enable #4 */
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 1u;
  /* VDDEXT Disable #4 */
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 0u;
  /* VDDEXT Enable #5 */
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 1u;
  /* VDDEXT Disable #5 */
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 0u;
  /* VDDEXT Enable #6 */
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 1u;
  /* VDDEXT Disable #6 */
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 0u;
  /* VDDEXT Enable #7 */
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 1u;
  /* VDDEXT Disable #7 */
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 0u;
  /* VDDEXT Enable #8 */
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 1u;
  /* enable irqs */
  __enable_irq();
  /* delay 200us, to give VDDEXT some time to ramp up */
  Delay_us(200);
  /* return the status of VDDEXT_CTRL.STABLE */
  return((bool)((PMU->VDDEXT_CTRL.reg & PMU_VDDEXT_STABLE) == PMU_VDDEXT_STABLE));
}
#endif /* RTE_DEVICE_BF_STEP */
