/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : Manage_cfg.h
* Project         : 
* Created         : YYYY/MM/DD
* Description     : 
* 
* 
************************************************************************************************************************
* Revison History:
* YYYY/MM/DD             Version              Description                      Author
* 2018/01/02             v1.0                 Draft first version
*
************************************************************************************************************************
*/


#ifndef MANAGE_CFG_H
#define MANAGE_CFG_H

/*
************************************************************************************************************************
*                                                  Include header files
************************************************************************************************************************
*/
#include "Std_Types.h"
#include "Variant_cfg.h"

/*
************************************************************************************************************************
*                                                   Defines and macros
************************************************************************************************************************
*/
#define INITIAL_SLEEP_CNT_MAX (uint16_t)(  100u) /*     1 sec */

/* LIMP HOME RPM VARIANT */
#if((EWP_TYPE == PEWP_TYPE2_0X37) || (EWP_TYPE == PEWP_TYPE2_0X38) || (EWP_TYPE == IEWP_TYPE2_0X37))
#define LIMP_HOME_RPM         (sint16_t)( 3800) /*  3800 RPM */
#define SPEED_MAX_RUN         (sint16_t)( 3800) /*  3800 RPM */
#define SPEED_MIN_RUN         (sint16_t)(  500) /*   500 RPM */
#define SLEEP_CNT_MAX         (sint16_t)( 3000) /*    30 sec */
#define STALL_SPEED_ADD       (sint16_t)(    0) /*     0 RPM */
#define IQ_DIAG_COOLANT       (sint16_t)(  400) /*  iq 4.0 A */

#elif(EWP_TYPE == AEWP_TYPE2_0X37)
#define LIMP_HOME_RPM         (sint16_t)( 1500) /*  1500 RPM */
#define SPEED_MAX_RUN         (sint16_t)( 3800) /*  3800 RPM */
#define SPEED_MIN_RUN         (sint16_t)(  500) /*   500 RPM */
#define SLEEP_CNT_MAX         (sint16_t)( 3000) /*    30 sec */
#define STALL_SPEED_ADD       (sint16_t)( 3300) /*  3300 RPM */
#define IQ_DIAG_COOLANT       (sint16_t)(  400) /*  iq 4.0 A */

#elif(EWP_TYPE == AEWP_TYPE2_0X37_WITHOUTHOTSPOT)
#define LIMP_HOME_RPM         (sint16_t)( 1500) /*  1500 RPM */
#define SPEED_MAX_RUN         (sint16_t)( 3800) /*  3800 RPM */
#define SPEED_MIN_RUN         (sint16_t)(  500) /*   500 RPM */
#define SLEEP_CNT_MAX         (sint16_t)(    0) /*     0 sec */
#define STALL_SPEED_ADD       (sint16_t)( 3300) /*  3300 RPM */
#define IQ_DIAG_COOLANT       (sint16_t)(  400) /*  iq 4.0 A */

#elif((EWP_TYPE == PEWP_TYPE3_0X37) || (EWP_TYPE == PEWP_TYPE3_0X38))
#define LIMP_HOME_RPM         (sint16_t)( 3500) /*  3500 RPM */
#define SPEED_MAX_RUN         (sint16_t)( 3500) /*  3500 RPM */
#define SPEED_MIN_RUN         (sint16_t)(  500) /*   500 RPM */
#define SLEEP_CNT_MAX         (sint16_t)( 3000) /*    30 sec */
#define STALL_SPEED_ADD       (sint16_t)(    0) /*     0 RPM */
#define IQ_DIAG_COOLANT       (sint16_t)(  400) /*  iq 4.0 A */

#elif(EWP_TYPE == IEWP_TYPE3_0X37)
#define LIMP_HOME_RPM         (sint16_t)( 3200) /*  3200 RPM */
#define SPEED_MAX_RUN         (sint16_t)( 3200) /*  3200 RPM */
#define SPEED_MIN_RUN         (sint16_t)(  500) /*   500 RPM */
#define SLEEP_CNT_MAX         (sint16_t)( 3000) /*    30 sec */
#define STALL_SPEED_ADD       (sint16_t)( 2700) /*  2700 RPM */
#define IQ_DIAG_COOLANT       (sint16_t)(  400) /*  iq 4.0 A */

#elif((EWP_TYPE == BEWP_TYPE3_0X37) || (EWP_TYPE == BEWP_TYPE3_0X38))
#define LIMP_HOME_RPM         (sint16_t)( 6000) /*  6000 RPM */
#define SPEED_MAX_RUN         (sint16_t)( 6000) /*  6000 RPM */
#define SPEED_MIN_RUN         (sint16_t)( 1000) /*  1000 RPM */
//#define SLEEP_CNT_MAX         (sint16_t)( 6000) 
#define IQ_DIAG_COOLANT       (sint16_t)(  400) /*  iq 4.0 A */


#else
/* No Action */
#endif

#define SPEED_MAX_DEGRADE      (sint16_t)( 4125) /*  4125 RPM */
#define DIAG_COOLANT_KP        (sint16_t)( 2000) /*   P Gain  */
#define DIAG_COOLANT_KI        (sint16_t)(30000) /*   I Gain  */
#define DIAG_COOLANT_SPEED_MAX (sint16_t)( 6000) /*  SpeedMax */
//#define STAY_STOP_CNT_MAX      (sint16_t)(   50) 

/*
************************************************************************************************************************
*                                                         Typedefs
************************************************************************************************************************
*/

/*
************************************************************************************************************************
*                                                    Exported variables
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                               Exported function prototypes
************************************************************************************************************************
*/

#endif    /* MANAGE_CFG_H */


