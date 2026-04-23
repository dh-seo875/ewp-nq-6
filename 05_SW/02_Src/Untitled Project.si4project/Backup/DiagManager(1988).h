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
#ifndef DIAG_MANAGER_H
#define DIAG_MANAGER_H

/*
************************************************************************************************************************
*                                                  Include header files
************************************************************************************************************************
*/
#include "Std_Types.h"

/*
************************************************************************************************************************
*                                                   Defines and macros
************************************************************************************************************************
*/

//#define USE_DTC
#define DIAG_USE_SERVICE_DIAGNOSTICSESSIONCONTROL
//#define DIAG_USE_SERVICE_ECURESET
//#define DIAG_USE_SERVICE_CLEARDIAGNOSTICINFORMATION
//#define DIAG_USE_SERVICE_READDTCINFORMATION
#define DIAG_USE_SERVICE_READDATABYIDENTIFIER
//#define DIAG_USE_SERVICE_READMEMORYBYADDRESS
//#define DIAG_USE_SERVICE_WRITEMEMORYBYADDRESS
//#define DIAG_USE_SERVICE_READSCALINGDATABYIDENTIFIER
//#define DIAG_USE_SERVICE_SECURITYACCESS
#define DIAG_USE_SERVICE_WRITEDATABYIDENTIFIER
//#define DIAG_USE_SERVICE_ROUTINECONTROL
#define DIAG_USE_SERVICE_TESTERPRESENT
//#define DIAG_USE_SERVICE_CONTROLDTCSETTING
//#define DIAG_USE_SERVICE_READDATABYPERIODICIDENTIFIER
//#define DIAG_USE_SERVICE_DYNAMICALLYDEFINEDATAIDENTIFIER
//#define DIAG_USE_SERVICE_INPUTOUTPUTCONTROLBYIDENTIFIER
//#define DIAG_USE_SERVICE_COMMUNICATIONCONTROL
//#define DIAG_USE_SERVICE_REQUESTDOWNLOAD
//#define DIAG_USE_SERVICE_REQUESTUPLOAD
//#define DIAG_USE_SERVICE_TRANSFERDATA
//#define DIAG_USE_SERVICE_REQUESTTRANSFEREXIT



/* Diag buffer max Length */
#define DiagBufferMax (160)

/* error code */
//#define ERR_CODE_OV  0x01
//#define ERR_CODE_UV  0x02


#define DIAG_PROTOCOL_NORMAL_ADDR_TYPE     0
#define DIAG_PROTOCOL_FUNCTIONAL_ADDR_TYPE 1u
#define DIAG_PROTOCOL_PHYSICAL_ADDR_TYPE   2

//#define DCM_PROTOCAL_TP_MAX_LENGTH 0x1000
//#define DCM_INVALID_PDU_ID 0xFFFF


/* Diag_SecLevelType */
typedef uint8_t Diag_SecLevelType;
//#define DIAG_SEC_LEV_LOCKED ((Diag_SecLevelType)0x00)
//#define DIAG_SEC_LEV_L1     ((Diag_SecLevelType)0x01)
//#define DIAG_SEC_LEV_ALL    ((Diag_SecLevelType)0xFF)


/* Diag_SesCtrlType */
typedef uint8_t Diag_SesCtrlType;
#define DIAG_DEFAULT_SESSION                  ((Diag_SesCtrlType)0x01)
#define DIAG_PROGRAMMING_SESSION              ((Diag_SesCtrlType)0x02)
#define DIAG_EXTENDED_DIAGNOSTIC_SESSION      ((Diag_SesCtrlType)0x03)
//#define DIAG_SAFTEY_SYSTEM_DIAGNOSTIC_SESSION ((Diag_SesCtrlType)0x04)
//#define DIAG_OBD_SESSION                      ((Diag_SesCtrlType)0x05) //only used for OBD diagnostic
//#define DIAG_ALL_SESSION_LEVEL                ((Diag_SesCtrlType)0xFF)


/* SID table */
typedef uint8_t Diag_SID_Type;
#define SID_DIAGNOSTIC_SESSION_CONTROL         0x10
//#define SID_ECU_RESET                          0x11
//#define SID_CLEAR_DIAGNOSTIC_INFORMATION       0x14
//#define SID_READ_DTC_INFORMATION               0x19
#define SID_READ_DATA_BY_IDENTIFIER            0x22
//#define SID_READ_MEMORY_BY_ADDRESS             0x23
//#define SID_READ_SCALING_DATA_BY_IDENTIFIER    0x24
//#define SID_SECURITY_ACCESS                    0x27
//#define SID_COMMUNICATION_CONTROL              0x28
//#define SID_READ_DATA_BY_PERIODIC_IDENTIFIER   0x2A
//#define SID_DYNAMICALLY_DEFINE_DATA_IDENTIFIER 0x2C
#define SID_WRITE_DATA_BY_IDENTIFIER           0x2E
//#define SID_INPUT_OUTPUT_CONTROL_BY_IDENTIFIER 0x2F
//#define SID_ROUTINE_CONTROL                    0x31
//#define SID_WRITE_MEMORY_BY_ADDRESS            0x3D
#define SID_TESTER_PRESENT                     0x3E
#define SID_NEGATIVE_RESPONSE                  0x7F
//#define SID_CONTROL_DTC_SETTING                0x85

//#define SID_REQUEST_DOWNLOAD      0x34
//#define SID_REQUEST_UPLOAD        0x35
//#define SID_TRANSFER_DATA         0x36
//#define SID_REQUEST_TRANSFER_EXIT 0x37


/* Misc definitions */
//#define SUPPRESS_POS_RESP_BIT (uint8_t)0x80
#define SID_RESPONSE_BIT      (uint8_t)0x40
//#define VALUE_IS_NOT_USED     (uint8_t)0x00


/* Diag_NegativeResponseCodeType */
typedef uint8_t Diag_NegativeResponseCodeType;
//#define NRC_E_GENERALREJECT                             ((Diag_NegativeResponseCodeType)0x10)
//#define NRC_E_BUSYREPEATREQUEST                         ((Diag_NegativeResponseCodeType)0x21)
//#define NRC_E_CONDITIONSNOTCORRECT                      ((Diag_NegativeResponseCodeType)0x22)
//#define NRC_E_REQUESTSEQUENCEERROR                      ((Diag_NegativeResponseCodeType)0x24)
#define NRC_E_REQUESTOUTOFRANGE                         ((Diag_NegativeResponseCodeType)0x31)
//#define NRC_E_SECURITYACCESSDENIED                      ((Diag_NegativeResponseCodeType)0x33)
//#define NRC_E_INVALIDKEY                                ((Diag_NegativeResponseCodeType)0x35)
//#define NRC_E_EXCEEDNUMBEROFATTEMPTS                    ((Diag_NegativeResponseCodeType)0x36)
//#define NRC_E_REQUIREDTIMEDELAYNOTEXPIRED               ((Diag_NegativeResponseCodeType)0x37)
//#define NRC_E_GENERALPROGRAMMINGFAILURE                 ((Diag_NegativeResponseCodeType)0x72)
//#define NRC_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION    ((Diag_NegativeResponseCodeType)0x7E)
//#define NRC_E_RPMTOOHIGH                                ((Diag_NegativeResponseCodeType)0x81)
//#define NRC_E_RPMTOLOW                                  ((Diag_NegativeResponseCodeType)0x82)
//#define NRC_E_ENGINEISRUNNING                           ((Diag_NegativeResponseCodeType)0x83)
//#define NRC_E_ENGINEISNOTRUNNING                        ((Diag_NegativeResponseCodeType)0x84)
//#define NRC_E_ENGINERUNTIMETOOLOW                       ((Diag_NegativeResponseCodeType)0x85)
//#define NRC_E_TEMPERATURETOOHIGH                        ((Diag_NegativeResponseCodeType)0x86)
//#define NRC_E_TEMPERATURETOOLOW                         ((Diag_NegativeResponseCodeType)0x87)
//#define NRC_E_VEHICLESPEEDTOOHIGH                       ((Diag_NegativeResponseCodeType)0x88)
//#define NRC_E_VEHICLESPEEDTOOLOW                        ((Diag_NegativeResponseCodeType)0x89)
//#define NRC_E_THROTTLE_PEDALTOOHIGH                     ((Diag_NegativeResponseCodeType)0x8A)
//#define NRC_E_THROTTLE_PEDALTOOLOW                      ((Diag_NegativeResponseCodeType)0x8B)
//#define NRC_E_TRANSMISSIONRANGENOTINNEUTRAL             ((Diag_NegativeResponseCodeType)0x8C)
//#define NRC_E_TRANSMISSIONRANGENOTINGEAR                ((Diag_NegativeResponseCodeType)0x8D)
//#define NRC_E_BRAKESWITCH_NOTCLOSED                     ((Diag_NegativeResponseCodeType)0x8F)
//#define NRC_E_SHIFTERLEVERNOTINPARK                     ((Diag_NegativeResponseCodeType)0x90)
//#define NRC_E_TORQUECONVERTERCLUTCHLOCKED               ((Diag_NegativeResponseCodeType)0x91)
//#define NRC_E_VOLTAGETOOHIGH                            ((Diag_NegativeResponseCodeType)0x92)
//#define NRC_E_VOLTAGETOOLOW                             ((Diag_NegativeResponseCodeType)0x93)

#define NRC_E_POSITIVERESPONSE                          ((Diag_NegativeResponseCodeType)0x00)
#define NRC_E_SERVICENOTSUPPORTED                       ((Diag_NegativeResponseCodeType)0x11)
#define NRC_E_SUBFUNCTIONNOTSUPPORTED                   ((Diag_NegativeResponseCodeType)0x12)
#define NRC_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT     ((Diag_NegativeResponseCodeType)0x13)
#define NRC_E_SERVICENOTSUPPORTEDINACTIVESESSION        ((Diag_NegativeResponseCodeType)0x7F)



#define P2_SERVER_MAX    (5000)   /* 5000ms */
/*
************************************************************************************************************************
*                                                         Typedefs
************************************************************************************************************************
*/
typedef uint8_t (*FunctionPointer)(uint8_t* mData, uint16_t mDataLength);
typedef uint8_t  ProtocolAddrType;
typedef uint16_t PduIdType;
typedef uint16_t PduLengthType;
typedef uint8_t  TxConfirmationStatusType;
typedef uint16_t P2_TimeOutCntType;
typedef uint16_t P2_ServerMaxTimeOutCntType;

typedef struct {
    uint8_t *SduDataPtr;         /*  payload   */
    PduLengthType SduLength;     /*  length of SDU */
} PduInfoType;


typedef struct {
    PduInfoType pduRxData;
    PduInfoType pduTxData;
    ProtocolAddrType addrType;
} MsgDataType;


typedef void (*SessionFunctionPointer)(void);
typedef struct SessionControl
{
    Diag_SesCtrlType Session;
    boolean_t sessionPending;
} SessionCtrl_t;


typedef struct {
    boolean_t initRun;
    struct {
        uint8_t*  Tp_RxDataPtr;
        uint16_t* Tp_RxDataMaxLengthPtr;
        uint8_t*  Tp_TxDataPtr;
        uint16_t* Tp_TxDataMaxLengthPtr;
    } DataPtr;
    
    SessionCtrl_t sessionCtrl;
    P2_TimeOutCntType P2_TimeOutCnt;
    P2_ServerMaxTimeOutCntType P2_ServerTimeOutCntMax;    
    
} DiagGlobalType;

extern DiagGlobalType DiagInfo;
extern MsgDataType MsgData;

/*
************************************************************************************************************************
*                                                    Exported variables
************************************************************************************************************************
*/
extern void DiagManager_Init (void);
extern void DiagManager_MainFunction (void);
extern void DiagManager_RxIndicationFromRouter (ProtocolAddrType addrType, uint16_t length, const uint8_t* txConfirmation);
extern void DiagManager_TxIndicationFromRouter (const uint8_t* txConfirmation);
extern void DiagManager_AddParameterOfDataIdentifier (uint16_t mDid, uint8_t* mDataPtr, uint16_t mDataLength, const FunctionPointer ptrFunction);

/*
************************************************************************************************************************
*                                               Exported function prototypes
************************************************************************************************************************
*/

#endif    /* DIAG_MANAGER_H */

