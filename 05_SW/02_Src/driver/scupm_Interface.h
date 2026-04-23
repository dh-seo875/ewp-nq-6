/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : scupm_interface.h
* Project         : 
* Created         : YYYY/MM/DD
* Description     : 
* 
* 
************************************************************************************************************************
* Revison History:
* YYYY/MM/DD             Version              Description                      Author
* 2018/02/12             v1.0                 Draft first version              Alex.kim
*
************************************************************************************************************************
*/


#ifndef SCUPM_INTERFACE_H
#define SCUPM_INTERFACE_H

/*
************************************************************************************************************************
*                                                  Include header files
************************************************************************************************************************
*/
#include "Std_Types.h"
#include "bdrv.h"


/*
************************************************************************************************************************
*                                                   Defines and macros
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                                         Typedefs
************************************************************************************************************************
*/


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
__STATIC_INLINE Std_ReturnType Scupm_ClearVsUnderVoltageStatusFlag (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->SYS_SUPPLY_IRQ_CLR.bit.VS_UV_SCLR = 1;
    return retVal;
}


__STATIC_INLINE boolean_t Scupm_GetVsdLowthStatusOfBdrv (void)
{
    return (boolean_t)SCUPM->BDRV_IS.bit.VSD_LOWTH_STS;
}


__STATIC_INLINE boolean_t Scupm_GetVsdUpthStatusOfBdrv (void)
{
    return (boolean_t)SCUPM->BDRV_IS.bit.VSD_UPTH_STS;
}


__STATIC_INLINE boolean_t Scupm_GetVcpUpthStatusOfBdrv (void)
{
    return (boolean_t)SCUPM->BDRV_IS.bit.VCP_UPTH_STS;
}


__STATIC_INLINE boolean_t Scupm_GetVcpLowth1StatusOfBdrv (void)
{
    return (boolean_t)SCUPM->BDRV_IS.bit.VCP_LOWTH1_STS;
}


__STATIC_INLINE boolean_t Scupm_GetVcpLowth2StatusOfBdrv (void)
{
    return (boolean_t)SCUPM->BDRV_IS.bit.VCP_LOWTH2_STS;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearVsdUpthStatusFlagOfBdrv (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.VSD_UPTH_SCLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearVsdLowthStatusFlagOfBdrv (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.VSD_LOWTH_SCLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearVcpUpthStatusOfBdrv (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.VCP_UPTH_SCLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearVcpLowth1StatusOfBdrv (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.VCP_LOWTH1_SCLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearVcpLowth2StatusOfBdrv (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.VCP_LOWTH2_SCLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearVsdUpthInterruptStatusFlagOfBdrv (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.VSD_UPTH_ICLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearVsdLowthInterruptStatusFlagOfBdrv (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.VSD_LOWTH_ICLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearVcpUpthInterruptStatusOfBdrv (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.VCP_UPTH_ICLR = 1;
    return retVal;
}

__STATIC_INLINE Std_ReturnType Scupm_ClearVcpLowth1InterruptStatusOfBdrv (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.VCP_LOWTH1_ICLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearVcpLowth2InterruptStatusOfBdrv (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.VCP_LOWTH2_ICLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearOverCurrentStatusFlagOfHighSideDrv3 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.HS3_OC_ICLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearOverCurrentStatusFlagOfLowSideDrv3 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.LS3_OC_ICLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearOverCurrentStatusFlagOfHighSideDrv2 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.HS2_OC_ICLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearOverCurrentStatusFlagOfHighSideDrv1 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.HS1_OC_ICLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearOverCurrentStatusFlagOfLowSideDrv2 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.LS2_OC_ICLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearOverCurrentStatusFlagOfLowSideDrv1 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.LS1_OC_ICLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearShortCircuitStatusFlagOfHighSideDrv3 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.HS3_DS_ICLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearShortCircuitStatusFlagOfLowSideDrv3 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.LS3_DS_ICLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearShortCircuitStatusFlagOfHighSideDrv2 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.HS2_DS_ICLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearShortCircuitStatusFlagOfHighSideDrv1 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.HS1_DS_ICLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearShortCircuitStatusFlagOfLowSideDrv2 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.LS2_DS_ICLR = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Scupm_ClearShortCircuitStatusFlagOfLowSideDrv1 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    SCUPM->BDRV_ISCLR.bit.LS1_DS_ICLR = 1;
    return retVal;
}


#endif    /* SCUPM_INTERFACE_H */

