/**
 * @cond
 ***********************************************************************************************************************
 *
 * Copyright (c) 2015, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **********************************************************************************************************************/
/*******************************************************************************
**                      Author(s) Identity                                    **
********************************************************************************
**                                                                            **
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** DM           Daniel Mysliwitz                                              **
**                                                                            **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/* 
 * V0.1.0: 2014-05-20, DM:   Initial version
 */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "genLinConfig.h"
#include "lin_driver_api.h"
#include "Variant_cfg.h"
#include "linmain.h"
#include <Timer2x.h>
#include "RteStd.h"
#include "def_lin.h"
#include "DiagManager.h"
#include "CommRouter.h"
#include "lin_config.h"

/*******************************************************************************
**                                   macro                                    **
*******************************************************************************/

/*******************************************************************************
**                      External CallBacks                                    **
*******************************************************************************/
extern void uds_state_check(void);
static void l_callback_RxIndication (const l_u8* transmitFlag);
static void l_callback_TxIndication (const l_u8* transmitFlag);
/*******************************************************************************
**                     Local function                                         **
*******************************************************************************/

static void LIN_TransportLayerMain(void);
static void LIN_CoolantDiagFlag (void);
static void LIN_LLD_Service_TX_Cal(void);
static void LIN_LLD_Service_Timeout(void);
static void LIN_LLD_Service_RefSpdCal(void);
static void LIN_LLD_Service_InitialFlag(void);
static void LIN_LLD_Service_Successful_chk(void);
static void ld_task_Cal(void);
static void LIN_COM_Handler_ifc_t2(void);
static void LIN_LLD_Service_1ms(void);
static void LIN_LLD_Service_TX_Cal_ReadFunction1(void);
static void LIN_LLD_Service_TX_Cal_ReadFunction2(void);
static void LIN_LLD_Service_Tx_Rx(void);



/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/
/* Global variables */

static l_bool protectionFlag;
static l_bool faultFlag;
static l_u8 faultStateMerged_Byte1=0;
static l_u8 faultStateMerged_Byte2=0;
static l_u8 warningStateMerged=0;
static l_u8 actDuty=0;
static l_u8 CoolantFlag=0;
static l_u8 MCUFault=0;	
static signed short idcFiltered=0;
static signed short vdcFiltered=0;
static signed short iqFiltered=0;
static sint16_t TemperatureFiltered;
 	










extern l_u8 aliveCheck;

static l_u16 NadTimeOutCounter = 0;





uint8_t linSuccessfulFlag = 0;
uint8_t forceNvmWriteFlag = 0;


#if((EWP_TYPE == AEWP_TYPE2_0X37) || (EWP_TYPE == AEWP_TYPE2_0X37_WITHOUTHOTSPOT))

/* CheckSum and AliveCounter variables */
l_u8 EWP_aliveCounter = 0;
l_u8 EMS_aliveCounter = 0;
l_u8 EWP_checkSum = 0;
l_u8 EMS_checkSum = 0;
l_u8 EMS_checkSumCal = 0;


#elif((EWP_TYPE == PEWP_TYPE2_0X37) || (EWP_TYPE == PEWP_TYPE2_0X38) || (EWP_TYPE == IEWP_TYPE2_0X37) || (EWP_TYPE == PEWP_TYPE3_0X37) || (EWP_TYPE == PEWP_TYPE3_0X38) || (EWP_TYPE == IEWP_TYPE3_0X37) || (EWP_TYPE == BEWP_TYPE3_0X37) || (EWP_TYPE == BEWP_TYPE3_0X38))

#else
/* No Action */
#endif


static volatile l_u8 lin_diag_statemachine = 0;

static l_u16 lin_rx_diag_buffer_Max_length = 0;
static l_u16 lin_rx_diag_buffer_length = 0;
static l_u16 lin_tx_diag_buffer_length = 6;
static const l_u8* lin_diag_Transmit_buffer;
static l_u8* lin_diag_received_buffer;
static l_u8  lin_diag_Transmit_flag = 0;


/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
static void LIN_LLD_Service_Tx_Rx(void)
{
    LIN_LLD_Service_Timeout();
    LIN_CoolantDiagFlag();
	LIN_LLD_Service_RefSpdCal();
    LIN_LLD_Service_InitialFlag();
	LIN_LLD_Service_TX_Cal();
}


static void LIN_TransportLayerMain(void)
{
    /* Diagnostic Handling */
    switch (lin_diag_statemachine)
    {
        case 0u:
            /* Start diagnostic state machine
            * Prepare buffer and wait for master request */
            lin_rx_diag_buffer_length = lin_rx_diag_buffer_Max_length;				
            ld_receive_message ((l_u16* const)&lin_rx_diag_buffer_length, lin_diag_received_buffer);
            lin_diag_statemachine = lin_diag_statemachine + 1u;;
        break;


        case 1u:
            if (ld_rx_status() > LD_COMPLETED)
            {
                /* some error occured, dummy implementation - reset transport layer
                * and restart diagnostic */
                (void)ld_init();
                lin_diag_statemachine = 0u;
            } else {
                if (ld_rx_status() != LD_IN_PROGRESS)
                {
                    /* Buffer is filled with data, check data and prepare response */
                    l_callback_RxIndication(&lin_diag_Transmit_flag);

                    /* It aim to accept right response from diag manager. If this flag is set, It means to complete to process for response. */
                    if (lin_diag_Transmit_flag == TRUE)
                    {
                        /* dummy implementation - send data received back as response */
                        ld_send_message (lin_tx_diag_buffer_length, lin_diag_Transmit_buffer);
                        lin_diag_statemachine = lin_diag_statemachine + 1u;                        
                    }
                }
            }
        break;


        case 2u:
            if (ld_tx_status() > LD_COMPLETED)
            {
                /* some error occured, dummy implementation - reset transport layer * and restart diagnostic */
                (void)ld_init();
                lin_diag_statemachine = 0u;
                /* It aim to accept right response from diag manager. If this flag is set, It means to complete to process for response. */
                lin_diag_Transmit_flag = FALSE;
                /* Flag is set and prepared next behavior */
                l_callback_TxIndication(&lin_diag_Transmit_flag);                
            } else {
                if (ld_tx_status() == LD_COMPLETED)
                {
                    lin_diag_statemachine = 0u;
                    /* It aim to accept right response from diag manager. If this flag is set, It means to complete to process for response. */
                    lin_diag_Transmit_flag = FALSE;
                    /* Flag is set and prepared next behavior */
                    l_callback_TxIndication(&lin_diag_Transmit_flag);
                }
            }
        break;
        
        default:
	    /* Intentionally Empty */
        break;
    }
}

l_u8 LIN_TP_INIT (l_u8* rx_diag_buffer, const l_u8* tx_diag_buffer, l_u8 rx_diag_buffer_max)
{
    l_u8 retValue = TRUE;
    lin_diag_received_buffer = rx_diag_buffer;
    lin_diag_Transmit_buffer = tx_diag_buffer;
    lin_diag_statemachine = 0;
    lin_rx_diag_buffer_Max_length = rx_diag_buffer_max;
    lin_rx_diag_buffer_length = 0;
    lin_tx_diag_buffer_length = 0;

    if((lin_diag_received_buffer == NULL_PTR) || (lin_diag_Transmit_buffer == NULL_PTR) || (lin_rx_diag_buffer_Max_length == 0u))
    {
        retValue = FALSE;
    }
    
    return retValue;
}


void LIN_LLD_Init(void)
{
    TIMER21_Start();
   
    /* LIN driver init */
    (void)l_sys_init();
}

void LIN_LLD_Service(void)
{
	static unsigned int BusIdleTimeOutCounter, tmpStatus;

    /* Variable for IRQ mask */
    static l_irqmask lin_irqState;
	//static volatile unsigned char timer_1ms;



    /* cyclic communication task has to be called once every byte */
    /* location is changed in accordance with IHR poring guid. */
//    l_cyclic_com_task();
    LIN_TransportLayerMain();

    /* myunghwa code for using Nvm.c */
	LIN_LLD_Service_Successful_chk();
    
    /* 1ms cyclic task */
    BusIdleTimeOutCounter++;
    /* Every interrupt that contains either signal read (incl.
     * status flag management), signal write, driver status management,
     * driver tasks (incl. ld_task, cyclic_com_task, l_ifc_rx) should be
     * protected against interruption from other.
     */
    lin_irqState = l_sys_irq_disable();
    /* Call the cyclic task according to the configured value
     * (field "LIN Task Cycletime" in the LDC-Tool) */
    //ld_task_Cal();
      
    /* for more detailed information */
    //tmpStatus = l_ifc_read_status();
    l_sys_irq_restore(lin_irqState);

    /* get status of lin driver from status word, see LIN specification 
    for more detailed information */
    tmpStatus = l_ifc_read_status();
    
    /* If one of the statusflags is set then there was something on the bus
    reset counter and wake up */
	BusIdleTimeOutCounter = ((tmpStatus & 0x00FFu) != 0u) ? 0u : BusIdleTimeOutCounter;
	
    /* After 4 seconds no communcation or other disturbtion - go to sleep */
	BusIdleTimeOutCounter = (BusIdleTimeOutCounter >= 4000u) ? 4000u : BusIdleTimeOutCounter;
    
    /* cyclic task of driver (timeout and diagnostic control */
    ld_task_Cal();
    LIN_LLD_Service_Tx_Rx();

}

static void ld_task_Cal(void)
{
	ld_task();
}
static void LIN_LLD_Service_Successful_chk(void)
{
	if (linSuccessfulFlag == 0u)
	{
        if(g_lin_status_word.flag.successful_transfer == 1u)
        {
			linSuccessfulFlag = 1u;
		}
	}
	else
	{
		linSuccessfulFlag = 0u;
	}

}
static void LIN_LLD_Service_Timeout(void)
{
	static l_u16 LinTimeOutCounter = 0;
	static volatile l_u8  LinTimeOutErrorFlag = 0;

	/* Lin time out detection routine */
	if (l_flg_tst_frm_COMMAND_EWP())
	{
		l_flg_clr_frm_COMMAND_EWP();
		LinTimeOutCounter = 0;			  
	}
	else
	{
		LinTimeOutCounter++;
		NadTimeOutCounter++;
	}
	
	/* using to execute the EEPROM write in Nvm */
	if (LinTimeOutCounter > 250u) /* 250ms */
	{
		forceNvmWriteFlag = 1;
	}
	else
	{
		forceNvmWriteFlag = 0;
	}
	
	if ((LinTimeOutCounter >= 4000u) && (NadTimeOutCounter >= 4000u))
	{
		LinTimeOutCounter = 4000;
		NadTimeOutCounter = 4000;
		LinTimeOutErrorFlag = 1; /* occurred time out */
	}
	else
	{
		LinTimeOutErrorFlag = 0; /* Clear time out */
	}
    (void)WriteLinTimeOutFlag(LinTimeOutErrorFlag);
}




static void LIN_LLD_Service_RefSpdCal(void)
{
	
	uint8_t lin_rx_duty = 0u;

	if (l_flg_tst_Ewp1_Target_Spd_RPM())
	{
		l_flg_clr_Ewp1_Target_Spd_RPM();
		lin_rx_duty = l_u8_rd_Ewp1_Target_Spd_RPM();
		(void)WriteLinRequireDuty(lin_rx_duty);
	}
}

static void LIN_LLD_Service_InitialFlag(void)
{
	
	uint8_t lin_rx_initFlag = 0u;
	uint8_t lin_initialflag = 0u;

	if (l_flg_tst_Ewp_Initial_Flag())
	{
		l_flg_clr_Ewp_Initial_Flag();
		lin_rx_initFlag = l_bool_rd_Ewp_Initial_Flag();
		lin_initialflag = (lin_rx_initFlag >> 1u);
		(void)WriteInitialFlag(lin_initialflag);
	}
}

static void LIN_CoolantDiagFlag (void)
{
	static volatile l_u8  LinCoolantDiagFlag = 0u;

	if (l_flg_tst_Ewp_Coolant_DiagEn())
    {
        /* Signal Read Access for Signal Ewp_Coolant_DiagEn */
        LinCoolantDiagFlag = l_bool_rd_Ewp_Coolant_DiagEn();
        l_flg_clr_Ewp_Coolant_DiagEn(); 	   
    }

    (void)WriteCoolantFlag(LinCoolantDiagFlag);
}

static void LIN_LLD_Service_TX_Cal(void)
{
    uint8_t uint8_temp =0;
	sint16_t sint16_temp =0;
    static volatile l_u8 timer_100ms = 0u;
	timer_100ms = timer_100ms + 1u;
    if (timer_100ms > 100u )
    {
        timer_100ms = 0;
		LIN_LLD_Service_TX_Cal_ReadFunction1();
		LIN_LLD_Service_TX_Cal_ReadFunction2();

		
        /********************************* Byte 0 ************************************/
        l_u8_wr_Ewp1_ActRotSpd_rpm((uint8_t)(actDuty));		  /* Signal Write Access for Signal Ewp1_ActRotSpd_rpm */
		/********************************* Byte 1 ************************************/
        l_bool_wr_Ewp1_Under_Voltage (((uint8_t)faultStateMerged_Byte1) & 0x01u);       /* Signal Write Access for Signal Ewp1_Under_Voltage */
        l_bool_wr_Ewp1_Over_Voltage  (((uint8_t)faultStateMerged_Byte1 >> 1u) & 0x01u);		/* Signal Write Access for Signal Ewp1_Over_Voltage */
        l_bool_wr_Ewp1_Short_Circuit (((uint8_t)faultStateMerged_Byte1 >> 2u) & 0x01u);       /* Signal Write Access for Signal Ewp1_Short_Circuit */
        l_bool_wr_Ewp1_Open_Circuit	 (((uint8_t)faultStateMerged_Byte1 >> 3u) & 0x01u);	 /* Signal Write Access for Signal Ewp1_Warn_Temp */
        l_bool_wr_Ewp1_Stall_Error   (((uint8_t)faultStateMerged_Byte1 >> 4u) & 0x01u);
        l_bool_wr_Ewp1_FaultStatus(faultFlag | MCUFault);                           /* Signal Write Access for Signal Ewp1_FaultStatus */
        l_bool_wr_Ewp1_ProtectMode(protectionFlag);                      /* Signal Write Access for Signal Ewp1_ProtectMode */            
        l_bool_wr_Ewp1_Speed_Error   (((uint8_t)faultStateMerged_Byte1 >> 7u)  | ((uint8_t)warningStateMerged >> 7u));      /* Signal Write Access for Signal Ewp1_Speed_Error */
        /********************************* Byte 2 ************************************/
		l_bool_wr_Ewp1_Diag_ForcedStatus(CoolantFlag);
        l_bool_wr_Ewp1_Over_Temp     (((uint8_t)faultStateMerged_Byte2 >> 1u) & 0x01u);       /* Signal Write Access for Signal Ewp1_Over_Temp */
        l_bool_wr_Ewp1_Warn_Temp     (((uint8_t)warningStateMerged     >> 2u) & 0x01u);       /* Signal Write Access for Signal Ewp1_Warn_Temp */	
        l_bool_wr_Ewp1_Low_Current   (((uint8_t)faultStateMerged_Byte2 >> 3u) & 0x01u);       /* Signal Write Access for Signal Ewp1_Low_Current */
        l_bool_wr_Ewp1_Warn_Current  (((uint8_t)warningStateMerged     >> 4u) & 0x01u);       /* Signal Write Access for Signal Ewp1_Warn_Current */
        l_bool_wr_Ewp1_Over_Current  (((uint8_t)faultStateMerged_Byte2 >> 5u) & 0x01u);       /* Signal Write Access for Signal Ewp1_Over_Current */
        l_bool_wr_Ewp1_Comm_Error    (((uint8_t)warningStateMerged     >> 6u) & 0x01u);       /* Signal Write Access for Signal Ewp1_Comm_Error */
        l_bool_wr_Ewp1_GateDrv_UV_OV (((uint8_t)faultStateMerged_Byte2 >> 7u));       /* Signal Write Access for Signal Ewp1_GateDrv_UV_OV */
        /********************************* Byte 3 ************************************/
        l_u8_wr_Ewp1_Version((EWP_TYPE<<4u)|VERSION);                     /* Signal Write Access for Signal Ewp1_Version */
        /********************************* Byte 4 ************************************/
        l_u8_wr_Ewp1_Voltage_Dc(((uint16_t)vdcFiltered/10u)); 							 /* Signal Write Access for Signal Ewp1_Torque */
        /********************************* Byte    5 ************************************/
        l_u8_wr_Ewp1_Current_DC(((uint16_t)idcFiltered/10u));                     /* Signal Write Access for Signal Ewp1_PWM_Duty_Ratio */
        /********************************* Byte 6 ************************************/
        l_u8_wr_Ewp1_Current_iq(((uint16_t)iqFiltered/10u));                     /* Signal Write Access for Signal Ewp1_PWM_Duty_Ratio */
        /********************************* Byte 7 ************************************/
        if(TemperatureFiltered >= 0)
        {
            uint8_temp = ((uint8_t)TemperatureFiltered + 40u);
		}
		else
		{
		    sint16_temp = TemperatureFiltered + 40;
			if(sint16_temp < 0)
			{
			    uint8_temp = 0u;
			}
			else
			{
    			uint8_temp = (uint8_t)sint16_temp;
			}

		}
        l_u8_wr_Ewp1_Temp(uint8_temp);


         }
}


static void LIN_LLD_Service_TX_Cal_ReadFunction1(void)
{
	(void)ReadConvertActDuty(&actDuty);
	(void)ReadWarningStateMerged(&warningStateMerged);
	(void)ReadCoolantFlag(&CoolantFlag);
	(void)ReadLINFaultStateFlag_Byte1(&faultStateMerged_Byte1);
	(void)ReadLINFaultStateFlag_Byte2(&faultStateMerged_Byte2); 	
	(void)ReadFaultFlag(&faultFlag);
	(void)ReadProtectionFlag(&protectionFlag);

}

static void LIN_LLD_Service_TX_Cal_ReadFunction2(void)
{
	(void)ReadIdcFiltered(&idcFiltered);
	(void)ReadVdcFiltered(&vdcFiltered);
	(void)ReadIqFiltered(&iqFiltered);
	(void)ReadTemperatureFiltered(&TemperatureFiltered);		
	(void)ReadMCUFault(&MCUFault);		
    if(vdcFiltered < 0)
    {
        vdcFiltered = 0;
    }

    if(idcFiltered < 0)
    {
        idcFiltered = 0;
    }

    if(iqFiltered < 0)
    {
        iqFiltered = 0;
    }
}


l_irqmask l_sys_irq_disable (void)
{
    return 0u;
}

void l_sys_irq_restore (l_irqmask mask)
{
	(void)mask;
   return;
}


void LIN_COM_Handler(void)
{
	uint8_t EXF2 = TIMER2->T2CON.bit.EXF2;
	uint8_t SCON = UART1->SCON.reg;
#ifdef AUTOBAUD_ENABLED
    if (EXF2 == 1u)
    {
        LIN_COM_Handler_ifc_t2();
        TIMER2->T2ICLR.bit.EXF2CLR = 1u;
    }
#endif /* end #ifdef AUTOBAUD_ENABLED */
    if ((SCON & 0x01u) == 1u)
    {
        l_ifc_rx();
        UART1->SCONCLR.bit.RICLR = 1u;
		l_cyclic_com_task();
    }
}

static void LIN_COM_Handler_ifc_t2(void)
{
    l_u16 CAP_H = 0u;
	l_u16 CAP_L = 0u;
    if (lin_ab_t2_state == 5u)  
    { 
        LIN_TIMER_RUN = 0u;
		LIN_TIMER_EXT_IR_EN = 0u;                                       
        LIN_TIMER_EXT_EN = 0u;
		lin_ab_t2_state = 0u;
		CAP_H = (l_u16)LIN_TIMER_CAP_H;
		CAP_L = (l_u16)LIN_TIMER_CAP_L;
        lin_synch_time = ((l_u16)(CAP_H << 8) | CAP_L);  
        lin_synch_time = (l_u16)((l_u16)(lin_synch_time >> 2u) >> BRG_PRE);              
	    LIN_UART_BCON_FDSEL = (l_u8)(((l_u8)(lin_synch_time & 0x1Fu)) >> 1u); 

        lin_synch_time = (l_u16)(lin_synch_time >> 5u);                                           
        if (lin_synch_time >= MIN_BDREG)                                               
        {
            if (lin_synch_time <= MAX_BDREG)                                           
            {
                LIN_UART_BCON_BGL = (l_u8)(lin_synch_time & 0x07u);                    
                LIN_UART_BCON_BGH = (l_u8)(lin_synch_time >> 3u);                      
                LIN_UART_BCON_RUN = 1u;                                                
                lin_slave_state_g = sync_received;                                     
             }
			 else
			 {                                                                         
                 lin_ab_timeout_timer = LIN_AUTOBAUD_TIMEOUT + 1u;                              
                 lin_ab_t2_state = 7u;
			 }                                                         
        }
        else
        {                                                                           
            lin_ab_timeout_timer = LIN_AUTOBAUD_TIMEOUT + 1u;                                
            lin_ab_t2_state = 7u;
		}
    } 
	else 
    {
        lin_ab_t2_state = lin_ab_t2_state + 1u; 
	}
	
}




/* Driver Callback to get Serial Number of ECU. */
/* Argument 1:  Output: pointer to data buffer for serial number  */
 void l_callback_RBI_ReadEcuSerialNumber(l_u8 SerialnumberResult[])
{
   /* If read ECU serial number (read by ID service - ID 1) is active,
    * fill 4 bytes of data of the array for transmitting the correct
    * data in the response.
    */
  
   SerialnumberResult[0] = 0x08u;
   SerialnumberResult[1] = 0x15u;
   SerialnumberResult[2] = 0x47u;
   SerialnumberResult[3] = 0x11u;
}


/* Driver Callback to get User defined answer for specific id. */
/* Argument 1: Input: ID from request */
/* Argument 2: Output: pointer to data buffer response (5 bytes of data) */
l_u8 l_callback_RBI_UserDefined(l_u8 id,l_u8 UserDefinedResult[])
{
   l_u8 retval = 0u;

   /* Assemble data and return data to LIN driver to transmit. Length of
    * response data different for different LIN standards:
    * LIN 2.0 / 2.1: Response data has to be filled with 5 bytes. Return value
    *                for positive response 1 to 5,
    *                for negative response = 0
    *                for no response > 5 (not recommended)
    * LIN 2.2: Allows response length to be configurable, so the return value
    *          is used as information how many databytes are transferred:
    *          for negative response = 0
    *          for positive response 1 to 5
    *                for no response > 5 (not recommended)
    *          This option can be activated by adding a define to genLinConfig.h:
    *          #define LIN_RBI_UDEF_2_2
    */
   switch (id)
   {
      case 32:
         /* define cell serial number here */
         UserDefinedResult[0] = id;
         UserDefinedResult[1] = id;
         UserDefinedResult[2] = 0x08u;
         UserDefinedResult[3] = 0x09u;
         UserDefinedResult[4] = 0xFFu;
         retval = 5u;
         break;
      case 33:
         /* define cell serial number here */
         UserDefinedResult[0] = id;
         UserDefinedResult[1] = id;
         UserDefinedResult[2] = 0x08u;
         UserDefinedResult[3] = 0x09u;
         UserDefinedResult[4] = 0xFFu;
         retval = 5u;
         break;
      case 34:
         /* define cell serial number here */
         UserDefinedResult[0] = id;
         UserDefinedResult[1] = id;
         UserDefinedResult[2] = 0x08u;
         UserDefinedResult[3] = 0x09u;
         UserDefinedResult[4] = 0xFFu;
         retval = 5u;
         break;
      case 35:
         /* define cell serial number here */
         UserDefinedResult[0] = id;
         UserDefinedResult[1] = id;
         UserDefinedResult[2] = 0x08u;
         UserDefinedResult[3] = 0x09u;
         UserDefinedResult[4] = 0xFFu;
         retval = 5u;
         break;
      default:
	  	 /* Intentionally Empty */
         break;
   }
   return retval;
}




/* myung api: transfer data to Diag manager stoped by Router*/
static void l_callback_RxIndication (const l_u8* transmitFlag)
{
    if ( (LIN_DIAG_RECEIVE_FRAME.frame.NAD == g_lin_prod_id.NAD) || (LIN_DIAG_RECEIVE_FRAME.frame.NAD == g_lin_prod_id.Initial_NAD))
    {
        if ((ld_rx_status() == LD_COMPLETED))
        {
            CommRouter_RxIndication (DIAG_PROTOCOL_PHYSICAL_ADDR_TYPE, (uint16_t)lin_rx_diag_buffer_length, ROUTER_DEST_DIAG_LIN, transmitFlag);
            NadTimeOutCounter = 0;
        }
        else {};
    }
    else if(LIN_DIAG_RECEIVE_FRAME.frame.NAD == LIN_FUNCTIONAL_NAD)
    {
        if ((ld_rx_status() == LD_COMPLETED))
        {    
            CommRouter_RxIndication (DIAG_PROTOCOL_PHYSICAL_ADDR_TYPE, (uint16_t)lin_rx_diag_buffer_length, ROUTER_DEST_DIAG_LIN, transmitFlag);
            NadTimeOutCounter = 0;
        }
        else { /* no action */};
    }
	else
	{
	/* no action */
	}
}

/* myung api: transfer tx indication flag to Diag manager stoped by Router*/
static void l_callback_TxIndication (const l_u8* transmitFlag)
{
    CommRouter_TxIndication(ROUTER_DEST_DIAG_LIN, transmitFlag);
}


/* myung api: receive data from Diag manager stoped by Router*/
l_u16 l_callback_ReponseData (l_u16 txBufferLength)
{
    /* enable to transmit function */
    lin_diag_Transmit_flag = TRUE;
    
    /* set a length for transmit */
    lin_tx_diag_buffer_length = txBufferLength;

    return lin_tx_diag_buffer_length;
}


