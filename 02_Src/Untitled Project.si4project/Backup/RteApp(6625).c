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
* 2018/02/08             v1.0                 Draft first version              Alex.kim
*
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                                   Include header files
************************************************************************************************************************
*/
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


/*
************************************************************************************************************************
*                                                     Local variables
************************************************************************************************************************
*/
static sint16_t  VdcMeasured;
static sint16_t  VdcFiltered;   
static sint16_t  IdMeasured;
static sint16_t  IqMeasured;
static sint16_t  IqFiltered;
static sint16_t  VdMeasured;
static sint16_t  VqMeasured;
static sint16_t  IdcMeasured;
static sint16_t  IdcFiltered;   
static sint16_t  SpeedRef;
static sint16_t  SpeedCmd;
static sint16_t  SpeedMax;
static sint16_t  SpeedRequired;
static sint16_t  SpeedMeasured;
static sint16_t  SpeedFiltered;
static sint16_t  TemperatureMeasured;      
static sint16_t  TemperatureFiltered;      
static DiagState_t HwShortCircuitState;
static DiagState_t HwGateDrvErrorState;
static DiagState_t LinTimeOutErrorState;
static DiagState_t SC_DiagState;
static DiagState_t GE_DiagState;
static DiagState_t SE_DiagState;
static DiagState_t OT_DiagState;
static DiagState_t OC_DiagState;
static DiagState_t LC_DiagState;
static DiagState_t OV_DiagState;
static DiagState_t UV_DiagState;
static DiagState_t LE_DiagState;
static sint16_t  IgnMeasured;
static sint16_t  IgnFiltered;   
static sint8_t   IgnFlag;
static uint8_t   FaultStateMerged;
static uint8_t   FailureStateMerged;
static uint8_t   WarningStateMerged;
static uint8_t   RetryStateMerged;
static uint8_t   ProtectionFlag;
static uint8_t   FaultFlag;
static DiagState_t HwOpenPhaseState;
static sint16_t    FluxAbsValueMeasured;
static sint16_t    FluxAbsValueFiltered;
static uint8_t   ErrorCodeIndex;
static uint8_t   CoolantFlag;


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

/** === VDC ======================================================================= */
/*Std_ReturnType ReadVdcMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = VdcMeasured;
    return retVal;
}*/


Std_ReturnType WriteVdcMeasured (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    VdcMeasured = data;
    return retVal;
}


Std_ReturnType WriteVdcFiltered (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    VdcFiltered = data;    
    return retVal;
}


Std_ReturnType ReadVdcFiltered (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = VdcFiltered;
    return retVal;
}

/** === Id current =================================================================== */
/*Std_ReturnType ReadIdMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = IdMeasured;
    return retVal;
}*/


Std_ReturnType WriteIdMeasured (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    IdMeasured = data;
    return retVal;
}


/** === Iq current =================================================================== */
/*Std_ReturnType ReadIqMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = IqMeasured;
    return retVal;
}*/


Std_ReturnType WriteIqMeasured (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    IqMeasured = data;
    return retVal;
}


Std_ReturnType WriteIqFiltered(sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    IqFiltered = data;    
    return retVal;
}


Std_ReturnType ReadIqFiltered (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = IqFiltered;
    return retVal;
}


/** === Idc current =================================================================== */
/*Std_ReturnType ReadIdcMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = IdcMeasured;
    return retVal;
}*/


Std_ReturnType WriteIdcMeasured (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    IdcMeasured = data;
    return retVal;
}


Std_ReturnType WriteIdcFiltered(sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    IdcFiltered = data;    
    return retVal;
}


Std_ReturnType ReadIdcFiltered (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = IdcFiltered;
    return retVal;
}


/** === Vd current =================================================================== */
/*Std_ReturnType ReadVdMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = VdMeasured;
    return retVal;
}*/


/*Std_ReturnType WriteVdMeasured (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    VdMeasured = data;
    return retVal;
}*/


/** === Vq current =================================================================== */
/*Std_ReturnType ReadVqMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = VqMeasured;
    return retVal;
}*/


/*Std_ReturnType WriteVqMeasured (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    VqMeasured = data;
    return retVal;
}*/


/** === reference Speed =================================================================== */
Std_ReturnType ReadSpeedRef (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = SpeedRef;
    return retVal;
}


Std_ReturnType WriteSpeedRef (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    SpeedRef = data;
    return retVal;
}


Std_ReturnType ReadSpeedCmd (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = SpeedCmd;
    return retVal;
}


Std_ReturnType WriteSpeedCmd (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    SpeedCmd = data;
    return retVal;
}


/*Std_ReturnType ReadSpeedMax (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = SpeedMax;
    return retVal;
}*/


/*Std_ReturnType WriteSpeedMax (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    SpeedMax = data;
    return retVal;
}*/


Std_ReturnType ReadSpeedRequired (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = SpeedRequired;
    return retVal;
}


Std_ReturnType WriteSpeedRequired (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    SpeedRequired = data;
    return retVal;
}


/** === Actual Speed =================================================================== */
/*Std_ReturnType ReadSpeedMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = SpeedMeasured;
    return retVal;
}*/


Std_ReturnType WriteSpeedMeasured (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    SpeedMeasured = data;
    return retVal;
}

Std_ReturnType ReadSpeedFiltered (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = SpeedFiltered;
    return retVal;
}


Std_ReturnType WriteSpeedFiltered (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    SpeedFiltered = data;
    return retVal;
}


/** === Temperature =================================================================== */
/*Std_ReturnType ReadTemperatureMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = TemperatureMeasured;
    return retVal;
}*/


Std_ReturnType WriteTemperatureMeasured (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    TemperatureMeasured = data;
    return retVal;
}


Std_ReturnType ReadTemperatureFiltered (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = TemperatureFiltered;
    return retVal;
}


Std_ReturnType WriteTemperatureFiltered (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    TemperatureFiltered = data;
    return retVal;
}


/** === ShortCircuit =================================================================== */
Std_ReturnType ReadHwShortCircuitState (DiagState_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = HwShortCircuitState;
    return retVal;
}


Std_ReturnType WriteHwShortCircuitState (DiagState_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    HwShortCircuitState = data;
    return retVal;
}


/** === GateDrvError =================================================================== */
Std_ReturnType ReadHwGateDrvErrorState (DiagState_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = HwGateDrvErrorState;
    return retVal;
}


Std_ReturnType WriteHwGateDrvErrorState (DiagState_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    HwGateDrvErrorState = data;
    return retVal;
}


/** === LinTimeOutError =================================================================== */
Std_ReturnType ReadLinTimeOutErrorState (DiagState_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = LinTimeOutErrorState;
    return retVal;
}


Std_ReturnType WriteLinTimeOutErrorState (DiagState_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    LinTimeOutErrorState = data;
    return retVal;
}


/** === Short circuit Diag State =========================================================== */
/*Std_ReturnType ReadShortCircuitState (DiagState_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = SC_DiagState;
    return retVal;
}*/


/*Std_ReturnType WriteShortCircuitState (DiagState_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    SC_DiagState = data;
    return retVal;
}*/


/** === Short circuit Diag State =========================================================== */
/*Std_ReturnType ReadGateErrorState (DiagState_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = GE_DiagState;
    return retVal;
}*/


/*Std_ReturnType WriteGateErrorState (DiagState_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    GE_DiagState = data;
    return retVal;
}*/


/** === Speed Error Diag State =========================================================== */
/*Std_ReturnType ReadSpeedErrorState (DiagState_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = SE_DiagState;
    return retVal;
}*/


/*Std_ReturnType WriteSpeedErrorState (DiagState_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    SE_DiagState = data;
    return retVal;
}*/


/** === Over Temperature Diag State =========================================================== */
/*Std_ReturnType ReadOverTemperatureState (DiagState_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = OT_DiagState;
    return retVal;
}*/


/*Std_ReturnType WriteOverTemperatureState (DiagState_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    OT_DiagState = data;
    return retVal;
}*/


/** === Over Current Diag State =========================================================== */
/*Std_ReturnType ReadOverCurrentState (DiagState_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = OC_DiagState;
    return retVal;
}*/


/*Std_ReturnType WriteOverCurrentState (DiagState_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    OC_DiagState = data;
    return retVal;
}*/


/** === Low current Diag State =========================================================== */
/*Std_ReturnType ReadLowCurrentState (DiagState_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = LC_DiagState;
    return retVal;
}*/


/*Std_ReturnType WriteLowCurrentState (DiagState_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    LC_DiagState = data;
    return retVal;
}*/


/** === Over Voltage Diag State =========================================================== */
/*Std_ReturnType ReadOverVoltageState (DiagState_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = OV_DiagState;
    return retVal;
}*/


/*Std_ReturnType WriteOverVoltageState (DiagState_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    OV_DiagState = data;
    return retVal;
}*/


/** === Under Voltage Diag State =========================================================== */
/*Std_ReturnType ReadUnderVoltageState (DiagState_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = UV_DiagState;
    return retVal;
}*/


/*Std_ReturnType WriteUnderVoltageState (DiagState_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    UV_DiagState = data;
    return retVal;
}*/


/** === Communication error Diag State =========================================================== */
/*Std_ReturnType ReadLinComunicationErrorState (DiagState_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = LE_DiagState;
    return retVal;
}*/


/*Std_ReturnType WriteLinComunicationErrorState (DiagState_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    LE_DiagState = data;
    return retVal;
}*/

/** === IGN ======================================================================= */
/*Std_ReturnType ReadIgnMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = IgnMeasured;
    return retVal;
}*/


Std_ReturnType WriteIgnMeasured (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    IgnMeasured = data;
    return retVal;
}


Std_ReturnType WriteIgnFiltered (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    IgnFiltered = data;    
    return retVal;
}


/*Std_ReturnType ReadIgnFiltered (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = IgnFiltered;
    return retVal;
}*/


Std_ReturnType WriteIgnFlag (sint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    IgnFlag = data;    
    return retVal;
}


Std_ReturnType ReadIgnFlag (sint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = IgnFlag;
    return retVal;
}


/** === Diag State result =============================================================== */
Std_ReturnType ReadFailureStateMerged (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = FailureStateMerged;
    return retVal;
}


Std_ReturnType WriteFailureStateMerged (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    FailureStateMerged = data;
    return retVal;
}


Std_ReturnType ReadFaultStateMerged (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = FaultStateMerged;
    return retVal;
}


Std_ReturnType WriteFaultStateMerged (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    FaultStateMerged = data;
    return retVal;
}


Std_ReturnType ReadWarningStateMerged (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = WarningStateMerged;
    return retVal;
}


Std_ReturnType WriteWarningStateMerged (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    WarningStateMerged = data;
    return retVal;
}


Std_ReturnType ReadRetryStateMerged (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = RetryStateMerged;
    return retVal;
}


Std_ReturnType WriteRetryStateMerged (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    RetryStateMerged = data;
    return retVal;
}


Std_ReturnType ReadProtectionFlag (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = ProtectionFlag;
    return retVal;
}


Std_ReturnType WriteProtectionFlag (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    ProtectionFlag = data;
    return retVal;
}


Std_ReturnType ReadFaultFlag (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = FaultFlag;
    return retVal;
}


Std_ReturnType WriteFaultFlag (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    FaultFlag = data;
    return retVal;
}


/** === OpenPhase =================================================================== */
Std_ReturnType ReadHwOpenPhaseState (DiagState_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = HwOpenPhaseState;
    return retVal;
}


Std_ReturnType WriteHwOpenPhaseState (DiagState_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    HwOpenPhaseState = data;
    return retVal;
}


/** === ResetModule =================================================================== */



/** === FluxAbsValue =================================================================== */
Std_ReturnType ReadFluxAbsValueMeasured (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = FluxAbsValueMeasured;
    return retVal;
}


Std_ReturnType WriteFluxAbsValueMeasured (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    FluxAbsValueMeasured = data;
    return retVal;
}

/*Std_ReturnType ReadFluxAbsValueFiltered (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = FluxAbsValueFiltered;
    return retVal;
}*/


/*Std_ReturnType WriteFluxAbsValueFiltered (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    FluxAbsValueFiltered = data;
    return retVal;
}*/


/*Std_ReturnType ReadErrorCodeIndex (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = ErrorCodeIndex;
    return retVal;
}*/


Std_ReturnType WriteErrorCodeIndex (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    ErrorCodeIndex = data;
    return retVal;
}


Std_ReturnType WriteCoolantFlag (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    CoolantFlag = data;
    return retVal;
}

Std_ReturnType ReadCoolantFlag (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = CoolantFlag;
    return retVal;
}


/*
************************************************************************************************************************
*                                                      Local functions
************************************************************************************************************************
*/











