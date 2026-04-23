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
/*****************************************************************************/
/** BLDC: Motor Drive with Sensorless FOC for the QBL4208-61-04-013 motor   **/
/*****************************************************************************/
/** Use the RefSpeed received via LIN as reference speed for the BLDC Motor **/
/*****************************************************************************/
/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* 
 * V3.8.7: 2016-11-11, DM:   SVM update
 */
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Variant_cfg.h"
#include "tle_device.h"
#include "Main.h"
#include "scheduler.h"

#include "linmain.h"
#include "genLinConfig.h"
#include "lin_driver_api.h"
#include "Emo_RAM.h"
#include "RteStd.h"
#include "DiagManager.h"
#include "Usr.h"

#include "Sensor.h"
#include "Diag.h"
#include "Manage.h"



/* 
 * If you implement some feature which provide information to Scanning tool, 
 * You should delete below ExampleCode files such as .c, .h. 
 */
#include "ExampleCode.h"

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
static void Main_lStartMotor(void);
static void Main_lStopMotor(void);
static void CreateTask (void);
void Uart2_SendByte(unsigned char data);


/*******************************************************************************
**                      Globale Variable Definitions                          **
*******************************************************************************/
uint8 uart2byte;

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
int main(void)
{
    sint16_t main_speedRef = 0;
    uint8_t* main_ptrErrorCodePage;

    ChangeVectorTable();
    
    /* Initialize device drivers */
    /* Note: Watchdog is already initialized during startup! */
    TLE_Init();
    Emo_Init();

    /* Diagnostic mode init. It should be located in upper than LIN INIT. */
    DiagManager_Init();
    /* Lin Tp, Com init */
    LIN_LLD_Init();
    
    /* If you use the function that DiagManager_AddParameterOfDataIdentifier for read ErrorCode from PC program, must be used that function after check address via Nvm_Init */
    main_ptrErrorCodePage = Nvm_Init();
    
    /* regist service information */

    /*  1. make CheckSum code of application */
    MakeCheckSum();
    /*  2. regist UDS_CHECKSUM to ReadDataByIdentifier Service */
    DiagManager_AddParameterOfDataIdentifier(UDS_CHECKSUM, OnLineCheckSum, 4, CallBack_ReportEcuCheckSum);
    DiagManager_AddParameterOfDataIdentifier(UDS_BSIDID, main_ptrErrorCodePage, 128, CallBack_BootSoftwareIdentifier);
    DiagManager_AddParameterOfDataIdentifier(UDS_RESET_ERROR, CommandOfEeprom, 3, CallBack_ReportEraseEeprom);
	
    /* begin myunghwa software */
    schedulerInit();
    CreateTask();

    while (1)
    { 
        /* Watchdog Service */
        WDT1_Service();

        schedulerRun(schedulerGetTick());
        /* Find */
        CheckDiagnosis();

        GetSpeedRef(&main_speedRef);
        
        if (main_speedRef >= 500)
        {
            /* set speed as reference speed for the BLDC motor */            
            Emo_setspeedreferenz(main_speedRef);
            /* start the motor if not running yet */
            Main_lStartMotor();
        }
        else if (main_speedRef < 500)
        {            
            SetDiagnosisCurrentSource();
            ClearVsUnderVoltage();
            ClearShortCircuit();
            ClearGateDrvError();
            ClearOverCurrent();
            ClearVcpWarningStatus();
            ClearVddExtShortCircuitStatus();

            Emo_setspeedreferenz(main_speedRef);
            
            /* if speed is below Speed Min, stop the motor */
            Main_lStopMotor();
        }
        else
        {
        }
    }
}
/* End of main() */


/* In case the VCP supply fails the motor stops */
void BDRV_Diag_Supply(void)
{
    Main_lStopMotor();
}

/* in case an over current is detected on one of   **
** the MOSFETs then the motor will be switched off */
void BDRV_Diag(void)
{
    Main_lStopMotor();
}

void Uart2_RxInterrupt( void )
{
    uart2byte = UART2_Get_Byte();
    Uart2_SendByte(uart2byte);
}

void Uart2_SendByte(unsigned char data)
{
    int redefinedData = 0;

    redefinedData = data;
    ttywrch(redefinedData);
}

void Clear_VsUvStatus(void)
{
    ClearVsUnderVoltage();
}

uint8 Emo_OpenPhaseStatus(void)
{
    return bMotorOPFlag;
}

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
static void CreateTask (void)
{
    schedulerAddTask(Priority_1  , schedulerGetTick(),   INFINITE_LOOP,    1/* 0.25ms*/, (taskfuncptr)&LIN_LLD_Service);
    schedulerAddTask(Priority_2  , schedulerGetTick()+1, INFINITE_LOOP,   20/*    5ms*/, (taskfuncptr)&SensorSWComponentPack);
    schedulerAddTask(Priority_3  , schedulerGetTick()+3, INFINITE_LOOP,   40/*   10ms*/, (taskfuncptr)&DiagSWComponentPack);
    schedulerAddTask(Priority_4  , schedulerGetTick()+7, INFINITE_LOOP,   40/*   10ms*/, (taskfuncptr)&ManageSWComponentPack);
    schedulerAddTask(Priority_5  , schedulerGetTick()+9, INFINITE_LOOP,    4/*    1ms*/, (taskfuncptr)&DiagManager_MainFunction);
    schedulerAddTask(Priority_6  , schedulerGetTick(),	 INFINITE_LOOP,    1/* 0.25ms*/, (taskfuncptr)&ManageNvm);
}

static void Main_lStartMotor(void)
{
    uint32 Error;

    Error = EMO_ERROR_NONE;
    if(Error == EMO_ERROR_NONE)
    {
        Error = Emo_StartMotor((uint32)1u);
    }
    if(Error == EMO_ERROR_NONE)
    {
    }  
    else /* Error */     
    {
    
    }  
} /* End of Main_lStartMotor */


static void Main_lStopMotor(void)
{
    uint32 Error;
    
    Error = Emo_StopMotor(); 
    
    if(Error != EMO_ERROR_NONE)
    {
    
    }  
} /* End of Main_lStopMotor */

