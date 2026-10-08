/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : ExampleCode.h
* Project         : iEWP
* Created         : 2018.09.27
* Description     : 
* 
*
************************************************************************************************************************
* Revison History:
* YYYY/MM/DD             Version              Description                      Author
* 2018/09/27             v1.0                 Draft                            Alex.kim
*
*
************************************************************************************************************************
*/


#ifndef EXAMPLE_CODE_H
#define EXAMPLE_CODE_H

/*
************************************************************************************************************************
*                                                  Include header files
************************************************************************************************************************
*/
#include "DiagManager.h"


/*
************************************************************************************************************************
*                                                   Defines and macros
************************************************************************************************************************
*/
/* Diag_NegativeResponseCodeType */
typedef uint8_t Diag_FaultCodeType;
#define FAULT_E_SHORTCIRCUIT           ((Diag_FaultCodeType)0x01)
#define FAULT_E_GATEERROR              ((Diag_FaultCodeType)0x02)
#define FAULT_E_SPEEDERROR             ((Diag_FaultCodeType)0x03)
#define FAULT_E_OVERTEMPERATURE        ((Diag_FaultCodeType)0x04)
#define FAULT_E_WARNTEMPERATURE        ((Diag_FaultCodeType)0x05)
//#define FAULT_E_OVERCURRENT            ((Diag_FaultCodeType)0x06)
#define FAULT_E_UNDERVOLTAGE           ((Diag_FaultCodeType)0x07)
#define FAULT_E_OVERVOLTAGE            ((Diag_FaultCodeType)0x08)
#define FAULT_E_LINERROR               ((Diag_FaultCodeType)0x09)



#define UDS_BSIDID				  0xF180			// BootSoftware identification data identifier
//#define UDS_ASIDID				  0xF181			// Application software identification data identifier
//#define UDS_VMECUSVNDID	0xF189			// VehicleManufacturerECUSoftwareVersionNumberDataIdentifier
//#define UDS_RSCOTSNDID	 0xF198			// reportRepairShopCodeOrTesterSerialNumber
//#define UDS_PDDID				   0xF199			// reportProgrammingDate
#define UDS_CHECKSUM	   0xF1F0			// Identification option that means It is specified with CheckSum data
#define UDS_RESET_ERROR 0xF1F1   // Identification option that measn If this service is executed, Eeprom is erased.
//#define UDS_WRITE_CALIBRATION 0xF1F2   // Identification option that means it could be way to calibrate some factor
//#define UDS_READ_CALIBRATION  0xF1F3   // Identification option that means it could be way to read some factor calibrated.



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
extern Diag_FaultCodeType FailureCode[128];
extern uint8_t ItemVersion[16];
extern uint8_t HWVersion[3];
extern uint8_t BOOTVersion[3];
extern uint8_t DateInfo[10];
extern uint8_t OnLineCheckSum[4];
extern uint8_t CommandOfEeprom[3];
extern uint8_t CalibrationData[24];
extern uint8_t NvmEraseFlag;


/*
************************************************************************************************************************
*                                               Exported function prototypes
************************************************************************************************************************
*/
extern void MakeCheckSum (void);
extern uint8_t CallBack_BootSoftwareIdentifier (void);
//extern uint8_t CallBack_ApplicationSoftwareIdentifier (void);
//extern uint8_t CallBack_VehicleManufacturer (void);
//extern uint8_t CallBack_ReportTesterSerialNumber (void);
//extern uint8_t CallBack_ReportProgrammingDate (void);
extern uint8_t CallBack_ReportEcuCheckSum (void);
extern uint8_t CallBack_ReportEraseEeprom (const uint8_t* mData, uint16_t mDataLength);
//extern uint8_t CallBack_ReportCalibratingData (uint8_t* mData, uint16_t mDataLength);
//extern uint8_t CallBack_ReportDataCalibrated (void);
//extern void EraseNvm (void);


#endif    /* EXAMPLE_CODE_H */

