/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : RteHal.h
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


#ifndef RTEHAL_H
#define RTEHAL_H

/*
************************************************************************************************************************
*                                                  Include header files
************************************************************************************************************************
*/
#include "Std_Types.h"
#include "RteApp.h"


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
Std_ReturnType GetVdcMeasured (sint16_t* data);
//Std_ReturnType GetIdMeasured (sint16_t* data);
Std_ReturnType GetIqMeasured (sint16_t* data);
//Std_ReturnType GetVdMeasured (sint16_t* data);
//Std_ReturnType GetVqMeasured (sint16_t* data);
Std_ReturnType GetIdcMeasured (sint16_t* data);
Std_ReturnType GetSpeedRef (sint16_t* data);
Std_ReturnType SetSpeedRef (sint16_t data);
Std_ReturnType GetSpeedMeasured (sint16_t* data);
Std_ReturnType GetTemperatureMeasured (sint16_t* data);
//Std_ReturnType GetShortCircuit (uint8_t* data);
Std_ReturnType ClearShortCircuit (void);
Std_ReturnType GetGateDrvError (uint8_t* data);
Std_ReturnType ClearGateDrvError (void);
Std_ReturnType ClearVsUnderVoltage (void);
Std_ReturnType SetDiagnosisCurrentSource (void);
Std_ReturnType GetOverCurrent (uint8_t* data);
Std_ReturnType ClearOverCurrent (void);
//Std_ReturnType GetVcpWarningStatus (uint8_t* data);
Std_ReturnType ClearVcpWarningStatus (void);
/*
    value 0: no lin timeout error
    value 1: occurred lin timeout error
*/
//Std_ReturnType GetLinTimeOutError (uint8_t* data);
/*
    value 0: Keep with normal state
    value 1: be Started with being Coolant mode
*/
Std_ReturnType GetIgnMeasured (sint16_t* data);
Std_ReturnType SetSleepMode (void);
/*
    value 0: No Open Phase
    value 1: Open Phase
*/
//Std_ReturnType GetOpenPhase (uint8_t* data);

Std_ReturnType ClearVddExtShortCircuitStatus (void);

Std_ReturnType GetFluxAbsValue (sint16_t* data);

//Std_ReturnType GetErrorCode (uint8_t* data);
Std_ReturnType SetErrorCode (uint8_t data);
Std_ReturnType GetErrorCodeIndex (uint8_t* data);
Std_ReturnType SetErrorCodeIndex (uint8_t data);

//Std_ReturnType GetPowerLimitState (uint8_t* data);
Std_ReturnType GetMotorState (uint8_t* data);
Std_ReturnType GetPhaseCurrA (sint16_t* data);
Std_ReturnType GetPhaseCurrB (sint16_t* data);
Std_ReturnType GetPhaseCurrC (sint16_t* data);





#endif    /* RTEHAL_H */




