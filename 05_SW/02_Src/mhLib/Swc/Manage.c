/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : Manage.c
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


/*
************************************************************************************************************************
*                                                   Include header files
************************************************************************************************************************
*/
#include "Mat.h"
#include "Manage.h"
#include "Diag.h"
#include "RteStd.h"
#include "Variant_cfg.h"
#include "Mon.h"

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


/*
************************************************************************************************************************
*                                                     Local variables
************************************************************************************************************************
*/

/* via Rte */
static sint16_t                      manage_speedCmd           = 0;
static sint16_t                      manage_IRV_SpeedRef       = 0;
static sint16_t                      manage_SpeedFiltered           = 0;
static sint16_t                      manage_Prev_SpeedRef           = 0;
static sint16_t                      manage_Diff_SpeedRef           = 0;
static sint16_t                      manage_IRV_Step_SpeedRef       = 0;
static ManageEWPStepControlState_t   manage_RPMStepControlState     = STEPCONTROLOFF;

/*
************************************************************************************************************************
*                                                 Local function prototypes
************************************************************************************************************************
*/
static void DiagCoolant (void);
static void ManageSystemOnOff (void);
static void ManageProtection (void);
static void ManageRPMStepControl(void);
static void ManageStepControlVariables(void);
static void ManageStepControlOff(void);
static void ManageStepControlOn(void);



/*
************************************************************************************************************************
*                                                    Exported functions
************************************************************************************************************************
*/
void ManageSWComponentPack(void)
{
    static uint8_t ManageTask_Cnt = 0;
	if(ManageTask_Cnt == 0u) //14us
	{		
		DiagCoolant();
		ManageSystemOnOff();
		ManageTask_Cnt++;	
	}
	else //14us
	{ 	
		ManageProtection();
		ManageRPMStepControl();
		ManageTask_Cnt = 0u;	
	}
}


/*
************************************************************************************************************************
*                                                      Local functions
************************************************************************************************************************
*/
static void DiagCoolant(void)
{
	static uint8_t manage_coolantFlag = 0;
	static sint16_t manage_iqFiltered 	= 0;
	static sint16_t manage_speedRequired = 0;
	
	static sint8_t DiagCoolantState = SPEED_CONTROL;
	sint16_t CAL_DiagCoolantKp       = DIAG_COOLANT_KP;
	sint16_t CAL_DiagCoolantKi       = DIAG_COOLANT_KI;
	sint16_t CAL_DiagCoolantSpeedMax = DIAG_COOLANT_SPEED_MAX;
	sint16_t CAL_IqDiagCoolant       = IQ_DIAG_COOLANT;
	
	static sint16_t iqDiagCoolantError	 = 0;
	static TMat_Pi diagCoolantPi;      
	static sint16_t iqDiagCoolant = 0;
	
    (void)ReadConvertRPM(&manage_speedRequired);
    (void)ReadCoolantFlag(&manage_coolantFlag);
    (void)ReadIqFiltered(&manage_iqFiltered);
    
    iqDiagCoolant = CAL_IqDiagCoolant; /* iq 4.0A */

    switch (DiagCoolantState)
    {
        case SPEED_CONTROL:
            manage_speedCmd = manage_speedRequired;

            if (manage_coolantFlag == 1u)
            {
                DiagCoolantState = IQ_CONTROL;

                diagCoolantPi.Kp = CAL_DiagCoolantKp;
                diagCoolantPi.Ki = CAL_DiagCoolantKi;
                diagCoolantPi.IMin = 0;
                diagCoolantPi.IMax = CAL_DiagCoolantSpeedMax;
                diagCoolantPi.PiMin = 0;
                diagCoolantPi.PiMax = CAL_DiagCoolantSpeedMax;
                diagCoolantPi.IOut = (((sint32_t)manage_speedRequired)*32768);
            }
            else
            {
            /* Intentionally Empty */
            }		 
            break;

        default:
            iqDiagCoolantError = iqDiagCoolant - manage_iqFiltered;
            manage_speedCmd = Mat_ExePi(&diagCoolantPi, iqDiagCoolantError);
			if(manage_speedCmd < 1000)
            {
			    manage_speedCmd = 1000;
			}

            if (manage_coolantFlag == 0u)
            {
                DiagCoolantState = SPEED_CONTROL;
            }
            else
            {
            /* Intentionally Empty */
            }  
            break;

    }

    (void)WriteSpeedCmd(manage_speedCmd);
}

static void ManageSystemOnOff(void)
{
	static sint8_t manageSystemOnOffState = WAKE_UP;
	static sint8_t manage_ignState  = IGN_ON;
	static uint8_t manage_InitialFlag = 0;

	static const uint16_t CAL_InitialSleepCntMax = INITIAL_SLEEP_CNT_MAX;
	static const sint16_t CAL_LimpHomeRpm        = LIMP_HOME_RPM;

	static sint16_t sleepCnt = 0;
	static sint16_t LimphomeCnt = 0;
	static uint16_t initialSleepCnt = 0;
	static uint8_t error_done = 0;
	uint8_t manage_LINCheck = 0U;
    (void)ReadIgnFlag(&manage_ignState);
    (void)ReadSpeedCmd(&manage_speedCmd);
    (void)ReadSpeedFiltered(&manage_SpeedFiltered);
	(void)ReadInitialFlag(&manage_InitialFlag);
    (void)ReadWarningStateMerged(&manage_LINCheck); //LIN Check	
    switch (manageSystemOnOffState)
    {
        case WAKE_UP:
			initialSleepCnt = (manage_ignState == IGN_OFF) ? (initialSleepCnt + 1u) : initialSleepCnt;

			manageSystemOnOffState = (manage_ignState == IGN_ON) ? DRIVE : manageSystemOnOffState;
			initialSleepCnt = (manage_ignState == IGN_ON) ? 0u : initialSleepCnt;
			
			manageSystemOnOffState = (initialSleepCnt >= CAL_InitialSleepCntMax) ? POWER_LATCH : manageSystemOnOffState;
			initialSleepCnt = (initialSleepCnt >= CAL_InitialSleepCntMax) ? 0u : initialSleepCnt;
			
			if(manage_InitialFlag == 1u)
			{
                manage_IRV_SpeedRef = 0;
				manageSystemOnOffState = WAKE_UP;
			}			
            break;

        case DRIVE:            
			manageSystemOnOffState = (manage_ignState == IGN_OFF) ? POWER_LATCH : manageSystemOnOffState;
			if(((manage_LINCheck >> 6U) & 0x01U) == 1U) // Lin invalid value
			{
    			manage_IRV_SpeedRef = SPEED_MAX_RUN;
				LimphomeCnt = 0;
			}
			else if((((manage_LINCheck >> 5U) & 0x01U) == 1U)) // limp home
			{
			    LimphomeCnt = (LimphomeCnt >= 12000) ? 12000 : (LimphomeCnt+1);			
    			manage_IRV_SpeedRef = (LimphomeCnt >= 12000) ? 0 : CAL_LimpHomeRpm;
			}
			else                          // normal run (no limp hone)
			{
    			manage_IRV_SpeedRef = manage_speedCmd;
				LimphomeCnt = 0;
			}

			if(manage_InitialFlag == 1u)
			{
                manage_IRV_SpeedRef = 0;
				manageSystemOnOffState = WAKE_UP;
			}
            break;
            
        case POWER_LATCH:
            sleepCnt = (manage_ignState == IGN_ON) ? 0 : (sleepCnt + 1);
			manageSystemOnOffState = (manage_ignState == IGN_ON) ? DRIVE : manageSystemOnOffState;

            if ((sleepCnt >= 6000) && (error_done == 1u))
            {
				sleepCnt = 0;
				manageSystemOnOffState = SLEEP; 		
			}
			else if(sleepCnt >= 5500)
			{
				manage_IRV_SpeedRef = 0;								
				if((manage_SpeedFiltered == 0) && (error_done == 0u))
    			{
					(void)SetErrorCode(1u);
					error_done = 1;
				}
				else
				{
				    /* Intentionally Empty */
				}
			}
			else
		    {
    			if(((manage_LINCheck >> 6U) & 0x01U) == 1U) // Lin invalid value
                {
                    manage_IRV_SpeedRef = SPEED_MAX_RUN;
                    LimphomeCnt = 0;
                }
                else if((((manage_LINCheck >> 5U) & 0x01U) == 1U)) // Lin time out
                {
                    LimphomeCnt = (LimphomeCnt >= 12000) ? 12000 : (LimphomeCnt+1);			
                    manage_IRV_SpeedRef = (LimphomeCnt >= 12000) ? 0 : CAL_LimpHomeRpm;
                }
                else // normal run (no limp hone)
                {
                    manage_IRV_SpeedRef = manage_speedCmd;
                    LimphomeCnt = 0;
                }
			}

			if(manage_InitialFlag == 1u)
			{
                manage_IRV_SpeedRef = 0;
			}			
            break;

        case SLEEP:
                (void)SetSleepMode();           
            break;
            
        default: 
			      /* Intentionally Empty */
            break;
    }
	manage_IRV_SpeedRef = (manage_IRV_SpeedRef >= SPEED_MAX_RUN) ? SPEED_MAX_RUN : manage_IRV_SpeedRef;
    manage_IRV_SpeedRef = (manage_IRV_SpeedRef < SPEED_MIN_RUN) ? 0 : manage_IRV_SpeedRef;

}


static void ManageProtection (void)
{
	static sint8_t manageProtectionState = RUN;
	static const sint16_t CAL_SpeedMaxDegrade    = SPEED_MAX_DEGRADE;
	static uint8_t manage_failureStateMerged = 0;
	static uint8_t manage_faultStateMerged_Byte1   = 0;
	static uint8_t manage_faultStateMerged_Byte2 = 0;
	static uint8_t manage_warningStateMerged = 0;

    (void)ReadFaultStateMerged_Byte1(&manage_faultStateMerged_Byte1);
	(void)ReadFaultStateMerged_Byte2(&manage_faultStateMerged_Byte2);
    (void)ReadWarningStateMerged(&manage_warningStateMerged);
    (void)ReadFailureStateMerged(&manage_failureStateMerged);
    (void)ReadSpeedFiltered(&manage_SpeedFiltered);

    switch(manageProtectionState)
    {
        case RUN:       
            if ((manage_faultStateMerged_Byte1 != 0u) || (manage_faultStateMerged_Byte2 != 0u) || (manage_failureStateMerged != 0u))
            {
                manageProtectionState = STOP;
            }
            else if(((manage_warningStateMerged & 0x14u) != 0u)) // WT WC
            {
                manageProtectionState = DEGRADE;
            }
			else
			{
 			/* Intentionally Empty */
			}
            
            break;
            
        case DEGRADE:
            if (manage_IRV_SpeedRef > CAL_SpeedMaxDegrade)
            {
                manage_IRV_SpeedRef = CAL_SpeedMaxDegrade;
            }
            else
            {
            /* Intentionally Empty */
            }
            
            if ((manage_warningStateMerged & 0x14u) == 0u) // WT WC
            {
                manageProtectionState = RUN;
            }
            else
            {
            /* Intentionally Empty */
            }

            if ((manage_faultStateMerged_Byte1 != 0u) || (manage_faultStateMerged_Byte2 != 0u)|| (manage_failureStateMerged != 0u))
            {
                manageProtectionState = STOP;
            }
            else
            {
            /* Intentionally Empty */
            }
            
            break;
            
        default: //STOP
            
            //manage_errorCode = ((((uint16_t)manage_warningStateMerged & 0x02u) << 7u) | ((uint16_t)manage_failureStateMerged & 0xC0u) | ((uint16_t)manage_faultStateMerged & 0x3Fu));
            //manage_convertErrorCode = convertErrorCode(manage_errorCode);
            
            if ((manage_faultStateMerged_Byte1 == 0u) && (manage_faultStateMerged_Byte2 == 0u) && (manage_failureStateMerged == 0u))
            {          
                if (((manage_warningStateMerged & 0x14u) != 0u)) // WT WC
                {
                    manageProtectionState = DEGRADE;
                }
                else
                {
                    manageProtectionState = RUN;
                }
						}


			manage_IRV_SpeedRef = 0;
			manage_Prev_SpeedRef = 0;
			manage_IRV_Step_SpeedRef = 0;

            break;
    }

}

static void ManageRPMStepControl(void)
{
    ManageStepControlVariables();
    switch(manage_RPMStepControlState)
    {
        case STEPCONTROLOFF: ManageStepControlOff();
        break;
        case STEPCONTROLON: ManageStepControlOn();
        break;
        default:
        /* No action */
        break;
    }
	if(manage_RPMStepControlState == STEPCONTROLON)
    {
		(void)WriteSpeedRef(manage_IRV_Step_SpeedRef);
		(void)SetSpeedRef(manage_IRV_Step_SpeedRef);
    }
    else
    {
        (void)WriteSpeedRef(manage_IRV_SpeedRef);
        (void)SetSpeedRef(manage_IRV_SpeedRef);
    }
}


static void ManageStepControlVariables(void)
{
    (void)ReadSpeedFiltered(&manage_SpeedFiltered); 
	
    if(manage_IRV_SpeedRef > SPEED_MAX_RUN)
    {
        manage_IRV_SpeedRef = SPEED_MAX_RUN;
    }
    else
    {
        /* do nothing */
    }
    if(manage_IRV_SpeedRef >= manage_Prev_SpeedRef)
    {
        manage_Diff_SpeedRef = (manage_IRV_SpeedRef - manage_Prev_SpeedRef);
    }
    else
    {
        manage_Diff_SpeedRef = (manage_Prev_SpeedRef - manage_IRV_SpeedRef);
    }
}


static void ManageStepControlOff(void)
{
    if((manage_Diff_SpeedRef > 0))
    {
        manage_RPMStepControlState = STEPCONTROLON;
		
    }
    else
    {
        /* do nothing */
    }

    manage_IRV_Step_SpeedRef = manage_Prev_SpeedRef;

}


static void ManageStepControlOn(void)
{
    static sint16_t CAL_SPEED_STEP       = SPEED_STEP_60;
    static sint16_t Manage_ActSpeed = 0;
  
		(void)ReadSpeedFiltered(&Manage_ActSpeed);
	
	  /* STEP size decide*/
	  if(manage_Prev_SpeedRef == 0)
	  {
		    CAL_SPEED_STEP = SPEED_STEP_15;
	  }
	  else if(manage_IRV_SpeedRef > manage_IRV_Step_SpeedRef)
	  {
		    CAL_SPEED_STEP       = SPEED_STEP_300;
	  }
	  else if(manage_IRV_SpeedRef < manage_IRV_Step_SpeedRef)
	  {
		    if (manage_IRV_SpeedRef < 1000)
            { 
            CAL_SPEED_STEP = SPEED_STEP_15;
            }
		    else if (Manage_ActSpeed < 1500)
            {
            CAL_SPEED_STEP = SPEED_STEP_15;
            }
		    else
		    {
		        CAL_SPEED_STEP = SPEED_STEP_120;
		    }
	  }
    else
    {
			/* do nothing */
		}			
    if((manage_IRV_SpeedRef > manage_IRV_Step_SpeedRef))
    {
        if(manage_IRV_Step_SpeedRef < 15)
        {
            manage_IRV_Step_SpeedRef = 1000;
        }
        if (Emo_Status.MotorState == 2u)
				{				
        
				}
				else
				{
					manage_IRV_Step_SpeedRef = (((manage_IRV_Step_SpeedRef + CAL_SPEED_STEP) <= manage_IRV_SpeedRef)) ? (manage_IRV_Step_SpeedRef + CAL_SPEED_STEP) : manage_IRV_SpeedRef; // increase by 10 RPM
				}
		}
   	else if(manage_IRV_SpeedRef < manage_IRV_Step_SpeedRef)
    {
			  manage_IRV_Step_SpeedRef = (((manage_IRV_Step_SpeedRef - CAL_SPEED_STEP) >= manage_IRV_SpeedRef)) ? (manage_IRV_Step_SpeedRef - CAL_SPEED_STEP) : manage_IRV_SpeedRef; // Decrease by 10 RPM   
		}
	  else
	  {
		    manage_IRV_Step_SpeedRef = manage_IRV_SpeedRef;
	  }
    if(manage_IRV_Step_SpeedRef == manage_IRV_SpeedRef)
    {
        manage_RPMStepControlState = STEPCONTROLOFF;
        manage_Prev_SpeedRef = manage_IRV_SpeedRef;
    }
}



