#ifndef __USR_H__
#define __USR_H__

#include "Emo_RAM.h"

/* FIND */
/*******************************************************************************
**                            Define Declarations                             **
*******************************************************************************/
/* OpenPhase */
#define UPPER_LIMIT            15    /* Cal : 15 * 4 * 100 / 393 = 15.28, 0.15A, (* 100 / 393) is gain factor */
#define LOWER_LIMIT            -15   /* reasons of the multiply 4, translate from phase current to iq current in Clarke transformation. */
#define UPPER_COUNTER_LIMIT    1000  /* upper limit count */
#define LOWER_COUNTER_LIMIT    1     /* lower limit count */
#define CONFIRM_OP_COUNT       300   /* finally OpenPhase count */
#define UNCONFIRM_OP_COUNT     10    /* release OpenPhase count */
/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/
/* OpenPhase */
extern uint8 bMotorOPFlag;
/*******************************************************************************
**                         Function Declarations                              **
*******************************************************************************/
extern void CheckOpenPhaseState(TEmo_Svm*);
extern void CheckDiagnosis(void);

#endif

