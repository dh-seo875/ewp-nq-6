/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : sensor.h
* Project         : 
* Created         : YYYY/MM/DD
* Description     : 
* 
* 
************************************************************************************************************************
* Revison History:
* YYYY/MM/DD             Version              Description                      Author
* 2018/02/08             v1.0                 Draft first version
*
************************************************************************************************************************
*/


#ifndef SENSOR_CFG_H
#define SENSOR_CFG_H

/*
************************************************************************************************************************
*                                                  Include header files
************************************************************************************************************************
*/
#include "Std_Types.h"


/*
************************************************************************************************************************
*                                                   Defines and macros
************************************************************************************************************************
*/
#define VDC_CONVERSION_FACTOR    (sint16_t)( 10) /* 0.01V to mV */
#define VDC_COMPENSATION_FACTOR  (sint16_t)( 60) /*      0.6 wV  */
#define IDC_CONVERSION_FACTOR    (sint16_t)( 10) /* 0.01A to mA */
#define IGN_CONVERSION_FACTOR    (sint16_t)( 10) /* 0.01V to mV */
#define IGN_ON_TH                (sint16_t)(700) /*       7  V  */
#define IGN_OFF_TH               (sint16_t)(400) /*     4.0  V  */
#define IGN_FAULT_CNT_MAX        (sint16_t)(100) /*       1 sec */
#define IGN_NORMAL_CNT_MAX       (sint16_t)( 20) /*     0.1 sec */


/* OpenPhase */
#define UPPER_LIMIT            7    /* Cal : 7 * 4 * 100 / 197 = 15.28, 0.15A, (* 100 / 197) is gain factor */
#define LOWER_LIMIT            -7   /* reasons of the multiply 4, translate from phase current to iq current in Clarke transformation. */
#define UPPER_COUNTER_LIMIT    300  /* upper limit count */
#define LOWER_COUNTER_LIMIT    1     /* lower limit count */
#define CONFIRM_OP_COUNT       10    /* finally OpenPhase count 100ms, 10ms Task*/
#define UNCONFIRM_OP_COUNT     5     /* release OpenPhase count 50ms*/

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


#endif    /* SENSOR_CFG_H */


