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


