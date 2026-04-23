/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : CommRouter.c
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


/*
************************************************************************************************************************
*                                                   Include header files
************************************************************************************************************************
*/
#include "linmain.h"
#include "CommRouter.h"


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
uint8_t CommRouter_Init (OutModuleType srcModule, uint8_t* requestBuf, uint8_t* responseBuf, uint8_t requestBufMaxLength)
{
    uint8_t retValue = TRUE;

    switch (srcModule)
    {
        case ROUTER_SOURCE_CANIF:
        break;

        case ROUTER_SOURCE_LINIF:
        break;
        
        case ROUTER_SOURCE_FRIF:
        break;

        case ROUTER_SOURCE_CANTP:
        break;

        case ROUTER_SOURCE_LINTP:
            if (LIN_TP_INIT(requestBuf, responseBuf, requestBufMaxLength) != TRUE)
            {
                retValue = FALSE;
            }
        break;

        case ROUTER_SOURCE_J1939TP:
        break;

        case ROUTER_SOURCE_FRTP:
        break;

        default:
            retValue = FALSE;
        break;
    }

    return retValue;
}


void CommRouter_RxIndication (ProtocolAddrType addrType, uint16_t length, DestModuleType destModule, uint8_t* txConfirmation)
{
    switch (destModule)
    {
        case ROUTER_DEST_COMM_CAN:
        break;

        case ROUTER_DEST_COMM_FR:
        break;
        
        case ROUTER_DEST_DIAG_CAN:
        break;

        case ROUTER_DEST_DIAG_LIN:
            DiagManager_RxIndicationFromRouter (addrType, length, txConfirmation);
        break;

        case ROUTER_DEST_DIAG_FR:
        break;

        case ROUTER_DEST_DIAG_J1939TP:
        break;

        default: /* no action */
        break;
    } 
}


void CommRouter_TxIndication (DestModuleType destModule, const uint8_t* txConfirmation)
{
    switch (destModule)
    {
        case ROUTER_DEST_COMM_CAN:
        break;

        case ROUTER_DEST_COMM_FR:
        break;
        
        case ROUTER_DEST_DIAG_CAN:
        break;

        case ROUTER_DEST_DIAG_LIN:
            DiagManager_TxIndicationFromRouter (txConfirmation);
        break;

        case ROUTER_DEST_DIAG_FR:
        break;

        case ROUTER_DEST_DIAG_J1939TP:
        break;

        default: /* no action */
        break;
    } 
}

uint8_t CommRouter_Transmit (OutModuleType srcModule, uint16_t txBufferLength)
{
    uint8_t retValue = TRUE;

    switch (srcModule)
    {
        case ROUTER_SOURCE_CANIF:
        break;

        case ROUTER_SOURCE_LINIF:
        break;
        
        case ROUTER_SOURCE_FRIF:
        break;

        case ROUTER_SOURCE_CANTP:
        break;

        case ROUTER_SOURCE_LINTP:
            if(l_callback_ReponseData(txBufferLength) != txBufferLength)
            {
                retValue = FALSE;
            }
        break;

        case ROUTER_SOURCE_J1939TP:
        break;

        case ROUTER_SOURCE_FRTP:
        break;

        default:
            retValue = FALSE;
        break;
    } 

    return retValue;
}


/*
************************************************************************************************************************
*                                                      Local functions
************************************************************************************************************************
*/




