/******************************************************************************
*                _  _               ___             _                         *
*               (_)| |__   _ __    / _ \ _ __ ___  | |__    /\  /\            *
*               | || '_ \ | '__|  / /_\/| '_ ` _ \ | '_ \  / /_/ /            *
*               | || | | || |    / /_\\ | | | | | || |_) |/ __  /             *
*               |_||_| |_||_|    \____/ |_| |_| |_||_.__/ \/ /_/              *
*                                                                             *
*    ihr GmbH                                                                 *
*    Airport Boulevard B210                                                   *
*    77836 Rheinmünster - Germany                                             *
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
********************     Workfile:      lin_hal.h        **********************
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
@file lin_hal.h
@brief Header file of lin_hal.c
*/

#ifndef LIN_HW_DRIVER_H /* to interpret header file only once */
#define LIN_HW_DRIVER_H

/* ===========================================================================
 *  Header Files
 * ==========================================================================*/
#include <tle_device.h>
#include "genLinConfig.h"

/* ===========================================================================
 *  Constants
 * ==========================================================================*/

#ifndef LIN_FRAME_TIMEOUT
   #define LIN_FRAME_TIMEOUT       LIN_AUTOBAUD_TIMEOUT
#endif /* ifndef LIN_FRAME_TIMEOUT */

#ifdef LIN_UART2
   /* All UART control and access registers*/
   #define LIN_UART_BUFFER       (UART2->SBUF.bit.VAL)
   #define LIN_UART_CONTROL      (UART2->SCON.reg)
   #define LIN_UART_MODE0        (UART2->SCON.bit.SM0)
   #define LIN_UART_MODE1        (UART2->SCON.bit.SM1)
   #define LIN_UART_RUN          (UART2->SCON.bit.REN)
   #define LIN_UART_9BITREC      (UART2->SCON.bit.RB8)
    
   #define LIN_UART_BCON_PRE     (SCU->BCON2.bit.BRPRE)
   #define LIN_UART_BCON_FDSEL   (SCU->BGL2.bit.FD_SEL)
   #define LIN_UART_BCON_BGL     (SCU->BGL2.bit.BR_VALUE)
   #define LIN_UART_BCON_BGH     (SCU->BGH2.bit.BR_VALUE)
   #define LIN_UART_BCON_RUN     (SCU->BCON2.bit.R)

   #define LIN_UART_IR_SET_EN    (CPU->NVIC_ISER0.bit.Int_UART2)
   #define LIN_UART_IR_PENDING   (CPU->NVIC_ISPR0.bit.Int_UART2)
   #define LIN_UART_IR_PRIO      (CPU->NVIC_IPR2.bit.PRI_UART2)
   #define LIN_UART_IR_EN        (SCU->MODIEN2.bit.RIEN2)

   #define LIN_UART_STATUS          (SCU->LINST.reg)
   #define LIN_UART_BR_SYN_DET      (SCU->LINST.bit.BGSEL)
   #define LIN_UART_BR_SYN_DET_EN   (SCU->LINST.bit.BRDIS) 
  
  /* this define is added to ensure compatibility with current infineon software environment */
  #ifndef UART2_Tmr21_EINT2_Int
  #define UART2_Tmr21_EINT2_Int         ((l_u8)11)
  #endif /* end  #ifndef UART2_Tmr21_EINT2_Int*/
  #define LIN_UART_IRQ_NR  (UART2_Tmr21_EINT2_Int)
#ifdef AUTOBAUD_ENABLED
   /* Timer 21 is needed for baudrate synchronization when using UART 2*/
   #define LIN_TIMER_PRE         (TIMER21->T2MOD.bit.T2PRE)
   #define LIN_TIMER_PRE_EN      (TIMER21->T2MOD.bit.PREN)
   #define LIN_TIMER_COUNTER_EN  (TIMER21->T2MOD.bit.DCEN)
   #define LIN_TIMER_EDGE_SELECT (TIMER21->T2MOD.bit.T2REGS)
   #define LIN_TIMER_EXT_START   (TIMER21->T2MOD.bit.T2RHEN)
   #define LIN_TIMER_CAP_REL     (TIMER21->T2CON.bit.CP_RL2)
   #define LIN_TIMER_T_C_SEL     (TIMER21->T2CON.bit.C_T2)
   #define LIN_TIMER_EXT_EN      (TIMER21->T2CON.bit.EXEN2)
   #define LIN_TIMER_EXT_IR_EN   (TIMER21->T2CON1.bit.EXF2EN)
   #define LIN_TIMER_INT_IR_EN   (TIMER21->T2CON1.bit.TF2EN)
   #define LIN_TIMER_RUN         (TIMER21->T2CON.bit.TR2)
   #define LIN_TIMER_HIGH        (TIMER21->T2H.bit.T2H)
   #define LIN_TIMER_LOW         (TIMER21->T2L.bit.T2L)
   #define LIN_TIMER_CAP_H       (TIMER21->RC2H.bit.RC2)
   #define LIN_TIMER_CAP_L       (TIMER21->RC2L.bit.RC2)
#endif /* end #ifdef AUTOBAUD_ENABLED */
#endif /* end #ifdef LIN_UART2 */

#ifdef LIN_UART1
   /* All UART control and access registers*/
   #define LIN_UART_BUFFER        (UART1->SBUF.bit.VAL)
   #define LIN_UART_CONTROL       (UART1->SCON.reg)
   #define LIN_UART_MODE0         (UART1->SCON.bit.SM0)
   #define LIN_UART_MODE1         (UART1->SCON.bit.SM1)
   #define LIN_UART_RUN           (UART1->SCON.bit.REN)
   #define LIN_UART_9BITREC       (UART1->SCON.bit.RB8)
    
   #define LIN_UART_BCON_PRE      (SCU->BCON1.bit.BRPRE)
   #define LIN_UART_BCON_FDSEL    (SCU->BGL1.bit.FD_SEL)
   #define LIN_UART_BCON_BGL      (SCU->BGL1.bit.BR_VALUE)
   #define LIN_UART_BCON_BGH      (SCU->BGH1.bit.BR_VALUE)
   #define LIN_UART_BCON_RUN      (SCU->BCON1.bit.R)

   #define LIN_UART_IR_SET_EN     (CPU->NVIC_ISER0.bit.Int_UART1)
   #define LIN_UART_IR_PENDING    (CPU->NVIC_ISPR0.bit.Int_UART1)
   #define LIN_UART_IR_PRIO       (CPU->NVIC_IPR2.bit.PRI_UART1)
   #define LIN_UART_IR_EN         (SCU->MODIEN1.bit.RIEN1)
   #define LIN_UART_IRTX_EN       (SCU->MODIEN1.bit.TIEN1)

   #define LIN_UART_STATUS        (SCU->LINST.reg)
   #define LIN_UART_CLR_STATUS    (SCU->LINSCLR.reg)
   #define LIN_UART_BR_SYN_DET    (SCU->LINST.bit.BGSEL)
   #define LIN_UART_BR_SYN_DET_EN (SCU->LINST.bit.BRDIS)
   
  /* this define is added to ensure compatibility with current infineon software environment */
  #ifndef UART1_LIN_Tmr2_Int
  #define UART1_LIN_Tmr2_Int      ((l_u8)10)
  #endif /* end #ifndef UART1_LIN_Tmr2_Int*/
  #define LIN_UART_IRQ_NR         (UART1_LIN_Tmr2_Int)
#ifdef AUTOBAUD_ENABLED
   /* Timer 2 is needed for baudrate synchronization when using UART 1 */
   #define LIN_TIMER_PRE         (TIMER2->T2MOD.bit.T2PRE)
   #define LIN_TIMER_PRE_EN      (TIMER2->T2MOD.bit.PREN)
   #define LIN_TIMER_COUNTER_EN  (TIMER2->T2MOD.bit.DCEN)
   #define LIN_TIMER_EDGE_SELECT (TIMER2->T2MOD.bit.T2REGS)
   #define LIN_TIMER_EXT_START   (TIMER2->T2MOD.bit.T2RHEN)
   #define LIN_TIMER_CAP_REL     (TIMER2->T2CON.bit.CP_RL2)
   #define LIN_TIMER_T_C_SEL     (TIMER2->T2CON.bit.C_T2)
   #define LIN_TIMER_EXT_EN      (TIMER2->T2CON.bit.EXEN2)
   #define LIN_TIMER_EXT_IR_EN   (TIMER2->T2CON1.bit.EXF2EN)
   #define LIN_TIMER_INT_IR_EN   (TIMER2->T2CON1.bit.TF2EN)
   #define LIN_TIMER_RUN         (TIMER2->T2CON.bit.TR2)
   #define LIN_TIMER_HIGH        (TIMER2->T2H.bit.T2H)
   #define LIN_TIMER_LOW         (TIMER2->T2L.bit.T2L)
   #define LIN_TIMER_CAP_H       (TIMER2->RC2H.bit.RC2)
   #define LIN_TIMER_CAP_L       (TIMER2->RC2L.bit.RC2)
#endif /* end #ifdef AUTOBAUD_ENABLED */
#endif /* end #ifdef LIN_UART1 */

#define BREAK_FIELD_k  (0x0u)
#define SYNC_FIELD_k    (0x55u)

#ifndef CHECKSUM_NEXT
#define CHECKSUM_NEXT (2u)
#endif

#define lin_hal_tx_char(x)        (LIN_UART_BUFFER = (x))
#define lin_hal_is_byte_received() (lin_hal_status_g.byte_received == L_SET)
#define lin_hal_reset_byte_received() (lin_hal_status_g.byte_received = L_RESET)

#define lin_hal_set_rx_pid(pid)   (lin_temp_buffer.frame.PID = (pid))
#define lin_hal_set_tx_pid(pid)   (lin_temp_buffer.frame.PID = (pid))



/* ===========================================================================
 *  Function Macros
 * ==========================================================================*/

#ifdef AUTOBAUD_ENABLED
/* Autobaud synchronization, timer runs with no prescaler, formulae allows to calculate necessary baudrate
 * settings only with shifting. 
 * If not within valid limits, increase timeout timer to cause next call of ld_task to reset UART. 
 */

#define lin_hal_brreg_calc()  do { lin_synch_time = (l_u16)((l_u16)(lin_synch_time >> 2u) >> BRG_PRE);                    \
                                if (BRG_PRE == 0u) { LIN_UART_BCON_FDSEL = (l_u8)(((l_u8)(lin_synch_time & 0x1Fu)) >> 1u); \
                                } else { LIN_UART_BCON_FDSEL   = (l_u8)(lin_synch_time & 0x1Fu); }                 \
                                lin_synch_time = (l_u16)(lin_synch_time >> 5u);                                           \
                                if (lin_synch_time >= MIN_BDREG)                                                   \
                                { if (lin_synch_time <= MAX_BDREG)                                                 \
                                  { LIN_UART_BCON_BGL = (l_u8)(lin_synch_time & 0x07u);                            \
                                    LIN_UART_BCON_BGH = (l_u8)(lin_synch_time >> 3u);                              \
                                     LIN_UART_BCON_RUN = 1u;                                                       \
                                    lin_slave_state_g = sync_received;                                             \
                                  } else {                                                                         \
                                    lin_ab_timeout_timer = LIN_AUTOBAUD_TIMEOUT + 1u;                              \
                                    lin_ab_t2_state = 7u;}                                                         \
                                } else {                                                                           \
                                  lin_ab_timeout_timer = LIN_AUTOBAUD_TIMEOUT + 1u;                                \
                                  lin_ab_t2_state = 7u;} } while(0)



#define lin_hal_T2_ISR()      do { if (lin_ab_t2_state == 5u)                                                      \
                              { LIN_TIMER_RUN = 0u;LIN_TIMER_EXT_IR_EN = 0u;                                       \
                                LIN_TIMER_EXT_EN = 0u;lin_ab_t2_state = 0u;                                        \
                                lin_synch_time = ((l_u16)((l_u16)LIN_TIMER_CAP_H << 8) | (l_u16)LIN_TIMER_CAP_L);  \
                                lin_hal_brreg_calc();                                                              \
                              } else { lin_ab_t2_state++; } } while(0)                                   
#endif /* ifdef AUTOBAUD_ENABLED */
                                
#define l_hal_cyclic_call()   do { if(lin_hal_is_byte_received())                                                  \
                              { l_hal_rx_state_machine();                                                          \
                                lin_hal_reset_byte_received(); }                                                   \
                                if (l_txchk_byte_nr > 0u)                                                          \
                                { l_hal_txchecksum_calculation(); } }while(0)

#ifdef AUTOBAUD_ENABLED
#define l_autobaud_control()  do { if(lin_ab_t2_state != 0u)                                                       \
                              { lin_ab_timeout_timer++;                                                            \
                                if(lin_ab_timeout_timer >= LIN_AUTOBAUD_TIMEOUT)                                   \
                                { lin_slave_state_g = idle;                                                        \
                                  lin_hal_init_uart();                                                             \
                                  LIN_TIMER_RUN = 0u;                                                              \
                                  LIN_TIMER_EXT_IR_EN = 0u;                                                        \
                                  LIN_TIMER_EXT_EN = 0u;                                                           \
                                  lin_ab_t2_state = 0u;                                                            \
                                  lin_ab_timeout_timer = 0u; } } } while(0)
#else
#define l_autobaud_control()  do {} while(0)
#endif
#if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1
#define l_frame_timeout_control()  do { if(lin_timeout_ctrl.flag.frame != 0u)                                      \
                                   { lin_frm_timeout_timer++;                                                      \
                                     if(lin_frm_timeout_timer >= LIN_FRAME_TIMEOUT)                                \
                                     { lin_slave_state_g = idle;                                                   \
                                       if (lin_timeout_ctrl.flag.recept_started != 0u)                             \
                                       { lin_hal_set_resp_error();                                                 \
                                         lin_timeout_ctrl.flag.recept_started = 0u; }                              \
                                       if (lin_timeout_ctrl.flag.transm_ongoing != 0u)                             \
                                       { lin_timeout_ctrl.flag.transm_ongoing = 0u;                                \
                                         if ((g_lin_frame_index >= LIN_NUMBER_OF_FRAMES) ||                        \
                                            ((g_lin_frame_ctrl[g_lin_frame_index].frame.frame_type.transfer_type & 0x01u) == 0u)) \
                                         { lin_hal_set_resp_error(); } }                                           \
                                       lin_timeout_ctrl.flag.frame = 0u;                                           \
                                       lin_frm_timeout_timer = 0u; } } } while (0)
#else
#define l_frame_timeout_control()  do { if(lin_timeout_ctrl.flag.frame != 0u)                                      \
                                   { lin_frm_timeout_timer++;                                                      \
                                     if (lin_frm_timeout_timer >= LIN_FRAME_TIMEOUT)                               \
                                     { lin_slave_state_g = idle;                                                   \
                                       lin_timeout_ctrl.flag.frame = 0u;                                           \
                                       lin_frm_timeout_timer = 0u; } } } while (0)
#endif /* end #if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1 */


/* ===========================================================================
 *  Function Prototypes
 * ==========================================================================*/

extern void l_hal_txchecksum_calculation(void);
void lin_hal_tx_response (l_u8 l_type, const l_u8 l_data[], l_u8 l_len);
extern void l_hal_rx_state_machine(void);
extern void lin_hal_ISR(void);
extern void lin_hal_init_uart(void);
extern void lin_hal_rx_response (l_u8 l_type, l_u8 l_len);
extern void lin_hal_init (void);
extern void lin_hal_tx_wake_up(void);


#ifdef AUTOBAUD_ENABLED
extern void lin_hal_init_T2 (void);
#endif /* #ifdef AUTOBAUD_ENABLED */
extern void l_hal_baudrate_control(void);


#if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1
#if (defined J2602_PROTOCOL)
#ifdef GM_SPEC_ADAPTION
void lin_hal_set_resp_error (l_u8 j2602_fault);
#else
static void lin_hal_set_resp_error (l_u8 j2602_fault);
#endif /* #ifdef GM_SPEC_ADAPTION */
#else
/* Prototype for LIN protocols */
extern void lin_hal_set_resp_error (void);
#endif /* end #if defined J2602_PROTOCOL */
#endif /* end #if defined LIN_PROTOCOL_VERSION_2_0 || defined LIN_PROTOCOL_VERSION_2_1 */

/* ===========================================================================
 *  Function Macros
 * ==========================================================================*/

#ifdef LIN_MASTER
#define lin_hal_wait_for_break()        do                                                           \
                                        {                                                            \
                                           lin_slave_state_g = idle;                                 \
                                           lin_timeout_ctrl.flag.frame = 0u;                         \
                                           lin_frm_timeout_timer = 0u;                               \
                                        }while (0)
#else
#define lin_hal_wait_for_break()        (lin_slave_state_g = idle)
#endif /* #ifdef LIN_MASTER */
                                        
/* ===========================================================================
 *  Variables
 * ==========================================================================*/

extern LIN_hal_stat_reg_t        lin_hal_status_g;
extern t_lin_temp_frame_buffer   lin_temp_buffer;
extern LIN_SLAVE_STATE_t         lin_slave_state_g;
extern l_u8                      lin_ab_timeout_timer;
extern l_u8                      lin_frm_timeout_timer;
extern t_lin_timeout_ctrl        lin_timeout_ctrl;
extern l_u8                      l_txchk_byte_nr;
extern volatile l_u8             lin_ab_t2_state;

#ifdef AUTOBAUD_ENABLED
   extern volatile l_u16         lin_temp_word;
   extern volatile l_u16         lin_synch_time;
#endif /* end #ifdef AUTOBAUD_ENABLED */

#endif /* end #ifndef LIN_HW_DRIVER_H */





