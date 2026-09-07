/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : Diag.c
* Project         : iEWP
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


/*
************************************************************************************************************************
*                                                   Include header files
************************************************************************************************************************
*/
#include "Diag.h"
#include "RteStd.h"


/*
************************************************************************************************************************
*                                                 Local Defines and macros
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                                     Local typedefs
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                                   Exported variables
************************************************************************************************************************
*/
static DiagState_t IRV_SC_diagState = NORMAL;
static DiagState_t IRV_GE_diagState = NORMAL;
static DiagState_t IRV_SE_diagState = NORMAL;
static DiagState_t IRV_OT_diagState = NORMAL;
static DiagState_t IRV_WC_diagState = NORMAL;
static DiagState_t IRV_OC_diagState = NORMAL;
static DiagState_t IRV_ST_diagState = NORMAL;
static DiagState_t IRV_LC_diagState = NORMAL;
static DiagState_t IRV_OV_diagState = NORMAL;
static DiagState_t IRV_UV_diagState = NORMAL;
static DiagState_t IRV_LE_diagState = NORMAL;
static DiagState_t IRV_LIV_diagState = NORMAL;
static DiagState_t IRV_PO_diagState = NORMAL;


static uint8_t IRV_LC_Faultflag = 0U;
static uint8_t IRV_OC_Faultflag = 0U;
static uint8_t IRV_ST_Faultflag = 0U;
static uint8_t IRV_OV_Faultflag = 0U;
static uint8_t IRV_UV_Faultflag = 0U;
static uint8_t IRV_OT_Faultflag = 0U;
static uint8_t IRV_SE_Faultflag = 0U;

static uint8_t IRV_WC_Warningflag = 0U;
static uint8_t IRV_OT_Warningflag = 0U;
static uint8_t IRV_SE_Warningflag = 0U;
static uint8_t IRV_LIV_Warningflag = 0U;

//static uint8_t SE_faultflag                  = 0;
//static uint8_t OT_faultflag                  = 0;
//static uint8_t OC_faultflag                  = 0;
//static uint8_t LC_faultflag                  = 0;
//static uint8_t OV_faultflag                  = 0;
//static uint8_t UV_faultflag                  = 0;


/*
************************************************************************************************************************
*                                                     Local variables
************************************************************************************************************************
*/

/* via Rte */
static sint16_t diag_vdcFiltered              = 0;
static sint16_t diag_speedRef                 = 0;
static sint16_t diag_iqFiltered               = 0;


/* Only used Diag.c */


//static uint8_t WT_warningCheck                = 0;
//static uint8_t SE_warningCheck				  = 0;
//static uint8_t WC_warningCheck				  = 0;

static uint8_t SC_failureCheck                = 0U;
static uint8_t GE_failureCheck                = 0U;
static uint8_t OC_failureCheck				  = 0U;
static uint8_t LC_failureCheck				  = 0U;
//static uint8_t OT_failureCheck				  = 0;
static uint8_t UV_failureCheck				  = 0U;
static uint8_t OV_failureCheck				  = 0U;
static uint8_t SE_failureCheck				  = 0U;
static uint8_t PO_failureCheck				  = 0U;
static uint8_t ST_failureCheck				  = 0U;


/************************ Under Voltage **************************/
static uint8_t UV_normalCnt				  = 0U;
static uint8_t UV_faultCnt 				  = 0U;
static uint8_t UV_retryAttempt 			  = 0U;
/************************ Over Voltage **************************/
static uint8_t OV_normalCnt				  = 0U;
static uint8_t OV_faultCnt 				  = 0U;
static uint8_t OV_retryAttempt 			  = 0U;
/************************ Over Temperature **************************/
static sint16_t diag_temperatureFiltered	  = 0;
static uint8_t OT_normalCnt				  = 0U;
static uint8_t OT_warningCnt				  = 0U;
static uint8_t OT_faultCnt 				  = 0U;
static const sint16_t CAL_OT_FaultUprTh 		= OT_FAULT_UPR_TH;
//static sint16_t OT_retryAttempt 			  = 0;
//static sint16_t OT_AttemptCLRCnt              = 0;
static const sint16_t CAL_OT_NormalLwrTh 		= OT_NORMAL_LWR_TH;

static const uint8_t CAL_OT_NormalCntMax		= OT_NORMAL_CNT_MAX;
static const uint8_t CAL_OT_WarningCntMax		= OT_WARNING_CNT_MAX;
//sint16_t CAL_OT_RetryCLRCntMax		= OT_RETRYCLR_CNT_MAX;
//static const sint16_t CAL_OT_AttemptCLRCntMax	= OT_AttemptCLR_CNT_MAX;

/************************ Short Circuit **************************/
static uint8_t SC_normalCnt				  = 0U;
static uint8_t SC_faultCnt 				  = 0U;
static uint8_t SC_retryCnt 				  = 0U;
static uint8_t SC_retryAttempt 			  = 0U;
/************************ GateDrv Error **************************/
static uint8_t GE_normalCnt				  = 0U;
static uint8_t GE_faultCnt 				  = 0U;
static uint8_t GE_retryCnt 				  = 0U;
static uint8_t GE_retryAttempt 			  = 0U;

/************************ Warning Current **************************/
static const sint16_t CAL_SpeedMin = SPEED_MIN;
static uint8_t WC_WarningCnt = 0U;
static uint8_t WC_normalCnt = 0U;



/************************ Over Current **************************/
static uint8_t OC_faultCnt 				  = 0U;
static uint8_t OC_retryAttempt 			  = 0U;

static uint8_t OC_retryCnt 				  = 0U;

/************************ Low Current **************************/
static sint16_t LC_normalCnt				  = 0;
static sint16_t LC_faultCnt 				  = 0;
static sint16_t LC_retryCnt 				  = 0;
static uint8_t LC_retryAttempt 			  = 0U;


/************************ Stall Error **************************/
static uint8_t ST_FaultCnt 			  = 0U;
static uint8_t ST_NormalCnt			  = 0U;
static uint8_t ST_retryCnt 				  = 0U;
static uint8_t ST_retryAttempt 			  = 0U;

/************************ LIN Invalid Value **************************/
static uint16_t LIV_faultCnt = 0U;
static sint8_t diag_ignState = IGN_ON;


/************************ Speed Error **************************/
static sint16_t diag_speedError 			  = 0;
static sint16_t SE_normalCnt				  = 0;
static sint16_t SE_faultCnt 				  = 0;
static uint8_t SE_retryAttempt 			  = 0U;
static uint8_t SE_retryCnt 				  = 0U;
static sint16_t SE_WarningCnt				  = 0;

static const sint16_t CAL_SE_WarningLwrTh		 = SE_WARNING_LWR_TH;
//sint16_t CAL_SE_NormalLwrTh		= SE_NORMAL_LWR_TH;

static const sint16_t CAL_SE_FaultUprTh			= SE_FAULT_UPR_TH;
static const sint16_t CAL_SE_FaultCntMax 		= SE_FAULT_CNT_MAX;

/************************ Phase Open **************************/

static sint16_t PO_normalCnt				  = 0;
static sint16_t PO_faultCnt 				  = 0;
static uint8_t PO_retryCnt 				  = 0U;
static uint8_t PO_retryAttempt 			  = 0U;


/* 	
************************************************************************************************************************
*                                                 Local function prototypes
************************************************************************************************************************
*/
static void DiagShortCircuit (void);
static void DiagGateDrvOVUV(void);

static void DiagSpeedError (void);
static void DiagSpeedErrorNormal(void);
static void DiagSpeedErrorWarning(void);
static void DiagSpeedErrorFault(void);


static void DiagOverTemperature (void);
static void DiagOverTemperatureNormal (void);
static void DiagOverTemperatureWarning (void);
static void DiagOverTemperatureFault (void);


static void DiagWarningCurrent (void);
static void DiagWarningCurrentNormal(void);
static void DiagWarningCurrentWarning(void);


static void DiagOverCurrent (void);
static void DiagOverCurrentNormal(void);
static void DiagOverCurrentFault(void);
static void DiagStallError (void);
static void DiagStallErrorNormal(void);
static void DiagStallErrorFault(void);


static void DiagLowCurrent (void);
static void DiagOverVoltage (void);
static void DiagUnderVoltage (void);
static void DiagLINError (void);
static void DiagLINInvalidValue (void);
static void MergeDiagState (void);
static void SetDiagFlagState (void);
static void DiagPhaseOpen (void);
static void DiagEwpInital(void);

static void DiagSWComponent1(void);
static void DiagSWComponent2(void);
static void DiagSWComponent3(void);
static void DiagSWComponent4(void);
/*
************************************************************************************************************************
*                                                    Exported functions
************************************************************************************************************************
*/
void DiagSWComponentPack(void)
{
    static uint8_t DiagTask_Cnt = 0U;
	if(DiagTask_Cnt == 0u) // 26us
	{			
        DiagSWComponent1();
		DiagTask_Cnt++;
	}
	else if(DiagTask_Cnt == 1u) //26us
	{		
		DiagSWComponent2();	
		DiagTask_Cnt++;
	}
	else if(DiagTask_Cnt == 2u) // 26us
	{ 			
		DiagSWComponent3();
		DiagTask_Cnt++; 	
	}
	else //27us
	{ 	
		DiagSWComponent4();
		DiagTask_Cnt = 0u; 	
	}
}

static void DiagSWComponent1(void)
{
	DiagOverVoltage();
	DiagUnderVoltage();
	DiagWarningCurrent();
	DiagOverCurrent();	
}

static void DiagSWComponent2(void)
{
	DiagStallError();	
	DiagShortCircuit();
	DiagGateDrvOVUV();
	DiagPhaseOpen(); 	
}

static void DiagSWComponent3(void)
{
	DiagLINError();
	DiagLINInvalidValue();
	DiagSpeedError();
	DiagOverTemperature();
}

static void DiagSWComponent4(void)
{
	DiagLowCurrent();	
	DiagEwpInital();
	MergeDiagState();
	SetDiagFlagState();	
}

static void DiagOverVoltage(void)
{
	//static DiagEntryConditionState_t OV_entryCondition;

	sint16_t CAL_OV_NormalLwrTh 	    = OV_NORMAL_LWR_TH;
	sint16_t CAL_OV_FaultUprTh		    = OV_FAULT_UPR_TH;

	uint8_t CAL_OV_NormalCntMax	    = OV_NORMAL_CNT_MAX;
	uint8_t CAL_OV_FaultCntMax 	    = OV_FAULT_CNT_MAX;
	uint8_t CAL_OV_AttemptCLRCntMax 	= OV_AttemptCLR_CNT_MAX;
	uint8_t CAL_OV_RetryAttemptMax     = OV_RETRY_ATTEMPT_MAX;


    (void)ReadVdcFiltered(&diag_vdcFiltered);


   
    switch (IRV_OV_diagState)
    {
        case NORMAL:
            if(diag_vdcFiltered >= CAL_OV_FaultUprTh) 
            {
                OV_faultCnt++;
                OV_normalCnt = (OV_normalCnt <= 0U) ? 0U : (OV_normalCnt - 1U);
            }
            else
            {
                OV_faultCnt = (OV_faultCnt <= 0U) ? 0U : (OV_faultCnt - 1U);
				if(diag_vdcFiltered <= CAL_OV_NormalLwrTh)
				{
				    OV_normalCnt = (OV_retryAttempt > 0U) ? (OV_normalCnt + 1U) : (0U);
				}
       			
            }

			if(OV_normalCnt > CAL_OV_AttemptCLRCntMax)
			{
				OV_normalCnt = 0U;
				OV_retryAttempt = 0U;
			}

			
            if (OV_faultCnt >= CAL_OV_FaultCntMax)
            {
                OV_faultCnt = 0U;
				OV_normalCnt = 0U;
				OV_retryAttempt++;
                IRV_OV_diagState = FAULT;
				IRV_OV_Faultflag = 1u;
            }
            else
            {
            /* no action */
            }
			
            break;

        case FAULT:
            if (diag_vdcFiltered <= CAL_OV_NormalLwrTh)
            {
                OV_normalCnt++;
            }
            else
            {
                OV_normalCnt = (OV_normalCnt <= 0U) ? 0U : (OV_normalCnt - 1U);
            }      
                      
            if (OV_normalCnt >= CAL_OV_NormalCntMax)
            {
                OV_normalCnt = 0U;
                IRV_OV_diagState = NORMAL;
				IRV_OV_Faultflag = 0u;
            }
            else
            {
            /* no action */
            }
			
			if(OV_retryAttempt >= CAL_OV_RetryAttemptMax)
			{
				IRV_OV_diagState = FAILURE;
				IRV_OV_Faultflag = 1u;
			}
            break;
			
		case FAILURE:
		/* no action */
		break;
     
        default: /*no action */
            break;
    }
}

static void DiagUnderVoltage(void)
{
	//static DiagEntryConditionState_t UV_entryCondition;
	sint16_t CAL_UV_NormalUprTh 	  = UV_NORMAL_UPR_TH;
	sint16_t CAL_UV_FaultLwrTh		  = UV_FAULT_LWR_TH;
	uint8_t CAL_UV_NormalCntMax	  = UV_NORMAL_CNT_MAX;
	uint8_t CAL_UV_FaultCntMax 	  = UV_FAULT_CNT_MAX;
	uint8_t CAL_UV_AttemptCLRCntMax 	= UV_AttemptCLR_CNT_MAX;
	uint8_t CAL_UV_RetryAttemptMax   = UV_RETRY_ATTEMPT_MAX;

    (void)ReadVdcFiltered(&diag_vdcFiltered);

    
    switch (IRV_UV_diagState)
    {
        case NORMAL:
			/* Vdc < 8V */
            if(diag_vdcFiltered < CAL_UV_FaultLwrTh)
            {
                UV_faultCnt++;
       			UV_normalCnt = (UV_normalCnt <= 0U) ? 0U : (UV_normalCnt - 1U);  					
            }
			/* 8V <= Vdc  */
            else
            {
       			UV_faultCnt = (UV_faultCnt <= 0U) ? 0U : (UV_faultCnt - 1U);
				/* 9V <= Vdc  */
       			if(diag_vdcFiltered >= CAL_UV_NormalUprTh)
				{
				    UV_normalCnt = (UV_retryAttempt > 0U) ? (UV_normalCnt + 1U) : (0U);  					            
       			}
            }

			if(UV_normalCnt > CAL_UV_AttemptCLRCntMax)
			{
				UV_normalCnt = 0U;
				UV_retryAttempt = 0U;
			}
			
            if (UV_faultCnt >= CAL_UV_FaultCntMax)
            {
                UV_faultCnt = 0U;
				UV_normalCnt = 0U;
				UV_retryAttempt++;				
                IRV_UV_diagState = FAULT;
				IRV_UV_Faultflag = 1u;
            }
            else
            {
            /* no action */
            }
            break;

        case FAULT:
            if (diag_vdcFiltered >= CAL_UV_NormalUprTh)
            {
                UV_normalCnt++;
            }
            else
            {
       			UV_normalCnt = (UV_normalCnt <= 0U) ? 0U : (UV_normalCnt - 1U);               
            }            
                        
            if (UV_normalCnt >= CAL_UV_NormalCntMax)
            {
                UV_normalCnt = 0U;
                IRV_UV_diagState = NORMAL;
				IRV_UV_Faultflag = 0U;
            }
            else
            {
            /* no action */
            }        

			if(UV_retryAttempt >= CAL_UV_RetryAttemptMax)
			{
				IRV_UV_diagState = FAILURE;
				IRV_UV_Faultflag = 1U;
			}
            break;
		case FAILURE:
		/* no action */
		break;
         
        default: /*no action */
            break;
    }
}

static void DiagLowCurrent(void)
{
	static DiagEntryConditionState_t LC_entryCondition;

	static uint8_t	diag_coolantFlag			  = 0U;
	
	sint16_t CAL_LC_NormalUprTh		    = LC_NORMAL_UPR_TH;
	sint16_t CAL_LC_FaultLwrTh		    = LC_FAULT_LWR_TH;
	sint16_t CAL_LC_FaultCntMax 	    = LC_FAULT_CNT_MAX;
	sint16_t CAL_LC_RetryCntMax 	    = LC_RETRY_CNT_MAX;
	sint16_t CAL_LC_AttemptCLRCntMax 	= LC_AttemptCLR_CNT_MAX;
	sint16_t CAL_LC_SpeedMin		    = LC_SPEED_MIN;
	uint8_t CAL_LC_RetryAttemptMax     = LC_RETRY_ATTEMPT_MAX;

    (void)ReadIqFiltered(&diag_iqFiltered);
    (void)ReadSpeedRef(&diag_speedRef);
    (void)ReadCoolantFlag(&diag_coolantFlag);

    if ((diag_speedRef >= CAL_LC_SpeedMin) && (diag_coolantFlag == 0u) && (IRV_PO_diagState == NORMAL) && (IRV_SC_diagState == NORMAL) && (IRV_GE_diagState == NORMAL))
    {
        LC_entryCondition = ENABLE;
    }
    else
    {
        LC_entryCondition = DISABLE;
    }
    
    switch (IRV_LC_diagState)
    {
        case NORMAL:
            if (((diag_iqFiltered <= CAL_LC_FaultLwrTh)) && (LC_entryCondition == ENABLE) && (diag_vdcFiltered >= 900) && (diag_vdcFiltered <= 1600))
            {
                LC_faultCnt++;
            }
            else
			{
				LC_faultCnt = (LC_faultCnt <= 0) ? 0 : (LC_faultCnt - 1);
			}
			if((diag_iqFiltered >= CAL_LC_NormalUprTh) && (diag_speedRef >= CAL_SpeedMin))
			{
    			LC_normalCnt = (LC_retryAttempt > 0U) ? (LC_normalCnt + 1) : (0);
			}
			else
			{
				LC_normalCnt = (LC_normalCnt <= 0) ? 0 : (LC_normalCnt - 1);
			}
			
			if(LC_normalCnt > CAL_LC_AttemptCLRCntMax)
			{
				LC_normalCnt = 0;
				LC_retryAttempt = 0U;
				IRV_LC_Faultflag = 0U;
			}
			else
			{
                /* no action*/ 
			}
            if (LC_faultCnt >= CAL_LC_FaultCntMax)
            {
                LC_faultCnt = 0;
				LC_normalCnt = 0;
				LC_retryAttempt++;				
                IRV_LC_diagState = FAULT;
				IRV_LC_Faultflag = 1u;
            }
            else
            {
            /* no action */
            }
            break;

        case FAULT:
            LC_retryCnt++;

            if (LC_retryCnt >= CAL_LC_RetryCntMax)
            {
                LC_retryCnt = 0;
                IRV_LC_diagState = NORMAL;
            }

			if(LC_retryAttempt >= CAL_LC_RetryAttemptMax)
			{
				IRV_LC_diagState = FAILURE;
				IRV_LC_Faultflag = 1u;
			}
            

            break;

        case FAILURE:
            /* no action */
            break;
                 
        default: /*no action */
            break;
    }
}

static void DiagWarningCurrent(void)
{

    (void)ReadIqFiltered(&diag_iqFiltered);
    (void)ReadSpeedRef(&diag_speedRef);


    switch (IRV_WC_diagState)
    {
        case NORMAL: DiagWarningCurrentNormal();			
            break;

		case WARNING: DiagWarningCurrentWarning();				
		    break;

        default: /*no action */
            break;
    }
}

static void DiagWarningCurrentNormal(void)
{
	sint16_t CAL_WC_IqWarningUprTh		= WC_IQ_WARNING_UPR_TH;
	uint8_t CAL_WC_WarningCntMax		= WC_WARNING_CNT_MAX;
	if (((diag_iqFiltered > CAL_WC_IqWarningUprTh)) && (diag_vdcFiltered >= 900) && (diag_vdcFiltered <= 1600)
		&& (diag_speedRef >= CAL_SpeedMin) && (IRV_PO_diagState == NORMAL) && (IRV_SC_diagState == NORMAL) && (IRV_GE_diagState == NORMAL))
	{
		WC_WarningCnt++;
	}
	else                                                                     //  iq 14A, WC Flag Clear Period
	{
    	WC_WarningCnt = (WC_WarningCnt <= 0U) ? 0U : (WC_WarningCnt - 1U);
	}
	
	if (WC_WarningCnt >= CAL_WC_WarningCntMax)                                    //WC transition
	{
		WC_WarningCnt = 0U;
		IRV_WC_diagState = WARNING;
		IRV_WC_Warningflag = 1u;

	}
	
}

static void DiagWarningCurrentWarning(void)
{
	uint8_t CAL_WC_NormalCntMax		= WC_NORMAL_CNT_MAX;
	sint16_t CAL_WC_IqWarningLwrTh		= WC_IQ_WARNING_LWR_TH;

	if ((diag_iqFiltered <= CAL_WC_IqWarningLwrTh) 
		&& (diag_speedRef >= CAL_SpeedMin) && (IRV_PO_diagState == NORMAL) && (IRV_SC_diagState == NORMAL) && (IRV_GE_diagState == NORMAL))
	{
		WC_normalCnt++;
	}
	else if((diag_iqFiltered > CAL_WC_IqWarningLwrTh) 
		&& (diag_speedRef >= CAL_SpeedMin) && (IRV_PO_diagState == NORMAL) && (IRV_SC_diagState == NORMAL) && (IRV_GE_diagState == NORMAL))
	{
		WC_normalCnt = (WC_normalCnt <= 0U) ? 0U : (WC_normalCnt - 1U);							
	}
	else
	{
	 /* no action */
	}

	if (WC_normalCnt >= CAL_WC_NormalCntMax)
	{
		WC_normalCnt = 0U;
		IRV_WC_Warningflag = 0u;
		IRV_WC_diagState = NORMAL;
	}
}

static void DiagOverCurrent(void)
{

    (void)ReadIqFiltered(&diag_iqFiltered);
    (void)ReadSpeedRef(&diag_speedRef);

   

    switch (IRV_OC_diagState)
    {
        case NORMAL: DiagOverCurrentNormal();			
            break;

        case FAULT: DiagOverCurrentFault();
            break;

        case FAILURE:
            /* no action */
            break;

        default: /*no action */
            break;
    }
}

static void DiagOverCurrentNormal(void)
{
    static sint16_t OC_AttemptCLRCnt              = 0;
    static const uint8_t CAL_OC_FaultCntMax 		= OC_FAULT_CNT_MAX;
    static const sint16_t CAL_OC_IqFaultUprTh		= OC_IQ_FAULT_UPR_TH;	
    static const sint16_t CAL_OC_AttemptCLRCntMax	= OC_AttemptCLR_CNT_MAX;
    if((diag_iqFiltered >= CAL_OC_IqFaultUprTh) && (diag_vdcFiltered >= 900) && (diag_vdcFiltered <= 1600)
		&& (diag_speedRef >= CAL_SpeedMin) && (IRV_PO_diagState == NORMAL) && (IRV_SC_diagState == NORMAL) && (IRV_GE_diagState == NORMAL))
	{
	    OC_faultCnt++;
	}
    else
    {
    	OC_faultCnt = (OC_faultCnt <= 0U) ? (0U) : (OC_faultCnt - 1U);
	}
	if((diag_iqFiltered < CAL_OC_IqFaultUprTh)
		&& (diag_speedRef >= CAL_SpeedMin) && (IRV_PO_diagState == NORMAL) && (IRV_SC_diagState == NORMAL) && (IRV_GE_diagState == NORMAL))
	{
        OC_AttemptCLRCnt = (OC_retryAttempt > 0U) ? (OC_AttemptCLRCnt + 1) : (0);
	}
	else
	{
    	OC_AttemptCLRCnt = (OC_AttemptCLRCnt <= 0) ? 0 : (OC_AttemptCLRCnt - 1);
	}

	if (OC_AttemptCLRCnt >= CAL_OC_AttemptCLRCntMax)
	{
		OC_AttemptCLRCnt = 0;
		OC_retryAttempt = 0U;
		IRV_OC_Faultflag = 0U;
		IRV_OC_diagState = NORMAL;
	}

	
	if (OC_faultCnt >= CAL_OC_FaultCntMax)
	{
		OC_faultCnt = 0U;
		OC_AttemptCLRCnt = 0;
		IRV_OC_Faultflag = 1u;
		OC_retryAttempt++;
		IRV_OC_diagState = FAULT;
//		IRV_WC_Warningflag = 0u;
	}
}

static void DiagOverCurrentFault(void)
{
	uint8_t CAL_OC_RetryAttemptMax	= OC_RETRY_ATTEMPT_MAX;
	uint8_t CAL_OC_RetryCntMax		= OC_RETRY_CNT_MAX;

	OC_retryCnt++;
	
	if (OC_retryCnt >= CAL_OC_RetryCntMax)
	{
		OC_retryCnt = 0U;
		IRV_OC_diagState = NORMAL;
	}

	if (OC_retryAttempt >= CAL_OC_RetryAttemptMax)
	{
		IRV_OC_diagState = FAILURE; /* OC_Faultflag == 1U */
	}	  

}


static void DiagShortCircuit (void)
{
	static DiagState_t diag_hwShortCircuitState   = NORMAL;

	uint8_t CAL_SC_NormalCntMax	  = SC_NORMAL_CNT_MAX;
	uint8_t CAL_SC_FaultCntMax 	  = SC_FAULT_CNT_MAX;
	uint8_t CAL_SC_RetryCntMax 	  = SC_RETRY_CNT_MAX;
	uint8_t CAL_SC_RetryAttemptMax   = SC_RETRY_ATTEMPT_MAX;

    (void)ReadHwShortCircuitState(&diag_hwShortCircuitState);
    (void)ReadSpeedRef(&diag_speedRef);


    
    switch (IRV_SC_diagState)
    {
        case NORMAL:
             if ((diag_hwShortCircuitState == FAULT) 
			 	&& (diag_speedRef >= CAL_SpeedMin) && (diag_vdcFiltered >= 900) && (diag_vdcFiltered <= 1600))
             {
                 SC_faultCnt++;
             }
             else if ((diag_hwShortCircuitState == NORMAL) 
			 	|| (diag_speedRef < CAL_SpeedMin) || (diag_vdcFiltered < 900) || (diag_vdcFiltered > 1600))
             {
                 SC_faultCnt = (SC_faultCnt <= 0U) ? 0U : (SC_faultCnt - 1U); 
             }
             else
             {
                 /* no action */
             }
        
             if (SC_faultCnt >= CAL_SC_FaultCntMax)
             {
                 SC_faultCnt = 0U;
                 IRV_SC_diagState = FAULT;
             }
             else
             {
             /* no action */
             }
             break;
        
         case FAULT:
             SC_retryCnt++;
             
             if (SC_retryAttempt >= CAL_SC_RetryAttemptMax)
             {
                 IRV_SC_diagState = FAILURE;
             }
             else
             {
             /* no action */
             }
        
             if (SC_retryCnt >= CAL_SC_RetryCntMax)
             {
                 SC_retryCnt = 0U;
                 SC_retryAttempt++;
                 IRV_SC_diagState = RETRY;
             }
             else
             {
             /* no action */
             }            
             break;
    
         case FAILURE:
             /* no action */
             break;

       	 case RETRY:
             if ((diag_hwShortCircuitState == FAULT)
			 	&& (diag_speedRef >= CAL_SpeedMin) && (diag_vdcFiltered >= 900) && (diag_vdcFiltered <= 1600))
             {
                 SC_faultCnt++;
				 SC_normalCnt = (SC_normalCnt <= 0U) ? 0U : (SC_normalCnt - 1U);                  
             }
             else if ((diag_hwShortCircuitState == NORMAL)
			 	|| (diag_speedRef < CAL_SpeedMin) || (diag_vdcFiltered < 900) || (diag_vdcFiltered > 1600))
             {
                 SC_normalCnt++;
				 SC_faultCnt = (SC_faultCnt <= 0U) ? 0U : (SC_faultCnt - 1U);
             }
             else
             {
             /* no action */
             }

             if(SC_faultCnt >= CAL_SC_FaultCntMax)
             {
                 SC_faultCnt = 0U;
                 SC_normalCnt = 0U;
                 IRV_SC_diagState = FAULT;
             }
             else
             {
             /* no action */
             }

             if (SC_normalCnt >= CAL_SC_NormalCntMax)
             {
                 SC_faultCnt = 0U;
                 SC_normalCnt = 0U;
                 SC_retryAttempt = 0U;
                 IRV_SC_diagState = NORMAL;
             }
             else
             {
             /* no action */
             }
             break;
             
         default: /*no action */
             break;
    }
}


static void DiagGateDrvOVUV(void)
{
	static DiagState_t diag_hwGateDrvErrorState   = NORMAL;
	uint8_t CAL_GE_NormalCntMax	  = GE_NORMAL_CNT_MAX;
	uint8_t CAL_GE_FaultCntMax 	  = GE_FAULT_CNT_MAX;
	uint8_t CAL_GE_RetryCntMax 	  = GE_RETRY_CNT_MAX;
	uint8_t CAL_GE_RetryAttemptMax   = GE_RETRY_ATTEMPT_MAX;

    (void)ReadHwGateDrvErrorState(&diag_hwGateDrvErrorState);
    (void)ReadSpeedRef(&diag_speedRef);

    switch (IRV_GE_diagState)
    {
        case NORMAL:
            if ((diag_hwGateDrvErrorState == FAULT) 
				&& (diag_speedRef >= CAL_SpeedMin) && (IRV_SC_diagState == NORMAL) && (IRV_PO_diagState == NORMAL) && (diag_vdcFiltered >= 900) && (diag_vdcFiltered <= 1600))
            {
                GE_faultCnt++;
            }
            else if ((diag_hwGateDrvErrorState == NORMAL) 
				|| (diag_speedRef < CAL_SpeedMin) || (IRV_SC_diagState != NORMAL) || (IRV_PO_diagState != NORMAL) || (diag_vdcFiltered < 900) || (diag_vdcFiltered > 1600))
            {
                GE_faultCnt = (GE_faultCnt <= 0U) ? 0U : (GE_faultCnt - 1U);
            }
            else
            {
            /* no action */
            }
           

            if (GE_faultCnt >= CAL_GE_FaultCntMax)
            {
                GE_faultCnt = 0U;
                IRV_GE_diagState = FAULT;
            }
            else
            {
            /* no action */
            }
            break;

        case FAULT:
             GE_retryCnt++;
             
             if (GE_retryAttempt >= CAL_GE_RetryAttemptMax)
             {
                 IRV_GE_diagState = FAILURE;
             }
             else
             {
             /* no action */
             }
        
             
             if (GE_retryCnt >= CAL_GE_RetryCntMax)
             {
                 GE_retryCnt = 0U;
                 GE_retryAttempt++;
                 IRV_GE_diagState = RETRY;
             }
             else
             {
             /* no action */
             }
             break;

        case FAILURE:
            /* no action */
            break;

        case RETRY:
            if ((diag_hwGateDrvErrorState == FAULT)
				&& (diag_speedRef >= CAL_SpeedMin) && (IRV_SC_diagState == NORMAL) && (IRV_PO_diagState == NORMAL) && (diag_vdcFiltered >= 900) && (diag_vdcFiltered <= 1600))
            {
                GE_faultCnt++;
                GE_normalCnt = (GE_normalCnt <= 0U) ? 0U : (GE_normalCnt - 1U);

            }
            else if ((diag_hwGateDrvErrorState == NORMAL)
				|| (diag_speedRef < CAL_SpeedMin) || (IRV_SC_diagState != NORMAL) || (IRV_PO_diagState != NORMAL) || (diag_vdcFiltered < 900) || (diag_vdcFiltered > 1600))
            {
                GE_normalCnt++;
                GE_faultCnt = (GE_faultCnt <= 0U) ? 0U : (GE_faultCnt - 1U);

            }
            else
            {
            /* no action */
            }

            if (GE_faultCnt >= CAL_GE_FaultCntMax)
            {
                GE_faultCnt = 0U;
                GE_normalCnt = 0U;
                IRV_GE_diagState = FAULT;
            }
            else
            {
            /* no action */
            }
        
            if (GE_normalCnt >= CAL_GE_NormalCntMax)
            {
                GE_faultCnt = 0U;
                GE_normalCnt = 0U;
                GE_retryAttempt = 0U;
                IRV_GE_diagState = NORMAL;
            }
            else
            {
            /* no action */
            }
            break;
     
        default: /*no action */
            break;
    }
}

static void DiagLINError(void)
{
	static DiagState_t diag_linTimeOutErrorState   = NORMAL;


    (void)ReadLinTimeOutErrorState(&diag_linTimeOutErrorState);
    (void)ReadIgnFlag(&diag_ignState);

    switch (IRV_LE_diagState)
    {
        case NORMAL:
            if ((diag_linTimeOutErrorState == FAULT) 
				&& (diag_vdcFiltered >= 900) && (diag_vdcFiltered <= 1600) && (diag_ignState == IGN_ON))
            {
                IRV_LE_diagState = WARNING;
            }
            else
            {
            /* no action */
            }
            
            break;

        case WARNING:      
            if (diag_linTimeOutErrorState == NORMAL)
            {
                IRV_LE_diagState = NORMAL;
            }
            else
            {
            /* no action */
            }
            break;
     
        default: /*no action */
            break;
    }
}

static void DiagSpeedError(void)
{
	sint16_t diag_speedAct				  = 0;
    (void)ReadVdcFiltered(&diag_vdcFiltered);
    (void)ReadSpeedRef(&diag_speedRef);
    (void)ReadSpeedFiltered(&diag_speedAct);
	if(diag_speedRef>=diag_speedAct)
	{
    	diag_speedError = diag_speedRef - diag_speedAct;
	}
    else
    {
    	diag_speedError = diag_speedAct - diag_speedRef;
	}
     
    switch (IRV_SE_diagState)
    {
        case NORMAL: DiagSpeedErrorNormal();
            break;

        case WARNING: DiagSpeedErrorWarning();
            break;

        case FAULT: DiagSpeedErrorFault();
            break;

        case FAILURE:
            /* no action */
            break;

        default: /*no action */
            break;
    }
}
static void DiagSpeedErrorNormal(void)
{
	 sint16_t CAL_SE_WarningCntMax		 = SE_WARNING_CNT_MAX;
     sint16_t CAL_SE_AttemptCLRCntMax = SE_AttemptCLR_CNT_MAX;

     /* 980 < Error < 1340*/
	  if (((diag_speedError > CAL_SE_WarningLwrTh)&&(diag_speedError < CAL_SE_FaultUprTh)) && (diag_vdcFiltered >= 900) && (diag_vdcFiltered <= 1600)
	  	&& (diag_speedRef >= CAL_SpeedMin) && (IRV_SC_diagState == NORMAL) && (IRV_GE_diagState == NORMAL) && (IRV_OC_diagState == NORMAL))
	  {
		  SE_WarningCnt++;
		  SE_faultCnt = 0;
		  SE_normalCnt = 0;
	  }
     /* 1340 <= Error */
	  else if ((diag_speedError >= CAL_SE_FaultUprTh) && (diag_vdcFiltered >= 900) && (diag_vdcFiltered <= 1600)
	  	&& (diag_speedRef >= CAL_SpeedMin) && (IRV_SC_diagState == NORMAL) && (IRV_GE_diagState == NORMAL) && (IRV_OC_diagState == NORMAL))
	  {
		  SE_faultCnt++;
		  SE_WarningCnt = 0;
		  SE_normalCnt = 0;
	  }
	 /* Error <= 140 */
	  else if((diag_speedError <= CAL_SE_WarningLwrTh) 
	  	&& (diag_speedRef >= CAL_SpeedMin) && (IRV_SC_diagState == NORMAL) && (IRV_GE_diagState == NORMAL) && (IRV_OC_diagState == NORMAL))
	  {
		  SE_normalCnt = (SE_retryAttempt > 0U) ? (SE_normalCnt + 1) : (0);
		  SE_faultCnt = 0;
		  SE_WarningCnt = 0;
	  }
	  else
	  {
    	   /* no action */
	  }
		
	if (SE_WarningCnt >= CAL_SE_WarningCntMax)
	{
		SE_WarningCnt = 0;
		IRV_SE_diagState = WARNING;
		IRV_SE_Warningflag = 1u;
	}
	else
	{
	 /* no action */
	}
	 
	if (SE_faultCnt >= CAL_SE_FaultCntMax)
	{
		SE_faultCnt = 0;
		SE_retryAttempt++;
		IRV_SE_diagState = FAULT;
		IRV_SE_Faultflag = 1u;
	}
	else
	{
	 /* no action */
	}
	 
	if (SE_normalCnt >= CAL_SE_AttemptCLRCntMax)
	{
		SE_normalCnt = 0;
		SE_retryAttempt = 0U;
		IRV_SE_Faultflag = 0U;
	}
	else
	{
	 /* no action */
	}

}

static void DiagSpeedErrorWarning(void)
{
	sint16_t CAL_SE_NormalCntMax 	= SE_NORMAL_CNT_MAX;
     /* 1340 <= Error */    
	if ((diag_speedError >= CAL_SE_FaultUprTh) && (diag_vdcFiltered >= 900) && (diag_vdcFiltered <= 1600)
		&& (diag_speedRef >= CAL_SpeedMin) && (IRV_SC_diagState == NORMAL) && (IRV_GE_diagState == NORMAL) && (IRV_OC_diagState == NORMAL))
	{
		SE_faultCnt++;
		SE_normalCnt = 0;
	}
    /*  Error < 940 */    	
	else if((diag_speedError < CAL_SE_WarningLwrTh)
		&& (diag_speedRef >= CAL_SpeedMin) && (IRV_SC_diagState == NORMAL) && (IRV_GE_diagState == NORMAL) && (IRV_OC_diagState == NORMAL))
	{
		SE_normalCnt++;
        SE_faultCnt = 0;
	}
	else
	{
		SE_normalCnt = 0;
        SE_faultCnt = 0;	
	}
	
	if(SE_normalCnt >= CAL_SE_NormalCntMax)
	{
		SE_normalCnt = 0;
		IRV_SE_Warningflag = 0U;
		IRV_SE_diagState = NORMAL;
	}
	else
	{
	/* no action */
	}
	
	if (SE_faultCnt >= CAL_SE_FaultCntMax)
	{
		SE_faultCnt = 0;
		SE_normalCnt = 0;
		SE_retryAttempt++;
		IRV_SE_diagState = FAULT;
		IRV_SE_Faultflag = 1u;
		IRV_SE_Warningflag = 0U;
	}
	else
	{
	/* no action */
	}

}

static void DiagSpeedErrorFault(void)
{
	uint8_t CAL_SE_RetryCntMax		= SE_RETRY_CNT_MAX;
	uint8_t CAL_SE_RetryAttemptMax	= SE_RETRY_ATTEMPT_MAX;

	SE_retryCnt++;
	
	if (SE_retryCnt >= CAL_SE_RetryCntMax)
	{
		SE_retryCnt = 0U;
		IRV_SE_diagState = NORMAL;
	}
	else
	{
	/* no action */
	}
	
	if (SE_retryAttempt >= CAL_SE_RetryAttemptMax)
	{
		IRV_SE_diagState = FAILURE; /* IRV_SE_Faultflag = 1u; */
	}
	else
	{
	/* no action */
	}

}


static void DiagOverTemperature (void)
{
    (void)ReadTemperatureFiltered (&diag_temperatureFiltered);
 
    switch (IRV_OT_diagState)
    {
        case NORMAL: DiagOverTemperatureNormal();
            break;

        case WARNING: DiagOverTemperatureWarning();
            break;

        case FAULT:DiagOverTemperatureFault();
            break;

			case FAILURE:
            /* no action */
            break;
        default: /*no action */
            break;
    }
}
static void DiagOverTemperatureNormal(void)
{
	static const sint16_t CAL_OT_WarningUprTh		= OT_WARNING_UPR_TH;
	static const uint8_t CAL_OT_FaultCntMax 		= OT_FAULT_CNT_MAX;

    /* 140 <=  Temp < 150 , Warning State */
	if ((diag_temperatureFiltered >= CAL_OT_WarningUprTh) && (diag_temperatureFiltered < CAL_OT_FaultUprTh) && (diag_vdcFiltered >= 900) && (diag_vdcFiltered <= 1600)
        && (IRV_SC_diagState == NORMAL) && (IRV_GE_diagState == NORMAL) && (IRV_PO_diagState == NORMAL)) // WT Diagnostic condition
	{
		OT_warningCnt++;
	}
	else
	{
    	OT_warningCnt = (OT_warningCnt <= 0U) ? 0U : (OT_warningCnt - 1U);
	}
	/*  Temp >= 150, Fault */
	if((diag_temperatureFiltered >= CAL_OT_FaultUprTh) && (diag_vdcFiltered >= 900) && (diag_vdcFiltered <= 1600)
		&& (IRV_SC_diagState == NORMAL) && (IRV_GE_diagState == NORMAL) && (IRV_PO_diagState == NORMAL))
	{ 
		OT_faultCnt++;
	}
	else
	{
    	OT_faultCnt = (OT_faultCnt <= 0U) ? 0U : (OT_faultCnt - 1U);
	}
	
	if (OT_warningCnt >= CAL_OT_WarningCntMax)                                             // WT Transition
	{
		OT_warningCnt = 0U;
		IRV_OT_diagState = WARNING;
		IRV_OT_Warningflag = 1u;
	}
	else
	{
	/* no action */
	}
	
    if (OT_faultCnt >= CAL_OT_FaultCntMax)
	{
		OT_faultCnt = 0U;
		OT_normalCnt = 0U;
		IRV_OT_diagState = FAULT;
		IRV_OT_Faultflag = 1u;
		IRV_OT_Warningflag = 0U;
	}
	else
	{
	/* no action */
	}

}
static void DiagOverTemperatureWarning(void)
{
	static const uint8_t CAL_OT_FaultCntMax 		= OT_FAULT_CNT_MAX;

    /*  Temp >= 150, Fault */
	if ((diag_temperatureFiltered >= CAL_OT_FaultUprTh)  && (diag_vdcFiltered >= 900) && (diag_vdcFiltered <= 1600)
		&& (IRV_SC_diagState == NORMAL) && (IRV_GE_diagState == NORMAL) && (IRV_PO_diagState == NORMAL))
	{
    	OT_normalCnt = (OT_normalCnt <= 0U) ? 0U : (OT_normalCnt - 1U);
    	OT_faultCnt++;
	}
	/* 140 <=  Temp < 150 , Warning State */	
	else if((diag_temperatureFiltered > CAL_OT_NormalLwrTh) && (diag_temperatureFiltered < CAL_OT_FaultUprTh))
	{
    	OT_normalCnt = (OT_normalCnt <= 0U) ? 0U : (OT_normalCnt - 1U);
    	OT_faultCnt = (OT_faultCnt <= 0U) ? 0U : (OT_faultCnt - 1U);
	}
    /*  Temp <= 130, Normal State */
	else if(diag_temperatureFiltered <= CAL_OT_NormalLwrTh)
	{
    	OT_normalCnt++;
    	OT_faultCnt = (OT_faultCnt <= 0U) ? 0U : (OT_faultCnt - 1U);

	}
	else
	{
	/* no action */
	}		
	
	if (OT_normalCnt >= CAL_OT_NormalCntMax)
	{
		OT_faultCnt = 0U;
		OT_normalCnt = 0U;
		IRV_OT_diagState = NORMAL;
		IRV_OT_Warningflag = 0u;
	}
	else
	{
	/* no action */
	}
		
	if (OT_faultCnt >= CAL_OT_FaultCntMax)
	{
		OT_faultCnt = 0U;
		OT_normalCnt = 0U;
		IRV_OT_diagState = FAULT;
		IRV_OT_Faultflag = 1u;
		IRV_OT_Warningflag = 0U;
	}
	else
	{
	/* no action */
	}		

}
static void DiagOverTemperatureFault(void)
{
	static const sint16_t CAL_OT_WarningLwrTh		= OT_WARNING_LWR_TH;


    /* 130 >= Temp, Normal State */
	if (diag_temperatureFiltered <= CAL_OT_NormalLwrTh)
	{
		OT_normalCnt++;
		OT_warningCnt = (OT_warningCnt <= 0U) ? 0U : (OT_warningCnt - 1U); 				
	}		
    /* 130 < Temp <= 140, Warning State*/
	else if (diag_temperatureFiltered <= CAL_OT_WarningLwrTh)
	{
		OT_warningCnt++;
		OT_normalCnt = (OT_normalCnt <= 0U) ? 0U : (OT_normalCnt - 1U);				
	}
    /* 140 < Temp, Fault State*/	
	else
	{
		OT_warningCnt = (OT_warningCnt <= 0U) ? 0U : (OT_warningCnt - 1U); 			
		OT_normalCnt = (OT_normalCnt <= 0U) ? 0U : (OT_normalCnt - 1U); 
	}											
	
	if (OT_normalCnt >= CAL_OT_NormalCntMax)
	{
		OT_normalCnt = 0U;
		OT_warningCnt = 0U;
		IRV_OT_diagState = NORMAL;
		IRV_OT_Warningflag = 0u;
		IRV_OT_Faultflag = 0u;
	}
	else
	{
	/* no action */
	}
			
	if (OT_warningCnt >= CAL_OT_WarningCntMax)
	{
		OT_normalCnt = 0U;
		OT_warningCnt = 0U;
		IRV_OT_diagState = WARNING;
		IRV_OT_Warningflag = 1u;
		IRV_OT_Faultflag = 0u;
	}
	else
	{
	/* no action */
	}		  
}

static void DiagStallError(void)
{
    (void)ReadSpeedRef(&diag_speedRef);

    switch (IRV_ST_diagState)
    {
        case NORMAL: DiagStallErrorNormal();			
            break;

		case WARNING:		
		    break;

        case FAULT: DiagStallErrorFault();
            break;

        case FAILURE:
            /* no action */
            break;

        default: /*no action */
            break;
    }
}

static void DiagStallErrorNormal(void)
{
	static const sint16_t CAL_ST_FluxAbsFaultUprTh = ST_FLUXABS_FAULT_UPR_TH;
	static const uint8_t CAL_ST_FaultCntMax		     = ST_FAULT_CNT_MAX;
	static const uint8_t CAL_ST_AttemptCLRCntMax	= ST_AttemptCLR_CNT_MAX;
	sint16_t diag_fluxAbsValueMeasured	  = 0;
	(void)ReadFluxAbsValueMeasured(&diag_fluxAbsValueMeasured);

	if ((diag_fluxAbsValueMeasured <= CAL_ST_FluxAbsFaultUprTh) && ((uint16_t)diag_vdcFiltered >= 900u) && ((uint16_t)diag_vdcFiltered <= 1600u)
		&& (diag_speedRef >= CAL_SpeedMin) && (IRV_PO_diagState == NORMAL))
	{
		ST_FaultCnt++;
		ST_NormalCnt = (ST_NormalCnt <= 0U) ? 0U : (ST_NormalCnt - 1U);	
	}
	else                                                                         
	{
        ST_FaultCnt = (ST_FaultCnt <= 0U) ? 0U : (ST_FaultCnt - 1U);	
		ST_NormalCnt = (ST_retryAttempt > 0U) ? (ST_NormalCnt + 1U) : (0U);
	}
	
	if (ST_FaultCnt >= CAL_ST_FaultCntMax)                                    
	{
		ST_FaultCnt = 0U;
        ST_NormalCnt = 0U;
		IRV_ST_diagState = FAULT;
		ST_retryAttempt = ST_retryAttempt + 1u;
		IRV_ST_Faultflag = 1u;

	}	
	
	if (ST_NormalCnt >= CAL_ST_AttemptCLRCntMax)                              
	{
		ST_NormalCnt = 0U;
		ST_retryAttempt = 0U;
		IRV_ST_Faultflag = 0U;
		
	}
}

static void DiagStallErrorFault(void)
{
	static const uint8_t CAL_ST_RetryAttemptMax	= ST_RETRY_ATTEMPT_MAX;
	static const uint8_t CAL_ST_RetryCntMax		= ST_RETRY_CNT_MAX;

	ST_retryCnt++;
	
	if (ST_retryCnt >= CAL_ST_RetryCntMax)
	{
		ST_retryCnt = 0U;
		IRV_ST_diagState = NORMAL;
	}

	if (ST_retryAttempt >= CAL_ST_RetryAttemptMax)
	{
		IRV_ST_diagState = FAILURE;
		IRV_ST_Faultflag = 1U;
	}	  
	

}
//static void DiagPowerLimit(void)
//{
//	static DiagEntryConditionState_t PL_entryCondition;
//	static uint8_t diag_PowerLimiteState   = 0;

//	static sint8_t normalCnt = 0;
//	static sint8_t LimitCnt = 0;
//	sint16_t CAL_Volt_LwrTh 	    = PL_VOLT_LWR_TH;
//	sint16_t CAL_Volt_UprTh		    = PL_VOLT_UPR_TH;
//	sint16_t CAL_PL_NormalCntMax	  = PL_NORMAL_CNT_MAX;
//	sint16_t CAL_PL_WarningCntMax 	  = PL_WARNING_CNT_MAX;

//    (void)ReadPowerLimit(&diag_PowerLimiteState);
//    (void)ReadVdcFiltered(&diag_vdcFiltered);

//    if ((IRV_OV_diagState == NORMAL) && (IRV_UV_diagState == NORMAL) && (diag_vdcFiltered > CAL_Volt_LwrTh ) && (diag_vdcFiltered < CAL_Volt_UprTh))
//    {
//        PL_entryCondition = ENABLE;
//    }
//    else
//    {
//        PL_entryCondition = DISABLE;
//    }

//	switch (IRV_PL_diagState)
//    {
//        case NORMAL:
//            if ((diag_PowerLimiteState == 1u) && (PL_entryCondition == ENABLE))
//            {
//                LimitCnt++;
//            }
//            else
//            {
//            	LimitCnt--;	
//            }

//			LimitCnt = (LimitCnt <= 0) ?  0 : LimitCnt;

//			if(LimitCnt > CAL_PL_WarningCntMax)
//			{
//				IRV_PL_diagState = WARNING;
//				LimitCnt = 0;
//			}

//			
//            break;

//        case WARNING:      
//			if ((diag_PowerLimiteState == 0u) || (diag_vdcFiltered >= CAL_Volt_UprTh))
//            {
//                normalCnt++;
//            }
//            else
//            {
//            	normalCnt--;
//            }

//			normalCnt = (normalCnt <= 0) ? 0 : normalCnt;

//			if(normalCnt > CAL_PL_NormalCntMax)
//			{
//				IRV_PL_diagState = NORMAL;
//				normalCnt = 0;
//			}
//			
//            break;
//     
//        default: 
//            break;
//    }
//}
static void DiagLINInvalidValue(void)
{
	static DiagEntryConditionState_t LIV_entryCondition;
	uint8_t diag_linRequireDuty = 0U;

	(void)ReadLinRequireDuty(&diag_linRequireDuty);
    (void)ReadIgnFlag(&diag_ignState);


    if (((uint16_t)diag_vdcFiltered >= 900u) && ((uint16_t)diag_vdcFiltered <= 1600u) && (diag_ignState == IGN_ON))
    {
        LIV_entryCondition = ENABLE;
    }
    else
    {
        LIV_entryCondition = DISABLE;
    }
    
    switch (IRV_LIV_diagState)
    {
        case NORMAL:
            if ((diag_linRequireDuty >= 101U) && (LIV_entryCondition == ENABLE))
            {
                LIV_faultCnt = (LIV_faultCnt < 400U) ? (LIV_faultCnt + 1u) : 400u;
            }
            else
            {
    			LIV_faultCnt = (LIV_faultCnt < 1U) ? 0u : (LIV_faultCnt - 1u);

            }
			if(LIV_faultCnt >= 400u)
			{
    			IRV_LIV_diagState = WARNING;
				  IRV_LIV_Warningflag = 1u;
    			LIV_faultCnt = 0u;
			}

            break;

        case WARNING:      
            if ((diag_linRequireDuty <= 100U))
            {
                IRV_LIV_diagState = NORMAL;
							  IRV_LIV_Warningflag = 0u;
            }
            else
            {
            /* no action */
            }
            break;
     
        default: /*no action */
            break;
    }
}


static void DiagPhaseOpen(void)
{
	sint16_t CAL_PO_NormalCntMax	  = PO_NORMAL_CNT_MAX;
	sint16_t CAL_PO_FaultCntMax 	  = PO_FAULT_CNT_MAX;
	uint8_t CAL_PO_RetryCntMax 	  = PO_RETRY_CNT_MAX;
	uint8_t CAL_PO_RetryAttemptMax   = PO_RETRY_ATTEMPT_MAX;
    DiagState_t diag_hwOpenPhaseState      = NORMAL;
    (void)ReadHwOpenPhaseState(&diag_hwOpenPhaseState);
    
    switch (IRV_PO_diagState)
    {
        case NORMAL:
            if ((diag_hwOpenPhaseState == FAULT)
				&& (diag_vdcFiltered >= 900) && (diag_vdcFiltered <= 1600)) /* detected 2 Phase Open */
            {
                PO_faultCnt++;
            }
            else if ((diag_hwOpenPhaseState == NORMAL) 
				|| (diag_vdcFiltered < 900) || (diag_vdcFiltered > 1600))
            {
       			PO_faultCnt = (PO_faultCnt <= 0) ? 0 : (PO_faultCnt - 1);  	                 
            }
            else
            {
            /* no action */
            }
           

            if (PO_faultCnt >= CAL_PO_FaultCntMax)
            {
                PO_faultCnt = 0;
                IRV_PO_diagState = FAULT;
            }
            else
            {
            /* no action */
            }
            break;

        case FAULT:
             PO_retryCnt++;
             
             if (PO_retryAttempt >= CAL_PO_RetryAttemptMax)
             {
                 IRV_PO_diagState = FAILURE;
             }
             else
             {
             /* no action */
             }
        
             
             if (PO_retryCnt >= CAL_PO_RetryCntMax)
             {
                 PO_retryCnt = 0U;
                 PO_retryAttempt++;
                 IRV_PO_diagState = RETRY;
             }
             else
             {
             /* no action */
             }
             break;

        case FAILURE:
            /* no action */
            break;

        case RETRY:
            if ((diag_hwOpenPhaseState == FAULT)
				&& (diag_vdcFiltered >= 900) && (diag_vdcFiltered <= 1600)) /* detected 2 Phase Open */
            {
                PO_faultCnt++;
       			PO_normalCnt = (PO_normalCnt <= 0) ? 0 : (PO_normalCnt - 1);  					
            }
            else if ((diag_hwOpenPhaseState == NORMAL)
				|| (diag_vdcFiltered < 900) || (diag_vdcFiltered > 1600))
            {
                PO_normalCnt++;
       			PO_faultCnt = (PO_faultCnt <= 0) ? 0 : (PO_faultCnt - 1);  	 				
            }
            else
            {
            /* no action */
            }

            if (PO_faultCnt >= CAL_PO_FaultCntMax)
             {
                 PO_faultCnt = 0;
                 PO_normalCnt = 0;
                 IRV_PO_diagState = FAULT;
             }
             else
             {
             /* no action */
             }

             if (PO_normalCnt >= CAL_PO_NormalCntMax)
             {
                 PO_faultCnt = 0;
                 PO_normalCnt = 0;
                 PO_retryAttempt = 0U;
                 IRV_PO_diagState = NORMAL;
             }
             else
             {
             /* no action */
             }
             break;
     
        default: /*no action */
            break;
    }
}


static void DiagEwpInital(void)
{
    uint8_t diag_InitialFlag = 0U;
	(void)ReadInitialFlag(&diag_InitialFlag);

	if((diag_InitialFlag == 1u))
	{
        /**** Under Voltage Clear ****/
		UV_faultCnt = 0U; UV_retryAttempt = 0U; UV_normalCnt = 0U; IRV_UV_diagState = NORMAL; IRV_UV_Faultflag = 0U;
	    /**** Over Voltage Clear ****/
		OV_faultCnt = 0U; OV_retryAttempt = 0U; OV_normalCnt = 0U; IRV_OV_diagState = NORMAL; IRV_OV_Faultflag = 0U;
	    /**** Over Temperature Clear ****/
		OT_faultCnt = 0U; OT_warningCnt =0U; OT_normalCnt = 0U; IRV_OT_diagState = NORMAL; IRV_OT_Warningflag = 0U; IRV_OT_Faultflag = 0U;
	    /**** Short Circuit Clear ****/		
		SC_faultCnt = 0U; SC_retryCnt = 0U; SC_normalCnt = 0U; SC_retryAttempt = 0U; IRV_SC_diagState = NORMAL; 
	    /**** GateDrv Error Clear ****/				
		GE_faultCnt = 0U; GE_retryCnt = 0U; GE_normalCnt = 0U; GE_retryAttempt = 0U; IRV_GE_diagState = NORMAL;
	    /**** Speed Error Clear ****/				
		SE_faultCnt = 0; SE_retryCnt = 0U; SE_WarningCnt = 0; SE_normalCnt = 0; SE_retryAttempt = 0U; IRV_SE_diagState = NORMAL; IRV_SE_Warningflag = 0U; IRV_SE_Faultflag = 0U;
	    /**** Over Current Clear ****/		
		OC_faultCnt = 0U; OC_retryCnt = 0U; OC_retryAttempt = 0U;  IRV_OC_diagState = NORMAL; IRV_OC_Faultflag = 0U; 
	    /**** Warning Current Clear ****/		
		WC_WarningCnt = 0U; WC_normalCnt = 0U; IRV_WC_Warningflag = 0U;  IRV_WC_diagState = NORMAL;
	    /**** Low Current Clear ****/		
		LC_faultCnt = 0; LC_retryCnt = 0; LC_retryAttempt = 0U; LC_normalCnt = 0; IRV_LC_diagState = NORMAL; IRV_LC_Faultflag = 0U; 
		/**** Lin Invalid Value Clear ****/				
		LIV_faultCnt = 0U; IRV_LIV_diagState = NORMAL; IRV_LIV_Warningflag = 0U;
		/**** Lin Timeout Clear ****/				
		IRV_LE_diagState = NORMAL; 
	    /**** Phase Open  Clear ****/			
		PO_faultCnt = 0; PO_retryCnt = 0U; PO_retryAttempt = 0U; PO_normalCnt = 0; IRV_PO_diagState = NORMAL;
	    /**** Stall Error Clear ****/			
        ST_FaultCnt = 0U; ST_NormalCnt = 0U; ST_retryCnt  = 0U; ST_retryAttempt = 0U; IRV_ST_diagState = NORMAL;
	}

}

static void MergeDiagState(void)
{
	uint8_t diag_failureStateMerged		  = 0U;
	uint8_t diag_warningStateMerged        = 0U;
    uint8_t diag_faultStateMerged_Byte1	  = 0U;
	uint8_t diag_faultStateMerged_Byte2	  = 0U;
	uint8_t diag_faultStateflag_Byte1 = 0U;
    uint8_t diag_faultStateflag_Byte2 = 0U;

	uint8_t LE_warningCheck				  = 0U;				
	uint8_t diag_failureCheck              = 0U;
	uint8_t abnormalfailureCheck           = 0U;
	uint8_t SC_faultflag 				 = 0U;
	uint8_t PO_faultflag 				 = 0U;
	uint8_t GE_faultflag 				 = 0U;
	uint8_t OT_faultCheck				  = 0U;
	uint8_t OC_faultCheck				  = 0U;
	uint8_t LC_faultCheck				  = 0U;
	uint8_t ST_faultCheck				  = 0U;
	
	uint8_t OV_faultCheck				  = 0U;
	uint8_t UV_faultCheck				  = 0U;
	uint8_t SC_faultCheck				  = 0U;
	uint8_t GE_faultCheck				  = 0U;
	uint8_t SE_faultCheck                  = 0U;
    uint8_t PO_faultCheck                  = 0U;

	 uint8_t SE_faultflag 				 = 0U;uint8_t OT_faultflag 				 = 0U;uint8_t OC_faultflag 				 = 0U;uint8_t LC_faultflag 				 = 0U;uint8_t OV_faultflag 				 = 0U;uint8_t UV_faultflag 				 = 0U;	uint8_t ST_faultflag 				 = 0U;

	 uint8_t WT_warningCheck				  = 0U;  uint8_t LIV_warningCheck				  = 0U;  uint8_t SE_warningCheck				  = 0U; uint8_t OC_warningCheck				  = 0U;

	/************************* diag_faultStateMerged Byte 1 **********************/	
    UV_faultCheck = (((uint8_t)IRV_UV_diagState >> 1u) & 0x01u) ;       // bit 0
    OV_faultCheck = (((uint8_t)IRV_OV_diagState >> 1u) & 0x01u) << 1u;  // bit 1
	SC_faultCheck = (((uint8_t)IRV_SC_diagState >> 1u) & 0x01u) << 2u;  // bit 2
	PO_faultCheck = (((uint8_t)IRV_PO_diagState >> 1u) & 0x01u) << 3u;  // bit 3
	ST_faultCheck = (((uint8_t)IRV_ST_diagState >> 1u) & 0x01u) << 4u;  // bit 4
                                                                         // ProtectionFlag
                                                                         // FaultFlag
	SE_faultCheck = (((uint8_t)IRV_SE_diagState >> 1u) & 0x01u) << 7u;  // bit 7
	/************************* diag_faultStateMerged Byte 2 **********************/	
	                                                                     // DiagEN_forced
    OT_faultCheck = (((uint8_t)IRV_OT_diagState >> 1u) & 0x01u) << 2u;  // bit 1
                                                                         // Warning Temperature
    LC_faultCheck = (((uint8_t)IRV_LC_diagState >> 1u) & 0x01u) << 3u;  // bit 3
                                                                         // Warning Current
    OC_faultCheck = (((uint8_t)IRV_OC_diagState >> 1u) & 0x01u) << 5u;  // bit 5
                                                                         // comm_Error  LIV warning check
    GE_faultCheck = (((uint8_t)IRV_GE_diagState >> 1u) & 0x01u) << 7u;  // bit 7
	/************************* LIN   SIGNAL Byte 1 **********************/
    UV_faultflag = (IRV_UV_Faultflag & 0x01u);
	OV_faultflag = (IRV_OV_Faultflag & 0x01u) << 1u;
    SC_faultflag = (((uint8_t)IRV_SC_diagState >> 2u) & 0x01u) << 2u;
	PO_faultflag = (((uint8_t)IRV_PO_diagState >> 2u) & 0x01u) << 3u;
    ST_faultflag = (IRV_ST_Faultflag & 0x01u) << 4u;                                  // stall
                                                                                      // ProtectionFlag
                                                                                      // FaultFlag
    SE_faultflag = (uint8_t)(((IRV_SE_Faultflag & 0x01u) << 7u) | ((IRV_SE_Warningflag & 0x01u) << 7u));
	/************************* LIN   SIGNAL Byte 2 **********************/
                                                                       // DiagEN_forced
    OT_faultflag = (IRV_OT_Faultflag & 0x01u) << 1u;                   // bit 1
                                                                       // Warning Temperature
    LC_faultflag = (IRV_LC_Faultflag & 0x01u) << 3u;                   // bit 3
                                                                       // Warning Current
    OC_faultflag = (IRV_OC_Faultflag & 0x01u) << 5u;                   // bit 5
                                                                       // Comm_Error LIV warning check
    GE_faultflag = (((uint8_t)IRV_GE_diagState >> 2u) & 0x01u) << 7u; // bit 7
	/************************* diag warningStateMerged **********************/
    WT_warningCheck = ((uint8_t)IRV_OT_diagState & 0x01u) << 2u;
	OC_warningCheck = ((uint8_t)IRV_WC_diagState & 0x01u) << 4u;
    LE_warningCheck = ((uint8_t)IRV_LE_diagState & 0x01u) << 5u;
	LIV_warningCheck = ((uint8_t)IRV_LIV_diagState & 0x01u) << 6u;
    SE_warningCheck = ((uint8_t)IRV_SE_diagState & 0x01u) << 7u;
	/************* abnormal Failure check ********************/
	SE_failureCheck = (((uint8_t)IRV_SE_diagState >> 2u) & 0x01u);    
	LC_failureCheck = (((uint8_t)IRV_LC_diagState >> 2u) & 0x01u);
	OC_failureCheck = (((uint8_t)IRV_OC_diagState >> 2u) & 0x01u);
	OV_failureCheck = (((uint8_t)IRV_OV_diagState >> 2u) & 0x01u);
	UV_failureCheck = (((uint8_t)IRV_UV_diagState >> 2u) & 0x01u);
	ST_failureCheck = (((uint8_t)IRV_ST_diagState >> 2u) & 0x01u);
	diag_failureCheck= UV_failureCheck | OV_failureCheck | SE_failureCheck | LC_failureCheck | OC_failureCheck | ST_failureCheck;
	/************* diag   Failure State Merged ********************/    
    SC_failureCheck = (((uint8_t)IRV_SC_diagState >> 2u) & 0x01u) << 7u;
    GE_failureCheck = (((uint8_t)IRV_GE_diagState >> 2u) & 0x01u) << 6u;        
    PO_failureCheck = (((uint8_t)IRV_PO_diagState >> 2u) & 0x01u);
	abnormalfailureCheck = (diag_failureCheck) << 3u;
    /************* Result Merged ********************/     
    diag_faultStateMerged_Byte1 = UV_faultCheck | OV_faultCheck | SC_faultCheck | PO_faultCheck  | ST_faultCheck| SE_faultCheck;
	diag_faultStateMerged_Byte2 = OT_faultCheck | LC_faultCheck | OC_faultCheck | GE_faultCheck;
    
    diag_failureStateMerged = SC_failureCheck | GE_failureCheck | abnormalfailureCheck | PO_failureCheck;
    diag_warningStateMerged = SE_warningCheck | WT_warningCheck | LIV_warningCheck | LE_warningCheck | OC_warningCheck;
	diag_faultStateflag_Byte1 = UV_faultflag | OV_faultflag | SC_faultflag | PO_faultflag | ST_faultflag | SE_faultflag;
	diag_faultStateflag_Byte2 = OT_faultflag | LC_faultflag | OC_faultflag | GE_faultflag;
	//diag_powerlimitflag = PL_Limitflag;	
	(void)WriteWarningStateMerged(diag_warningStateMerged);
    (void)WriteFailureStateMerged(diag_failureStateMerged);
    (void)WriteFaultStateMerged_Byte1(diag_faultStateMerged_Byte1);
	(void)WriteFaultStateMerged_Byte2(diag_faultStateMerged_Byte2);    
	(void)WriteLINFaultStateFlag_Byte1(diag_faultStateflag_Byte1); // LIN SIGNAL
	(void)WriteLINFaultStateFlag_Byte2(diag_faultStateflag_Byte2); // LIN SIGNAL
	//(void)WritePowerLimitFlag(diag_powerlimitflag);
}

static void SetDiagFlagState (void)
{
	static uint8_t diag_faultFlagSet			  = 0U;
	static uint8_t diag_protectionFlagSet		  = 0U;
	
	
    diag_protectionFlagSet = IRV_SE_Faultflag | IRV_OT_Faultflag | IRV_OC_Faultflag | IRV_LC_Faultflag 
		| IRV_OV_Faultflag | IRV_UV_Faultflag | IRV_ST_Faultflag | IRV_OT_Warningflag 
		| IRV_SE_Warningflag | IRV_WC_Warningflag | IRV_LIV_Warningflag;
	
    diag_faultFlagSet = SC_failureCheck | GE_failureCheck | SE_failureCheck | UV_failureCheck 
		| OV_failureCheck | OC_failureCheck | LC_failureCheck | ST_failureCheck 
		| PO_failureCheck;
	
    if(diag_faultFlagSet >= 1u)
    {
        diag_protectionFlagSet = 0u;
	}


    (void)WriteProtectionFlag(diag_protectionFlagSet);
    (void)WriteFaultFlag(diag_faultFlagSet);
}




/*
************************************************************************************************************************
*                                                      Local functions
************************************************************************************************************************
*/

