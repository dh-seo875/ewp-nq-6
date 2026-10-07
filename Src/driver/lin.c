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
 * V0.1.5: 2016-10-12, DM:   LINST initialization added
 * V0.1.4: 2016-07-07, DM:   Init function corrected to be able to set corrected
 *                           slope mode
 * V0.1.3: 2015-02-10, DM:   individual header file added
 * V0.1.2: 2014-09-22, DM:   LIN_Get_Mode function added
 * V0.1.1: 2014-06-17, DM:   Adding functions to change Trx. Mode and Slope
 * V0.1.0: 2014-06-02, DM:   Initial version
 */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include <lin.h>

/*******************************************************************************
**                      External CallBacks                                    **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/** \brief Initializes the LIN module.
 *
 * \param None
 * \return None
 *
 * \ingroup drv_api
 */
void LIN_Init(void)
{
#if (LIN_Configuration_En == 1)
  uint32 timeout = 10000000u;
  /* set LIN Trx into Sleep Mode           */
  LIN->CTRL_STS.bit.MODE = LIN_MODE_SLEEP;
  /* wait until LIN Trx switched           *
   * into Sleep Mode                       */
  while (timeout > 0u)
  {
      timeout = timeout -1u;
      if(LIN_Get_Mode() == LIN_GET_MODE_SLEEP)
      {
          break;     
	  }
  }
  /* write register according to IFXConfigWizard **
  ** settings, incl. target LIN Slope Mode       */
  LIN->CTRL_STS.reg = (uint32) LIN_CTRL_STS;
  SCU->LINST.reg = (uint8) SCU_LINST;
#endif
}
