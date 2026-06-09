/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : RteApp.c
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
#include "scu.h"
#include "adc2.h"
#include "Emo_interface.h"
#include "RteStd.h"
#include "Temperature.h"
#include "bdrv_interface.h"
#include "scupm_interface.h"
#include "pmu_interface.h"
#include "Nvm_interface.h"
#include "linmain.h"


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


/*
************************************************************************************************************************
*                                                 Local function prototypes
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                                    Exported functions
************************************************************************************************************************
*/
Std_ReturnType GetVdcMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = (sint16_t)ADC2_VS_Result_mV();
    return retVal;
}


//Std_ReturnType GetIdMeasured (sint16_t* data)
//{
//    Std_ReturnType retVal = RTE_E_OK;
//    *data = Emo_GetIdFixedPoint();
//    return retVal;
//}


Std_ReturnType GetIqMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = Emo_GetIqFixedPoint();
    return retVal;
}

//Std_ReturnType GetPowerLimitState (uint8_t* data)
//{
//    Std_ReturnType retVal = RTE_E_OK;
//    *data = Emo_GetPowereLimitStatePoint();
//    return retVal;
//}




/*Std_ReturnType GetVdMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = Emo_GetVdFixedPoint();
    return retVal;
}*/


/*Std_ReturnType GetVqMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = Emo_GetVqFixedPoint();
    return retVal;
}*/

Std_ReturnType GetIdcMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = (sint16_t)Emo_GetIdcDecimalPoint();
    return retVal;
}


Std_ReturnType GetSpeedRef (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = Emo_GetRefSpeedDecimalPoint();
    return retVal;
}

Std_ReturnType GetMotorState (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = Emo_GetMotorState();
    return retVal;
}

Std_ReturnType GetPhaseCurrA (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = Emo_GetPhaseCurrA();
    return retVal;
}

Std_ReturnType GetPhaseCurrB (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = Emo_GetPhaseCurrB();
    return retVal;
}

Std_ReturnType GetPhaseCurrC (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = Emo_GetPhaseCurrC();
    return retVal;
}
Std_ReturnType SetSpeedRef (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    Emo_SetSpeedDecimalPoint(data);
    return retVal;
}


Std_ReturnType GetSpeedMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = Emo_GetActSpeedDecimalPoint();
    return retVal;
}

Std_ReturnType GetTemperatureMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    (void)GetTemperatureDecimalPoint(data);
    return retVal;
}


/*Std_ReturnType GetShortCircuit (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    uint8_t mData = 0;
    mData |= (Bdrv_GetShortCircuitStatusOfHighSideDrv1()<<5);
    mData |= (Bdrv_GetShortCircuitStatusOfHighSideDrv2()<<4);
    mData |= (Bdrv_GetShortCircuitStatusOfHighSideDrv3()<<3);
    mData |= (Bdrv_GetShortCircuitStatusOfLowSideDrv1()<<2);
    mData |= (Bdrv_GetShortCircuitStatusOfLowSideDrv2()<<1);
    mData |= (Bdrv_GetShortCircuitStatusOfLowSideDrv3()<<0);
    *data = mData;
    return retVal;
}*/


Std_ReturnType ClearShortCircuit (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    (void)Scupm_ClearShortCircuitStatusFlagOfHighSideDrv1();
    (void)Scupm_ClearShortCircuitStatusFlagOfHighSideDrv2();
    (void)Scupm_ClearShortCircuitStatusFlagOfHighSideDrv3();
    (void)Scupm_ClearShortCircuitStatusFlagOfLowSideDrv1();
    (void)Scupm_ClearShortCircuitStatusFlagOfLowSideDrv2();
    (void)Scupm_ClearShortCircuitStatusFlagOfLowSideDrv3();    
    return retVal;
}


Std_ReturnType GetGateDrvError (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    uint8_t mData = 0;
    mData = ((Scupm_GetVsdLowthStatusOfBdrv() & 0x01u)<<1);
    mData |= (Scupm_GetVsdUpthStatusOfBdrv() & 0x01u);
    *data = mData;
    return retVal;
}

Std_ReturnType ClearGateDrvError (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    (void)Scupm_ClearVsdLowthStatusFlagOfBdrv();
    (void)Scupm_ClearVsdUpthStatusFlagOfBdrv();
    (void)Scupm_ClearVsdLowthInterruptStatusFlagOfBdrv();
    (void)Scupm_ClearVsdUpthInterruptStatusFlagOfBdrv();
    return retVal;
}


Std_ReturnType ClearVsUnderVoltage (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    (void)Scupm_ClearVsUnderVoltageStatusFlag();
    return retVal;
}


Std_ReturnType SetDiagnosisCurrentSource (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    (void)Bdrv_EnableDiagnosisCurrentSourceOfHighSideDrv1();
    (void)Bdrv_EnableDiagnosisCurrentSourceOfHighSideDrv2();
    (void)Bdrv_EnableDiagnosisCurrentSourceOfHighSideDrv3();
    (void)Bdrv_EnableDiagnosisCurrentSourceOfLowSideDrv1();
    (void)Bdrv_EnableDiagnosisCurrentSourceOfLowSideDrv2();
    (void)Bdrv_EnableDiagnosisCurrentSourceOfLowSideDrv3();
    return retVal;
}


Std_ReturnType GetOverCurrent (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    uint8_t mData = 0;
    mData = ((Bdrv_GetOverCurrentStatusOfHighSideDrv1() & 0x01u) <<5);
    mData |= ((Bdrv_GetOverCurrentStatusOfHighSideDrv2() & 0x01u) <<4);
    mData |= ((Bdrv_GetOverCurrentStatusOfHighSideDrv3() & 0x01u) <<3);
    mData |= ((Bdrv_GetOverCurrentStatusOfLowSideDrv1() & 0x01u) <<2);
    mData |= ((Bdrv_GetOverCurrentStatusOfLowSideDrv2() & 0x01u) <<1);
    mData |= (Bdrv_GetOverCurrentStatusOfLowSideDrv3() & 0x01u);
    *data = mData;
    return retVal;
}


Std_ReturnType ClearOverCurrent (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    (void)Scupm_ClearOverCurrentStatusFlagOfHighSideDrv1();
    (void)Scupm_ClearOverCurrentStatusFlagOfHighSideDrv2();
    (void)Scupm_ClearOverCurrentStatusFlagOfHighSideDrv3();
    (void)Scupm_ClearOverCurrentStatusFlagOfLowSideDrv1();
    (void)Scupm_ClearOverCurrentStatusFlagOfLowSideDrv2();
    (void)Scupm_ClearOverCurrentStatusFlagOfLowSideDrv3();    
    return retVal;
}


/*Std_ReturnType GetVcpWarningStatus (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    uint8_t mData = 0;
    mData |= (Scupm_GetVcpUpthStatusOfBdrv()<<2);
    mData |= (Scupm_GetVcpLowth1StatusOfBdrv()<<1);
    mData |= (Scupm_GetVcpLowth2StatusOfBdrv()<<0);
    *data = mData;
    return retVal;
}*/


Std_ReturnType ClearVcpWarningStatus (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    (void)Scupm_ClearVcpUpthStatusOfBdrv();
    (void)Scupm_ClearVcpLowth1StatusOfBdrv();
    (void)Scupm_ClearVcpLowth2StatusOfBdrv();
    (void)Scupm_ClearVcpUpthInterruptStatusOfBdrv();
    (void)Scupm_ClearVcpLowth1InterruptStatusOfBdrv();
    (void)Scupm_ClearVcpLowth2InterruptStatusOfBdrv();  
    return retVal;
}


//Std_ReturnType GetLinTimeOutError (uint8_t* data)
//{
//    Std_ReturnType retVal = RTE_E_OK;
//    *data = LIN_TimerOutError();
//    return retVal;
//}


Std_ReturnType GetIgnMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = (sint16_t)ADC2_MON_Result_mV();
    return retVal;
}


Std_ReturnType SetSleepMode (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCU_EnterSleepMode();
    return retVal;
}


//Std_ReturnType GetOpenPhase (uint8_t* data)
//{
//    Std_ReturnType retVal = RTE_E_OK;
//    *data = (uint8_t)Emo_OpenPhaseStatus();
//    return retVal;
//}


Std_ReturnType ClearVddExtShortCircuitStatus (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    (void)Pmu_ClearVddExtShortCircuit();
    return retVal;
}


Std_ReturnType GetFluxAbsValue (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = Emo_GetFluxAbsValue();
    return retVal;
}

/*Std_ReturnType GetErrorCode (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = Nvm_GetErrorCode();
    return retVal;
}*/

Std_ReturnType SetErrorCode (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    Nvm_SetErrorCode(data);
    return retVal;
}
//Std_ReturnType GetErrorCodeIndex (uint8_t* data)
//{
//    Std_ReturnType retVal = RTE_E_OK;
//    *data = Nvm_GetErrorCodeIndex();
//    return retVal;
//}

Std_ReturnType SetErrorCodeIndex (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    Nvm_SetErrorCodeIndex(data);
    return retVal;
}


/*
************************************************************************************************************************
*                                                      Local functions
************************************************************************************************************************
*/












