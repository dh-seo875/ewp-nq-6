
#define uds_c
#include "tle_device.h"
#include "Main.h"

#include "linmain.h"
#include "genLinConfig.h"
#include "lin_driver_api.h"
#include "Emo_RAM.h"
#include "def_lin.h"
#include "util.h"
#include "uds.h"

T_UDS_ uds;


void uds_send_frame(void)
{
	uint16_t	i;
	

	if(uds.request_response==1) {
		uds.request_response=0;
	} else {
		return;	
	}
	UART1_RX_Int_Dis();
	//OutStr("\n\rATX:");
	for(i=1;i<9;i++) {
		lin_hal_tx_char(uds.tx_data[i]);
		Delay_us(530);
	}
	lin_hal_tx_char(uds.tx_data[i]);
	//put_char(uds.tx_data[i]);
	Delay_us(1280);
	i = LIN_UART_STATUS;
	i = LIN_UART_BUFFER;
	uds.state=EST_RX;
	UART1_RX_Int_En();
}


void Disable_Interrupt (uint8_t vector_number)
{
  __disable_irq();
}

void disable_all_interrupt(void)
{
  __disable_irq();
}
/* =================================================================================
 1ms

//lin_hal_tx_char('U');
//put_char(0x34);
 
================================================================================= */  
void uds_state_check(void)
{
    uint16_t n;
    
    if(uds.c_tog_1ms) {
        uds.c_tog_1ms=0;
        uds.c_timeout_p1_2ms++; 			
        if(uds.c_timeout_p1_2ms > 200u) {
            uds.c_timeout_p1_2ms=200u;
            uds_Init();
        } 
        uds.c_10ms++; 			
        if(uds.c_10ms > 5u) {
            uds.c_10ms=0;
            if(uds.c_timeout_p2_10ms > TIMEOUT_P2_10MS) {
                uds_Init();
            }
        }
        uds.c_prt++;
        if(uds.c_prt>1000) {
            uds.c_prt=0;
        }
        
        return;
    }	
    
    uds.c_tog_1ms=1;
    if(uds.state==EST_TX){
        uds_send_frame();
    } else if(uds.state==EST_RX){
        if(SCU->MODIEN1.bit.RIEN1==0) {
            UART1_RX_Int_En();
        }
        if(is_session_program) {
            OutStr("App reset\n\r");
            disable_all_interrupt();
            
            for(n=0;n<2000U;n++) {					// 2000 = 1.28ms
                ;
            }	
            goto_reset();
        }			
    } 
}
/* =================================================================================
 
================================================================================= */  
void uds_tx_data_set(void)
{
	uint8_t i;
	for(i=0;i<10;i++) {
		uds.tx_data[i]=0;
	}	
	uds.tx_data[0]=ECU_FRAME_ID;
	uds.tx_data[1]=NAD_PHY;
	uds.tx_data[2]=2u;
	uds.tx_data[3]=uds.rsid;
}
uint16_t GetCheckSumValue(uint8_t *pu8Buf)   
{
 	uint16_t i,CheckSum =0u;

	for (i=0 ; i<8u; i++) 		{
		CheckSum+=pu8Buf[i];
  	if (CheckSum>=256u) {
			CheckSum-=255u;
  	}	
	}
	CheckSum=(~CheckSum)&0xffu;
	return CheckSum;	
}

void uds_tx_checksum_set(void)
{
	uds.tx_data[9]=(uint8_t)GetCheckSumValue((void *)&uds.tx_data[1]);   
	uds.p_rx_data=0u;
	uds.p_tx_data=0u;
	uds.tx_data_len=10u;		// 0~9
}

/* =================================================================================
 sid .. data
================================================================================= */  
void uds_frame_parser(void)
{
	uint8_t *p;

	uds.c_timeout_p2_10ms=0u;				
	uds.sid=uds.packet_data[0];
	uds.rsid=uds.sid | 0x40u;
	uds_tx_data_set();
	if(uds.sid==SID_DiagnosticSessionControl) {
		uds.tx_data[4]=uds.packet_data[1];
		if(uds.packet_data[1]>NSESSION_EXTENDED_DIAGNOSTIC) {
			uds.tx_data[3]=UDS_NAK;
			uds.nrc=UDS_NRC_subFunctionNotSupported;
			uds.tx_data[5]=uds.nrc;
		}	else {
			uds.session=uds.packet_data[1];
			if(is_session_program) {
				p=(void *)BOOT_VERSION_ADDRESS;
				if(p[0]!='B') {
					uds.tx_data[3]=UDS_NAK;
					uds.nrc=UDS_NRC_OUTOFRANGE;			// no bootloader
					uds.tx_data[5]=uds.nrc;
					uds.session=0;
				}	else {
					OutStr("Found bootloader");put_cr();
				}
			}	
			//OutStr("Session:");put_char(uds.session);put_cr();
		}
		uds_tx_checksum_set();
		uds.state=EST_TX;
	}
#ifdef NEVER
	else 	if(uds.sid==SID_ReadDataByIdentifier) {
		//OutStr("SID_ReadDataByIdentifier");put_cr();

		uds.did=uds.packet_data[1];
		uds.did<<=8;
		uds.did+=uds.packet_data[2];
		uds.tx_data[4]=uds.packet_data[1];
		uds.tx_data[5]=uds.packet_data[2];
		if(uds.did==VMECUSVNDID) {
			p=(void *)APP_VERSION_ADDRESS; 
			uds.tx_data[2]=6u;
			uds.tx_data[6]=p[0];
			uds.tx_data[7]=p[1];
			uds.tx_data[8]=p[2];
		} else {
			uds.tx_data[2]=4u;
			uds.tx_data[3]=UDS_NAK;
			uds.tx_data[6]=UDS_NRC_OUTOFRANGE;
		}
		uds.state=EST_TX;
		uds_tx_checksum_set();
	}
#endif /* NEVER */
}

/***********************************************************************************************

************************************************************************************************/  
void uds_Init(void)
{
	uds.state=0u;
	uds.c_timeout_p1_2ms=0u;				
	uds.c_timeout_p2_10ms=0u;				
	
	uds.rx_data_len=0u;
	uds.p_rx_data=0u;
	uds.tx_data_len=0u;
	uds.p_tx_data=0u;
	uds.state=EST_RX;

}
/* =================================================================================

================================================================================= */  
void lin_data_set(void)
{
	uint16_t	i;
	if(is_single_frame) {
		for(i=0;i<6;i++) {
			uds.packet_data[i]=	uds.rx_data[i+3];
		}	
		uds.packet_data_len=6u;
		uds_frame_parser();
	}	
}

/***********************************************************************************************
* LIN
*
************************************************************************************************/
void LIN_IRQHandler (uint8_t c)
{
	uint8_t cs;
	
	if(uds.c_timeout_p1_2ms > V_c_timeout_p1_2ms) {
		uds.p_rx_data=0;
	}
	uds.c_timeout_p1_2ms=0;

	if(uds.state==EST_RX) {
		// ============================================  1frame 
		//OutDev0('[');//put_char(uds.p_rx_data);
		//put_char(c);OutDev0('0'+uds.p_rx_data);
		if(uds.p_rx_data==0) {
			if((c==TESTER_FRAME_ID) && (uds.rx_old==LIN_SYNC)) {
				uds.rx_data[uds.p_rx_data]=c;
				uds.p_rx_data++;
			} else  {
				;
			}	
		}else if(uds.p_rx_data==1u) {
			if((c==NAD_FUCTION) || (c==NAD_PHY)) {
				uds.rx_data[uds.p_rx_data]=c;
				uds.p_rx_data++;
			} else {
				uds.p_rx_data=0u;
			}	
		} else if(uds.p_rx_data==9u) {
			uds.request_response=0;
			cs=(uint8_t)GetCheckSumValue((void *)&uds.rx_data[1]);
			#ifdef LIN_CS_SKIP
				uds.p_rx_data=0u;
				lin_data_set();
			#else 			
				if(cs==c) {
					// cs ok
					uds.p_rx_data=0u;
					//OutStr("cs ok");put_cr();
					lin_data_set();
				} else {
					uds.p_rx_data=0u;
					//OutStr("cs error=");put_char(cs);put_char(c);put_cr();
				}
			#endif
		} else if(uds.p_rx_data<9u) {
			uds.rx_data[uds.p_rx_data]=c;
			uds.p_rx_data++;
		} else {
			//OutStr("p_rx over");put_char(uds.p_rx_data);put_cr();
		}
	}
	else {
		if(c==ECU_FRAME_ID) {
			//put_char(uds.rx_old);
			if(uds.rx_old==LIN_SYNC) {
				uds.request_response=1;
				//OutStr("request_response"); put_cr();
			}
		}	
		//put_char(c);OutDev0('X');
	}			
	uds.rx_old=c;
	//OutDev0('X');put_char(uds.rx_old);
}




