/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : Diag_cfg.h
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


#ifndef DIAG_CFG_H
#define DIAG_CFG_H

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



#if((EWP_TYPE == PEWP_TYPE3_0X37) || (EWP_TYPE == PEWP_TYPE3_0X38) || (EWP_TYPE == BEWP_TYPE3_0X37) || (EWP_TYPE == BEWP_TYPE3_0X38))
//#define SE_NORMAL_UPR_TH        (sint16_t)(  500)  500 RPM 
//#define SE_NORMAL_LWR_TH        (sint16_t)(  500)  500 RPM 
#define SE_FAULT_UPR_TH         (sint16_t)(  500) /* 500 RPM */
#define SE_WARNING_LWR_TH       (sint16_t)(  200) /* 200 RPM */
#define SE_NORMAL_LWR_TH        (sint16_t)(  140) /* 140 RPM */

//#define SE_FAULT_LWR_TH         (sint16_t)(  500)  500 RPM 
//#define SE_NORMAL_CNT_MAX       (sint16_t)( 2000)
#define SE_WARNING_CNT_MAX      (sint16_t)( 2000) /*  20 sec */ 
#define SE_FAULT_CNT_MAX        (sint16_t)( 2000) /*  20 sec */
#define SE_RETRY_ATTEMPT_MAX    (sint16_t)(   10) /* Retry 4 */
#define SE_RETRY_CNT_MAX        (sint16_t)(  200) /* 2 sec */
#define SE_AttemptCLR_CNT_MAX   (sint16_t)( 2000) /* 20 sec */


#define OC_IQ_WARNING_UPR_TH    (sint16_t)( 1450) /* iq 14.5 A */
#define OC_IQ_WARNING_LWR_TH    (sint16_t)( 1350) /* iq 13.5 A */

#define OC_IQ_FAULT_UPR_TH      (sint16_t)( 1600) /* iq 16 A */
#define OC_AttemptCLR_CNT_MAX   (sint16_t)( 500) /* 5s */



#define OC_NORMAL_CNT_MAX       (sint16_t)(  100) /* 1 sec */
#define OC_RETRY_ATTEMPT_MAX    (sint16_t)(   10) /* Retry 7 */
#define OC_WARNING_CNT_MAX      (sint16_t)(  100) /*  1 sec */
#define OC_FAULT_CNT_MAX        (sint16_t)(  250) /*  2.5 sec */
#define OC_RETRY_CNT_MAX        (sint16_t)(  200) /* 2 sec */


#define ST_FLUXABS_FAULT_UPR_TH (sint16_t)(  500) /* FluxAbs 500 */
#define ST_FAULT_CNT_MAX        (sint16_t)(  100) /* 1 sec */
#define ST_AttemptCLR_CNT_MAX   (sint16_t)(  100) /* 1s */
#define ST_RETRY_ATTEMPT_MAX    (sint16_t)(   10) /* Retry 10 */
#define ST_RETRY_CNT_MAX        (sint16_t)(  200) /* 10 sec */


//#define LC_FAULT_UPR_TH         (sint16_t)(  150)  iq 1.5 A 
#define LC_FAULT_LWR_TH         (sint16_t)(  90) /* iq 0.9 A */
#define LC_NORMAL_UPR_TH        (sint16_t)(  150) /* iq 1.5 A */

#define LC_FAULT_CNT_MAX        (sint16_t)(  500) /* 5 sec */
#define LC_RETRY_CNT_MAX        (sint16_t)(  500) /* 5 sec */
#define LC_RETRY_ATTEMPT_MAX  (sint16_t)(   10) /* Retry 20 */

#define LC_AttemptCLR_CNT_MAX   (sint16_t)(  500) /*  5 sec */
#define LC_SPEED_MIN            (sint16_t)( 3500) /* 3500 RPM */

#define SC_NORMAL_CNT_MAX       (sint16_t)(  100) /*  1 sec */
#define SC_FAULT_CNT_MAX        (sint16_t)(  100) /*  1 sec */
#define SC_RETRY_CNT_MAX        (sint16_t)(  100) /*  1 sec */
#define SC_RETRY_ATTEMPT_MAX    (sint16_t)(    4) /* Retry 4 */

#define GE_NORMAL_CNT_MAX       (sint16_t)(  100) /*  1 sec */
#define GE_FAULT_CNT_MAX        (sint16_t)(  100) /*  1 sec */
#define GE_RETRY_CNT_MAX        (sint16_t)(  100) /*  1 sec */
#define GE_RETRY_ATTEMPT_MAX    (sint16_t)(    4) /* Retry 4 */


//#define OT_RETRYCLR_CNT_MAX     (sint16_t)(  1000)   1 sec

//#define OT_NORMAL_UPR_TH        (sint16_t)(  140)  140 degree 
#define OT_NORMAL_LWR_TH        (sint16_t)(  135) /* 135 degree */
#define OT_WARNING_UPR_TH       (sint16_t)(  140) /* 140 degree */
#define OT_WARNING_LWR_TH       (sint16_t)(  140) /* 140 degree */
#define OT_FAULT_UPR_TH         (sint16_t)(  150) /* 150 degree */
//#define OT_FAULT_LWR_TH         (sint16_t)(  150)  150 degree 
#define OT_NORMAL_CNT_MAX       (sint16_t)(  100) /* 1 sec */
#define OT_WARNING_CNT_MAX      (sint16_t)(  100) /* 1 sec */
#define OT_FAULT_CNT_MAX        (sint16_t)(  100) /* 1 sec */

#define UV_NORMAL_UPR_TH        (sint16_t)(  900) /*  9.0 V */
//#define UV_NORMAL_LWR_TH        (sint16_t)(  890)   8.9 V 
//#define UV_FAULT_UPR_TH         (sint16_t)(  790)   7.9 V 
#define UV_FAULT_LWR_TH         (sint16_t)(  800) /*  8.0 V */
#define UV_NORMAL_CNT_MAX       (sint16_t)(  100) /*  1 sec */
#define UV_FAULT_CNT_MAX        (sint16_t)(  100) /*  1 sec */
#define UV_RETRY_ATTEMPT_MAX    (sint16_t)(   10) /* Retry 7 */
#define UV_AttemptCLR_CNT_MAX   (sint16_t)(  100) /*  1 sec */

//#define OV_NORMAL_UPR_TH        (sint16_t)( 1600)    16V 
#define OV_NORMAL_LWR_TH        (sint16_t)( 1600) /*  16   V */
#define OV_FAULT_UPR_TH         (sint16_t)( 1700) /*  17.0 V */
#define OV_RETRY_ATTEMPT_MAX    (sint16_t)(   10) /* Retry 7 */
#define OV_AttemptCLR_CNT_MAX   (sint16_t)(  100) /*  1 sec */
//#define OV_FAULT_LWR_TH         (sint16_t)( 1650) 16.5 V 
#define OV_NORMAL_CNT_MAX       (sint16_t)(  100) /*  1 sec  */
#define OV_FAULT_CNT_MAX        (sint16_t)(  100) /*  1 sec  */

#define PO_NORMAL_CNT_MAX       (sint16_t)(  100) /*  1 sec */
#define PO_FAULT_CNT_MAX        (sint16_t)(  100) /*  1 sec */
#define PO_RETRY_CNT_MAX        (sint16_t)(  100) /*  1 sec */
#define PO_RETRY_ATTEMPT_MAX    (sint16_t)(    4) /* Retry 4 */

#define SPEED_MIN               (sint16_t)( 1000) /* 1200 RPM */





#else
/* No Action */
#endif


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

#endif    /* DIAG_CFG_H */


