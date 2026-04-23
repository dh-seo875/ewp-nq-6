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
static void l_callback_RxIndication (l_u8* transmitFlag);
static void l_callback_TxIndication (l_u8* transmitFlag);
/*******************************************************************************
**                     Local function                                         **
*******************************************************************************/

static void LIN_TransportLayerMain(void);
static void LIN_CoolantDiagFlag (void);


/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/
/* Global variables */
static volatile unsigned char timer_1ms;



extern l_u8 aliveCheck;

static l_u16 NadTimeOutCounter = 0;
static volatile l_u8  LinTimeOutErrorFlag = 0;

l_u8 linSuccessfulFlag = 0;
l_u8 forceNvmWriteFlag = 0;

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
static l_u8  lin_diag_Transmit_timeOut = 0;

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
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

l_u8 LIN_TP_INIT (l_u8* rx_diag_buffer, l_u8* tx_diag_buffer, l_u8 rx_diag_buffer_max)
{
    l_u8 retValue = TRUE;
    lin_diag_received_buffer = rx_diag_buffer;
    lin_diag_Transmit_buffer = tx_diag_buffer;
    lin_diag_statemachine = 0;
    lin_rx_diag_buffer_Max_length = rx_diag_buffer_max;
    lin_rx_diag_buffer_length = 0;
    lin_tx_diag_buffer_length = 0;

    if((lin_diag_received_buffer == NULL_PTR) || (lin_diag_Transmit_buffer == 0u) || (lin_rx_diag_buffer_Max_length == 0u))
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
    l_u8 faultStateMerged=0;
    l_u8 warningStateMerged=0;
    l_u8 errorCodeIndex=0;
    l_bool protectionFlag;
    l_bool faultFlag;
    signed short speedFiltered=0;    
    uint16_t speedRequired=0;
    signed short idcFiltered=0;
    signed short iqFiltered=0;
	static volatile l_u8 timer_100ms;
	static unsigned int BusIdleTimeOutCounter, tmpStatus;
	static l_u16 LinTimeOutCounter = 0;
	static volatile unsigned char successful_timer_1ms;

    /* Variable for IRQ mask */
    static l_irqmask lin_irqState;

    timer_1ms = timer_1ms + 1u;

    /* cyclic communication task has to be called once every byte */
    /* location is changed in accordance with IHR poring guid. */
    l_cyclic_com_task();
    LIN_TransportLayerMain();

    /* myunghwa code for using Nvm.c */
    if (linSuccessfulFlag == 0u)
    {
        if (g_lin_status_word.flag.successful_transfer == 1u)
        {
            successful_timer_1ms = successful_timer_1ms + 1u;
            linSuccessfulFlag = 1u;           
        }
        else
        {
        }
    }
    else
    {
        successful_timer_1ms = successful_timer_1ms + 1u;
        if (successful_timer_1ms > 4u)
        {
            successful_timer_1ms = 0u;
            linSuccessfulFlag = 0u;
        }
        else
        {
        }
    }
	
    
    /* 1ms cyclic task */
    if (timer_1ms >= 4u)
    {
        timer_1ms = 0u;
        BusIdleTimeOutCounter++;
     
        /* Every interrupt that contains either signal read (incl.
         * status flag management), signal write, driver status management,
         * driver tasks (incl. ld_task, cyclic_com_task, l_ifc_rx) should be
         * protected against interruption from other.
         */
        lin_irqState = l_sys_irq_disable();
        /* Call the cyclic task according to the configured value
         * (field "LIN Task Cycletime" in the LDC-Tool) */
        ld_task();
          
        /* for more detailed information */
        //tmpStatus = l_ifc_read_status();
        l_sys_irq_restore(lin_irqState);

        /* get status of lin driver from status word, see LIN specification 
        for more detailed information */
        tmpStatus = l_ifc_read_status();
        
        /* If one of the statusflags is set then there was something on the bus
        reset counter and wake up */
        if ((tmpStatus & 0x00FFu) != 0u)
        {
            BusIdleTimeOutCounter = 0u;
        }
        
        /* After 4 seconds no communcation or other disturbtion - go to sleep */
        if(BusIdleTimeOutCounter >= 4000u)
        {
            BusIdleTimeOutCounter = 4000u; /* prevent overflow */
        }   
        
        /* cyclic task of driver (timeout and diagnostic control */
        ld_task();

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

        if ((LinTimeOutCounter >= 2000u) && (NadTimeOutCounter >= 2000u))
        {
            LinTimeOutCounter = 2000;
            NadTimeOutCounter = 2000;
            LinTimeOutErrorFlag = 1; /* occurred time out */
        }
        else
        {
            LinTimeOutErrorFlag = 0; /* Clear time out */
        }

#if((EWP_TYPE == PEWP_TYPE2_0X37) || (EWP_TYPE == PEWP_TYPE2_0X38) || (EWP_TYPE == PEWP_TYPE3_0X37) || (EWP_TYPE == PEWP_TYPE3_0X38) || (EWP_TYPE == BEWP_TYPE3_0X38))
        /* Write the Coolant flag */
        LIN_CoolantDiagFlag();
				
#elif((EWP_TYPE == IEWP_TYPE2_0X37) || (EWP_TYPE == AEWP_TYPE2_0X37) || (EWP_TYPE == IEWP_TYPE3_0X37) || (EWP_TYPE == AEWP_TYPE2_0X37_WITHOUTHOTSPOT) || (EWP_TYPE == BEWP_TYPE3_0X37))
		/* No Action */
#else
		/* No Action */
#endif

        

#if((EWP_TYPE == PEWP_TYPE2_0X37) || (EWP_TYPE == IEWP_TYPE2_0X37) || (EWP_TYPE == PEWP_TYPE3_0X37) || (EWP_TYPE == IEWP_TYPE3_0X37) || (EWP_TYPE == BEWP_TYPE3_0X37))
        if (l_flg_tst_Ewp1_Target_Spd_RPM())
        {
            l_flg_clr_Ewp1_Target_Spd_RPM();
            speedRequired = ((uint16)l_u8_rd_Ewp1_Target_Spd_RPM() * (uint16)20u);
            WriteSpeedRequired((sint16)speedRequired);
        }

        timer_100ms++;
        if (timer_100ms > 100 )
        {
            timer_100ms = 0;
            /* Insert database */
            ReadSpeedFiltered(&speedFiltered);
            ReadWarningStateMerged(&warningStateMerged);
            ReadFaultStateMerged(&faultStateMerged);
            ReadFaultFlag(&faultFlag);
            ReadProtectionFlag(&protectionFlag);
            ReadIdcFiltered(&idcFiltered);
            ReadIqFiltered(&iqFiltered);
			ReadErrorCodeIndex(&errorCodeIndex);            

            if(iqFiltered < 0)
            {
                iqFiltered = 0;
            }
            else
            {
            }

            if(idcFiltered < 0)
            {
                idcFiltered = 0;
            }
            else
            {
            }

            if(speedFiltered < 0)
            {
                speedFiltered = 0;
            }
            else
            {
            }
            
            l_bool_wr_Ewp1_FaultStatus(faultFlag);                           /* Signal Write Access for Signal Ewp1_FaultStatus */
            l_bool_wr_Ewp1_ProtectMode(protectionFlag);                      /* Signal Write Access for Signal Ewp1_ProtectMode */            
            l_u8_wr_Ewp1_ActRotSpd_rpm(speedFiltered / (uint16)20);          /* Signal Write Access for Signal Ewp1_ActRotSpd_rpm */
            l_u8_wr_Ewp1_Version((EWP_TYPE<<4)|VERSION);                     /* Signal Write Access for Signal Ewp1_Version */
            l_u8_wr_Ewp1_EEPROM_POPUP(errorCodeIndex);                       /* Signal Write Access for Signal Ewp1_EEPROM_POPUP */
            l_u8_wr_Ewp1_Current_DC(idcFiltered/10);                         /* Signal Write Access for Signal Ewp1_PWM_Duty_Ratio */
            l_u8_wr_Ewp1_Current_Iq(iqFiltered/10);                          /* Signal Write Access for Signal Ewp1_Torque */
            l_bool_wr_Ewp1_Warn_Temp     (SHIFT(warningStateMerged, 1));     /* Signal Write Access for Signal Ewp1_Warn_Temp */
            l_bool_wr_Ewp1_Short_Circuit (SHIFT(faultStateMerged, 7));       /* Signal Write Access for Signal Ewp1_Short_Circuit */
            l_bool_wr_Ewp1_GateDrv_UV_OV (SHIFT(faultStateMerged, 6));       /* Signal Write Access for Signal Ewp1_GateDrv_UV_OV */
            l_bool_wr_Ewp1_Under_Voltage (SHIFT(faultStateMerged, 5));       /* Signal Write Access for Signal Ewp1_Under_Voltage */
            l_bool_wr_Ewp1_Over_Voltage  (SHIFT(faultStateMerged, 4));       /* Signal Write Access for Signal Ewp1_Over_Voltage */
            l_bool_wr_Ewp1_Over_Current  (SHIFT(faultStateMerged, 3));       /* Signal Write Access for Signal Ewp1_Over_Current */
            l_bool_wr_Ewp1_Low_Current   (SHIFT(faultStateMerged, 2));       /* Signal Write Access for Signal Ewp1_Low_Current */
            l_bool_wr_Ewp1_Over_Temp     (SHIFT(faultStateMerged, 1));       /* Signal Write Access for Signal Ewp1_Over_Temp */
            l_bool_wr_Ewp1_Speed_Error   (SHIFT(faultStateMerged, 0) | SHIFT(warningStateMerged, 0));      /* Signal Write Access for Signal Ewp1_Speed_Error */
            l_bool_wr_Ewp1_Comm_Error(1);                  /* Don't write Access for Signal Ewp1_Comm_Error */
         }

#elif((EWP_TYPE == AEWP_TYPE2_0X37) || (EWP_TYPE == AEWP_TYPE2_0X37_WITHOUTHOTSPOT))
        if(l_flg_tst_Req_ChkSum())
        {
        	l_flg_clr_Req_ChkSum();
            EMS_checkSum = l_u8_rd_Req_ChkSum();

        	EMS_checkSumCal =0x0F&(0x10-(((l_LinData.frames.l_frm_COMMAND_EWP.frame_data[0]&0x0F) +(l_LinData.frames.l_frm_COMMAND_EWP.frame_data[1]&0x0F)
            +(l_LinData.frames.l_frm_COMMAND_EWP.frame_data[2]&0x0F) +(l_LinData.frames.l_frm_COMMAND_EWP.frame_data[3]&0x0F))
            +((l_LinData.frames.l_frm_COMMAND_EWP.frame_data[0]>>4 )+(l_LinData.frames.l_frm_COMMAND_EWP.frame_data[1]>>4)
            +(l_LinData.frames.l_frm_COMMAND_EWP.frame_data[2]>>4))));

            if(EMS_checkSum == EMS_checkSumCal)
            {
            }
            else
            {
            }
            speedRequired = ((uint16)l_u8_rd_Ewp1_Target_Spd_RPM() * (uint16)20u);
            WriteSpeedRequired((sint16)speedRequired);
        }
        else 
        {
        }

        timer_100ms++;        
        if (timer_100ms > 100 )
        {
            timer_100ms = 0;
            /* Insert database */
            ReadSpeedFiltered(&speedFiltered);
            ReadWarningStateMerged(&warningStateMerged);
            ReadFaultStateMerged(&faultStateMerged);
            ReadFaultFlag(&faultFlag);
            ReadProtectionFlag(&protectionFlag);
            ReadIqFiltered(&iqFiltered);
		
            if(speedFiltered < 0)
            {
            speedFiltered = 0;
            }
            else
            {
            }
					
            l_bool_wr_Ewp_FaultStatus(faultFlag);						 /* Signal Write Access for Signal Ewp_FaultStatus */
            l_bool_wr_Ewp_ProtectMode(protectionFlag); 					 /* Signal Write Access for Signal Ewp_ProtectMode */			  
            l_u8_wr_Ewp_ActRotSpd_rpm(speedFiltered / (uint16)20); 		 /* Signal Write Access for Signal Ewp_ActRotSpd_rpm */
            l_u8_wr_Ewp_Version((EWP_TYPE<<4)|VERSION);					 /* Signal Write Access for Signal Ewp_Version */
        }
        if (aliveCheck == 1)
		{
		    aliveCheck = 0;
			EWP_aliveCounter++;
			if(EWP_aliveCounter > 15)
			{
				EWP_aliveCounter = 0;
            }            
		    else
			{
			}
			
			l_u8_wr_Ewp_Req_Alive(EWP_aliveCounter);
		}
		
        EWP_checkSum = 0x0F&(0x10-(((l_LinData.frames.l_frm_STATUS_EWP1.frame_data[0]&0x0F) +(l_LinData.frames.l_frm_STATUS_EWP1.frame_data[1]&0x0F)
        +(l_LinData.frames.l_frm_STATUS_EWP1.frame_data[2]&0x0F) +(l_LinData.frames.l_frm_STATUS_EWP1.frame_data[3]&0x0F))
        +((l_LinData.frames.l_frm_STATUS_EWP1.frame_data[0]>>4 )+(l_LinData.frames.l_frm_STATUS_EWP1.frame_data[1]>>4)
        +(l_LinData.frames.l_frm_STATUS_EWP1.frame_data[2]>>4))));
        l_u8_wr_Ewp_Req_ChkSum(EWP_checkSum);

#elif((EWP_TYPE == PEWP_TYPE2_0X38) || (EWP_TYPE == PEWP_TYPE3_0X38) || (EWP_TYPE == BEWP_TYPE3_0X38))
        if (l_flg_tst_Ewp2_Target_Spd_RPM())
        {
        	l_flg_clr_Ewp2_Target_Spd_RPM();
        	speedRequired = ((uint16)l_u8_rd_Ewp2_Target_Spd_RPM() * (uint16)20u);
        	(void)WriteSpeedRequired((sint16_t)speedRequired);
        }

        timer_100ms = timer_100ms + 1u;
        if (timer_100ms > 100u )
        {
            timer_100ms = 0;
            /* Insert database */
            (void)ReadSpeedFiltered(&speedFiltered);
            (void)ReadWarningStateMerged(&warningStateMerged);
            (void)ReadFaultStateMerged(&faultStateMerged);
            (void)ReadFaultFlag(&faultFlag);
            (void)ReadProtectionFlag(&protectionFlag);
            (void)ReadIdcFiltered(&idcFiltered);
            (void)ReadIqFiltered(&iqFiltered);

            if(iqFiltered < 0)
            {
                iqFiltered = 0;
            }
            else
            {
            }

            if(idcFiltered < 0)
            {
                idcFiltered = 0;
            }
            else
            {
            }

            if(speedFiltered < 0)
            {
                speedFiltered = 0;
            }
            else
            {
            }

            l_bool_wr_Ewp1_FaultStatus(faultFlag);                           /* Signal Write Access for Signal Ewp1_FaultStatus */
            l_bool_wr_Ewp1_ProtectMode(protectionFlag);                      /* Signal Write Access for Signal Ewp1_ProtectMode */            
            l_u8_wr_Ewp1_ActRotSpd_rpm(speedFiltered / (sint16)20);          /* Signal Write Access for Signal Ewp1_ActRotSpd_rpm */
            l_u8_wr_Ewp1_Version((EWP_TYPE<<4u)|VERSION);                     /* Signal Write Access for Signal Ewp1_Version */
            l_u8_wr_Ewp1_EEPROM_POPUP(errorCodeIndex);                                    /* Signal Write Access for Signal Ewp1_EEPROM_POPUP */
            l_u8_wr_Ewp1_Current_DC(idcFiltered/10);                     /* Signal Write Access for Signal Ewp1_PWM_Duty_Ratio */
            l_u8_wr_Ewp1_Current_Iq(iqFiltered/10);                              /* Signal Write Access for Signal Ewp1_Torque */
            l_bool_wr_Ewp1_Warn_Temp     (((uint8_t)warningStateMerged >> 1u) & 0x01u);     /* Signal Write Access for Signal Ewp1_Warn_Temp */
            l_bool_wr_Ewp1_Short_Circuit (((uint8_t)faultStateMerged >> 7u));       /* Signal Write Access for Signal Ewp1_Short_Circuit */
            l_bool_wr_Ewp1_GateDrv_UV_OV (((uint8_t)faultStateMerged >> 6u) & 0x01u);       /* Signal Write Access for Signal Ewp1_GateDrv_UV_OV */
            l_bool_wr_Ewp1_Under_Voltage (((uint8_t)faultStateMerged >> 5u) & 0x01u);       /* Signal Write Access for Signal Ewp1_Under_Voltage */
            l_bool_wr_Ewp1_Over_Voltage  (((uint8_t)faultStateMerged >> 4u) & 0x01u);       /* Signal Write Access for Signal Ewp1_Over_Voltage */
            l_bool_wr_Ewp1_Over_Current  (((uint8_t)faultStateMerged >> 3u) & 0x01u);       /* Signal Write Access for Signal Ewp1_Over_Current */
            l_bool_wr_Ewp1_Low_Current   (((uint8_t)faultStateMerged >> 2u) & 0x01u);       /* Signal Write Access for Signal Ewp1_Low_Current */
            l_bool_wr_Ewp1_Over_Temp     (((uint8_t)faultStateMerged >> 1u) & 0x01u);       /* Signal Write Access for Signal Ewp1_Over_Temp */
            l_bool_wr_Ewp1_Speed_Error   ((((uint8_t)faultStateMerged) & 0x01u) | (((uint8_t)warningStateMerged) & 0x01u));      /* Signal Write Access for Signal Ewp1_Speed_Error */
            l_bool_wr_Ewp1_Comm_Error(1u);                  /* Don't write Access for Signal Ewp1_Comm_Error */
         }

#else
/* No Action */
#endif
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
#ifdef AUTOBAUD_ENABLED
    if (TIMER2->T2CON.bit.EXF2 == 1u)
    {
        l_ifc_t2();
        TIMER2->T2ICLR.bit.EXF2CLR = 1u;
    }
#endif /* end #ifdef AUTOBAUD_ENABLED */
    if ((UART1->SCON.reg & 0x01u) == 1u)
    {
        l_ifc_rx();
        UART1->SCONCLR.bit.RICLR = 1u;
    }
    
    /* cyclic communication task has to be called once every byte */
    //l_cyclic_com_task();
}

l_u8 LIN_TimerOutError (void)
{
    return LinTimeOutErrorFlag;
}

#if((EWP_TYPE == PEWP_TYPE2_0X37) || (EWP_TYPE == PEWP_TYPE2_0X38) || (EWP_TYPE == PEWP_TYPE3_0X37) || (EWP_TYPE == PEWP_TYPE3_0X38 || (EWP_TYPE == BEWP_TYPE3_0X37) || (EWP_TYPE == BEWP_TYPE3_0X38)))
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
#elif((EWP_TYPE == IEWP_TYPE2_0X37) || (EWP_TYPE == AEWP_TYPE2_0X37) || (EWP_TYPE == IEWP_TYPE3_0X37) || (EWP_TYPE == AEWP_TYPE2_0X37_WITHOUTHOTSPOT))
		/* No Action */
#else
		/* No Action */
#endif




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
static void l_callback_RxIndication (l_u8* transmitFlag)
{
    if ( (LIN_DIAG_RECEIVE_FRAME.frame.NAD == g_lin_prod_id.NAD) || (LIN_DIAG_RECEIVE_FRAME.frame.NAD == g_lin_prod_id.Initial_NAD))
    {
        if ((ld_rx_status() == LD_COMPLETED))
        {
            CommRouter_RxIndication (DIAG_PROTOCOL_PHYSICAL_ADDR_TYPE, lin_rx_diag_buffer_length, ROUTER_DEST_DIAG_LIN, transmitFlag);
            NadTimeOutCounter = 0;
        }
        else {};
    }
    else if(LIN_DIAG_RECEIVE_FRAME.frame.NAD == LIN_FUNCTIONAL_NAD)
    {
        if ((ld_rx_status() == LD_COMPLETED))
        {    
            CommRouter_RxIndication (DIAG_PROTOCOL_PHYSICAL_ADDR_TYPE, lin_rx_diag_buffer_length, ROUTER_DEST_DIAG_LIN, transmitFlag);
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
static void l_callback_TxIndication (l_u8* transmitFlag)
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


