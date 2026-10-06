/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : bdrv_interface.h
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


#ifndef BDRV_INTERFACE_H
#define BDRV_INTERFACE_H

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
//__STATIC_INLINE boolean_t Bdrv_GetShortCircuitStatusOfHighSideDrv1 (void)
//{
//    return (boolean_t)BDRV->CTRL1.bit.HS1_DS_STS;
//}


//__STATIC_INLINE boolean_t Bdrv_GetShortCircuitStatusOfHighSideDrv2 (void)
//{
//    return (boolean_t)BDRV->CTRL1.bit.HS2_DS_STS;
//}


//__STATIC_INLINE boolean_t Bdrv_GetShortCircuitStatusOfHighSideDrv3 (void)
//{
//    return (boolean_t)BDRV->CTRL2.bit.HS3_DS_STS;
//}


//__STATIC_INLINE boolean_t Bdrv_GetShortCircuitStatusOfLowSideDrv1 (void)
//{
//    return (boolean_t)BDRV->CTRL1.bit.LS1_DS_STS;
//}


//__STATIC_INLINE boolean_t Bdrv_GetShortCircuitStatusOfLowSideDrv2 (void)
//{
//    return (boolean_t)BDRV->CTRL1.bit.LS2_DS_STS;
//}


//__STATIC_INLINE boolean_t Bdrv_GetShortCircuitStatusOfLowSideDrv3 (void)
//{
//    return (boolean_t)BDRV->CTRL2.bit.LS3_DS_STS;
//}

__STATIC_INLINE boolean_t Bdrv_GetOverCurrentStatusOfHighSideDrv1 (void)
{
    return (boolean_t)BDRV->CTRL1.bit.HS1_OC_STS;
}


__STATIC_INLINE boolean_t Bdrv_GetOverCurrentStatusOfHighSideDrv2 (void)
{
    return (boolean_t)BDRV->CTRL1.bit.HS2_OC_STS;
}


__STATIC_INLINE boolean_t Bdrv_GetOverCurrentStatusOfHighSideDrv3 (void)
{
    return (boolean_t)BDRV->CTRL2.bit.HS3_OC_STS;
}


__STATIC_INLINE boolean_t Bdrv_GetOverCurrentStatusOfLowSideDrv1 (void)
{
    return (boolean_t)BDRV->CTRL1.bit.LS1_OC_STS;
}


__STATIC_INLINE boolean_t Bdrv_GetOverCurrentStatusOfLowSideDrv2 (void)
{
    return (boolean_t)BDRV->CTRL1.bit.LS2_OC_STS;
}


__STATIC_INLINE boolean_t Bdrv_GetOverCurrentStatusOfLowSideDrv3 (void)
{
    return (boolean_t)BDRV->CTRL2.bit.LS3_OC_STS;
}


__STATIC_INLINE Std_ReturnType Bdrv_EnableDiagnosisCurrentSourceOfHighSideDrv1 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    BDRV->CTRL1.bit.HS1_DCS_EN = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Bdrv_EnableDiagnosisCurrentSourceOfHighSideDrv2 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    BDRV->CTRL1.bit.HS2_DCS_EN = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Bdrv_EnableDiagnosisCurrentSourceOfHighSideDrv3 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    BDRV->CTRL2.bit.HS3_DCS_EN = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Bdrv_EnableDiagnosisCurrentSourceOfLowSideDrv1 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    BDRV->CTRL1.bit.LS1_DCS_EN = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Bdrv_EnableDiagnosisCurrentSourceOfLowSideDrv2 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    BDRV->CTRL1.bit.LS2_DCS_EN = 1;
    return retVal;
}


__STATIC_INLINE Std_ReturnType Bdrv_EnableDiagnosisCurrentSourceOfLowSideDrv3 (void)
{
    Std_ReturnType retVal = RTE_E_OK;
    BDRV->CTRL2.bit.LS3_DCS_EN = 1;
    return retVal;
}




#endif    /* BDRV_INTERFACE_H */

