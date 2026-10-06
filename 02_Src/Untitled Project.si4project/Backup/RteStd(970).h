/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : RteStd.h
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


#ifndef RTESTD_H
#define RTESTD_H

/*
************************************************************************************************************************
*                                                  Include header files
************************************************************************************************************************
*/
#include "Std_Types.h"
#include "RteHal.h"
#include "RteApp.h"


/*
************************************************************************************************************************
*                                                   Defines and macros
************************************************************************************************************************
*/
/** --- Error values ------------------------------------------------------------------------------*/
/** No error occurred. */
#define RTE_E_OK                          0

/** --- Standard Application Error Values: */

/** Generic application error indicated by signal invalidation in sender receiver
 *  communication with data semantics on the receiver side. */
#define RTE_E_INVALID	                 1

/** An IPDU group was disabled while the application was waiting for the transmission acknowledgment.
 *  No value is available. This is not considered a fault, since the IPDU group is switched off on purpose.
 *  This semantics are as follows:
 *  - The OUT buffers of a client or of explicit read APIs are not modified
 *  - no runnable with startOnEvent on a DataReceivedEvent for this VariableDataPrototype is triggered.
 *  - the buffers for implicit read access will keep the previous value. */
#define RTE_E_COM_STOPPED              128

/** A blocking API call returned due to expiry of a local timeout rather than the intended result.
 *  OUT buffers are not modified. The interpretation of this being an error depends on the application. */
#define RTE_E_TIMEOUT                  129

/** A internal RTE limit has been exceeded. Request could not be handled. */
#define RTE_E_LIMIT 130

/** An explicit read API call returned no data. (This is no error.) */
#define RTE_E_NO_DATA                  131

/** Transmission acknowledgement received. */
#define RTE_E_TRANSMIT_ACK             132

/** No data received for the corresponding unqueued data element since system start or partition restart. */
#define RTE_E_NEVER_RECEIVED           133

/** The port used for communication is not connected. */
#define RTE_E_UNCONNECTED              134

/** The error is returned by a blocking API and indicates that the runnable could not enter a wait state,
 *  because one ExecutableEntity of the current task's call stack has entered or is running in an ExclusiveArea.
 *  @req SWS_Rte_02739 */
#define RTE_E_IN_EXCLUSIVE_AREA        135

/** The error can be returned by an RTE API, if the parameters contain a direct or indirect
 *  reference to memory that is not accessible from the callers partition. */
#define RTE_E_SEG_FAULT                136

/** The received data is out of range. */
#define RTE_E_OUT_OF_RANGE             137

/** An error during serialization or deserialization occurred. */
#define RTE_E_SERIALIZATION_ERROR      138

/** Buffer for serialization operation could not be created. */
#define RTE_E_SERIALIZATION_LIMIT      139

/** --- Overlayed Errors
 *  These errors do not refer to the data returned with the API. They can be overlayed
 *  with other Application- or Immediate Infrastructure Errors. */

/** An API call for reading received data with event semantics indicates that some incoming data
 *  has been lost due to an overflow of the receive queue or due to an error of the underlying
 *  communication stack. */
#define RTE_E_LOST_DATA                 64

/** An API call for reading received data withdata semantics indicates that the available data
 *  has exceeded the aliveTimeout limit. A COM signal outdated callback will result in this error. */
#define RTE_E_MAX_AGE_EXCEEDED          64

/*
************************************************************************************************************************
*                                                         Typedefs
************************************************************************************************************************
*/
typedef enum 
{
    BDRV_ClearShortCircuitStatusFlag,
    BDRV_VsdThresholdMeasurementStatusFlag,
} ClearType_t;

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


#endif    /* RTESTD_H */

