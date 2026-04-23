/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : remapVector.c
* Project         : 
* Created         : YYYY/MM/DD
* Description     : 
* 
* 
************************************************************************************************************************
* Revison History:
* YYYY/MM/DD             Version              Description                      Author
* 2018/01/02             v1.0                 Draft first version
*
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                                   Include header files
************************************************************************************************************************
*/
#include "tle_device.h"


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
extern void GPT1_IRQHandler(void);
extern void GPT2_IRQHandler(void);
extern void ADC2_IRQHandler(void);  	
extern void ADC1_IRQHandler(void);		
extern void CCU6SR0_IRQHandler(void);
extern void CCU6SR1_IRQHandler(void); 
extern void CCU6SR2_IRQHandler(void); 
extern void CCU6SR3_IRQHandler(void); 
extern void SSC1_IRQHandler(void); 	
extern void SSC2_IRQHandler(void); 	
extern void UART1_IRQHandler(void);   
extern void UART2_IRQHandler(void);	
extern void EXINT0_IRQHandler(void);  
extern void EXINT1_IRQHandler(void);  
extern void BDRV_IRQHandler(void);    
extern void DMA_IRQHandler(void);    
extern void NMI_Handler(void);
extern void HardFault_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);
extern void SysTick_Handler(void);
extern void Reset_Handler(void);
extern void SVC_Handler(void);
extern void DebugMon_Handler(void);
extern void PendSV_Handler(void);

typedef void (*_Functions)(void);


const uint8 VersionString[64] __attribute__((at(0x11002100))) = "EWP_2018_02_26_VER_B01\r\n";

static const _Functions int_vector_table[32] __attribute__((section("IntteruptVector"))) =
{
    (void *)0x18001700,    /* 00:  __initial_sp  */
    Reset_Handler,         /* 01:  Reset_Handler    */
    NMI_Handler,           /* 02:  NMI_Handler  */
    HardFault_Handler,     /* 03:  HardFault_Handler  */
    MemManage_Handler,     /* 04:  MemManage_Handler  */
    BusFault_Handler,      /* 05:  BusFault_Handler  */
    UsageFault_Handler,    /* 06:  UsageFault_Handler */
    (void *)0x00000000,    /* 07:  Reserved */
    (void *)0x00000000,    /* 08:  Reserved */
    (void *)0x00000000,    /* 09:  Reserved */
    (void *)0x00000000,    /* 10:  Reserved */
    SVC_Handler,           /* 11:  SVC_Handler  */    
    DebugMon_Handler,      /* 12:  DebugMon_Handler */
    (void *)0x00000000,    /* 13:  Reserved */
    PendSV_Handler,        /* 14:  PendSV_Handler  */
    SysTick_Handler,       /* 15:  SysTick_Handler */
    GPT1_IRQHandler,       /* 16:  GPT1_IRQHandler */
    GPT2_IRQHandler,       /* 17:  GPT2_IRQHandler */
    ADC2_IRQHandler,       /* 18:  ADC2_IRQHandler */
    ADC1_IRQHandler,       /* 19:  ADC1_IRQHandler */
    CCU6SR0_IRQHandler,    /* 20:  CCU6SR0_IRQHandler */
    CCU6SR1_IRQHandler,    /* 21:  CCU6SR1_IRQHandler */
    CCU6SR2_IRQHandler,    /* 22:  CCU6SR2_IRQHandler */
    CCU6SR3_IRQHandler,    /* 23:  CCU6SR3_IRQHandler */
    SSC1_IRQHandler,       /* 24:  SSC1_IRQHandler */    
    SSC2_IRQHandler,       /* 25:  SSC2_IRQHandler */    
    UART1_IRQHandler,      /* 26:  UART1_IRQHandler */
    UART2_IRQHandler,      /* 27:  UART2_IRQHandler */
    EXINT0_IRQHandler,     /* 28:  EXINT0_IRQHandler */
    EXINT1_IRQHandler,     /* 29:  EXINT1_IRQHandler */
    BDRV_IRQHandler,       /* 30:  BDRV_IRQHandler */    
    DMA_IRQHandler         /* 31:  DMA_IRQHandler */    
};


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

/*
************************************************************************************************************************
*                                                    Exported functions
************************************************************************************************************************
*/
void ChangeVectorTable (void)
{
    volatile uint32 int_addr_vector_table = 0;
    int_addr_vector_table = (uint32)(&int_vector_table);
    CPU->VTOR.reg = int_addr_vector_table; 
}


/*
************************************************************************************************************************
*                                                      Local functions
************************************************************************************************************************
*/

