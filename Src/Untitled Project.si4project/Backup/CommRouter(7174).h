/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : CommRouter.h
* Project         : iEWP
* Created         : 2018.09.12
* Description     : 
* 
*
************************************************************************************************************************
* Revison History:
* YYYY/MM/DD             Version              Description                      Author
* 2018/09/12             v1.0                 Draft                            Alex.kim
*
*
************************************************************************************************************************
*/


#ifndef COMM_ROUTER_H
#define COMM_ROUTER_H

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
typedef enum {
    ROUTER_DEST_COMM_CAN = 0,
    ROUTER_DEST_COMM_LIN,
    ROUTER_DEST_COMM_FR,
    ROUTER_DEST_DIAG_CAN,
    ROUTER_DEST_DIAG_LIN,
    ROUTER_DEST_DIAG_FR,
    ROUTER_DEST_DIAG_J1939TP,
    ROUTER_DEST_END_OF_MODULES
} DestModuleType;

typedef enum {
    ROUTER_SOURCE_CANIF = 0,
    ROUTER_SOURCE_LINIF,
    ROUTER_SOURCE_FRIF,
    ROUTER_SOURCE_CANTP,
    ROUTER_SOURCE_LINTP,
    ROUTER_SOURCE_J1939TP,
    ROUTER_SOURCE_FRTP,
    ROUTER_SOURCE_END_OF_MODULES
} OutModuleType;

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
extern uint8_t CommRouter_Init (OutModuleType srcModule, uint8_t* requestBuf, uint8_t* responseBuf, uint8_t requestBufMaxLength);
extern void CommRouter_RxIndication (ProtocolAddrType addrType, uint16_t length, DestModuleType destModule, uint8_t* txConfirmation);
extern void CommRouter_TxIndication (DestModuleType destModule, const uint8_t* txConfirmation);
extern uint8_t CommRouter_Transmit (OutModuleType srcModule, uint16_t txBufferLength);




#endif    /* COMM_ROUTER_H */

