#include "Usr.h"
#include "emo.h"

/* FIND */
/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/
/* Observe */

/* OpenPhase */
uint8 bMotorOPFlag = 0;
static sint16 ErrCnt_Phs_A = 0;
static sint16 ErrCnt_Phs_B = 0;
static sint16 ErrCnt_Phs_C = 0;

/*******************************************************************************
**                      Function prototype Declarations                       **
*******************************************************************************/


/*******************************************************************************
**                       OpenPhase Function Declarations                      **
*******************************************************************************/
void CheckOpenPhaseState(const TEmo_Svm* check_Emo_Svm)
{
	static sint16 Phas_A = 0;
	static sint16 Phas_B = 0;
	static sint16 Phas_C = 0;
	
	if(Emo_Status.MotorState == EMO_MOTOR_STATE_RUN)
	{
		/******************************* Check A,B and C Phase *******************************/
		Phas_A = check_Emo_Svm->PhaseCurr.A;
		ErrCnt_Phs_A = ((Phas_A < UPPER_LIMIT) && (Phas_A > LOWER_LIMIT)) ? (ErrCnt_Phs_A+1) : (ErrCnt_Phs_A-1);
		
		Phas_B = check_Emo_Svm->PhaseCurr.B;
		ErrCnt_Phs_B = ((Phas_B < UPPER_LIMIT) && (Phas_B > LOWER_LIMIT)) ? (ErrCnt_Phs_B+1) : (ErrCnt_Phs_B-1);
			
		Phas_C = check_Emo_Svm->PhaseCurr.C;
		ErrCnt_Phs_C = ((Phas_C < UPPER_LIMIT) && (Phas_C > LOWER_LIMIT)) ? (ErrCnt_Phs_C+1) : (ErrCnt_Phs_C-1);

		/*****************************************************************************************/
		
		/**************************** Saturation OpenPhase Error Count ***************************/

		ErrCnt_Phs_A = (ErrCnt_Phs_A >= UPPER_COUNTER_LIMIT) ? UPPER_COUNTER_LIMIT : ErrCnt_Phs_A;
        ErrCnt_Phs_A = (ErrCnt_Phs_A < LOWER_COUNTER_LIMIT) ? LOWER_COUNTER_LIMIT : ErrCnt_Phs_A;

		ErrCnt_Phs_B = (ErrCnt_Phs_B >= UPPER_COUNTER_LIMIT) ? UPPER_COUNTER_LIMIT : ErrCnt_Phs_B;
        ErrCnt_Phs_B = (ErrCnt_Phs_B < LOWER_COUNTER_LIMIT) ? LOWER_COUNTER_LIMIT : ErrCnt_Phs_B;

		ErrCnt_Phs_C = (ErrCnt_Phs_C >= UPPER_COUNTER_LIMIT) ? UPPER_COUNTER_LIMIT : ErrCnt_Phs_C;
        ErrCnt_Phs_C = (ErrCnt_Phs_C < LOWER_COUNTER_LIMIT) ? LOWER_COUNTER_LIMIT : ErrCnt_Phs_C;		

		/************************************************************************************/
	}
	else
	{
		
	}
}

void CheckDiagnosis(void)
{
	static uint8 Confirm_OPflag_A = 0;
	static uint8 Confirm_OPflag_B = 0;
	static uint8 Confirm_OPflag_C = 0;
	static uint8 Confirm_OPflag_sum = 0;

	if(Emo_Status.MotorState == EMO_MOTOR_STATE_UNINIT)
	{
		
	}
	else if(Emo_Status.MotorState == EMO_MOTOR_STATE_STOP)
	{
		bMotorOPFlag = 0;
		ErrCnt_Phs_A = 0;
		ErrCnt_Phs_B = 0;
		ErrCnt_Phs_C = 0;
	}
	else if(Emo_Status.MotorState == EMO_MOTOR_STATE_START)
	{
	}
	else if(Emo_Status.MotorState == EMO_MOTOR_STATE_RUN)
	{		
		/************************** OpenPhase A,B and C Hysteresis************************/
		Confirm_OPflag_A = (ErrCnt_Phs_A >= CONFIRM_OP_COUNT) ? (uint8)1u : Confirm_OPflag_A;
		Confirm_OPflag_A = (ErrCnt_Phs_A <= UNCONFIRM_OP_COUNT) ? (uint8)0u : Confirm_OPflag_A;

		Confirm_OPflag_B = (ErrCnt_Phs_B >= CONFIRM_OP_COUNT) ? (uint8)1u : Confirm_OPflag_B;
		Confirm_OPflag_B = (ErrCnt_Phs_B <= UNCONFIRM_OP_COUNT) ? (uint8)0u : Confirm_OPflag_B;		

		Confirm_OPflag_C = (ErrCnt_Phs_C >= CONFIRM_OP_COUNT) ? (uint8)1u : Confirm_OPflag_C;
		Confirm_OPflag_C = (ErrCnt_Phs_C <= UNCONFIRM_OP_COUNT) ? (uint8)0u : Confirm_OPflag_C;		

		/*********************************************************************************/
		
		/************************************ Other **************************************/
		Confirm_OPflag_sum = Confirm_OPflag_A + Confirm_OPflag_B + Confirm_OPflag_C;
        if(Confirm_OPflag_sum >= 2u)
        {
            bMotorOPFlag = 2u;
		}
		else if(Confirm_OPflag_sum == 1u)
	    {
            bMotorOPFlag = 1u;
		}
		else
		{
            bMotorOPFlag = 0u;
		}
		/*********************************************************************************/
	}
	else if(Emo_Status.MotorState == EMO_MOTOR_STATE_FAULT)
	{
		
	}
	else
	{
		/* Intentionally Empty */
	}
}

