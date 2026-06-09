/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : DiagServiceProcess.h
* Project         : iEWP
* Created         : 2018.09.20
* Description     : 
* 
*
************************************************************************************************************************
* Revison History:
* YYYY/MM/DD             Version              Description                      Author
* 2018/09/20             v1.0                 Draft                            Alex.kim
*
*
************************************************************************************************************************
*/


#ifndef DIAG_SERVICE_PROCESS_H
#define DIAG_SERVICE_PROCESS_H

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
#define MAX_DID_NUMBER 3u

/*
************************************************************************************************************************
*                                                         Typedefs
************************************************************************************************************************
*/
typedef uint8_t (*DidFunctionPointer)(uint8_t* mData, uint16_t mDataLength);

typedef enum{
    EEPROM_DID = 1,
    REALTIME_DID = 2,
    IO_DID = 3,
} DidAttr_t;

typedef enum{
    READONLY = 1,
    WRITEONLY = 2,
    READWRITE = 3,
} ReadWriteAttr_t;

typedef struct
{
    uint16_t Identifier;
    uint16_t DataLeanth;
    uint8_t*  DataPtr;
    DidAttr_t DidType;
    ReadWriteAttr_t DidRW;
    DidFunctionPointer DidCallBack;
} DidNode_t;



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
extern Std_ReturnType DiagService_AddDidForReadDataByIdentifier (uint16_t mDid, uint8_t* mDataPtr, uint16_t mDataLength, DidAttr_t mType, ReadWriteAttr_t mRW, DidFunctionPointer mCallBack);
extern Diag_NegativeResponseCodeType DiagService_UdsReadDataByIdentifier(const PduInfoType *pduRxData, PduInfoType *pduTxData);
extern Diag_NegativeResponseCodeType DiagService_UdsWriteDataByIdentifier(const PduInfoType *pduRxData, PduInfoType *pduTxData, const SessionCtrl_t *sessionCtrl);
extern Diag_NegativeResponseCodeType DiagService_UdsSessionControl(const PduInfoType *pduRxData, PduInfoType *pduTxData, SessionCtrl_t *sessionCtrl);
extern Diag_NegativeResponseCodeType DiagService_UdsTesterPresent(const PduInfoType *pduRxData, PduInfoType *pduTxData);

#endif    /* DIAG_SERVICE_PROCESS_H */

