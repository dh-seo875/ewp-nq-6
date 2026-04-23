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
static void CreateTask (void);
static void Task_10ms(void) ;

static void Uart2_SendByte(unsigned char data);

void BDRV_Diag_Supply(void)__attribute__((used));
void BDRV_Diag(void)__attribute__((used));
void Uart2_RxInterrupt( void );
void Clear_VsUvStatus(void);
static void Init_Main(void);


/*******************************************************************************
**                      Globale Variable Definitions                          **
*******************************************************************************/


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
	Init_Main();
	
    /* begin myunghwa software */
    schedulerInit();
    CreateTask();

    while (1)
    { 
        /* Watchdog Service */
        (void)WDT1_Service();

        schedulerRun(schedulerGetTick());

        /* Find */
        CheckDiagnosis();
    }
}
/* End of main() */


/* In case the VCP supply fails the motor stops */
void BDRV_Diag_Supply(void)/* PRQA S 1503 1 */ /* Used as callback via function pointer */
{
    (void)Emo_StopMotor(); 

}

/* in case an over current is detected on one of   **
** the MOSFETs then the motor will be switched off */
void BDRV_Diag(void) /* PRQA S 1503 1 */ /* Used as callback via function pointer */
{
    /* Deviation: This function is referenced indirectly via a macro. */
    /* Short Circuit  is mapped to BDRV_Diag and used in interrupt. */

    (void)Emo_StopMotor(); 

}

void Uart2_RxInterrupt( void )
{
	static uint8 uart2byte;
    uart2byte = UART2_Get_Byte();
    Uart2_SendByte(uart2byte);
}

static void Uart2_SendByte(unsigned char data)
{
    int redefinedData = 0;

    redefinedData = (int)data;
    ttywrch(redefinedData);
}

void Clear_VsUvStatus(void)
{
    (void)ClearVsUnderVoltage();
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
    (void)schedulerAddTask(Priority_1  , schedulerGetTick(),    INFINITE_LOOP,    1u/* 0.25ms*/, (taskfuncptr)&LIN_LLD_Service);
    (void)schedulerAddTask(Priority_2  , schedulerGetTick()+1u, INFINITE_LOOP,   40u/*   10ms*/, (taskfuncptr)&SensorSWComponentPack1);
    (void)schedulerAddTask(Priority_3  , schedulerGetTick()+5u, INFINITE_LOOP,   40u/*   10ms*/, (taskfuncptr)&SensorSWComponentPack2);	
    (void)schedulerAddTask(Priority_4  , schedulerGetTick()+9u, INFINITE_LOOP,   40u/*   10ms*/, (taskfuncptr)&DiagSWComponentPack1);
    (void)schedulerAddTask(Priority_5  , schedulerGetTick()+13u, INFINITE_LOOP,  40u/*   10ms*/, (taskfuncptr)&DiagSWComponentPack2);	
    (void)schedulerAddTask(Priority_6  , schedulerGetTick()+17u, INFINITE_LOOP,  40u/*   10ms*/, (taskfuncptr)&Task_10ms);
    (void)schedulerAddTask(Priority_7  , schedulerGetTick()+21u, INFINITE_LOOP,  40u/*   10ms*/, (taskfuncptr)&ManageNvm);
    (void)schedulerAddTask(Priority_8  , schedulerGetTick()+23u, INFINITE_LOOP,   4u/*   1ms*/, (taskfuncptr)&DiagManager_MainFunction);
}

static void Task_10ms(void) 
{
	ManageSWComponentPack();
	Task_Emo_MotorControl();
}

static void Init_Main(void) 
{
    uint8_t* main_ptrErrorCodePage;

    ChangeVectorTable();
    
    /* Initialize device drivers */
    /* Note: Watchdog is already initialized during startup! */
    TLE_Init1();
    TLE_Init2();	
    (void)Emo_Init();

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
		
    
}

