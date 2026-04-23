/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : DiagManager.c
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
#include "DiagManager.h"
#include "CommRouter.h"
#include "DiagServiceProcess.h"
#include "util.h"


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


static DiagGlobalType DiagInfo;

/*
************************************************************************************************************************
*                                                     Local variables
************************************************************************************************************************
*/
MsgDataType MsgData;
static boolean_t RxDataIndication = FALSE;
static boolean_t TxDataIndication = FALSE;
static Diag_SID_Type CurrentSrvId = 0;

/*
************************************************************************************************************************
*                                                 Local function prototypes
************************************************************************************************************************
*/
static void DiagManager_InitRegister (DiagGlobalType* ctrlInfo);
static void DiagManager_TimingControl (DiagGlobalType* ctrlInfo, const MsgDataType* dataInfo);
static void DiagManager_MessageControl (const DiagGlobalType* ctrlInfo,const MsgDataType* dataInfo);
static void DiagManager_ServiceControl (DiagGlobalType* ctrlInfo, MsgDataType* dataInfo);
static void DiagManager_DataRxIndication (ProtocolAddrType addrType, uint16_t length,const uint8_t* txConfirmation);
static boolean_t DiagManager_SendResponse (uint16_t txDataLength);
static void DiagManager_SendNrc (Diag_NegativeResponseCodeType responseCode);


/*
************************************************************************************************************************
*                                                    Exported functions
************************************************************************************************************************
*/
void DiagManager_Init (void)
{
	static uint16_t DiagPhysicalTxBufferMaxLength = DiagBufferMax;
	static uint16_t DiagPhysicalRxBufferMaxLength = DiagBufferMax;
	static uint8_t DiagPhysicalTxBuffer[DiagBufferMax];
	static uint8_t DiagPhysicalRxBuffer[DiagBufferMax];
    DiagInfo.initRun = FALSE;
    DiagInfo.DataPtr.Tp_RxDataPtr = DiagPhysicalRxBuffer;
    DiagInfo.DataPtr.Tp_RxDataMaxLengthPtr = &DiagPhysicalRxBufferMaxLength;
    DiagInfo.DataPtr.Tp_TxDataPtr = DiagPhysicalTxBuffer;
    DiagInfo.DataPtr.Tp_TxDataMaxLengthPtr = &DiagPhysicalTxBufferMaxLength;
    DiagInfo.sessionCtrl.Session = DIAG_DEFAULT_SESSION;
    DiagInfo.sessionCtrl.sessionPending = FALSE;    
    DiagInfo.P2_TimeOutCnt = 0;
    DiagInfo.P2_ServerTimeOutCntMax = P2_SERVER_MAX;    

    MsgData.pduRxData.SduDataPtr = DiagInfo.DataPtr.Tp_RxDataPtr;
    MsgData.pduRxData.SduLength  = 0; 
    MsgData.pduTxData.SduDataPtr = DiagInfo.DataPtr.Tp_TxDataPtr;
    MsgData.pduTxData.SduLength  = 0;
    MsgData.addrType  = DIAG_PROTOCOL_NORMAL_ADDR_TYPE; 

    if((CommRouter_Init(ROUTER_SOURCE_LINTP, MsgData.pduRxData.SduDataPtr, MsgData.pduTxData.SduDataPtr, DiagBufferMax)) == 1u)
    {
        DiagInfo.initRun = TRUE;
    }
}


void DiagManager_AddParameterOfDataIdentifier (uint16_t mDid, uint8_t* mDataPtr, uint16_t mDataLength, const FunctionPointer ptrFunction)
{
    (void)DiagService_AddDidForReadDataByIdentifier (mDid, mDataPtr, mDataLength, REALTIME_DID, READONLY, (DidFunctionPointer)ptrFunction);
}


void DiagManager_MainFunction (void)
{
    /* we needs a cyclic time as 1ms. */
    DiagManager_TimingControl (&DiagInfo, &MsgData);
    DiagManager_MessageControl (&DiagInfo, &MsgData);
    DiagManager_ServiceControl (&DiagInfo, &MsgData);
}


void DiagManager_RxIndicationFromRouter (ProtocolAddrType addrType, uint16_t length, const uint8_t* txConfirmation)
{
    DiagManager_DataRxIndication (addrType, length, txConfirmation);
}

void DiagManager_TxIndicationFromRouter (const uint8_t* txConfirmation)
{
    /* complete or init transmit data */
    TxDataIndication = *txConfirmation;
}

/*
************************************************************************************************************************
*                                                      Local functions
************************************************************************************************************************
*/
static void DiagManager_InitRegister (DiagGlobalType* ctrlInfo)
{
    RxDataIndication = FALSE;
    TxDataIndication = FALSE;
    CurrentSrvId = 0;
    ctrlInfo->sessionCtrl.Session = DIAG_DEFAULT_SESSION;
    ctrlInfo->sessionCtrl.sessionPending = FALSE;    
    ctrlInfo->P2_TimeOutCnt = 0;
    ctrlInfo->P2_ServerTimeOutCntMax = P2_SERVER_MAX;        
}


static void DiagManager_TimingControl (DiagGlobalType* ctrlInfo, const MsgDataType* dataInfo)
{
 	uint8_t access = 1u;

	(void)dataInfo;
	
    if (ctrlInfo->initRun != TRUE)
    {
		access = 0u;
	}
	
	if(access == 1u)
	{
    /* It should be incremented every 1ms. */
	    ctrlInfo->P2_TimeOutCnt++;

	    if (RxDataIndication == TRUE)
	    {
	        /* Whenever we've got a normal message, It should be cleared. */
	        ctrlInfo->P2_TimeOutCnt = 0;
	    }

	    if (ctrlInfo->P2_TimeOutCnt > ctrlInfo->P2_ServerTimeOutCntMax)
	    {
	        /* init session and all service status. */
	        DiagManager_InitRegister (ctrlInfo);
	    }
	}
}

static void DiagManager_MessageControl (const DiagGlobalType* ctrlInfo, const MsgDataType* dataInfo)
{
	uint8_t access = 1u;
	uint8_t access1 = 1u;
    if (ctrlInfo->initRun != TRUE)
    {
        access = 0u;
    }
#ifdef NEVER
    if (ctrlInfo->sessionCtrl.Session == DIAG_PROGRAMMING_SESSION)
    {
        /* Ready to Branch Bootloader */
        if (TxDataIndication == FALSE)
        {
            /* completed to send response data */
            goto_reset();
        }
    }
#endif /* NEVER */
    if(access == 1u)
    {
	    if (RxDataIndication != TRUE)
	    {
	        access1 = 0u;
	    }
		
		if(access1 == 1u)
		{
		    if (dataInfo->pduRxData.SduLength != 0u)
		    {
		        /* extract service id from buffer */
		        CurrentSrvId = dataInfo->pduRxData.SduDataPtr[0];
		    }

		    if ((CurrentSrvId & 0x7Fu) < 0x40u)
		    {

		    }
		    else
		    {
		        /* not response */
		    }
		}
    }
}


static void DiagManager_ServiceControl (DiagGlobalType* ctrlInfo, MsgDataType* dataInfo)
{
	uint8_t access = 1u;
    Diag_NegativeResponseCodeType mValue = NRC_E_POSITIVERESPONSE;
    
    if (RxDataIndication != TRUE)
    {
        access = 0u;
    }

	if(access == 1u)
	{
	
    switch (CurrentSrvId)
    {
#ifdef DIAG_USE_SERVICE_DIAGNOSTICSESSIONCONTROL
        case SID_DIAGNOSTIC_SESSION_CONTROL:
            mValue = DiagService_UdsSessionControl(&dataInfo->pduRxData, &dataInfo->pduTxData, &ctrlInfo->sessionCtrl);        
        break;
#endif

#ifdef DIAG_USE_SERVICE_ECURESET
        case SID_ECU_RESET:
        break;
#endif

#if defined(USE_DTC) && defined(DIAG_USE_SERVICE_CLEARDIAGNOSTICINFORMATION)
        case SID_CLEAR_DIAGNOSTIC_INFORMATION:
        break;
#endif

#if defined(USE_DTC) && defined(DIAG_USE_SERVICE_READDTCINFORMATION)
        case SID_READ_DTC_INFORMATION:
        break;
#endif

#ifdef DIAG_USE_SERVICE_READDATABYIDENTIFIER
        case SID_READ_DATA_BY_IDENTIFIER:
            mValue = DiagService_UdsReadDataByIdentifier(&dataInfo->pduRxData, &dataInfo->pduTxData, &ctrlInfo->sessionCtrl);
        break;
#endif

#ifdef DIAG_USE_SERVICE_READMEMORYBYADDRESS
        case SID_READ_MEMORY_BY_ADDRESS:
        break;        
#endif

#ifdef DIAG_USE_SERVICE_WRITEMEMORYBYADDRESS
        case SID_WRITE_MEMORY_BY_ADDRESS:
        break;        
#endif

#ifdef DIAG_USE_SERVICE_READSCALINGDATABYIDENTIFIER
        case SID_READ_SCALING_DATA_BY_IDENTIFIER:
        break;
#endif

#ifdef DIAG_USE_SERVICE_SECURITYACCESS
        case SID_SECURITY_ACCESS:
        break;
#endif

#ifdef DIAG_USE_SERVICE_WRITEDATABYIDENTIFIER
        case SID_WRITE_DATA_BY_IDENTIFIER:
            mValue = DiagService_UdsWriteDataByIdentifier(&dataInfo->pduRxData, &dataInfo->pduTxData, &ctrlInfo->sessionCtrl);        
        break;        
#endif

#ifdef DIAG_USE_SERVICE_ROUTINECONTROL
        case SID_ROUTINE_CONTROL:
        break;
#endif

#ifdef DIAG_USE_SERVICE_TESTERPRESENT
        case SID_TESTER_PRESENT:
            mValue = DiagService_UdsTesterPresent(&dataInfo->pduRxData, &dataInfo->pduTxData, &ctrlInfo->sessionCtrl);        
        break;
#endif

#if defined(USE_DTC) && defined(DIAG_USE_SERVICE_CONTROLDTCSETTING)
        case SID_CONTROL_DTC_SETTING:
        break;        
#endif

#ifdef DIAG_USE_SERVICE_READDATABYPERIODICIDENTIFIER
        case SID_READ_DATA_BY_PERIODIC_IDENTIFIER:
        break;
#endif

#ifdef DIAG_USE_SERVICE_DYNAMICALLYDEFINEDATAIDENTIFIER
        case SID_DYNAMICALLY_DEFINE_DATA_IDENTIFIER:
        break;
#endif

#ifdef DIAG_USE_SERVICE_INPUTOUTPUTCONTROLBYIDENTIFIER
        case SID_INPUT_OUTPUT_CONTROL_BY_IDENTIFIER:
        break;        
#endif

#ifdef DIAG_USE_SERVICE_COMMUNICATIONCONTROL
        case SID_COMMUNICATION_CONTROL:
        break;        
#endif

#ifdef DIAG_USE_SERVICE_REQUESTDOWNLOAD
        case SID_REQUEST_DOWNLOAD:
        break;
#endif

#ifdef DIAG_USE_SERVICE_REQUESTUPLOAD
        case SID_REQUEST_UPLOAD:
        break;
#endif

#ifdef DIAG_USE_SERVICE_TRANSFERDATA
        case SID_TRANSFER_DATA:
        break;        
#endif

#ifdef DIAG_USE_SERVICE_REQUESTTRANSFEREXIT
        case SID_REQUEST_TRANSFER_EXIT:
        break;        
#endif

        default: 
            mValue = NRC_E_SERVICENOTSUPPORTED;
        break;
    }

    if (mValue != NRC_E_POSITIVERESPONSE)
    {
        DiagManager_SendNrc(mValue);
    }
    else
    {
    
    }

    /* set response */
    (void)DiagManager_SendResponse(dataInfo->pduTxData.SduLength);

    /* complete to process a service */
    RxDataIndication = FALSE;
	}
}


static void DiagManager_DataRxIndication (ProtocolAddrType addrType, uint16_t length, const uint8_t* txConfirmation)
{
    if ((RxDataIndication == FALSE) && (*txConfirmation == FALSE))
    {
        /* Active to check service */
        RxDataIndication  = TRUE;
        MsgData.addrType = addrType;
        MsgData.pduRxData.SduLength = length;
    }
    else
    {
        /* not complete */
        
    }
}


static void DiagManager_SendNrc (Diag_NegativeResponseCodeType responseCode)
{
    if (!((MsgData.addrType == DIAG_PROTOCOL_FUNCTIONAL_ADDR_TYPE) && ((responseCode == NRC_E_SERVICENOTSUPPORTED) || 
          (responseCode == NRC_E_SUBFUNCTIONNOTSUPPORTED) || (responseCode == NRC_E_REQUESTOUTOFRANGE)))) {
        MsgData.pduTxData.SduDataPtr[0] = SID_NEGATIVE_RESPONSE;
        MsgData.pduTxData.SduDataPtr[1] = CurrentSrvId;
        MsgData.pduTxData.SduDataPtr[2] = responseCode;
        MsgData.pduTxData.SduLength = 3;
    }
}


static boolean_t DiagManager_SendResponse (uint16_t txDataLength)
{
    boolean_t retValue = TRUE;

    if (CommRouter_Transmit(ROUTER_SOURCE_LINTP, txDataLength) == FALSE)
    {
        /* fault reaction */
        retValue = FALSE;
    }
    else
    {
        /* start to transmit so this flag must be changed to TRUE. */
        TxDataIndication = TRUE;
    }

    return retValue;
}

