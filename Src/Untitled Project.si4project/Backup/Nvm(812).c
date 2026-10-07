/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : Nvm.c
* Project         : 
* Created         : YYYY/MM/DD
* Description     : 
* 
* 
************************************************************************************************************************
* Revison History:
* YYYY/MM/DD             Version              Description                      Author
* 2018/11/05             v1.0                 Draft first version
*
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                                   Include header files
************************************************************************************************************************
*/
#include "string.h"
#include "linmain.h"
#include "RteHal.h"
#include "Std_Types.h"
#include "Nvm.h"
#include "Emo_RAM.h"
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


/*
************************************************************************************************************************
*                                                     Local variables
************************************************************************************************************************
*/
static uint8_t ErrorCodePageA[FlashPageSize];
static uint8_t ErrorCodePageB[FlashPageSize];
static uint8_t* IRV_ptrErrorCodePage;

#define errorCodeAddressPageA (0x1100F000u)
#define errorCodeAddressPageB (0x1100F080u)
#define errorCodeIndexMax (128u)





/*
************************************************************************************************************************
*                                                 Local function prototypes
************************************************************************************************************************
*/

static uint8_t checkNvmStatus (const uint8_t* errorCode);

/*
************************************************************************************************************************
*                                                    Exported functions
************************************************************************************************************************
*/
uint8_t Nvm_errorCode  = 0;
uint8_t Nvm_errorCodeIndex = 0;
static uint16_t Nvm_drivingCycleIndicator = 0;
static uint8_t countErrorCodeIndex (const uint8_t* errorCode);
static void readNvm (const uint8_t* errorCodeAddress, uint8_t* readData);
static uint8_t writeNvm (const uint8_t* writeData, uint32_t errorCodeAddress);
static void EraseNvm (void);
static void checkDrivingCycle (void);


/*
************************************************************************************************************************
*                                                      Local functions
************************************************************************************************************************
*/

uint8_t* Nvm_Init (void)
{
    uint8_t nvmStatusPageA = 0;
    uint8_t nvmStatusPageB = 0;
    uint8_t nvmStatusPageAB = 0;
    //uint8_t nvmWriteResult = 0;
	static uint8* ptrErrorCodeAddressPageA = (uint8*)(uintptr_t)errorCodeAddressPageA;
	static uint8* ptrErrorCodeAddressPageB = (uint8*)(uintptr_t)errorCodeAddressPageB;

    readNvm(ptrErrorCodeAddressPageA, ErrorCodePageA);
    readNvm(ptrErrorCodeAddressPageB, ErrorCodePageB);
    nvmStatusPageA = checkNvmStatus(ErrorCodePageA);
    nvmStatusPageB = checkNvmStatus(ErrorCodePageB);
    nvmStatusPageAB = nvmStatusPageA | (nvmStatusPageB <<1);

    /* nvmStatusPageAB = 0b00 (0) : Normal Page1 and 2
       nvmStatusPageAB = 0b01 (1) : Normal Page1 and Fault Page2
       nvmStatusPageAB = 0b10 (2) : Fault Page1 and Normal Page2
       nvmStatusPageAB = 0b11 (3) : Fault Page1 and 2 */
    switch(nvmStatusPageAB)
    {
        case 0:
            Nvm_errorCodeIndex = countErrorCodeIndex(ErrorCodePageA);
            IRV_ptrErrorCodePage = ErrorCodePageA;
            break;

        case 1:
            Nvm_errorCodeIndex = countErrorCodeIndex(ErrorCodePageA);
            IRV_ptrErrorCodePage = ErrorCodePageA;
            (void)writeNvm(ErrorCodePageA, errorCodeAddressPageB);
			
           /* if (nvmWriteResult == 1u)
            {
                nvmWriteResult = writeNvm(ErrorCodePageA, errorCodeAddressPageB);
            }
            else
            {
            }
            break;*/

        case 2:
            Nvm_errorCodeIndex = countErrorCodeIndex(ErrorCodePageB);
            IRV_ptrErrorCodePage = ErrorCodePageB;
            (void)writeNvm(ErrorCodePageB, errorCodeAddressPageA);
    
            /*if (nvmWriteResult == 1u)
            {
                nvmWriteResult = writeNvm(ErrorCodePageB, errorCodeAddressPageA);
            }
            else
            {
            }*/
            break;

        case 3:
            EraseNvm();
    
            break;

        default: 
			/* Intentionally Empty */
            break;
    }
    checkDrivingCycle();

    return IRV_ptrErrorCodePage;
}

void ManageNvm (void)
{
    uint8_t nvmRepeatCnt = 0;
    //uint8_t nvmWriteResult = 0;

    if (Nvm_errorCode != 0u)
    {
        if (linSuccessfulFlag == 1u) /* check the finish of LIN */
        {
            if (Nvm_errorCodeIndex >= errorCodeIndexMax)
            {
                for (nvmRepeatCnt = 0; nvmRepeatCnt < FlashPageSize; nvmRepeatCnt++)
                {
                    IRV_ptrErrorCodePage[nvmRepeatCnt] = IRV_ptrErrorCodePage[nvmRepeatCnt + 1u];
                }
                IRV_ptrErrorCodePage[127u] = (uint8_t)((uint16)Nvm_errorCode + Nvm_drivingCycleIndicator);
            }
            else
            {
                IRV_ptrErrorCodePage[Nvm_errorCodeIndex] = (uint8_t)((uint16)Nvm_errorCode + Nvm_drivingCycleIndicator);
                Nvm_errorCodeIndex = countErrorCodeIndex(IRV_ptrErrorCodePage);
            }


            /* PORT_ChangePin(0x11, PORT_ACTION_TOGGLE); */						
            (void)writeNvm(IRV_ptrErrorCodePage, errorCodeAddressPageA);
            /*if (nvmWriteResult == 1u)
            {
                nvmWriteResult = writeNvm(IRV_ptrErrorCodePage, errorCodeAddressPageA);
            }
            else
            {
            }*/

            (void)writeNvm(IRV_ptrErrorCodePage, errorCodeAddressPageB);
            /*if (nvmWriteResult == 1u)
            {
                nvmWriteResult = writeNvm(IRV_ptrErrorCodePage, errorCodeAddressPageB);
            }
            else
            {
            }*/
            Nvm_errorCode = 0;
            /* PORT_ChangePin(0x11, PORT_ACTION_CLEAR); */
        }
        else if ((forceNvmWriteFlag == 1u) && (Emo_Status.MotorState == EMO_MOTOR_STATE_STOP))
        {
            if (Nvm_errorCodeIndex >= errorCodeIndexMax)
            {
                for (nvmRepeatCnt = 0; nvmRepeatCnt < FlashPageSize; nvmRepeatCnt++)
                {
                    IRV_ptrErrorCodePage[nvmRepeatCnt] = IRV_ptrErrorCodePage[nvmRepeatCnt + 1u];
                }
                IRV_ptrErrorCodePage[127u] = (uint8_t)((uint16)Nvm_errorCode + Nvm_drivingCycleIndicator);
            }
            else
            {
                IRV_ptrErrorCodePage[Nvm_errorCodeIndex] = (uint8_t)((uint16)Nvm_errorCode + Nvm_drivingCycleIndicator);
                Nvm_errorCodeIndex = countErrorCodeIndex(IRV_ptrErrorCodePage);
            }

        
            (void)writeNvm(IRV_ptrErrorCodePage, errorCodeAddressPageA);
            /*if (nvmWriteResult == 1u)
            {
                nvmWriteResult = writeNvm(IRV_ptrErrorCodePage, errorCodeAddressPageA);
            }
            else
            {
            }*/

            (void)writeNvm(IRV_ptrErrorCodePage, errorCodeAddressPageB);
            /*if (nvmWriteResult == 1u)
            {
                nvmWriteResult = writeNvm(IRV_ptrErrorCodePage, errorCodeAddressPageB);
            }
            else
            {
            }*/
            Nvm_errorCode = 0;
        }
        else
        {
        /* Intentionally Empty */
        }
    }
    else
    {
    }

    if (NvmEraseFlag == 1u)
    {
        EraseNvm();
        NvmEraseFlag = 0;
    }
    (void)SetErrorCodeIndex(Nvm_errorCodeIndex);
}

/*
sint8_t CompareNvm (void)
{
    sint8_t resultCompare;

    resultCompare = memcmp((void*)nvm_Page1Address, (void*)(nvm_Page2Address), FlashPageSize);

    return (resultCompare)
}
*/

static uint8_t checkNvmStatus (const uint8_t*  errorCode)
{
    uint8_t nvmRepeatCnt = 0;
    uint8_t CheckData = 0;
    uint8_t Checkresult = 0;
    
    for (nvmRepeatCnt = 0; nvmRepeatCnt < FlashPageSize; nvmRepeatCnt++)
    {
    	CheckData = errorCode[nvmRepeatCnt];
    	if (CheckData == 0xFFu)
    	{
    	    Checkresult = 1u;
    	    break;
    	}
    	else
    	{
    	}
    }
    return Checkresult;
}

static uint8_t countErrorCodeIndex (const uint8_t* errorCode)
{
    uint8_t nvmRepeatCnt = 0;
    uint8_t CheckData = 0;
    
    for (nvmRepeatCnt = 0; nvmRepeatCnt < FlashPageSize; nvmRepeatCnt++)
    {
    	CheckData = errorCode[nvmRepeatCnt];
    	if (CheckData == 0x00u)
    	{
    	    break;
    	}
    	else
    	{
    	}
    }
    return nvmRepeatCnt;
}

static void readNvm (const uint8_t*  errorCodeAddress, uint8_t* readData)
{
    uint8_t nvmRepeatCnt = 0;
    for (nvmRepeatCnt = 0; nvmRepeatCnt < FlashPageSize; nvmRepeatCnt++)
    {
    	readData[nvmRepeatCnt] = errorCodeAddress[nvmRepeatCnt];
    }
}

static uint8_t writeNvm (const uint8_t*  writeData, uint32_t errorCodeAddress)
{
    uint8_t WriteResult = 0;

    SCU_ChangeNVMProtection(NVM_DATA_WRITE, PROTECTION_CLEAR);
    SCU_ChangeNVMProtection(NVM_CODE_WRITE, PROTECTION_CLEAR);
    
    WriteResult = (uint8)ProgramPage(errorCodeAddress, writeData, 0, 0, 0);
	
    SCU_ChangeNVMProtection(NVM_DATA_WRITE, PROTECTION_SET);
    SCU_ChangeNVMProtection(NVM_CODE_WRITE, PROTECTION_SET);
	
    return WriteResult;
}

static void EraseNvm (void)
{
    uint8_t nvmRepeatCnt = 0;

    for (nvmRepeatCnt = 0; nvmRepeatCnt < FlashPageSize; nvmRepeatCnt++)
    {
    	ErrorCodePageA[nvmRepeatCnt] = 0;
    	ErrorCodePageB[nvmRepeatCnt] = 0;
    }

    SCU_ChangeNVMProtection(NVM_DATA_WRITE, PROTECTION_CLEAR);
    SCU_ChangeNVMProtection(NVM_CODE_WRITE, PROTECTION_CLEAR);
    
    (void)USER_ERASEPG((uint32_t *)(uintptr_t)errorCodeAddressPageA, 0);
    (void)USER_ERASEPG((uint32_t *)(uintptr_t)errorCodeAddressPageB, 0);

    SCU_ChangeNVMProtection(NVM_DATA_WRITE, PROTECTION_SET);
    SCU_ChangeNVMProtection(NVM_CODE_WRITE, PROTECTION_SET);
    
    Nvm_errorCodeIndex = 0;
}

static void checkDrivingCycle (void)
{
    uint8 drivingCycleIndicator;

    /* Manage of index overflow */
    if (Nvm_errorCodeIndex == 0u)
    {
        drivingCycleIndicator = 0;
    }
    else
    {
        drivingCycleIndicator = (IRV_ptrErrorCodePage[Nvm_errorCodeIndex - 1u] >> 7u);
    }

    /* Print the driving cycle */
    if (drivingCycleIndicator == 0u)
    {
        Nvm_drivingCycleIndicator = 0x80;
    }
    else
    {
        Nvm_drivingCycleIndicator = 0x00;
    }

}
