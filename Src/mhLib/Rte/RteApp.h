/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : RteApp.h
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


#ifndef RTEAPP_H
#define RTEAPP_H

/*
************************************************************************************************************************
*                                                  Include header files
************************************************************************************************************************
*/
#include "Std_Types.h"
#include "Diag.h"
#include "Manage.h"

/*
************************************************************************************************************************
*                                                   Defines and macros
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                                         Typedefs
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                                    Exported variables
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                               Exported function prototypes
************************************************************************************************************************
*/
/** === VDC ======================================================================= */
//Std_ReturnType ReadVdcMeasured (sint16_t* data);
//Std_ReturnType WriteVdcMeasured (sint16_t data);
Std_ReturnType WriteVdcFiltered (sint16_t data);
Std_ReturnType ReadVdcFiltered (sint16_t* data);
/** === Id current =================================================================== */
//Std_ReturnType ReadIdMeasured (sint16_t* data);
//Std_ReturnType WriteIdMeasured (sint16_t data);
/** === Iq current =================================================================== */
//Std_ReturnType ReadIqMeasured (sint16_t* data);
//Std_ReturnType WriteIqMeasured (sint16_t data);
Std_ReturnType WriteIqFiltered(sint16_t data);
Std_ReturnType ReadIqFiltered (sint16_t* data);
/** === Idc current =================================================================== */
//Std_ReturnType ReadIdcMeasured (sint16_t* data);
//Std_ReturnType WriteIdcMeasured (sint16_t data);
Std_ReturnType WriteIdcFiltered(sint16_t data);
Std_ReturnType ReadIdcFiltered (sint16_t* data);
/** === Vd current =================================================================== */
//Std_ReturnType ReadVdMeasured (sint16_t* data);
//Std_ReturnType WriteVdMeasured (sint16_t data);
/** === Vq current =================================================================== */
//Std_ReturnType ReadVqMeasured (sint16_t* data);
//Std_ReturnType WriteVqMeasured (sint16_t data);
/** === reference Speed =================================================================== */
Std_ReturnType ReadSpeedRef (sint16_t* data);
Std_ReturnType WriteSpeedRef (sint16_t data);
Std_ReturnType ReadSpeedCmd (sint16_t* data);
Std_ReturnType WriteSpeedCmd (sint16_t data);
//Std_ReturnType ReadSpeedMax (sint16_t* data);
//Std_ReturnType WriteSpeedMax (sint16_t data);
//Std_ReturnType ReadSpeedRequired (sint16_t* data);
//Std_ReturnType WriteSpeedRequired (sint16_t data);
/** === Actual Speed =================================================================== */
//Std_ReturnType ReadSpeedMeasured (sint16_t* data);
//Std_ReturnType WriteSpeedMeasured (sint16_t data);
Std_ReturnType ReadSpeedFiltered (sint16_t* data);
Std_ReturnType WriteSpeedFiltered (sint16_t data);
/** === Temperature =================================================================== */
//Std_ReturnType ReadTemperatureMeasured (sint16_t* data);
//Std_ReturnType WriteTemperatureMeasured (sint16_t data);
Std_ReturnType ReadTemperatureFiltered (sint16_t* data);
Std_ReturnType WriteTemperatureFiltered (sint16_t data);
/** === ShortCircuit =================================================================== */
Std_ReturnType ReadHwShortCircuitState (DiagState_t* data);
Std_ReturnType WriteHwShortCircuitState (DiagState_t data);
/** === GateDrvError =================================================================== */
Std_ReturnType ReadHwGateDrvErrorState (DiagState_t* data);
Std_ReturnType WriteHwGateDrvErrorState (DiagState_t data);
/** === LinTimeOutError =================================================================== */
Std_ReturnType ReadLinTimeOutErrorState (DiagState_t* data);
Std_ReturnType WriteLinTimeOutErrorState (DiagState_t data);
Std_ReturnType ReadLinTimeOutFlag(uint8_t* data);
Std_ReturnType WriteLinTimeOutFlag(uint8_t data);



/** === Short circuit Diag State =========================================================== */
//Std_ReturnType ReadShortCircuitState (DiagState_t* data);
//Std_ReturnType WriteShortCircuitState (DiagState_t data);
/** === Short circuit Diag State =========================================================== */
//Std_ReturnType ReadGateErrorState (DiagState_t* data);
//Std_ReturnType WriteGateErrorState (DiagState_t data);
/** === Speed Error Diag State =========================================================== */
//Std_ReturnType ReadSpeedErrorState (DiagState_t* data);
//Std_ReturnType WriteSpeedErrorState (DiagState_t data);
/** === Over Temperature Diag State =========================================================== */
//Std_ReturnType ReadOverTemperatureState (DiagState_t* data);
//Std_ReturnType WriteOverTemperatureState (DiagState_t data);
/** === Over Current Diag State =========================================================== */
//Std_ReturnType ReadOverCurrentState (DiagState_t* data);
//Std_ReturnType WriteOverCurrentState (DiagState_t data);
/** === Low current Diag State =========================================================== */
//Std_ReturnType ReadLowCurrentState (DiagState_t* data);
//Std_ReturnType WriteLowCurrentState (DiagState_t data);
/** === Over Voltage Diag State =========================================================== */
//Std_ReturnType ReadOverVoltageState (DiagState_t* data);
//Std_ReturnType WriteOverVoltageState (DiagState_t data);
/** === Under Voltage Diag State =========================================================== */
//Std_ReturnType ReadUnderVoltageState (DiagState_t* data);
//Std_ReturnType WriteUnderVoltageState (DiagState_t data);
/** === Communication error Diag State =========================================================== */
//Std_ReturnType ReadLinComunicationErrorState (DiagState_t* data);
//Std_ReturnType WriteLinComunicationErrorState (DiagState_t data);
/** === IGN ======================================================================= */
//Std_ReturnType ReadIgnMeasured (sint16_t* data);
//Std_ReturnType WriteIgnMeasured (sint16_t data);
//Std_ReturnType WriteIgnFiltered (sint16_t data);
//Std_ReturnType ReadIgnFiltered (sint16_t* data);
Std_ReturnType WriteIgnFlag (sint8_t data);
Std_ReturnType ReadIgnFlag (sint8_t* data);
/** === Diag State result =============================================================== */
Std_ReturnType ReadFailureStateMerged (uint8_t* data);
Std_ReturnType WriteFailureStateMerged (uint8_t data);
Std_ReturnType ReadFaultStateMerged_Byte1 (uint8_t* data);
Std_ReturnType WriteFaultStateMerged_Byte1 (uint8_t data);
Std_ReturnType ReadFaultStateMerged_Byte2 (uint8_t* data);
Std_ReturnType WriteFaultStateMerged_Byte2 (uint8_t data);
Std_ReturnType ReadLINFaultStateFlag_Byte1 (uint8_t* data);
Std_ReturnType WriteLINFaultStateFlag_Byte1 (uint8_t data);
Std_ReturnType ReadLINFaultStateFlag_Byte2 (uint8_t* data);
Std_ReturnType WriteLINFaultStateFlag_Byte2 (uint8_t data);
Std_ReturnType ReadWarningStateMerged (uint8_t* data);
Std_ReturnType WriteWarningStateMerged (uint8_t data);
Std_ReturnType ReadRetryStateMerged (uint8_t* data);
Std_ReturnType WriteRetryStateMerged (uint8_t data);
Std_ReturnType ReadProtectionFlag (uint8_t* data);
Std_ReturnType WriteProtectionFlag (uint8_t data);
Std_ReturnType ReadFaultFlag (uint8_t* data);
Std_ReturnType WriteFaultFlag (uint8_t data);
//Std_ReturnType ReadPowerLimitFlag (uint8_t* data);
//Std_ReturnType WritePowerLimitFlag (uint8_t data);

/** === OpenPhase =================================================================== */
Std_ReturnType ReadHwOpenPhaseState (DiagState_t* data);
Std_ReturnType WriteHwOpenPhaseState (DiagState_t data);
/** === ResetModule =================================================================== */



/** === FluxAbsValue =================================================================== */
Std_ReturnType ReadFluxAbsValueMeasured (uint16_t* data);
Std_ReturnType WriteFluxAbsValueMeasured (uint16_t data);
//Std_ReturnType ReadFluxAbsValueFiltered (sint16_t* data);
//Std_ReturnType WriteFluxAbsValueFiltered (sint16_t data);
//Std_ReturnType ReadErrorCodeIndex (uint8_t* data);
//Std_ReturnType WriteErrorCodeIndex (uint8_t data);
Std_ReturnType ReadCoolantFlag (uint8_t* data);
Std_ReturnType WriteCoolantFlag (uint8_t data);
//Std_ReturnType WritePowerLimit (uint8_t data);
//Std_ReturnType ReadPowerLimit (uint8_t* data);
Std_ReturnType WriteInitialFlag (uint8_t data);
Std_ReturnType ReadInitialFlag (uint8_t* data);
Std_ReturnType WriteLinRequireDuty (uint8_t data);
Std_ReturnType ReadLinRequireDuty (uint8_t* data);
Std_ReturnType WriteConvertRPM (sint16_t data);
Std_ReturnType ReadConvertRPM (sint16_t* data);
Std_ReturnType WriteConvertActDuty (uint8_t data);
Std_ReturnType ReadConvertActDuty (uint8_t* data);

Std_ReturnType WriteMCUFault (uint8_t data);
Std_ReturnType ReadMCUFault (uint8_t* data);





#endif    /* RTEAPP_H */



