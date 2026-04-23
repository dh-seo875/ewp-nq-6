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

static sint16_t  VdcFiltered;   
static sint16_t  IqFiltered;
static sint16_t  IdcFiltered;   
static sint16_t  SpeedRef;
static sint16_t  SpeedCmd;
//static sint16_t  SpeedMax;
static sint16_t  SpeedFiltered;
static sint16_t  TemperatureFiltered;      
static DiagState_t HwShortCircuitState;
static DiagState_t HwGateDrvErrorState;
static DiagState_t LinTimeOutErrorState;
static uint8_t     LinTimeOutFlag;

static sint8_t   IgnFlag;
static uint8_t   FaultStateMerged_Byte1;
static uint8_t   FaultStateMerged_Byte2;
static uint8_t   FaultStateflag_Byte1;
static uint8_t   FaultStateflag_Byte2;
static uint8_t   FailureStateMerged;
static uint8_t   WarningStateMerged;
static uint8_t   ProtectionFlag;
static uint8_t   FaultFlag;
static DiagState_t HwOpenPhaseState;
static sint16_t    FluxAbsValueMeasured;
static uint8_t   CoolantFlag;
//static uint8_t	 ErrorCodeIndex;
//static uint8_t Power_Limit;
static uint8_t InitialFlag;
static uint8_t LinRequireDuty;
static sint16_t ConvertRPM;
static uint8_t ConvertDuty;
static uint8_t MCUFault;




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
//Std_ReturnType ReadVdcMeasured (sint16_t* data)
//{
//    Std_ReturnType retVal = RTE_E_OK;
//    *data = VdcMeasured;
//    return retVal;
//}


//Std_ReturnType WriteVdcMeasured (sint16_t data)
//{
//	static sint16_t  VdcMeasured;

//    Std_ReturnType retVal = RTE_E_OK;
//    VdcMeasured = data;
//    return retVal;
//}


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
//Std_ReturnType ReadIdMeasured (sint16_t* data)
//{
//    Std_ReturnType retVal = RTE_E_OK;
//    *data = IdMeasured;
//    return retVal;
//}


//Std_ReturnType WriteIdMeasured (sint16_t data)
//{
//	static sint16_t  IdMeasured;

//    Std_ReturnType retVal = RTE_E_OK;
//    IdMeasured = data;
//    return retVal;
//}


/** === Iq current =================================================================== */
//Std_ReturnType ReadIqMeasured (sint16_t* data)
//{
//    Std_ReturnType retVal = RTE_E_OK;
//    *data = IqMeasured;
//    return retVal;
//}


//Std_ReturnType WriteIqMeasured (sint16_t data)
//{
//	static sint16_t  IqMeasured;

//    Std_ReturnType retVal = RTE_E_OK;
//    IqMeasured = data;
//    return retVal;
//}


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
//Std_ReturnType ReadIdcMeasured (sint16_t* data)
//{
//    Std_ReturnType retVal = RTE_E_OK;
//    *data = IdcMeasured;
//    return retVal;
//}


//Std_ReturnType WriteIdcMeasured (sint16_t data)
//{
//	static sint16_t  IdcMeasured;

//    Std_ReturnType retVal = RTE_E_OK;
//    IdcMeasured = data;
//    return retVal;
//}


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


Std_ReturnType ReadSpeedCmd (sint16_t* data) // iq control or speed control
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = SpeedCmd;
    return retVal;
}


Std_ReturnType WriteSpeedCmd (sint16_t data) // iq control or speed control
{
    Std_ReturnType retVal = RTE_E_OK;
    SpeedCmd = data;
    return retVal;
}


Std_ReturnType ReadLinRequireDuty (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = LinRequireDuty;
    return retVal;
}


Std_ReturnType WriteLinRequireDuty (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    LinRequireDuty = data;
    return retVal;
}

Std_ReturnType ReadConvertRPM (sint16_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = ConvertRPM;
    return retVal;
}


Std_ReturnType WriteConvertRPM (sint16_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    ConvertRPM = data;
    return retVal;
}

Std_ReturnType ReadConvertActDuty (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = ConvertDuty;
    return retVal;
}


Std_ReturnType WriteConvertActDuty (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    ConvertDuty = data;
    return retVal;
}


/** === Actual Speed =================================================================== */
//Std_ReturnType ReadSpeedMeasured (sint16_t* data)
//{
//    Std_ReturnType retVal = RTE_E_OK;
//    *data = SpeedMeasured;
//    return retVal;
//}


//Std_ReturnType WriteSpeedMeasured (sint16_t data)
//{

//	static sint16_t  SpeedMeasured;

//    Std_ReturnType retVal = RTE_E_OK;
//    SpeedMeasured = data;
//    return retVal;
//}

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


//Std_ReturnType WriteTemperatureMeasured (sint16_t data)
//{
//	static sint16_t  TemperatureMeasured;	   

//    Std_ReturnType retVal = RTE_E_OK;
//    TemperatureMeasured = data;
//    return retVal;
//}


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

Std_ReturnType WriteLinTimeOutFlag(uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    LinTimeOutFlag = data;    
    return retVal;
}


Std_ReturnType ReadLinTimeOutFlag (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = LinTimeOutFlag;
    return retVal;
}




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


//Std_ReturnType WriteIgnMeasured (sint16_t data)
//{
//	static sint16_t  IgnMeasured;

//    Std_ReturnType retVal = RTE_E_OK;
//    IgnMeasured = data;
//    return retVal;
//}


//Std_ReturnType WriteIgnFiltered (sint16_t data)
//{
//	static sint16_t  IgnFiltered;	

//    Std_ReturnType retVal = RTE_E_OK;
//    IgnFiltered = data;    
//    return retVal;
//}


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


Std_ReturnType ReadFaultStateMerged_Byte1 (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = FaultStateMerged_Byte1;
    return retVal;
}


Std_ReturnType WriteFaultStateMerged_Byte1 (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    FaultStateMerged_Byte1 = data;
    return retVal;
}

Std_ReturnType ReadFaultStateMerged_Byte2 (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = FaultStateMerged_Byte2;
    return retVal;
}


Std_ReturnType WriteFaultStateMerged_Byte2 (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    FaultStateMerged_Byte2 = data;
    return retVal;
}


Std_ReturnType ReadLINFaultStateFlag_Byte1 (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = FaultStateflag_Byte1;
    return retVal;
}


Std_ReturnType WriteLINFaultStateFlag_Byte1 (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    FaultStateflag_Byte1 = data;
    return retVal;
}

Std_ReturnType ReadLINFaultStateFlag_Byte2 (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = FaultStateflag_Byte2;
    return retVal;
}


Std_ReturnType WriteLINFaultStateFlag_Byte2 (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    FaultStateflag_Byte2 = data;
    return retVal;
}



//Std_ReturnType ReadPowerLimitFlag (uint8_t* data)
//{
//    Std_ReturnType retVal = RTE_E_OK;
//    *data = PowerLimitFlag;
//    return retVal;
//}


//Std_ReturnType WritePowerLimitFlag (uint8_t data)
//{
//    Std_ReturnType retVal = RTE_E_OK;
//    PowerLimitFlag = data;
//    return retVal;
//}



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


//Std_ReturnType ReadErrorCodeIndex (uint8_t* data)
//{
//    Std_ReturnType retVal = RTE_E_OK;
//    *data = ErrorCodeIndex;
//    return retVal;
//}


//Std_ReturnType WriteErrorCodeIndex (uint8_t data)
//{
//    Std_ReturnType retVal = RTE_E_OK;
//    ErrorCodeIndex = data;
//    return retVal;
//}


//Std_ReturnType ReadPowerLimit (uint8_t* data)
//{
//    Std_ReturnType retVal = RTE_E_OK;
//    *data = Power_Limit;
//    return retVal;
//}


//Std_ReturnType WritePowerLimit (uint8_t data)
//{
//    Std_ReturnType retVal = RTE_E_OK;
//    Power_Limit = data;
//    return retVal;
//}


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


Std_ReturnType WriteInitialFlag (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    InitialFlag = data;
    return retVal;
}

Std_ReturnType ReadInitialFlag (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = InitialFlag;
    return retVal;
}



Std_ReturnType WriteMCUFault (uint8_t data)
{
    Std_ReturnType retVal = RTE_E_OK;
    MCUFault = data;
    return retVal;
}

Std_ReturnType ReadMCUFault (uint8_t* data)
{
    Std_ReturnType retVal = RTE_E_OK;
    *data = MCUFault;
    return retVal;
}




/*
************************************************************************************************************************
*                                                      Local functions
************************************************************************************************************************
*/











