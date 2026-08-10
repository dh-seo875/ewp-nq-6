#ifndef DEF_LIN
#define DEF_LIN

//#define LIN_CS_SKIP
#define DEBUG_UART


#define APP_VECTOR_ADD		0x3000		// 16k




#define		MAX_sci0d						40U
#define 	MAX_UDS_DATA_BUF		16U
#define 	MAX_UDS_DATA_BUF_LIN	12U

#define 	MAX_UDS_BLOCK_SIZE		0x40u			// 64


#define 	NAD_FUCTION				0x7Eu
#define 	NAD_PHY							0x10u

#define 	TESTER_FRAME_ID		0x3Cu
#define 	ECU_FRAME_ID				0x7Du			// 7D=3D+PARITY
#define 	LIN_SYNCF					0x55u			

#define 	TIMEOUT_P2_10MS					500u		// Time between reception of the last frame of a diagnostic request on the LIN bus and the slave node being able to provide data for a response.
#define 	V_c_timeout_p1_2ms		25U			

#define 	VMECUSVNDID						0xF389u			// VehicleManufacturerECUSoftwareVersionNumberDataIdentifier ±ÇÀå.

#define 	SID_DiagnosticSessionControl	0x10u
#define 	SID_ECUReset										0x11u
#define 	SID_SecurityAccess							0x27u
#define 	SID_TesterPresent							0x3Eu
#define 	SID_RoutineControl							0x31u
#define 	SID_RequestDownload						0x34u
#define 	SID_TransferData								0x36u
#define 	SID_RequestTransferExit				0x37u
#define 	SID_ReadDataByIdentifier 		0x22 

// ========================== ERROR
#define UDS_NAK																	0x7Fu
#define UDS_NRC_LEN														0x13u		// incorrectMessageLength
#define UDS_NRC_subFunctionNotSupported					0x12u		// subFunctionNotSupported
#define UDS_NRC_OUTOFRANGE										0x31u		// 
#define UDS_NRC_securityAccessDenied					0x33u		// securityAccessDenied
#define UDS_NRC_serviceNotSupport						0x11u		// 
#define UDS_NRC_conditionNotCorrect					0x22u		// 
#define UDS_NRC_requestSequenceError					0x24u		// 
#define UDS_NRC_invalidKey										0x35u		// 
#define UDS_NRC_generalProgrammingFailure		0x72u		// 
#define UDS_NRC_wrongBlockSequenceCounter		0x73u		// 
#define UDS_NRC_subFunctionNotSupportedInActiveSession		0x7eu		// 
#define UDS_NRC_serviceNotSupportedInActiveSession		0x7Fu		// 



#define NSESSION_DEFAULT		1u
#define NSESSION_PROGRAMMING		2u
#define NSESSION_EXTENDED_DIAGNOSTIC		3u

#define is_session_default (uds.session<=NSESSION_DEFAULT)
#define is_session_extended_diagnostic (uds.session==NSESSION_EXTENDED_DIAGNOSTIC)
#define is_session_program (uds.session==NSESSION_PROGRAMMING)

#define is_single_frame ((uds.rx_data[2U] & 0xF0u)==0)
#define is_first_frame ((uds.rx_data[2U] & 0xF0u)==0x10u)
#define is_consecutive_frame ((uds.rx_data[2U] & 0xF0u)==0x20u)

// ================ TEST BOOTLOADER SIZE Å©°Ô.	
#define APP_MESSAGE_ADDRESS	 0x1100FF00u			// DATAFLASH (CHIP ¹Ù²î¸é º¯°æ)
#define APP_START_ADDRESS		0x2000u
#define APP_END_ADDRESS			(APP_START_ADDRESS+0x2000u)
#define APP_RESET_VECTOR    (APP_START_ADDRESS+4U)
#define APP_SP_VECTOR				APP_START_ADDRESS
#define BOOT_SIGN_ADDRESS		0x1FFCU
#define BOOT_SIGN						0xAA5555AAu

#define APP_VERSION_ADDRESS		0x11002020U
#define BOOT_VERSION_ADDRESS	0x11000020U

#define APP_MESSAGE_SIGN		0x55AA1234U


#define FLASH_SECTOR_SIZE		0x200u

#define is_app_address ((uds.start_address >=APP_START_ADDRESS) && (uds.start_address < APP_END_ADDRESS))
#define is_app_vector (uds.start_address < 0x410u)
#define is_response_supressed ((uds.rx_data[2]&0x80U)==0x80U)




enum {
	EST_RX=0,
	EST_TX,	
	EST_TX_WAIT_ACTION,	
	EST_END
};	
enum {
	EACT_NONE=0,
	EACT_RESET,
	EACT_ERASE,
	EACT_PROG,
	EACT_PROG_VECTOR,
	EACT_END
};	
		
		
typedef struct {
	uint8_t	tempo;
	uint8_t	X2MS;
	uint8_t	c_500ms;

}counter_;


typedef struct {
	uint16_t	ndata;			
	uint8_t		packet_data[MAX_UDS_DATA_BUF+12u];			// packet_data[2] ºÎÅÍ 32bit aline Àý´ëÀÚ¸®¿Å±âÁö¸»°Í.
	uint16_t	packet_data_len,p_packet_data;

	uint8_t	c_tog_1ms;				
	uint8_t	c_timeout_p1_2ms;				
	uint8_t	c_10ms;				
	uint16_t	c_timeout_p2_10ms;				
	uint16_t	c_prt;				

	uint8_t	state;	
	uint8_t	req_action;
	uint8_t	program_size;

	uint8_t	rx_data[MAX_UDS_DATA_BUF_LIN],rx_data_len,p_rx_data;
	uint8_t	n_cf;
	uint8_t	tx_data[MAX_UDS_DATA_BUF_LIN],tx_data_len,p_tx_data;

	uint16_t	block_num;				
	uint16_t	ntable_did;				// ¿ø·¡ table 					

	uint16_t	check_sum;			

	uint16_t	did;				
	uint8_t	sid,rsid;		//

	uint8_t	nrc;		//
	uint8_t	session;
	uint8_t	request_reset;
	uint8_t	request_response;
	uint8_t	rx_old;	
	
}T_UDS_;



  		
		
#endif 

