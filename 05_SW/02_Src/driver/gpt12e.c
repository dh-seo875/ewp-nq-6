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
 * V0.1.4: 2017-05-26, DM:   API extended
 * V0.1.3: 2015-08-27, DM:   timer readout functions added
 * V0.1.2: 2015-02-10, DM:   individual header file added
 * V0.1.1: 2015-01-20, DM:   GPT12E timer stop API added
 * V0.1.0: 2014-05-13, DM:   Initial version
 */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include <gpt12e.h>

/*******************************************************************************
**                      External CallBacks                                    **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/** \brief Initializes the GPT12E module.
 *
 * \param None
 * \return None
 *
 * \ingroup drv_api
 */
void GPT12E_Init(void)
{
  GPT12E->T2CON.reg = GPT12E_T2CON;
  GPT12E->T2.reg = GPT12E_T2;
  GPT12E->T3CON.reg = GPT12E_T3CON;
  GPT12E->T3.reg = GPT12E_T3;
  GPT12E->T4CON.reg = GPT12E_T4CON;
  GPT12E->T4.reg = GPT12E_T4;
  GPT12E->T5CON.reg = GPT12E_T5CON;
  GPT12E->T5.reg = GPT12E_T5;
  GPT12E->T6CON.reg = GPT12E_T6CON;
  GPT12E->T6.reg = GPT12E_T6;
  GPT12E->CAPREL.reg = GPT12E_CAPREL;
  GPT12E->PISEL.reg = GPT12E_PISEL;
}


