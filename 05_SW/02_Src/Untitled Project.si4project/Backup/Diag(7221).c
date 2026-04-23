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
static DiagState_t IRV_OC_diagState = NORMAL;
static DiagState_t IRV_LC_diagState = NORMAL;
static DiagState_t IRV_OV_diagState = NORMAL;
static DiagState_t IRV_UV_diagState = NORMAL;
static DiagState_t IRV_LE_diagState = NORMAL;
static DiagState_t IRV_PO_diagState = NORMAL;



/*
************************************************************************************************************************
*                                                     Local variables
************************************************************************************************************************
*/

/* via Rte */
static DiagState_t diag_hwOpenPhaseState      = NORMAL;
static sint16_t diag_vdcFiltered              = 0;
static sint16_t diag_speedRef                 = 0;
static sint16_t diag_iqFiltered               = 0;


/* Only used Diag.c */

static uint8_t SE_faultCheck                  = 0;
static uint8_t OT_faultCheck                  = 0;
static uint8_t OC_faultCheck                  = 0;
static uint8_t LC_faultCheck                  = 0;

static uint8_t OV_faultCheck                  = 0;
static uint8_t UV_faultCheck                  = 0;
static uint8_t SC_faultCheck                  = 0;
static uint8_t GE_faultCheck                  = 0;
static uint8_t WT_warningCheck                = 0;

static uint8_t SC_failureCheck                = 0;
static uint8_t GE_failureCheck                = 0;
static uint8_t SE_faultOrFailureCheck         = 0;


/* 	
************************************************************************************************************************
*                                                 Local function prototypes
************************************************************************************************************************
*/
static void DiagShortCircuit (void);
static void DiagGateDrvOVUV(void);
static void DiagSpeedError (void);
static void DiagOverTemperature (void);
static void DiagOverCurrent (void);
static void DiagLowCurrent (void);
static void DiagOverVoltage (void);
static void DiagUnderVoltage (void);
static void DiagLINError (void);
static void MergeDiagState (void);
static void SetDiagFlagState (void);
static void DiagPhaseOpen (void);


/*
************************************************************************************************************************
*                                                    Exported functions
************************************************************************************************************************
*/
void DiagSWComponentPack(void)
{
    DiagLINError();
    DiagOverVoltage();
    DiagUnderVoltage();
    DiagShortCircuit();
    DiagGateDrvOVUV();
    /*DiagPhaseOpen();*/
    DiagSpeedError();
    DiagOverTemperature();
    DiagOverCurrent();
    DiagLowCurrent();
    MergeDiagState();
    SetDiagFlagState();
}

static void DiagShortCircuit (void)
{
	static DiagEntryConditionState_t SC_entryCondition;
	static DiagState_t diag_hwShortCircuitState   = NORMAL;
	static sint16_t SC_normalCnt				  = 0;
	static sint16_t SC_faultCnt 				  = 0;
	static sint16_t SC_retryCnt 				  = 0;
	static sint16_t SC_retryAttempt 			  = 0;
	sint16_t CAL_SC_NormalCntMax	  = SC_NORMAL_CNT_MAX;
	sint16_t CAL_SC_FaultCntMax 	  = SC_FAULT_CNT_MAX;
	sint16_t CAL_SC_RetryCntMax 	  = SC_RETRY_CNT_MAX;
	sint16_t CAL_SC_RetryAttemptMax   = SC_RETRY_ATTEMPT_MAX;

    (void)ReadHwShortCircuitState(&diag_hwShortCircuitState);
    (void)ReadSpeedRef(&diag_speedRef);

    if ((diag_speedRef >= CAL_SpeedMin) && (IRV_OV_diagState == NORMAL) && (IRV_UV_diagState == NORMAL))
    {
        SC_entryCondition = ENABLE;
    }
    else
    {
        SC_entryCondition = DISABLE;
    }
    
    switch (IRV_SC_diagState)
    {
        case NORMAL:
             if ((diag_hwShortCircuitState == FAULT) && (SC_entryCondition == ENABLE))
             {
                 SC_faultCnt++;
             }
             else if ((diag_hwShortCircuitState == NORMAL) || (SC_entryCondition == DISABLE))
             {
                 SC_faultCnt--;
                 if (SC_faultCnt < 0)  /* zero saturation */
	                {
	                    SC_faultCnt = 0;
	                }
             }
             else
             {
                 /* no action */
             }
        
             if (SC_faultCnt >= CAL_SC_FaultCntMax)
             {
                 SC_faultCnt = 0;
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
                 SC_retryCnt = 0;
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
             if ((diag_hwShortCircuitState == FAULT) && (SC_entryCondition == ENABLE))
             {
                 SC_faultCnt++;
                 SC_normalCnt--;
                 if (SC_normalCnt < 0)  /* zero saturation */
	                {
	                    SC_normalCnt = 0;
	                }
                 
             }
             else if ((diag_hwShortCircuitState == NORMAL) || (SC_entryCondition == DISABLE))
             {
                 SC_normalCnt++;
                 SC_faultCnt--;
                 if (SC_faultCnt < 0)  /* zero saturation */
	                {
	                    SC_faultCnt = 0;
	                }
             }
             else
             {
             /* no action */
             }

             if(SC_faultCnt >= CAL_SC_FaultCntMax)
             {
                 SC_faultCnt = 0;
                 SC_normalCnt = 0;
                 IRV_SC_diagState = FAULT;
             }
             else
             {
             /* no action */
             }

             if (SC_normalCnt >= CAL_SC_NormalCntMax)
             {
                 SC_faultCnt = 0;
                 SC_normalCnt = 0;
                 SC_retryAttempt = 0;
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
	static DiagEntryConditionState_t GE_entryCondition;
	static DiagState_t diag_hwGateDrvErrorState   = NORMAL;
	static sint16_t GE_normalCnt				  = 0;
	static sint16_t GE_faultCnt 				  = 0;
	static sint16_t GE_retryCnt 				  = 0;
	static sint16_t GE_retryAttempt 			  = 0;
	sint16_t CAL_GE_NormalCntMax	  = GE_NORMAL_CNT_MAX;
	sint16_t CAL_GE_FaultCntMax 	  = GE_FAULT_CNT_MAX;
	sint16_t CAL_GE_RetryCntMax 	  = GE_RETRY_CNT_MAX;
	sint16_t CAL_GE_RetryAttemptMax   = GE_RETRY_ATTEMPT_MAX;

    (void)ReadHwGateDrvErrorState(&diag_hwGateDrvErrorState);
    (void)ReadSpeedRef(&diag_speedRef);

    if ((diag_speedRef >= CAL_SpeedMin) && (IRV_OV_diagState == NORMAL) && (IRV_UV_diagState == NORMAL))
    {
        GE_entryCondition = ENABLE;
    }
    else
    {
        GE_entryCondition = DISABLE;
    }
    
    switch (IRV_GE_diagState)
    {
        case NORMAL:
            if ((diag_hwGateDrvErrorState == FAULT) && (GE_entryCondition == ENABLE))
            {
                GE_faultCnt++;
            }
            else if ((diag_hwGateDrvErrorState == NORMAL) || (GE_entryCondition == DISABLE))
            {
                GE_faultCnt--;
                if (GE_faultCnt < 0)  /* zero saturation */
                {
                    GE_faultCnt = 0;
                }
            }
            else
            {
            /* no action */
            }
           

            if (GE_faultCnt >= CAL_GE_FaultCntMax)
            {
                GE_faultCnt = 0;
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
                 GE_retryCnt = 0;
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
            if ((diag_hwGateDrvErrorState == FAULT) && (GE_entryCondition == ENABLE))
            {
                GE_faultCnt++;
                GE_normalCnt--;
                if (GE_normalCnt < 0)  /* zero saturation */
                {
                    GE_normalCnt = 0;
                }
            }
            else if ((diag_hwGateDrvErrorState == NORMAL) || (GE_entryCondition == DISABLE))
            {
                GE_normalCnt++;
                GE_faultCnt--;
                if (GE_faultCnt < 0)  /* zero saturation */
                {
                    GE_faultCnt = 0;
                }
            }
            else
            {
            /* no action */
            }

            if (GE_faultCnt >= CAL_GE_FaultCntMax)
            {
                GE_faultCnt = 0;
                GE_normalCnt = 0;
                IRV_GE_diagState = FAULT;
            }
            else
            {
            /* no action */
            }
        
            if (GE_normalCnt >= CAL_GE_NormalCntMax)
            {
                GE_faultCnt = 0;
                GE_normalCnt = 0;
                GE_retryAttempt = 0;
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


static void DiagSpeedError(void)
{
	static DiagEntryConditionState_t SE_entryCondition;
	static sint16_t diag_speedAct				  = 0;
	static sint16_t diag_speedError 			  = 0;
	static sint16_t SE_normalCnt				  = 0;
	static sint16_t SE_faultCnt 				  = 0;
	static sint16_t SE_retryCnt 				  = 0;
	static sint16_t SE_retryAttempt 			  = 0;

	sint16_t CAL_SE_NormalLwrTh 	  = SE_NORMAL_LWR_TH;
	sint16_t CAL_SE_FaultUprTh		  = SE_FAULT_UPR_TH;

	sint16_t CAL_SE_NormalCntMax	  = SE_NORMAL_CNT_MAX;
	sint16_t CAL_SE_FaultCntMax 	  = SE_FAULT_CNT_MAX;
	sint16_t CAL_SE_RetryCntMax 	  = SE_RETRY_CNT_MAX;
	sint16_t CAL_SE_RetryAttemptMax   = SE_RETRY_ATTEMPT_MAX;

    (void)ReadVdcFiltered(&diag_vdcFiltered);
    (void)ReadSpeedRef(&diag_speedRef);
    (void)ReadSpeedFiltered(&diag_speedAct);
    diag_speedError = diag_speedRef - diag_speedAct;

    if ((IRV_OV_diagState == NORMAL) && (IRV_UV_diagState == NORMAL) && (IRV_SC_diagState == NORMAL) && (IRV_GE_diagState == NORMAL) && (IRV_OC_diagState == NORMAL))
    {
        SE_entryCondition = ENABLE;
    }
    else
    {
        SE_entryCondition = DISABLE;
    }
     
    switch (IRV_SE_diagState)
    {
        case NORMAL:
            if ((diag_speedError > CAL_SE_FaultUprTh) && (SE_entryCondition == ENABLE))
            {
                SE_faultCnt++;
            }
            else
            {
                SE_faultCnt--;
                if (SE_faultCnt < 0)  /* zero saturation */
                {
                    SE_faultCnt = 0;
                }
            }
            
            if (SE_faultCnt >= CAL_SE_FaultCntMax)
            {
                SE_faultCnt = 0;
                IRV_SE_diagState = FAULT;
            }
            else
            {
            /* no action */
            }
            break;

        case WARNING:
            if (diag_speedError <= CAL_SE_NormalLwrTh)
            {
                SE_normalCnt++;
            }
            else
            {
                SE_normalCnt--;
                if (SE_normalCnt < 0)
                {
                    SE_normalCnt = 0;
                }
            }
            
            if ((SE_normalCnt >= CAL_SE_NormalCntMax) || (SE_entryCondition == DISABLE))
            {
                SE_normalCnt = 0;
                IRV_SE_diagState = NORMAL;
            }
            else
            {
            /* no action */
            }
            break;

        case FAULT:
            SE_retryCnt++;
            if (SE_retryAttempt >= CAL_SE_RetryAttemptMax)
            {
                SE_retryAttempt = 0;
                SE_retryCnt = 0;
                IRV_SE_diagState = WARNING;
            }
            else
            {
            /* no action */
            }
            
            if (SE_retryCnt >= CAL_SE_RetryCntMax)
            {
                SE_retryCnt = 0;
                SE_retryAttempt++;
                IRV_SE_diagState = RETRY;
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
            if ((diag_speedError > CAL_SE_FaultUprTh) && (SE_entryCondition == ENABLE))
            {
                SE_faultCnt++;
                SE_normalCnt--;
                if (SE_normalCnt < 0)  /* zero saturation */
                {
                    SE_normalCnt = 0;
                }
            }
            else
            {
                SE_normalCnt++;
                SE_faultCnt--;
                if (SE_faultCnt < 0)  /* zero saturation */
                {
                    SE_faultCnt = 0;
                }
            }
            
            if (SE_normalCnt >= CAL_SE_NormalCntMax)
            {
                SE_faultCnt = 0;
                SE_normalCnt = 0;
                SE_retryAttempt = 0;
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
                IRV_SE_diagState = FAULT;
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


static void DiagOverTemperature (void)
{
	static DiagEntryConditionState_t OT_entryCondition;
	static sint16_t diag_temperatureFiltered	  = 0;
	static sint16_t OT_normalCnt				  = 0;
	static sint16_t OT_warningCnt				  = 0;
	static sint16_t OT_faultCnt 				  = 0;

	sint16_t CAL_OT_NormalLwrTh 	  = OT_NORMAL_LWR_TH;
	sint16_t CAL_OT_WarningUprTh	  = OT_WARNING_UPR_TH;
	sint16_t CAL_OT_WarningLwrTh	  = OT_WARNING_LWR_TH;
	sint16_t CAL_OT_FaultUprTh		  = OT_FAULT_UPR_TH;

	sint16_t CAL_OT_NormalCntMax	  = OT_NORMAL_CNT_MAX;
	sint16_t CAL_OT_WarningCntMax	  = OT_WARNING_CNT_MAX;
	sint16_t CAL_OT_FaultCntMax 	  = OT_FAULT_CNT_MAX;

    (void)ReadTemperatureFiltered (&diag_temperatureFiltered);
    (void)ReadSpeedRef(&diag_speedRef);

    if ((diag_speedRef >= CAL_SpeedMin) && (IRV_OV_diagState == NORMAL) && (IRV_UV_diagState == NORMAL))
    {
        OT_entryCondition = ENABLE;
    }
    else
    {
        OT_entryCondition = DISABLE;
    }
             
    switch (IRV_OT_diagState)
    {
        case NORMAL:
            if ((diag_temperatureFiltered >= CAL_OT_WarningUprTh) && (OT_entryCondition == ENABLE))
            {
                OT_warningCnt++;
            }
            else
            {
                OT_warningCnt--;
                if (OT_warningCnt < 0)  /* zero saturation */
                {
                OT_warningCnt = 0;
                }
            }
            
            if (OT_warningCnt >= CAL_OT_WarningCntMax)
            {
                OT_warningCnt = 0;
                IRV_OT_diagState = WARNING;
            }
            else
            {
            /* no action */
            }
            break;

        case WARNING:
            if (diag_temperatureFiltered <= CAL_OT_NormalLwrTh)
            {
                OT_normalCnt++;
            }
            else
            {
                OT_normalCnt--;
                if (OT_normalCnt < 0)
                {
                    OT_normalCnt = 0;
                }                
            }            
            
            if (diag_temperatureFiltered >= CAL_OT_FaultUprTh)
            {
                OT_faultCnt++;
            }
            else
            {
                OT_faultCnt--;
                if (OT_faultCnt < 0)
                {
                    OT_faultCnt = 0;
                }                
            }                       
            
            if (OT_normalCnt >= CAL_OT_NormalCntMax)
            {
                OT_faultCnt = 0;
                OT_normalCnt = 0;
                IRV_OT_diagState = NORMAL;
            }
            else
            {
            /* no action */
            }

        
            if (OT_faultCnt >= CAL_OT_FaultCntMax)
            {
                OT_faultCnt = 0;
                OT_normalCnt = 0;
                IRV_OT_diagState = FAULT;
            }
            else
            {
            /* no action */
            }
            break;

        case FAULT:
            if (diag_temperatureFiltered <= CAL_OT_WarningLwrTh)
            {
                OT_warningCnt++;
            }
            else
            {
                OT_warningCnt--;
                if (OT_warningCnt < 0)
                {
                    OT_warningCnt = 0;
                }                
            }            
                    
            if (OT_warningCnt >= CAL_OT_WarningCntMax)
            {
                OT_warningCnt = 0;
                IRV_OT_diagState = WARNING;
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


static void DiagOverCurrent(void)
{
	static DiagEntryConditionState_t OC_entryCondition;
	static sint16_t diag_fluxAbsValueMeasured	  = 0;
	static sint16_t OC_normalCnt				  = 0;
	static sint16_t OC_faultCnt 				  = 0;
	static sint16_t OC_retryCnt 				  = 0;
	static sint16_t OC_retryAttempt 			  = 0;
	sint16_t CAL_OC_FluxAbsFaultUprTh = OC_FLUXABS_FAULT_UPR_TH;

	sint16_t CAL_OC_IqFaultUprTh	  = OC_IQ_FAULT_UPR_TH;

	sint16_t CAL_OC_NormalCntMax	  = OC_NORMAL_CNT_MAX;
	sint16_t CAL_OC_FaultCntMax 	  = OC_FAULT_CNT_MAX;
	sint16_t CAL_OC_RetryCntMax 	  = OC_RETRY_CNT_MAX;
	sint16_t CAL_OC_RetryAttemptMax   = OC_RETRY_ATTEMPT_MAX;

    (void)ReadIqFiltered(&diag_iqFiltered);
    (void)ReadSpeedRef(&diag_speedRef);
    (void)ReadFluxAbsValueMeasured(&diag_fluxAbsValueMeasured);
    (void)ReadHwOpenPhaseState(&diag_hwOpenPhaseState);
   
    if ((diag_speedRef >= CAL_SpeedMin) && (diag_hwOpenPhaseState == NORMAL) && (IRV_OV_diagState == NORMAL) && (IRV_UV_diagState == NORMAL))
    {
        OC_entryCondition = ENABLE;
    }
    else
    {
        OC_entryCondition = DISABLE;
    }

    
    switch (IRV_OC_diagState)
    {
        case NORMAL:
            if (((diag_fluxAbsValueMeasured <= CAL_OC_FluxAbsFaultUprTh) || (diag_iqFiltered >= CAL_OC_IqFaultUprTh)) && (OC_entryCondition == ENABLE))
            {
                OC_faultCnt++;
            }
            else
            {
                OC_faultCnt--;
                if (OC_faultCnt < 0)  /* zero saturation */
                {
                    OC_faultCnt = 0;
                }
            }
            
            if (OC_faultCnt >= CAL_OC_FaultCntMax)
            {
                OC_faultCnt = 0;
                IRV_OC_diagState = FAULT;
            }
            else
            {
            /* no action */
            }
            break;

        case FAULT:
             OC_retryCnt++;
             
             if (OC_retryAttempt >= CAL_OC_RetryAttemptMax)
             {
                 IRV_OC_diagState = FAILURE;
             }
             else
             {
             /* no action */
             }
        
             
             if (OC_retryCnt >= CAL_OC_RetryCntMax)
             {
                 OC_retryCnt = 0;
                 OC_retryAttempt++;
                 IRV_OC_diagState = RETRY;
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
            if (((diag_fluxAbsValueMeasured <= CAL_OC_FluxAbsFaultUprTh) || (diag_iqFiltered >= CAL_OC_IqFaultUprTh)) && (OC_entryCondition == ENABLE))
            {
                OC_faultCnt++;
                OC_normalCnt--;
                if (OC_normalCnt < 0)  /* zero saturation */
                {
                    OC_normalCnt = 0;
                }
            }
            else
            {
                OC_normalCnt++;
                OC_faultCnt--;
                if (OC_faultCnt < 0)  /* zero saturation */
                {
                    OC_faultCnt = 0;
                }
            }
            
            if (OC_faultCnt >= CAL_OC_FaultCntMax)
            {
                OC_faultCnt = 0;
                OC_normalCnt = 0;
                IRV_OC_diagState = FAULT;
            }
            else
            {
            /* no action */
            }
        
            if (OC_normalCnt >= CAL_OC_NormalCntMax)
            {
                OC_faultCnt = 0;
                OC_normalCnt = 0;
                OC_retryAttempt = 0;
                IRV_OC_diagState = NORMAL;
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


static void DiagLowCurrent(void)
{
	static DiagEntryConditionState_t LC_entryCondition;
	static uint8_t	diag_coolantFlag			  = 0;
	static sint16_t LC_normalCnt				  = 0;
	static sint16_t LC_faultCnt 				  = 0;
	static sint16_t LC_retryCnt 				  = 0;

	sint16_t CAL_LC_FaultLwrTh		  = LC_FAULT_LWR_TH;
	sint16_t CAL_LC_NormalCntMax	  = LC_NORMAL_CNT_MAX;
	sint16_t CAL_LC_FaultCntMax 	  = LC_FAULT_CNT_MAX;
	sint16_t CAL_LC_RetryCntMax 	  = LC_RETRY_CNT_MAX;
	sint16_t CAL_LC_SpeedMin		  = LC_SPEED_MIN;

    (void)ReadIqFiltered(&diag_iqFiltered);
    (void)ReadSpeedRef(&diag_speedRef);
    (void)ReadCoolantFlag(&diag_coolantFlag);
    
    if ((diag_speedRef >= CAL_LC_SpeedMin) && (diag_coolantFlag == 0u) && (IRV_OV_diagState == NORMAL) && (IRV_UV_diagState == NORMAL) && (IRV_SC_diagState == NORMAL) && (IRV_GE_diagState == NORMAL) && (IRV_SE_diagState == NORMAL) && (IRV_OC_diagState == NORMAL))
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
            if ((diag_iqFiltered <= CAL_LC_FaultLwrTh) && (LC_entryCondition == ENABLE))
            {
                LC_faultCnt++;
            }
            else
            {
                LC_faultCnt--;
                if (LC_faultCnt < 0)  /* zero saturation */
                {
                    LC_faultCnt = 0;
                }
            }
            
            if (LC_faultCnt >= CAL_LC_FaultCntMax)
            {
                LC_faultCnt = 0;
                IRV_LC_diagState = FAULT;
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
                IRV_LC_diagState = RETRY;
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
            if ((diag_iqFiltered <= CAL_LC_FaultLwrTh) && (LC_entryCondition == ENABLE))
            {
                LC_faultCnt++;
                LC_normalCnt--;
                if (LC_normalCnt < 0)  /* zero saturation */
                {
                    LC_normalCnt = 0;
                }
            }
            else
            {
                LC_normalCnt++;
                LC_faultCnt--;
                if (LC_faultCnt < 0)  /* zero saturation */
                {
                    LC_faultCnt = 0;
                }
            }

            if (LC_normalCnt >= CAL_LC_NormalCntMax)
            {
                LC_faultCnt = 0;
                LC_normalCnt = 0;
                IRV_LC_diagState = NORMAL;
            }
            else
            {
            /* no action */
            }

            if (LC_faultCnt >= CAL_LC_FaultCntMax)
            {
                LC_faultCnt = 0;
                LC_normalCnt = 0;
                IRV_LC_diagState = FAULT;
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

static void DiagOverVoltage(void)
{
	static DiagEntryConditionState_t OV_entryCondition;
	static sint16_t OV_normalCnt				  = 0;
	static sint16_t OV_faultCnt 				  = 0;

	sint16_t CAL_OV_NormalLwrTh 	  = OV_NORMAL_LWR_TH;
	sint16_t CAL_OV_FaultUprTh		  = OV_FAULT_UPR_TH;

	sint16_t CAL_OV_NormalCntMax	  = OV_NORMAL_CNT_MAX;
	sint16_t CAL_OV_FaultCntMax 	  = OV_FAULT_CNT_MAX;


    (void)ReadVdcFiltered(&diag_vdcFiltered);
    (void)ReadSpeedRef(&diag_speedRef);

    if (diag_speedRef >= CAL_SpeedMin)
    {
        OV_entryCondition = ENABLE;
    }
    else
    {
        OV_entryCondition = DISABLE;
    }
   
    switch (IRV_OV_diagState)
    {
        case NORMAL:
            if ((diag_vdcFiltered >= CAL_OV_FaultUprTh) && (OV_entryCondition == ENABLE))
            {
                OV_faultCnt++;
            }
            else
            {
                OV_faultCnt--;
                if (OV_faultCnt < 0)  /* zero saturation */
                {
                    OV_faultCnt = 0;
                }
            }
            
            if (OV_faultCnt >= CAL_OV_FaultCntMax)
            {
                OV_faultCnt = 0;
                IRV_OV_diagState = FAULT;
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
                OV_normalCnt--;
                if (OV_normalCnt < 0)
                {
                    OV_normalCnt = 0;
                }                
            }            
                        
            if (OV_normalCnt >= CAL_OV_NormalCntMax)
            {
                OV_normalCnt = 0;
                IRV_OV_diagState = NORMAL;
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

static void DiagUnderVoltage(void)
{
	static DiagEntryConditionState_t UV_entryCondition;
	static sint16_t UV_normalCnt				  = 0;
	static sint16_t UV_faultCnt 				  = 0;
	sint16_t CAL_UV_NormalUprTh 	  = UV_NORMAL_UPR_TH;


	sint16_t CAL_UV_FaultLwrTh		  = UV_FAULT_LWR_TH;
	sint16_t CAL_UV_NormalCntMax	  = UV_NORMAL_CNT_MAX;
	sint16_t CAL_UV_FaultCntMax 	  = UV_FAULT_CNT_MAX;

    (void)ReadVdcFiltered(&diag_vdcFiltered);
    (void)ReadSpeedRef(&diag_speedRef);

    if (diag_speedRef >= CAL_SpeedMin)
    {
        UV_entryCondition = ENABLE;
    }
    else
    {
        UV_entryCondition = DISABLE;
    }
    
    switch (IRV_UV_diagState)
    {
        case NORMAL:
            if ((diag_vdcFiltered <= CAL_UV_FaultLwrTh) && (UV_entryCondition == ENABLE))
            {
                UV_faultCnt++;
            }
            else
            {
                UV_faultCnt--;
                if (UV_faultCnt < 0)  /* zero saturation */
                {
                    UV_faultCnt = 0;
                }
            }
            
            if (UV_faultCnt >= CAL_UV_FaultCntMax)
            {
                UV_faultCnt = 0;
                IRV_UV_diagState = FAULT;
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
                UV_normalCnt--;
                if (UV_normalCnt < 0)
                {
                    UV_normalCnt = 0;
                }                
            }            
                        
            if (UV_normalCnt >= CAL_UV_NormalCntMax)
            {
                UV_normalCnt = 0;
                IRV_UV_diagState = NORMAL;
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
	static DiagEntryConditionState_t LE_entryCondition;
	static DiagState_t diag_linTimeOutErrorState   = NORMAL;

    (void)ReadLinTimeOutErrorState(&diag_linTimeOutErrorState);

    if ((IRV_OV_diagState == NORMAL) && (IRV_UV_diagState == NORMAL))
    {
        LE_entryCondition = ENABLE;
    }
    else
    {
        LE_entryCondition = DISABLE;
    }
    
    switch (IRV_LE_diagState)
    {
        case NORMAL:
            if ((diag_linTimeOutErrorState == FAULT) && (LE_entryCondition == ENABLE))
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

static void DiagPhaseOpen(void)
{
	static DiagEntryConditionState_t PO_entryCondition;
	static sint16_t PO_normalCnt				  = 0;
	static sint16_t PO_faultCnt 				  = 0;
	static sint16_t PO_retryCnt 				  = 0;
	static sint16_t PO_retryAttempt 			  = 0;
	sint16_t CAL_PO_NormalCntMax	  = PO_NORMAL_CNT_MAX;
	sint16_t CAL_PO_FaultCntMax 	  = PO_FAULT_CNT_MAX;
	sint16_t CAL_PO_RetryCntMax 	  = PO_RETRY_CNT_MAX;
	sint16_t CAL_PO_RetryAttemptMax   = PO_RETRY_ATTEMPT_MAX;

    (void)ReadHwOpenPhaseState(&diag_hwOpenPhaseState);

    if ((IRV_OV_diagState == NORMAL) && (IRV_UV_diagState == NORMAL) && (IRV_SC_diagState == NORMAL) && (IRV_GE_diagState == NORMAL) && (IRV_OC_diagState == NORMAL))
    {
        PO_entryCondition = ENABLE;
    }
    else
    {
        PO_entryCondition = DISABLE;
    }
    
    switch (IRV_PO_diagState)
    {
        case NORMAL:
            if ((diag_hwOpenPhaseState == FAULT) && (PO_entryCondition == ENABLE)) /* detected 2 Phase Open */
            {
                PO_faultCnt++;
            }
            else if ((diag_hwOpenPhaseState == NORMAL) || (PO_entryCondition == DISABLE))
            {
                PO_faultCnt--;
                if (PO_faultCnt < 0)  /* zero saturation */
	            {
                    PO_faultCnt = 0;
	            }
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
                 PO_retryCnt = 0;
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
            if (diag_hwOpenPhaseState == FAULT)
            {
                PO_faultCnt++;
                PO_normalCnt--;
                if (PO_normalCnt < 0)  /* zero saturation */
	            {
                    PO_normalCnt = 0;
	            }
            }
            else if (diag_hwOpenPhaseState == NORMAL)
            {
                PO_normalCnt++;
                PO_faultCnt--;
                if (PO_faultCnt < 0)  /* zero saturation */
	            {
                    PO_faultCnt = 0;
	            }
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
                 PO_retryAttempt = 0;
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



static void MergeDiagState(void)
{
	static uint8_t diag_failureStateMerged		  = 0;
	static uint8_t diag_warningStateMerged		  = 0;
	static uint8_t diag_faultStateMerged		  = 0;
	static uint8_t diag_retryStateMerged		  = 0;
	static uint8_t LE_warningCheck				  = 0;
	static uint8_t PO_failureCheck				  = 0;
	static uint8_t SE_retryCheck				  = 0;
	static uint8_t OC_retryCheck				  = 0;
	static uint8_t SC_faultOrFailureCheck		  = 0;
	static uint8_t GE_faultOrFailureCheck		  = 0;
	static uint8_t OC_faultOrFailureCheck		  = 0;
	static uint8_t SE_warningCheck				  = 0;
	static uint8_t OC_failureCheck				  = 0;

    SE_faultCheck = (((uint8_t)IRV_SE_diagState >> 1u) | ((uint8_t)IRV_PO_diagState >> 1)) & 0x01u;
    OT_faultCheck = (((uint8_t)IRV_OT_diagState >> 1u) & 0x01u) << 1u;
    LC_faultCheck = (((uint8_t)IRV_LC_diagState >> 1u) & 0x01u) << 2u;
    OC_faultCheck = (((uint8_t)IRV_OC_diagState >> 1u) & 0x01u) << 3u;
    OV_faultCheck = (((uint8_t)IRV_OV_diagState >> 1u) & 0x01u) << 4u;
    UV_faultCheck = (((uint8_t)IRV_UV_diagState >> 1u) & 0x01u) << 5u;
    GE_faultCheck = (((uint8_t)IRV_GE_diagState >> 1u) & 0x01u) << 6u;
    SC_faultCheck = (((uint8_t)IRV_SC_diagState >> 1u) & 0x01u) << 7u;

    SE_retryCheck = (((uint8_t)IRV_SE_diagState >> 3u) & 0x01u);
    OC_retryCheck = (((uint8_t)IRV_OC_diagState >> 3u) & 0x01u) << 3u;

    SE_warningCheck = (uint8_t)IRV_SE_diagState & 0x01u;
    WT_warningCheck = ((uint8_t)IRV_OT_diagState & 0x01u) << 1u;
    LE_warningCheck = ((uint8_t)IRV_LE_diagState & 0x01u) << 2u;

    SC_failureCheck = (((uint8_t)IRV_SC_diagState >> 2u) & 0x01u) << 7u;
    GE_failureCheck = (((uint8_t)IRV_GE_diagState >> 2u) & 0x01u) << 6u;    
    OC_failureCheck = (((uint8_t)IRV_OC_diagState >> 2u) & 0x01u) << 3u;
    PO_failureCheck = (((uint8_t)IRV_PO_diagState >> 2u) & 0x01u);

    SC_faultOrFailureCheck = SC_faultCheck | SC_failureCheck;
    GE_faultOrFailureCheck = GE_faultCheck | GE_failureCheck;
    SE_faultOrFailureCheck = SE_faultCheck | PO_failureCheck;
    OC_faultOrFailureCheck = OC_faultCheck | OC_failureCheck;
    
    diag_failureStateMerged = SC_failureCheck | GE_failureCheck | PO_failureCheck | OC_failureCheck;
    diag_faultStateMerged = SE_faultOrFailureCheck | OT_faultCheck | OC_faultOrFailureCheck | LC_faultCheck | OV_faultCheck | UV_faultCheck | SC_faultOrFailureCheck | GE_faultOrFailureCheck;
    diag_warningStateMerged = SE_warningCheck | WT_warningCheck | LE_warningCheck;
    diag_retryStateMerged = OC_retryCheck | SE_retryCheck;

    (void)WriteFailureStateMerged(diag_failureStateMerged);
    (void)WriteFaultStateMerged(diag_faultStateMerged);
    (void)WriteWarningStateMerged(diag_warningStateMerged);
    (void)WriteRetryStateMerged(diag_retryStateMerged);
}

static void SetDiagFlagState (void)
{
	static uint8_t diag_faultFlagSet			  = 0;
	static uint8_t diag_protectionFlagSet		  = 0;

    SE_faultCheck = (((uint8_t)IRV_SE_diagState >> 1u) | ((uint8_t)IRV_PO_diagState >> 1u)) & 0x01u;
    OT_faultCheck = ((uint8_t)IRV_OT_diagState >> 1u) & 0x01u;
    LC_faultCheck = ((uint8_t)IRV_LC_diagState >> 1u) & 0x01u;
    OC_faultCheck = ((uint8_t)IRV_OC_diagState >> 1u) & 0x01u;
    OV_faultCheck = ((uint8_t)IRV_OV_diagState >> 1u) & 0x01u;
    UV_faultCheck = ((uint8_t)IRV_UV_diagState >> 1u) & 0x01u;
    GE_faultCheck = ((uint8_t)IRV_GE_diagState >> 1u) & 0x01u;
    SC_faultCheck = ((uint8_t)IRV_SC_diagState >> 1u) & 0x01u;

    WT_warningCheck = (uint8_t)IRV_OT_diagState & 0x01u;

    SC_failureCheck = ((uint8_t)IRV_SC_diagState >> 2u) & 0x01u;
    GE_failureCheck = ((uint8_t)IRV_GE_diagState >> 2u) & 0x01u;

    SE_faultOrFailureCheck = SE_faultCheck;
    
    diag_protectionFlagSet = SE_faultOrFailureCheck | OT_faultCheck | OC_faultCheck | LC_faultCheck | OV_faultCheck | UV_faultCheck | WT_warningCheck;

    diag_faultFlagSet = SC_failureCheck | GE_failureCheck;    

    (void)WriteProtectionFlag(diag_protectionFlagSet);
    (void)WriteFaultFlag(diag_faultFlagSet);
}



/*
************************************************************************************************************************
*                                                      Local functions
************************************************************************************************************************
*/

