#include "Usr.h"

/* FIND */
/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/
/* Observe */

/* OpenPhase */
uint8 bMotorOPFlag = 0;
sint16 ErrCnt_Phs_A = 0;
sint16 ErrCnt_Phs_B = 0;
sint16 ErrCnt_Phs_C = 0;
uint8 Confirm_OPflag_A = 0;
uint8 Confirm_OPflag_B = 0;
uint8 Confirm_OPflag_C = 0;
uint8 Confirm_OPflag_sum = 0;
/*******************************************************************************
**                      Function prototype Declarations                       **
*******************************************************************************/
void CheckOpenPhaseState(TEmo_Svm* Emo_Svm);
void CheckDiagnosis(void);

/*******************************************************************************
**                       OpenPhase Function Declarations                      **
*******************************************************************************/
void CheckOpenPhaseState(TEmo_Svm* Emo_Svm)
{
	static sint16 Phas_A = 0;
	static sint16 Phas_B = 0;
	static sint16 Phas_C = 0;
	
	if(Emo_Status.MotorState == EMO_MOTOR_STATE_RUN)
	{
		/******************************* Check A,B and C Phase *******************************/
		Phas_A = Emo_Svm->PhaseCurr.A;
		if((Phas_A < UPPER_LIMIT) && (Phas_A > LOWER_LIMIT))
		{
			ErrCnt_Phs_A++;
		}
		else
		{
			ErrCnt_Phs_A--;
		}			
		
		Phas_B = Emo_Svm->PhaseCurr.B;
		if((Phas_B < UPPER_LIMIT) && (Phas_B > LOWER_LIMIT))
		{
			ErrCnt_Phs_B++;
		}
		else
		{
			ErrCnt_Phs_B--;
		}				
		
		Phas_C = Emo_Svm->PhaseCurr.C;
		if((Phas_C < UPPER_LIMIT) && (Phas_C > LOWER_LIMIT))
		{
			ErrCnt_Phs_C++;
		}
		else
		{
			ErrCnt_Phs_C--;
		}
		/*****************************************************************************************/
		
		/**************************** Saturation OpenPhase Error Count ***************************/
		if(ErrCnt_Phs_A >= UPPER_COUNTER_LIMIT)
		{
			ErrCnt_Phs_A = UPPER_COUNTER_LIMIT;
		}
		else if(ErrCnt_Phs_A < LOWER_COUNTER_LIMIT)
		{
			ErrCnt_Phs_A = LOWER_COUNTER_LIMIT;
		}
		else
		{
			
		}
		
		if(ErrCnt_Phs_B >= UPPER_COUNTER_LIMIT)
		{
			ErrCnt_Phs_B = UPPER_COUNTER_LIMIT;
		}
		else if(ErrCnt_Phs_B < LOWER_COUNTER_LIMIT)
		{
			ErrCnt_Phs_B = LOWER_COUNTER_LIMIT;
		}
		else
		{
			
		}
		
		if(ErrCnt_Phs_C >= UPPER_COUNTER_LIMIT)
		{
			ErrCnt_Phs_C = UPPER_COUNTER_LIMIT;
		}
		else if(ErrCnt_Phs_C < LOWER_COUNTER_LIMIT)
		{
			ErrCnt_Phs_C = LOWER_COUNTER_LIMIT;
		}
		else
		{
			
		}
		/************************************************************************************/
	}
	else
	{
		
	}
}

void CheckDiagnosis(void)
{
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
		if(ErrCnt_Phs_A >= CONFIRM_OP_COUNT)
		{
			Confirm_OPflag_A = 1;
		}
		else if(ErrCnt_Phs_A <= UNCONFIRM_OP_COUNT)
		{
			Confirm_OPflag_A = 0;
		}
		
		if(ErrCnt_Phs_B >= CONFIRM_OP_COUNT)
		{
			Confirm_OPflag_B = 1;
		}
		else if(ErrCnt_Phs_B <= UNCONFIRM_OP_COUNT)
		{
			Confirm_OPflag_B = 0;
		}
		
		if(ErrCnt_Phs_C >= CONFIRM_OP_COUNT)
		{
			Confirm_OPflag_C = 1;
		}
		else if(ErrCnt_Phs_C <= UNCONFIRM_OP_COUNT)
		{
			Confirm_OPflag_C = 0;
		}
		/*********************************************************************************/
		
		/************************************ Other **************************************/
		Confirm_OPflag_sum = Confirm_OPflag_A + Confirm_OPflag_B + Confirm_OPflag_C;
		if(Confirm_OPflag_sum >= 2)
		{
		    bMotorOPFlag = 2;
		}
		else if(Confirm_OPflag_sum == 1)
		{
		    bMotorOPFlag = 1;
		}
    else if(Confirm_OPflag_sum == 0)
		{
		    bMotorOPFlag = 0;
		}
		/*********************************************************************************/
	}
	else if(Emo_Status.MotorState == EMO_MOTOR_STATE_FAULT)
	{
		
	}
	else
	{
		
	}
}

