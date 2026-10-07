/*sha256=A5CC0B0D3842B29996E799348F91988AF6875989E9349F0FC4EE8406CA8AE92B*/
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

/*------------------------------------------------------------------------------
IFXConfigWizard output file
created on:? 7 20 17:56:34 2021
------------------------------------------------------------------------------*/

#ifndef FOC_DEFINES_H
#define FOC_DEFINES_H

#ifndef IFXConfigWizard_Version
  #define IFXConfigWizard_Version 1.8.6
#endif

/* XML Version 1.0.5 */
#ifndef FOC_XML_VERSION
  #define FOC_XML_VERSION 10005
#endif

#ifndef FOC_CUR_ADJUST
  #define FOC_CUR_ADJUST 0.2
#endif

#ifndef FOC_END_START_SPEED
  #define FOC_END_START_SPEED 300
#endif

#ifndef FOC_En
  #define FOC_En 1
#endif

#ifndef FOC_FLUX_ADJUST
  #define FOC_FLUX_ADJUST 0.02
#endif

#ifndef FOC_L_PHASE
  #define FOC_L_PHASE 0.0001878
#endif

#ifndef FOC_MAX_CUR_SPEED
  #define FOC_MAX_CUR_SPEED 1000
#endif

#ifndef FOC_MAX_NEG_REF_CUR
  #define FOC_MAX_NEG_REF_CUR -15
#endif

#ifndef FOC_MAX_POS_REF_CUR
  #define FOC_MAX_POS_REF_CUR 15
#endif

#ifndef FOC_MAX_SPEED
  #define FOC_MAX_SPEED 4000
#endif

#ifndef FOC_MIN_CUR_SPEED
  #define FOC_MIN_CUR_SPEED -1000
#endif

#ifndef FOC_MIN_NEG_REF_CUR
  #define FOC_MIN_NEG_REF_CUR -5
#endif

#ifndef FOC_MIN_POS_REF_CUR
  #define FOC_MIN_POS_REF_CUR 5
#endif

#ifndef FOC_NOM_CUR
  #define FOC_NOM_CUR 8
#endif

/*FOC_POLE_PAIRS: 3*/
#ifndef FOC_POLE_PAIRS
  #define FOC_POLE_PAIRS (0x3u)
#endif

#ifndef FOC_PWM_FREQ
  #define FOC_PWM_FREQ 15000
#endif

#ifndef FOC_PWM_PERIOD
  #define FOC_PWM_PERIOD 66.6667
#endif

#ifndef FOC_R_PHASE
  #define FOC_R_PHASE 0.07948
#endif

#ifndef FOC_R_SHUNT
  #define FOC_R_SHUNT 0.003
#endif

#ifndef FOC_SPEED_FILT_TIME
  #define FOC_SPEED_FILT_TIME 0.01
#endif

#ifndef FOC_SPEED_KI
  #define FOC_SPEED_KI 800
#endif

#ifndef FOC_SPEED_KP
  #define FOC_SPEED_KP 1200
#endif

#ifndef FOC_START_ACCEL
  #define FOC_START_ACCEL 3000
#endif

#ifndef FOC_START_CUR
  #define FOC_START_CUR 3
#endif

#ifndef FOC_START_CUR_IF
  #define FOC_START_CUR_IF 3
#endif

#ifndef FOC_START_FREQ_ZERO
  #define FOC_START_FREQ_ZERO 1
#endif

#ifndef FOC_SWITCH_ON_SPEED
  #define FOC_SWITCH_ON_SPEED 100
#endif

#ifndef FOC_ZERO_VEC_TIME
  #define FOC_ZERO_VEC_TIME 0.05
#endif


#endif
