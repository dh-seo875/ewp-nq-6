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
** TA           Thomas Albersinger                                            **
**                                                                            **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*
 * V0.1.4: 2017-05-26, DM:   API extended
 * V0.1.3: 2017-02-16, DM:   Timer2/Timer21 prefix changed to capital letters
 *                           macros for Timer2x bit accesses added
 *                           Set_Reload function added
 * V0.1.2: 2015-02-10, DM:   individual header file added
 * V0.1.1: 2014-11-02, DM:   Timer values init added
 * V0.1.0: 2014-05-13, TA:   Initial version
 */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include <timer2x.h>

/*******************************************************************************
**                      External CallBacks                                    **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/** \brief Initializes the Timer2 module.
 *
 * \param None
 * \return None
 *
 * \ingroup timer2x_api
 */
void TIMER2_Init(void)
{
#if (TIMER2_Configuration_En == 1)
  TIMER2->RC2H.reg = (uint8) TIMER2_RC2H;
  TIMER2->RC2L.reg = (uint8) TIMER2_RC2L;
  TIMER2->T2H.reg = (uint8) TIMER2_T2H;
  TIMER2->T2L.reg = (uint8) TIMER2_T2L;
  TIMER2->T2MOD.reg = (uint8) TIMER2_T2MOD;
  TIMER2->T2CON.reg = (uint8) TIMER2_T2CON;
  TIMER2->T2CON1.reg = (uint8) TIMER2_T2CON1;
#endif
}

/** \brief Initializes the Timer21 module.
 *
 * \param None
 * \return None
 *
 * \ingroup timer2x_api
 */
void TIMER21_Init(void)
{
#if (TIMER21_Configuration_En == 1)
  TIMER21->RC2H.reg = (uint8) TIMER21_RC2H;
  TIMER21->RC2L.reg = (uint8) TIMER21_RC2L;
  TIMER21->T2H.reg = (uint8) TIMER21_T2H;
  TIMER21->T2L.reg = (uint8) TIMER21_T2L;
  TIMER21->T2MOD.reg = (uint8) TIMER21_T2MOD;
  TIMER21->T2CON.reg = (uint8) TIMER21_T2CON;
  TIMER21->T2CON1.reg = (uint8) TIMER21_T2CON1;
#endif
}

/** \brief Initializes the TIMER2 in Reload Mode
 *
 * \param timer interval in microseconds
 * \return true = setup done, false = setup failed
 *
 * \ingroup drv_api
 */

