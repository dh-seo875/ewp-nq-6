/******************************************************************************
*                _  _               ___             _                         *
*               (_)| |__   _ __    / _ \ _ __ ___  | |__    /\  /\            *
*               | || '_ \ | '__|  / /_\/| '_ ` _ \ | '_ \  / /_/ /            *
*               | || | | || |    / /_\\ | | | | | || |_) |/ __  /             *
*               |_||_| |_||_|    \____/ |_| |_| |_||_.__/ \/ /_/              *
*                                                                             *
*    ihr GmbH                                                                 *
*    Airport Boulevard B210                                                   *
*    77836 Rheinm?ster - Germany                                             *
*    http://www.ihr.de                                                        *
*    Phone +49(0) 7229-18475-0                                                *
*    Fax   +49(0) 7229-18475-11                                               *
*                                                                             *
*******************************************************************************
*                                                                             *
* (c) Alle Rechte bleiben bei IHR GmbH, auch fuer den Fall von Schutzrechts-  *
* anmeldungen. Jede Verfuegungsbefugnis, wie Kopier- und Weitergaberecht      *
* bleibt bei IHR GmbH.                                                        *
*                                                                             *
* (c) All rights reserved by IHR GmbH including the right to file             *
* industrial property rights. IHR GmbH retains the sole power of              *
* disposition such as reproduction or distribution.                           *
*                                                                             *
********************     Workfile:      lin_hal.c        **********************
*                                                                             *
*  PROJECT-DESCRIPTION:  Infineon Hardware Abstraction Layer                  *
*  FILE-DESCRIPTION:     All Hardware Access Routines for LIN Driver          *
*                                                                             *
*******************************************************************************
*                                                                             *
*       Revision:        $Rev:: 407         $                                 *
*       Responsible:     B.Reiss                                              *
*       Co-Responsible:  P.Koch                                               *
*       Last Modtime:    $Date:: 2017-09-13#$                                 *
*                                                                             *
******************************************************************************/
/**
@file lin_hal.c
@brief Infineon TLE986x/7x specific implementation of the LIN Driver communication
*/

/* ===========================================================================
 *  Header files
 * ==========================================================================*/

#include "genLinConfig.h"
#include "lin_main.h"
#include "lin_slave_task.h"
#include "lin_driver_api.h"
#include "lin_config.h"
#include "lin_type.h"
#include "lin_hal.h"

#include "..\uds\def_lin.h"
#include "..\uds\uds.h"
#include "..\uds\util.h"
/* ===========================================================================
 *  Global Variables
 * ==========================================================================*/

LIN_hal_stat_reg_t         lin_hal_status_g;
static l_u8                lin_hal_rx_data_guc;
LIN_SLAVE_STATE_t          lin_slave_state_g = idle;
t_lin_temp_frame_buffer    lin_temp_buffer;
static l_u8                uc_frame_lin_version;
static l_u8                temp_data_size_uc;
static LIN_TX_DATA_t       lin_tx_data_g;
static LIN_FLAGS_t         lin_flag_g;
l_u8                       lin_ab_timeout_timer;
l_u8                       lin_frm_timeout_timer;
t_lin_timeout_ctrl         lin_timeout_ctrl;
static l_u16               l_chk_sum_ui;
l_u8                       l_txchk_byte_nr;
volatile l_u8              lin_ab_t2_state;

#ifdef AUTOBAUD_ENABLED 
volatile l_u16              lin_temp_word;
volatile l_u16             lin_synch_time;
#endif /* end #ifdef AUTOBAUD_ENABLED */

/* ===========================================================================
 *  Function Prototypes
 * ==========================================================================*/

static void l_hal_tx_state_machine(void);
static void lin_hal_tx_checksum(l_u8 checksum);
static void lin_hal_tx_byte(l_u8 lin_hal_bytetoTX);

/* ===========================================================================
 *  Functions
 * ==========================================================================*/


/* ---------------------------------------------------------------------------
 *  void lin_hal_init (void)
 * --------------------------------------------------------------------------*/
/**
   @brief  Initializes the HAL layer and UART / SCI
   @param  void
   @retval void
*/
void lin_hal_init (void) 
{
   l_u8 loc_i = 0u;

   lin_hal_rx_data_guc              = 0u;
   lin_slave_state_g                = idle;
   uc_frame_lin_version             = 0u;  
   temp_data_size_uc                = L_RESET; 
   lin_ab_timeout_timer             = 0u;
   lin_frm_timeout_timer            = 0u;
   lin_timeout_ctrl.byte            = 0u;
   l_chk_sum_ui                     = 0u;
   l_txchk_byte_nr                  = 0u;
   lin_ab_t2_state                 = 0u;

   lin_hal_status_g.byte_received   = 0u;
   lin_hal_status_g.ferror          = 0u;
   lin_hal_status_g.stopbit         = 0u;
   lin_hal_status_g.isr_rec_break   = 0u;
   lin_hal_status_g.isr_rec_sync    = 0u;
   lin_hal_status_g.res0            = 0u;

   lin_tx_data_g.checksum           = 0u;
   lin_tx_data_g.index_uc           = 0u;
   lin_tx_data_g.length_c           = 0u;
   lin_tx_data_g.data_puc           = 0u;
   lin_tx_data_g.old_data_uc        = 0u;

   lin_flag_g.res0                  = 0u;
   lin_flag_g.tx_finished           = 0u;
   lin_flag_g.tx_next               = 0u;
      
   for(loc_i = 0u; loc_i < 11u; loc_i++)
   {
      lin_temp_buffer.byte[loc_i] = 0u;
   }
   
   lin_timeout_ctrl.byte = 0u;
   lin_hal_init_uart();

#ifdef AUTOBAUD_ENABLED 
   lin_temp_word = 0u;
   lin_synch_time = 0u;
#endif /* end #ifdef AUTOBAUD_ENABLED */

   return;
}

/* ---------------------------------------------------------------------------
 *  void lin_hal_tx_byte(l_u8 lin_hal_bytetoTX)
 * --------------------------------------------------------------------------*/
/**
   @brief  Transmits one byte on bus
   @pre    LIN driver initialized
   @param  lin_hal_bytetoTX           pointer to frame buffer
   @retval void
*/
static void lin_hal_tx_byte(l_u8 lin_hal_bytetoTX)
{
   /* Store value for receive check later */
   lin_tx_data_g.old_data_uc = lin_hal_bytetoTX;

   /* send data to UART, for transmit */
   lin_hal_tx_char(lin_hal_bytetoTX);

   /* make configuration for tx next data  */
   lin_tx_data_g.length_c--;
   /* check if next field is CRC */
   if(0u == lin_tx_data_g.length_c)
   {
      lin_flag_g.tx_next = CHECKSUM_NEXT;  
   } 
   else 
   {
      lin_flag_g.tx_next = L_SET;
   }
}

/* ---------------------------------------------------------------------------
 *  void lin_hal_tx_response (l_u8 l_type, l_u8 l_data[], l_u8 l_len)
 * --------------------------------------------------------------------------*/
/**
   @brief  This function prepares the driver for transmitting a response frame.
   @pre    LIN driver initialized
   @param  l_u8 l_type   LIN type (1.3, 2.0, or 2.1)
   @param  l_u8 l_data[] Databuffer for frame data
   @param  l_u8 l_len    Length information of frame
   @retval void
*/
void lin_hal_tx_response (l_u8 l_type, const l_u8 l_data[], l_u8 l_len) 
{                                                                                     
   l_u16 ui_I = 0u;
   l_u8 uc_temp_sum = 0u;   
   lin_tx_data_g.index_uc = 0u;
   lin_slave_state_g = tx_data;
   lin_tx_data_g.length_c = l_len;
   g_lin_frame_data_size = l_len;
   if(l_type == LIN_2_X)
   {
      /* For LIN 2.x use PID & Data for CRC */
      l_chk_sum_ui = lin_temp_buffer.frame.PID;
   }
   else 
   {
      l_chk_sum_ui = 0u;
   }
   g_lin_irqState = l_sys_irq_disable();
   /* Copy data from main buffer to the tx buffer */
   for(ui_I = 0u; ui_I < l_len; ui_I++)
   {
      lin_temp_buffer.frame.DataBuffer[ui_I] = l_data[ui_I];
   }
   l_sys_irq_restore(g_lin_irqState);

   l_txchk_byte_nr = lin_tx_data_g.length_c - 1u;
   l_chk_sum_ui += lin_temp_buffer.frame.DataBuffer[l_txchk_byte_nr];
   /* if sum is bigger than 16 bit */
   uc_temp_sum = (l_u8)(l_chk_sum_ui >> 8);
   l_chk_sum_ui = l_chk_sum_ui + uc_temp_sum;
   l_chk_sum_ui &= 0xFFu;
   /* frame with only 1 databyte */
   if (l_txchk_byte_nr == 0u)
   {
      lin_tx_data_g.checksum = ~(l_u8)l_chk_sum_ui;
   }

   /* start transmit of data fields */
   lin_hal_tx_byte(lin_temp_buffer.frame.DataBuffer[0]);
   lin_temp_buffer.frame.BufferIndex = 1u; /* Index of next data to transmit */
   lin_timeout_ctrl.flag.transm_ongoing = 1u;
#ifdef LIN_PROTOCOL_VERSION_1_3
   /* Start timeout measurement for LIN 1.3 also in TX branch. */
   lin_timeout_ctrl.flag.frame = 1u;
   lin_frm_timeout_timer = 0u;
#endif /* end #ifdef LIN_PROTOCOL_VERSION_1_3 */
   return;                                                                                                                                                    
}

/* ---------------------------------------------------------------------------
 *  void l_hal_tx_state_machine (void)
 * --------------------------------------------------------------------------*/
/**
   @brief  This function controls the transmission of frame data and checksum
   @pre    LIN driver initialized
   @param  void
   @retval void
*/
static void l_hal_tx_state_machine (void)
{
   /* tx crc of frame */
   if (lin_flag_g.tx_next == CHECKSUM_NEXT)
   {
      lin_hal_tx_checksum(lin_tx_data_g.checksum);
   }
   /* tx next byte of frame */
   if (lin_flag_g.tx_next == L_SET)
   {
      /* Set state machine to transmit data */
      lin_slave_state_g = tx_data;
      lin_hal_tx_byte(lin_temp_buffer.frame.DataBuffer[lin_temp_buffer.frame.BufferIndex]);
      lin_temp_buffer.frame.BufferIndex ++; /* Index of next data to transmit */
   }
}

/* ---------------------------------------------------------------------------
 *  void lin_hal_tx_wake_up(void)
 * --------------------------------------------------------------------------*/
/**
   @brief  This function transmits a wake-up pulse on the bus
   @pre    LIN driver initialized
   @param  void
   @retval void
*/
void lin_hal_tx_wake_up(void)
{
   l_u8 loc_data = 0u;

   /* Send dummy data to UART to send Break */
#ifdef LIN_PROTOCOL_VERSION_1_3
   loc_data = 0x80;
#else
   loc_data = 0xe0u;
#endif /* end #ifdef LIN_1_3 */
   lin_hal_tx_char(loc_data);
}

/* ---------------------------------------------------------------------------
 *  void l_hal_txchecksum_calculation(void)
 * --------------------------------------------------------------------------*/
/**
   @brief  This function calculates the checksum of a publish frame step-wise
           for each byte that is actually transmitted.
   @pre    LIN driver initialized
   @param  void
   @retval void
*/
void l_hal_txchecksum_calculation(void)
{
   l_u8 uc_temp_sum;

   l_txchk_byte_nr--;
   /*  l_chk_sum_ui += l_txchk_data_p[l_txchk_byte_nr]; */
   l_chk_sum_ui += lin_temp_buffer.frame.DataBuffer[l_txchk_byte_nr];
   /* if sum is bigger than 16 bit */
   uc_temp_sum = (l_u8)(l_chk_sum_ui >> 8);
   l_chk_sum_ui = l_chk_sum_ui + uc_temp_sum;
   l_chk_sum_ui &= 0xFFu;
   if (l_txchk_byte_nr == 0u)
   {
      lin_tx_data_g.checksum = ~(l_u8)l_chk_sum_ui;
      l_chk_sum_ui = 0u;
   }
}

/* ---------------------------------------------------------------------------
 *  lin_hal_tx_checksum(l_u8 checksum)
 * --------------------------------------------------------------------------*/
/**
   @brief  This function transmits the checksum on the bus.
   @pre    LIN driver initialized
   @param  l_u8 checksum  The checksum of the frame.
   @retval void
*/
static void lin_hal_tx_checksum(l_u8 checksum)
{
   /* set next state in state machine */
   lin_slave_state_g = tx_checksum;

   /* send data to UART, for transmit */
   lin_hal_tx_char(checksum);

   /* store data for receive check later */
   lin_tx_data_g.old_data_uc = checksum;
   /* reset tx state finish transmit of frame */
   lin_flag_g.tx_next = L_RESET;
}

/* ---------------------------------------------------------------------------
 *  void lin_hal_rx_response (l_u8 l_type, l_u8 l_len) 
 * --------------------------------------------------------------------------*/
/**
   @brief  This function prepares the driver for receiving a subscribe frame.
   @pre    LIN driver initialized
   @param  l_u8 l_type  The LIN protocol version of the frame.
   @param  l_u8 l_len   The length information of the frame.
   @retval void
*/
void lin_hal_rx_response (l_u8 l_type, l_u8 l_len)
{
   g_lin_frame_data_size = l_len;
   temp_data_size_uc = g_lin_frame_data_size;
   uc_frame_lin_version = l_type;
   lin_slave_state_g = rx_data;
   lin_timeout_ctrl.flag.frame = 1u;
#ifndef LIN_PROTOCOL_VERSION_1_3
   lin_frm_timeout_timer = 0u;
#endif
   return;
}

/* ---------------------------------------------------------------------------
 *  void l_hal_rx_state_machine (void) 
 * --------------------------------------------------------------------------*/
/**
   @brief  This function steps through the states for receiving frame headers
           and frame data. 
   @pre    LIN driver initialized
   @param  void
   @retval void
*/
void l_hal_rx_state_machine (void)
{
   l_u8 uc_temp_sum = 0u;
   /* next computations depend on receive task state (state machine) */
   switch(lin_slave_state_g)
   {
      case idle:
         break;
      
      /* if break received next data is sync field */
      case break_received:
         break;

      /* if sync field received next data is pid field */
      case sync_received:
         /* after sync field a pid field was received */
         /* reset lin_state_after receive of PID (Protocol layer will set for further rx/tx action) */
         lin_slave_state_g = idle;
         /* check if pid is valid and if its tx or rx frame */
         lin_slave_task_rx_pid(lin_hal_rx_data_guc);
         break;

      /* if it is rx frame, then data field comes next */
      case rx_data:    
         /* store received data and check for type of next field */
         lin_temp_buffer.frame.DataBuffer[(g_lin_frame_data_size - temp_data_size_uc)] = lin_hal_rx_data_guc;
         lin_timeout_ctrl.flag.frame = 1u;
         lin_timeout_ctrl.flag.transm_ongoing = 1u;
         lin_timeout_ctrl.flag.recept_started = 1u;
         lin_frm_timeout_timer = 0u;

         /* calculate temporary checksum (step wise after each byte) */
         if(g_lin_frame_data_size == temp_data_size_uc)
         {
            if (uc_frame_lin_version == LIN_2_X)
            {
               /* For Lin 2.x use PID & Data for CRC */
               l_chk_sum_ui = lin_temp_buffer.frame.PID;
            }
            else
            {
               l_chk_sum_ui = 0u;
            }
         }
         l_chk_sum_ui+= lin_temp_buffer.frame.DataBuffer[(g_lin_frame_data_size - temp_data_size_uc)];
         /* if sum is bigger than 16 bit */
         uc_temp_sum = (l_u8)(l_chk_sum_ui>>8);
         l_chk_sum_ui = l_chk_sum_ui + uc_temp_sum;
         l_chk_sum_ui &= 0xFFu;

         /* get number of data still to receive */
         temp_data_size_uc--;  

         /* if no more data to receive got to rx_checksum state */
         if ( 0u == temp_data_size_uc)
         {
            lin_slave_state_g = rx_checksum;  
         }
         break;

      /* if last data was received next is crc field */
      case rx_checksum:
         /* Store crc data and check if crc is okay */
         l_chk_sum_ui = ((l_u16)(~l_chk_sum_ui)) & 0x00FFu;
         /* check if checksum was faulty */
         if (lin_hal_rx_data_guc != (l_chk_sum_ui))
         {
#if ((defined LIN_PROTOCOL_VERSION_2_0) || ((defined LIN_PROTOCOL_VERSION_2_1) && (!defined LIN_PROTOCOL_VERSION_2_2)))
            if(g_lin_active_pid == 0x3Cu)
            {
               lin_diag_rx_chksum_error();
            }
#endif /* end #if ((defined LIN_PROTOCOL_VERSION_2_0) || ((defined LIN_PROTOCOL_VERSION_2_1) && (!defined LIN_PROTOCOL_VERSION_2_2))) */
#if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1
            lin_hal_set_resp_error();
#endif /* if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1 */
         } 
         else
         {
            /* notify upper layer about received frame. */
            lin_slave_task_rx_data();
         }
         lin_timeout_ctrl.flag.frame = 0u;
         lin_timeout_ctrl.flag.transm_ongoing = 0u;
         lin_timeout_ctrl.flag.recept_started = 0u;
         lin_frm_timeout_timer = 0u;
         /* Set lin slave state to idle to wait for next frame */
         lin_slave_state_g = idle;
         break;
      case tx_data:
         /* handled in lin_hal_tx_response() */
         break;

      case tx_checksum:
         /* check if transmission has crashed */
         if(lin_hal_rx_data_guc != lin_tx_data_g.old_data_uc)
         {
            /* TX Response ERROR */
            lin_slave_state_g = idle;
            lin_flag_g.tx_next = L_RESET;
#if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1
            lin_hal_set_resp_error();
#endif /* end #ifdef #if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1 */
         } 
         else 
         {
            /* if tx was okay, finish tx.  */
            lin_slave_state_g = idle;
            lin_slave_task_tx_data();
         }
         /* Reset transmission ongoing flag */
         lin_timeout_ctrl.flag.transm_ongoing = 0u;
#ifdef LIN_PROTOCOL_VERSION_1_3         
         lin_timeout_ctrl.flag.frame = 0u;
         lin_frm_timeout_timer = 0u;
#endif /* end #ifdef LIN_PROTOCOL_VERSION_1_3 */
         break;

      default:
         /* Impossible Slave State... */
         lin_slave_state_g = idle;
         break;
   }
}

/* ---------------------------------------------------------------------------
 *  void lin_hal_ISR(void) 
 * --------------------------------------------------------------------------*/
/**
   @brief  Interrupt service routine for receive interrupt.
   @pre    LIN driver initialized
   @param  void
   @retval void
*/
void lin_hal_ISR(void)
{
   l_u8 temp_linst;
   temp_linst = LIN_UART_STATUS;
   lin_hal_rx_data_guc = LIN_UART_BUFFER;

#ifdef AUTOBAUD_ENABLED
   /* Detect Break characters, configure and start T2 to start on next falling edge (startbit of Synch-field) */
   if (((temp_linst & 0x38u) == 0u) && (lin_hal_rx_data_guc == 0u) && ((LIN_UART_9BITREC == 0u)))
   {
      lin_ab_t2_state = 1u;
      lin_ab_timeout_timer = 0u;
      lin_hal_set_resp_error();
      lin_slave_state_g = break_received;
      lin_hal_init_T2();
   }
   
#endif /* end #ifdef AUTOBAUD_ENABLED */
   
#ifndef LIN_PROTOCOL_VERSION_1_3
   g_lin_status_word.flag.bus_activity = L_SET;
#endif /* end #ifndef LIN_PROTOCOL_VERSION_1_3 */

   /* Break and End of Synch Field detected */
   if ((temp_linst & 0x38u) == 0x18u)
   {
      lin_slave_state_g = sync_received;
#ifndef AUTOBAUD_ENABLED
      LIN_UART_BR_SYN_DET_EN = 0u;
#else
      LIN_UART_BR_SYN_DET_EN = 1u;
#endif /* end #ifndef AUTOBAUD_ENABLED */
      LIN_UART_CLR_STATUS =   0x38u; 
   }
   else
   {
      /* End of Synch Field detected w/o Break */
      if (temp_linst & 0x10u)
      {
         lin_slave_state_g = idle;
#ifndef AUTOBAUD_ENABLED
         LIN_UART_BR_SYN_DET_EN = 0u;
#else
         LIN_UART_BR_SYN_DET_EN = 1u;
#endif /* end #ifndef AUTOBAUD_ENABLED */
         LIN_UART_CLR_STATUS =   0x38u;
      }
      else 
      {
         /* Break detected w/o End of Synch Field */
         if (temp_linst & 0x08u)
         {
            lin_slave_state_g = idle;
#ifndef AUTOBAUD_ENABLED
            LIN_UART_BR_SYN_DET_EN = 0u;
#else
            LIN_UART_BR_SYN_DET_EN = 1u;
#endif /* end #ifndef AUTOBAUD_ENABLED */
            LIN_UART_CLR_STATUS =   0x38u;
            
         } 
         else 
         {
            if ((lin_slave_state_g >= rx_data) && (LIN_UART_9BITREC != 1u))
            {
               /* Framing error */
               lin_slave_state_g = idle;
               lin_flag_g.tx_next = L_RESET;
               lin_timeout_ctrl.flag.frame = 0u;
               lin_frm_timeout_timer = 0u;
#if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1
               if ((g_lin_frame_index >= LIN_NUMBER_OF_FRAMES) || ((g_lin_frame_ctrl[g_lin_frame_index].frame.frame_type.transfer_type & 0x01u) == 0u))
               {
                  /* FERR received in the slot intended for receiving or transmitting the
                   * first databyte of a frame, set flag to have response error bit set correctly */
                  lin_timeout_ctrl.flag.recept_started = 1u;
               }
               lin_hal_set_resp_error();
#if !(defined LIN_PROTOCOL_VERSION_2_2)
               if(g_lin_active_pid == 0x3Cu)
               {
                  lin_diag_rx_chksum_error();
               }
#endif /* end #if !(defined LIN_PROTOCOL_VERSION_2_2) */
#endif /* end #if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1 */
            }
            else 
            {
               if (lin_slave_state_g == tx_data)
               {
                  /* check if transmission has crashed */
                  if((lin_hal_rx_data_guc != lin_tx_data_g.old_data_uc) || (LIN_UART_9BITREC != 1u))
                  {
                     /* TX Response ERROR */
                     lin_slave_state_g = idle;
                     lin_flag_g.tx_next = L_RESET;
#if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1
                     lin_hal_set_resp_error();
#endif /* end #if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1 */
                  } 
                  else 
                  {
                     /* if tx was okay, send next field */
                     l_hal_tx_state_machine();
                  }
               }
               else 
               {
                  
                  if ((lin_slave_state_g != idle) && (LIN_UART_9BITREC == 1u))
                  {
                     lin_hal_status_g.byte_received = L_SET;
                  }
               }
            }
         }
      }
   }
 return;  
}

/* ---------------------------------------------------------------------------
 *  void lin_hal_init_uart(void) 
 * --------------------------------------------------------------------------*/
/**
   @brief  Initializes the UART / SCI
   @param  void
   @retval void
*/
void lin_hal_init_uart(void)
{
   LIN_UART_IR_EN =        0u;
#ifdef AUTOBAUD_ENABLED
   LIN_TIMER_EXT_IR_EN = 0u;
#endif /* end #ifdef AUTOBAUD_ENABLED */
   
   /* Reset UART to initial values */ 
   LIN_UART_CONTROL =      0x00u;
  
   /* Set UART to 8bit shift UART */
   LIN_UART_MODE0 =        0u;
   LIN_UART_MODE1 =        1u;

   /* Set Baudrate Generator and enable */
   LIN_UART_BCON_PRE =     BRG_PRE;
   LIN_UART_BCON_FDSEL =   BRG_FD_SEL;
   
   /* may only be written if LIN_UART_BCON_RUN == 0. This is true for first initialization 
      and after initialization after error in autobauding (Baudrate Generator is only restarted
      after successful sync (stopping Generator in this function may not suffice). */
   LIN_UART_BCON_BGL =     BRGL_VAL;
   LIN_UART_BCON_BGH =     BRGH_VAL;
   
   LIN_UART_BCON_RUN =     1u;
   
   /* configure autobauding */
#ifndef AUTOBAUD_ENABLED
   LIN_UART_BR_SYN_DET_EN =       0u;
#else

   LIN_UART_BR_SYN_DET_EN = 1u;

#endif /* end #ifndef AUTOBAUD_ENABLED */
   
   LIN_UART_CLR_STATUS =   0x38u;
   LIN_UART_BR_SYN_DET =   BG_SEL;
 
   /* Enable UART reception */
   LIN_UART_RUN =          1u;

   /* UART interrupt enable */
   LIN_UART_IR_SET_EN =    1u;
   LIN_UART_IR_EN =        1u;
   LIN_UART_IRTX_EN =      0u;
   LIN_UART_IR_PENDING =   1u;
   NVIC_SetPriority((IRQn_Type)LIN_UART_IRQ_NR ,LIN_INTERRUPT_PRIORITY);

#ifdef AUTOBAUD_ENABLED
   LIN_TIMER_PRE =         0u;  /* T2CLK = PCLK (20MHz) */
   LIN_TIMER_PRE_EN =      1u;  /* Timer2 Prescaler is used (T2PRE) */
   LIN_TIMER_COUNTER_EN =  0u;  /* UpDown Counter function disabled */
   LIN_TIMER_EDGE_SELECT = 0u;  /* Timer2 starts on external falling edge */
   LIN_TIMER_EXT_START =   1u;  /* Timer2 starts on external event */

   LIN_TIMER_CAP_REL =     1u;  /* Capture Mode */
   LIN_TIMER_T_C_SEL =     0u;  /* Timer function */
   LIN_TIMER_EXT_EN =      0u;  /* External Events disabled */

   /* set Timer2 interrupts */
   LIN_TIMER_EXT_IR_EN =   0u;  /* Interrupt for external events disabled */
   LIN_TIMER_INT_IR_EN =   0u;  /* Interrupt for Timer2 overflow disabled */

   /* Timer2 is needed for baudrate synchronization */
   LIN_UART_IR_SET_EN =    1u;
#endif /* end #ifdef AUTOBAUD_ENABLED */
}

#ifdef AUTOBAUD_ENABLED
void lin_hal_init_T2 (void)
{
   LIN_UART_BCON_RUN = 0u;          /* stop baudrate generator */
   LIN_TIMER_RUN = 0u;              /* stop timer for reinit */
   
   LIN_TIMER_HIGH = 0u;             /* reset the count value to '0' */
   LIN_TIMER_LOW = 0u;

   LIN_TIMER_EXT_EN = 1u;           /* External Events enabled */
   LIN_TIMER_EXT_IR_EN = 1u;        /* Interrupt for external events enabled */
   lin_ab_timeout_timer = 0u;
}
#endif /* end ifdef AUTOBAUD_ENABLED */

#if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1
/* ---------------------------------------------------------------------------
 *  void lin_hal_set_resp_error ()
 * --------------------------------------------------------------------------*/

/**
   @brief  Set Response Error flag and Statusword Update correctly dependent
           of current driver state.
   @note   Response error is set latest at beginning of a new sync break - if
           not timed out before via ld_task(). Flags are set again earliest at
           PID recognition.
   @param  void
   @retval void
 */
#if (defined J2602_PROTOCOL)
#ifdef GM_SPEC_ADAPTION
void lin_hal_set_resp_error (l_u8 j2602_fault)
#else
static void lin_hal_set_resp_error (l_u8 j2602_fault)
#endif /* #ifdef GM_SPEC_ADAPTION */
#else
/* Prototype for LIN protocols */
void lin_hal_set_resp_error (void)
#endif /* end #if defined J2602_PROTOCOL */
{
   /* Protect setting of variables in different contexts */
   g_lin_irqState = l_sys_irq_disable();
#ifndef GM_SPEC_ADAPTION
   if ((lin_timeout_ctrl.flag.recept_started != 0u) || (lin_timeout_ctrl.flag.transm_ongoing != 0u))
#else
   /* GM specific J2602 adaption needs also the PID errors shown in the response error flag */
   if ((lin_timeout_ctrl.flag.recept_started != 0u)
         || (lin_timeout_ctrl.flag.transm_ongoing != 0u)
         || ((g_j2602_status.flags.errorstate & 0x10) != 0u))
#endif /* #ifndef GM_SPEC_ADAPTION */
   {
#ifdef LIN_EVENT_TRIGGERED_FRAME_ENABLE
      if(g_lin_rx_ctrl.flag.evttrgfrm == 0u)
#endif /* #ifdef LIN_EVENT_TRIGGERED_FRAME_ENABLE */
      {
#if ((defined(J2602_PROTOCOL)) && (!(defined(GM_SPEC_ADAPTION))))
         /* J2602 errors are assembled in a variable for assembling the statusword when required for
          * transmission of a designated frame containing the status byte.
          * If receiving a broadcast frame ignore errors. */
         if (lin_timeout_ctrl.flag.recept_started == 1u)
         {
            if ((g_lin_active_pid != 0x78u) && (g_lin_active_pid != 0xBAu)
               && (g_lin_active_pid != 0x39u) && (g_lin_active_pid != 0xFBu))
            {
               g_j2602_status.flags.errorstate |= j2602_fault;
            }
         }
         else
         {
            /* If - Else is necessary because after reception of the first byte during a subscribe frame
             * the transm_ongoing flag is also set to indicate that a first data byte of a frame has been
             * received.
             * If a frame was being transmitted, indicate that the status byte was not successfully transmitted. */
            if (lin_timeout_ctrl.flag.transm_ongoing == 1u)
            {
               g_j2602_status.flags.errorstate |= j2602_fault;
               g_j2602_status.flags.status_byte_transm = 0u;
            }
         }
#endif /* end #ifndef J2602_PROTOCOL */
#ifdef GM_SPEC_ADAPTION
         /* Set errors except for broadcast frames. */
         if ((g_lin_active_pid != 0x78u) && (g_lin_active_pid != 0xBAu)
            && (g_lin_active_pid != 0x39u) && (g_lin_active_pid != 0xFBu))
         {
            LIN_STATUS_BYTE(1u);
         }
#endif /* end #ifdef GM_SPEC_ADAPTION */
#if ((defined LIN_PROTOCOL_VERSION_2_0) || (defined LIN_PROTOCOL_VERSION_2_1))
         l_Set_Response_Error_Flag();
#endif /* end #if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1 */
      }
      if (g_lin_status_word.flag.last_pid != 0x0u)
      {
         g_lin_status_word.flag.overrun = L_SET;
      }
      g_lin_status_word.flag.last_pid = g_lin_active_pid;
      g_lin_status_word.flag.error_in_resp = L_SET;
   } 
   lin_timeout_ctrl.flag.recept_started = 0u;
   lin_timeout_ctrl.flag.transm_ongoing = 0u;
#ifdef LIN_EVENT_TRIGGERED_FRAME_ENABLE
   /* Reset flag to indicate an event triggered frame (receive or transmit) */
   g_lin_rx_ctrl.flag.evttrgfrm = L_RESET;
#endif /* #ifdef LIN_EVENT_TRIGGERED_FRAME_ENABLE */
   l_sys_irq_restore(g_lin_irqState);
}
#endif /* end #if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1 || defined J2602_PROTOCOL */

#ifdef AUTOBAUD_ENABLED
void l_hal_baudrate_control(void)
{

   /* do not try to check the baudrate setting here, reading the register BGL1/BGL2 will return the current timer value, not the timer reload value! */

   /* if the transmission is disturbed by a dominant pulse, a TX timeout will occur in the LIN transceiver and the transceiver will automatically switch to receive only mode.
      In order to recover from this error, the error flag is cleared. The same applies for overtemperature and overcurrent errors. */
   if (( LIN->CTRL_STS.bit.TXD_TMOUT_STS != 0u) || ( LIN->CTRL_STS.bit.OT_STS != 0u) || ( LIN->CTRL_STS.bit.OC_STS != 0u))
   {
      SCUPM->SYS_ISCLR.bit.LIN_OC_ICLR = 1u;
      SCUPM->SYS_ISCLR.bit.LIN_OT_ICLR = 1u;
      SCUPM->SYS_ISCLR.bit.LIN_TMOUT_ICLR = 1u;
   }
}
#else /* end #ifdef AUTOBAUD_ENABLED */
void l_hal_baudrate_control(void)
{
   /* if the transmission is disturbed by a dominant pulse, a TX timeout will occur in the LIN transceiver and the transceiver will automatically switch to receive only mode.
      In order to recover from this error, the error flag is cleared. The same applies for overtemperature and overcurrent errors. */
   if (( LIN->CTRL_STS.bit.TXD_TMOUT_STS != 0u) || ( LIN->CTRL_STS.bit.OT_STS != 0u) || ( LIN->CTRL_STS.bit.OC_STS != 0u))
   {
      SCUPM->SYS_ISCLR.bit.LIN_OC_ICLR = 1u;
      SCUPM->SYS_ISCLR.bit.LIN_OT_ICLR = 1u;
      SCUPM->SYS_ISCLR.bit.TXD_TMOUT_ICLR = 1u;
   }
}
#endif /* end #ifdef AUTOBAUD_ENABLED */
