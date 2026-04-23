/******************************************************************************
 * @file     system_tle987x.c
 * @brief    Device specific initialization for the TLE987x-Series according 
 * to CMSIS
 * @version  V0.3
 * @date     20 Jul 2017
 *
 * @note
 * Copyright (C) 2012-2017 Infineon Technologies AG. All rights reserved.

 *
 * @par
 * Infineon Technologies AG (Infineon) is supplying this software for use with 
 * Infineon’s microcontrollers.
 *   
 * This file can be freely distributed within development tools that are 
 * supporting such microcontrollers.
 *  
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

#include <tle_device.h>
#include "Variant_cfg.h"

#if((EWP_TYPE == PEWP_TYPE2_0X37) || (EWP_TYPE == PEWP_TYPE2_0X38) || (EWP_TYPE == IEWP_TYPE2_0X37) || (EWP_TYPE == AEWP_TYPE2_0X37) || (EWP_TYPE == AEWP_TYPE2_0X37_WITHOUTHOTSPOT))
#include "../driver/Type2/bsl_defines.h"

#elif((EWP_TYPE == PEWP_TYPE3_0X37) || (EWP_TYPE == PEWP_TYPE3_0X38) || (EWP_TYPE == IEWP_TYPE3_0X37) || (EWP_TYPE == BEWP_TYPE3_0X37) || (EWP_TYPE == BEWP_TYPE3_0X38))
#include "../driver/Type3/bsl_defines.h"

#else
/* No Action */
#endif

void SystemInit (void);

/*----------------------------------------------------------------------------
  Define BSL parameter
 *----------------------------------------------------------------------------*/
#if (NAC_NAD_EN == 1)
/*lint -e950 Suppressing MISRA 2012 Directive 2.1 */ \
const uint32 p_NACNAD  __attribute__((at(NACStart),used)) = (uint32)NAD_NAC;
/*lint +e950 */
#endif

void SystemInit(void)
{
  /* Vector table relocation */
  CPU->VTOR.reg	  = ProgFlashStart; 
  SCU_ClkInit();
  WDT1_Init();

  /* erratas */
  /* clear IE flags */
  TIMER2->T2CON1.reg = 0;
  TIMER21->T2CON1.reg = 0;
}
