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
#define MAX_DID_NUMBER 9

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

typedef struct DidNode
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
extern Diag_NegativeResponseCodeType DiagService_UdsReadDataByIdentifier(PduInfoType *pduRxData, PduInfoType *pduTxData, SessionCtrl_t *sessionCtrl);
extern Diag_NegativeResponseCodeType DiagService_UdsWriteDataByIdentifier(PduInfoType *pduRxData, PduInfoType *pduTxData, SessionCtrl_t *sessionCtrl);
extern Diag_NegativeResponseCodeType DiagService_UdsSessionControl(PduInfoType *pduRxData, PduInfoType *pduTxData, SessionCtrl_t *sessionCtrl);
extern Diag_NegativeResponseCodeType DiagService_UdsTesterPresent(PduInfoType *pduRxData, PduInfoType *pduTxData, SessionCtrl_t *sessionCtrl);

#endif    /* DIAG_SERVICE_PROCESS_H */

