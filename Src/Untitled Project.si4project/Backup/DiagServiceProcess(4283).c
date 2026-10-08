/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : DiagServiceProcess.c
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


/*
************************************************************************************************************************
*                                                   Include header files
************************************************************************************************************************
*/
#include "DiagServiceProcess.h"


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
DidNode_t DIDS [MAX_DID_NUMBER];
uint8_t DidIndex = 0;


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
static uint8_t DiagService_SearchDid (uint16_t mDid, DidNode_t* mDidNode);


/*
************************************************************************************************************************
*                                                    Exported functions
************************************************************************************************************************
*/
Std_ReturnType DiagService_AddDidForReadDataByIdentifier (uint16_t mDid, uint8_t* mDataPtr, uint16_t mDataLength, DidAttr_t mType, ReadWriteAttr_t mRW, DidFunctionPointer mCallBack)
{
    Std_ReturnType mRetValue = E_OK;

    if (DidIndex == MAX_DID_NUMBER)
    {
        mRetValue = E_NOT_OK;
    }
    else if (DidIndex < MAX_DID_NUMBER)
    {
        DIDS[DidIndex].Identifier = mDid;
        DIDS[DidIndex].DataPtr = mDataPtr;
        DIDS[DidIndex].DataLeanth = (1 + 2 + mDataLength); /* Sid(1)+DID(2)+Data(x) except NAD and PCI */
        DIDS[DidIndex].DidType = mType;
        DIDS[DidIndex].DidRW = mRW;
        DIDS[DidIndex].DidCallBack = mCallBack;
        
        DidIndex++;
    }
    else
    {
        mRetValue = E_NOT_OK;
    }
    
    return mRetValue;
}


Diag_NegativeResponseCodeType DiagService_UdsReadDataByIdentifier(PduInfoType *pduRxData, PduInfoType *pduTxData, SessionCtrl_t *sessionCtrl)
{
    Diag_NegativeResponseCodeType retValueNrc = NRC_E_POSITIVERESPONSE;
    PduInfoType* mPduRxData = pduRxData;
    uint8_t mSid = 0;
    uint16_t mDid = 0;
    DidNode_t mDidNode;
    uint16_t i = 0;
    
    if(mPduRxData->SduLength != 3)
    {
        /* negative response */
        retValueNrc = NRC_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }
    else
    {
        mSid = mPduRxData->SduDataPtr[0];
        mDid = (uint16_t)((uint16_t)mPduRxData->SduDataPtr[1] << 8) | mPduRxData->SduDataPtr[2];
        if (DiagService_SearchDid(mDid, &mDidNode) != TRUE)
        {
            retValueNrc = NRC_E_SUBFUNCTIONNOTSUPPORTED;
        }
        else
        {
            pduTxData->SduLength = mDidNode.DataLeanth;
            pduTxData->SduDataPtr[0] = mSid | SID_RESPONSE_BIT;
            pduTxData->SduDataPtr[1] = mPduRxData->SduDataPtr[1];
            pduTxData->SduDataPtr[2] = mPduRxData->SduDataPtr[2];
            for (i = 0; i < mDidNode.DataLeanth; i++)
            {
                pduTxData->SduDataPtr[i+3] = mDidNode.DataPtr[i];
            }
    
            if(mDidNode.DidCallBack)
            {
                if (mDidNode.DidCallBack(&mPduRxData->SduDataPtr[3], (mPduRxData->SduLength-3)) == FALSE)
                {
                    retValueNrc = NRC_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                }
            }
        }
    }

    return retValueNrc;
}

Diag_NegativeResponseCodeType DiagService_UdsWriteDataByIdentifier(PduInfoType *pduRxData, PduInfoType *pduTxData, SessionCtrl_t *sessionCtrl)
{
    Diag_NegativeResponseCodeType retValueNrc = NRC_E_POSITIVERESPONSE;
    PduInfoType* mPduRxData = pduRxData;
    uint8_t mSid = 0;
    uint16_t mDid = 0;
    DidNode_t mDidNode;

    mSid = mPduRxData->SduDataPtr[0];
    mDid = (uint16_t)((uint16_t)mPduRxData->SduDataPtr[1] << 8) | mPduRxData->SduDataPtr[2];

    if (DiagService_SearchDid(mDid, &mDidNode) != TRUE)
    {
        retValueNrc = NRC_E_SUBFUNCTIONNOTSUPPORTED;
    }
    else
    {
        if (mDidNode.DataLeanth != mPduRxData->SduLength)
        {
            /* negative response */
            retValueNrc = NRC_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        }
        else
        {
            if(sessionCtrl->Session == DIAG_EXTENDED_DIAGNOSTIC_SESSION)
            {
                /* extended session */
                pduTxData->SduLength = 3; /* If it's positive reponse, Length must be 3. (sid+did) */
                pduTxData->SduDataPtr[0] = mSid | SID_RESPONSE_BIT;
                pduTxData->SduDataPtr[1] = mPduRxData->SduDataPtr[1];
                pduTxData->SduDataPtr[2] = mPduRxData->SduDataPtr[2];
                
                if(mDidNode.DidCallBack)
                {
                    if (mDidNode.DidCallBack(&mPduRxData->SduDataPtr[3], (mPduRxData->SduLength-3)) == FALSE)
                    {
                        retValueNrc = NRC_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                    }
                }
            }
            else
            {
                /* if there is command during default session, It will be negative response. */
                retValueNrc = NRC_E_SERVICENOTSUPPORTEDINACTIVESESSION;
            }
        }
    }
    
    return retValueNrc;
}


void DiagService_UdsInitSession (SessionCtrl_t *sessionCtrl)
{
    sessionCtrl->Session = DIAG_DEFAULT_SESSION;
    sessionCtrl->sessionPending = FALSE;
}

Diag_NegativeResponseCodeType DiagService_UdsSessionControl(PduInfoType *pduRxData, PduInfoType *pduTxData, SessionCtrl_t *sessionCtrl)
{
    Diag_NegativeResponseCodeType retValueNrc = NRC_E_POSITIVERESPONSE;
    PduInfoType* mPduRxData = pduRxData;
    SessionCtrl_t mSessionCtrl;
    uint8_t *mDataBootVersion;
    uint8_t mSid = 0;

    if(mPduRxData->SduLength != 2)
    {
        /* negative response */
        retValueNrc = NRC_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }
    else
    {
        mSid = mPduRxData->SduDataPtr[0];
        mSessionCtrl.Session = mPduRxData->SduDataPtr[1];

        if(mSessionCtrl.Session > DIAG_EXTENDED_DIAGNOSTIC_SESSION)
        {
            DiagService_UdsInitSession(sessionCtrl);
            /* negative response */
            retValueNrc = NRC_E_SUBFUNCTIONNOTSUPPORTED;
        }
        else if(mSessionCtrl.Session == DIAG_PROGRAMMING_SESSION)
        {
            DiagService_UdsInitSession(sessionCtrl);
            /* negative response */
            retValueNrc = NRC_E_SUBFUNCTIONNOTSUPPORTED;                  
#ifdef NEVER
            mDataBootVersion = (void *) 0x11000020U; /* It's a location of bootloader version string. */
            if (mDataBootVersion[0] != 'B')
            {
                DiagService_UdsInitSession(sessionCtrl);
                /* negative response */
                retValueNrc = NRC_E_REQUESTOUTOFRANGE;
            }
            else
            {
                /* Found bootloader.! */
                /* It's time to branch a bootloader */
                pduTxData->SduLength = 2;
                pduTxData->SduDataPtr[0] = mSid | SID_RESPONSE_BIT;
                pduTxData->SduDataPtr[1] = mSessionCtrl.Session;

                sessionCtrl->Session = mSessionCtrl.Session;
                sessionCtrl->sessionPending = TRUE;
            }
#endif /* NEVER */
        }
        else if(mSessionCtrl.Session == DIAG_EXTENDED_DIAGNOSTIC_SESSION)
        {
            pduTxData->SduLength = 2;
            pduTxData->SduDataPtr[0] = mSid | SID_RESPONSE_BIT;
            pduTxData->SduDataPtr[1] = mSessionCtrl.Session;

            sessionCtrl->Session = mSessionCtrl.Session;
            sessionCtrl->sessionPending = TRUE;
        }
        else if(mSessionCtrl.Session == DIAG_DEFAULT_SESSION)
        {
            DiagService_UdsInitSession(sessionCtrl);
            pduTxData->SduLength = 2;
            pduTxData->SduDataPtr[0] = mSid | SID_RESPONSE_BIT;
            pduTxData->SduDataPtr[1] = mSessionCtrl.Session;

            sessionCtrl->Session = mSessionCtrl.Session;
            sessionCtrl->sessionPending = TRUE;            
        }
        else
        {
            DiagService_UdsInitSession(sessionCtrl);
            /* negative response */
            retValueNrc = NRC_E_SUBFUNCTIONNOTSUPPORTED;        
        }
    }

    return retValueNrc;
}


Diag_NegativeResponseCodeType DiagService_UdsTesterPresent(PduInfoType *pduRxData, PduInfoType *pduTxData, SessionCtrl_t *sessionCtrl)
{
    Diag_NegativeResponseCodeType retValueNrc = NRC_E_POSITIVERESPONSE;
    PduInfoType* mPduRxData = pduRxData;
    uint8_t mSid = 0;

    if(mPduRxData->SduLength != 2)
    {
        /* negative response */
        retValueNrc = NRC_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }
    else
    {
        mSid = mPduRxData->SduDataPtr[0];
        /* "pduTxData->SduLength = 0" means reponse no data.*/
        pduTxData->SduLength = 2; 
        pduTxData->SduDataPtr[0] = mSid | SID_RESPONSE_BIT;
        pduTxData->SduDataPtr[1] = mPduRxData->SduDataPtr[1];
        
    }

    return retValueNrc;
}



/*
************************************************************************************************************************
*                                                      Local functions
************************************************************************************************************************
*/
static uint8_t DiagService_SearchDid (uint16_t mDid, DidNode_t* mDidNode)
{
    uint8_t retValue = FALSE;
    uint8_t i = 0;

    for (i = 0; i < (MAX_DID_NUMBER); i++)
    {
        if (DIDS[i].Identifier == mDid)
        {
            mDidNode->DataLeanth = DIDS[i].DataLeanth;
            mDidNode->DataPtr = DIDS[i].DataPtr;
            mDidNode->DidType = DIDS[i].DidType;
            mDidNode->DidRW = DIDS[i].DidRW;
            mDidNode->DidCallBack = DIDS[i].DidCallBack;

            retValue = TRUE;
            break;
        }
    }

    return retValue;
}



