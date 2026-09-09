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
static uint8_t* IRV_ptrErrorCodePage;

#define errorCodeAddressPageA (0x1100F000u)
#define errorCodeIndexMax (128u)





/*
************************************************************************************************************************
*                                                 Local function prototypes
************************************************************************************************************************
*/

static uint8_t checkNvmStatus (const uint8_t* errorCode);
static uint8_t convertErrorCode(uint16_t diagErrorCode);
static void ManageNVMConvertErrorCode(void);
static void ManageNVMEWP(uint16_t Nvm_convert_errorCode);
static void CheckPrevValueNVMPEEWP(void);
static void NvmVariables(void);
static void ManageNVMBuildArray(void);





/*
************************************************************************************************************************
*                                                    Exported functions
************************************************************************************************************************
*/
uint8_t Nvm_errorCode  = 0;
uint8_t Nvm_errorCodeIndex = 0;
static uint16_t Nvm_drivingCycleIndicator = 0;

static uint8_t NVM_fault_merged = 0;
static uint8_t NVM_failure_mereged = 0;
static uint8_t NVM_warning_mereged = 0;

static uint16_t NVM_fault_merged_convert = 0;
static uint16_t NVM_failure_merged_convert = 0;
static uint16_t NVM_warning_merged_convert = 0;

static uint16_t NVM_result_FaultCode = 0;
static uint16_t NVM_result_FailureCode = 0;
static uint16_t NVM_result_WarningCode = 0;

static uint16_t NVM_compareErrorCodeResult = 0;

static uint16_t NVM_convertWarningActualCodeResult = 0;
static uint16_t NVM_convertFaultActualCodeResult = 0;
static uint16_t NVM_convertFailureActualCodeResult = 0;

static uint8_t countErrorCodeIndex (const uint8_t* errorCode);
static void readNvm (const uint8_t* errorCodeAddress, uint8_t* readData);
static uint8_t writeNvm (const uint8_t* writeData, uint32_t errorCodeAddress);
static void EraseNvm (void);
static void checkDrivingCycle (void);
static void NVM_Watchdog_Fault(void);

static void VAREF_Claer(void);


/*
************************************************************************************************************************
*                                                      Local functions
************************************************************************************************************************
*/

static void NVM_Watchdog_Fault(void)
{
	uint8_t Nvm_WDT_Cnt = 0;
	uint8_t PMU_Reset_Flag = 0;
    uint8_t	Nvm_Watchdog_Fault_FLAG = 0;

	/* Get PMU Reset Flag */
	PMU_Reset_Flag = PMU_Get_Reset_Status();
	/* Clear PMU Reset Flag */
    PMU_Clear_Reset_Status();	

	if(((PMU_Reset_Flag >> 5u) & 0x01u) == 1u) //WDT1 Reset
	{
        Nvm_WDT_Cnt = PMU->GPUDATA00.reg;	
        if(Nvm_WDT_Cnt >= 2u)   // Check if WDT1 fail occur 3 time
        {
            Nvm_Watchdog_Fault_FLAG = 1;
            PMU->GPUDATA00.reg = 0;
			(void)WriteMCUFault(Nvm_Watchdog_Fault_FLAG); 					 
			SCU_EnterSleepMode(); // Go to MCU Sleep Mode
		}				
		else
		{
			PMU->GPUDATA00.reg = PMU->GPUDATA00.reg + 1u;
		}
	}
	else if((PMU_Reset_Flag >> 7u) == 1u) //Power On Reset
    {
          PMU->GPUDATA00.reg = 0;	
	}
	else if((PMU->PMU_SUPPLY_STS.bit.PMU_1V5_OVERVOLT == 1u ) || (PMU->PMU_SUPPLY_STS.bit.PMU_5V_OVERVOLT == 1u))
    {
        Nvm_Watchdog_Fault_FLAG = 1;
        PMU->GPUDATA00.reg = 0;
        (void)WriteMCUFault(Nvm_Watchdog_Fault_FLAG); 					 
        SCU_EnterSleepMode(); // Go to MCU Sleep Mode
	}
    else
    {
        /* no action */ 
	}
   
}



uint8_t* Nvm_Init (void)
{
    uint8_t nvmStatusPageA = 0;


    //uint8_t nvmWriteResult = 0;
	static const uint8* const ptrErrorCodeAddressPageA = (uint8*)(uintptr_t)errorCodeAddressPageA;

	NVM_Watchdog_Fault();
    readNvm(ptrErrorCodeAddressPageA, ErrorCodePageA);
    nvmStatusPageA = checkNvmStatus(ErrorCodePageA);

    /* nvmStatusPageAB = 0b00 (0) : Normal Page1 and 2
       nvmStatusPageAB = 0b01 (1) : Normal Page1 and Fault Page2
       nvmStatusPageAB = 0b10 (2) : Fault Page1 and Normal Page2
       nvmStatusPageAB = 0b11 (3) : Fault Page1 and 2 */
    if(nvmStatusPageA == 0u)
    {
        Nvm_errorCodeIndex = countErrorCodeIndex(ErrorCodePageA);
        IRV_ptrErrorCodePage = ErrorCodePageA;
    }
    else
    {
        EraseNvm();
		/* EraseNvm() recreates the RAM image; keep all later users on it. */
		IRV_ptrErrorCodePage = ErrorCodePageA;
	}

    checkDrivingCycle();

    return IRV_ptrErrorCodePage;
}

void ManageNvm (void)
{
    //uint8_t nvmWriteResult = 0;
	NvmVariables();
	ManageNVMBuildArray();
	ManageNVMConvertErrorCode();
	VAREF_Claer();

    if (Nvm_errorCode != 0u)
    {
        if (linSuccessfulFlag == 1u) /* check the finish of LIN */
        {

            /* PORT_ChangePin(0x11, PORT_ACTION_TOGGLE); */						
            (void)writeNvm(IRV_ptrErrorCodePage, errorCodeAddressPageA);
            Nvm_errorCode = 0;
            /* PORT_ChangePin(0x11, PORT_ACTION_CLEAR); */
        }
        else if ((forceNvmWriteFlag == 1u) && (Emo_Status.MotorState == EMO_MOTOR_STATE_STOP))
        {
        
            (void)writeNvm(IRV_ptrErrorCodePage, errorCodeAddressPageA);
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

static void VAREF_Claer(void)
{
    sint16_t vdc = 0;
	(void)ReadVdcFiltered(&vdc);
	SCUPM->SYS_ISCLR.bit.VREF5V_LOWTH_ICLR = 1;
    if((vdc>=600) && (SCUPM->SYS_IS.bit.VREF5V_LOWTH_IS == 1u))
    {
        /* 1. LDO Power Down */
		MF->REF2_CTRL.bit.VREF5V_PD_N = 0;
        /* 2. Clear VREF5V Interrupt */
        SCUPM->SYS_ISCLR.bit.VREF5V_LOWTH_ICLR = 1;
        SCUPM->SYS_ISCLR.bit.VREF5V_UPTH_ICLR = 1;
        SCUPM->SYS_ISCLR.bit.VREF5V_OVL_ICLR = 1;	

        /* 3. LDO Power ON - VAREF(Re-enable) */
        MF->REF2_CTRL.bit.VREF5V_PD_N = 1;
    }
	else
    {
	}
}



static uint8_t checkNvmStatus (const uint8_t*  errorCode)
{
    uint8_t nvmRepeatCnt;
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
    uint8_t nvmRepeatCnt;
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
    uint8_t nvmRepeatCnt;
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
    }

    SCU_ChangeNVMProtection(NVM_DATA_WRITE, PROTECTION_CLEAR);
    SCU_ChangeNVMProtection(NVM_CODE_WRITE, PROTECTION_CLEAR);
    
    (void)USER_ERASEPG((uint32_t *)(uintptr_t)errorCodeAddressPageA, 0);

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


static void NvmVariables(void)
{
	uint8_t NVM_fault_merged_Byte1 = 0;
	uint8_t NVM_fault_merged_Byte2 = 0;

    (void)ReadFaultStateMerged_Byte1(&NVM_fault_merged_Byte1);
	(void)ReadFaultStateMerged_Byte2(&NVM_fault_merged_Byte2);
	
    NVM_fault_merged_Byte1 = NVM_fault_merged_Byte1 & 0x93u; // UV0 OV1 Stall4 SE7 Fault         
    NVM_fault_merged_Byte2 = NVM_fault_merged_Byte2 & 0x2Cu; // OT2 LC3 OC5       Fault
    NVM_fault_merged = (NVM_fault_merged_Byte1 | NVM_fault_merged_Byte2); // UV OV OT LC Stall OC SE Fault
    NVM_fault_merged_convert = (uint16_t)NVM_fault_merged + 0x100u; 

	(void)ReadFailureStateMerged(&NVM_failure_mereged);
	NVM_failure_mereged = NVM_failure_mereged & 0xC9u;              //SC GE abnormal PO
    NVM_failure_merged_convert = (uint16_t)NVM_failure_mereged + 0x200u;
	
	(void)ReadWarningStateMerged(&NVM_warning_mereged);
	NVM_warning_mereged = NVM_warning_mereged & 0x94u;             // WC WT SE warning
    NVM_warning_merged_convert = (uint16_t)NVM_warning_mereged + 0x300u;
    
}

static void ManageNVMBuildArray(void)
{
    static uint16_t NVM_FaultCodeSum = 0;  
    /* Previous sums are snapshots for this call, not persistent state. */
    uint16_t NVM_preFaultCodeSum = 0;

    static uint16_t NVM_FailureCodeSum = 0;  
    uint16_t NVM_preFailureCodeSum = 0;
	
    static uint16_t NVM_WarningCodeSum = 0;       
    uint16_t NVM_preWarningCodeSum = 0;



    NVM_preFaultCodeSum = NVM_FaultCodeSum;
    NVM_FaultCodeSum = NVM_FaultCodeSum | NVM_fault_merged;
    NVM_result_FaultCode = NVM_FaultCodeSum - NVM_preFaultCodeSum;
        
    NVM_preFailureCodeSum = NVM_FailureCodeSum;
    NVM_FailureCodeSum = NVM_FailureCodeSum | NVM_failure_mereged;
    NVM_result_FailureCode = NVM_FailureCodeSum - NVM_preFailureCodeSum;
       
    NVM_preWarningCodeSum = NVM_WarningCodeSum;
    NVM_WarningCodeSum = NVM_WarningCodeSum | NVM_warning_mereged;
    NVM_result_WarningCode = NVM_WarningCodeSum - NVM_preWarningCodeSum;    
    
    NVM_compareErrorCodeResult = (NVM_result_FaultCode | NVM_result_FailureCode | NVM_result_WarningCode);
}


static uint8_t convertErrorCode(uint16_t diagErrorCode)
{
    uint8_t convertedErrorCode = 0u;

    switch(diagErrorCode)
    {
        //case 0x00:
        //    convertedErrorCode = 0;  Normal 
        //    break;
        
        case 0x380:
            convertedErrorCode = 3; /* SE warning bit 7*/
            break;

        case 0x180:
            convertedErrorCode = 13; /* SE Fault bit 7*/
            break;

        case 0x104:
            convertedErrorCode = 4; /* OT bit 2*/
            break;

        case 0x108:
            convertedErrorCode = 6; /* LC bit 3*/
            break;

        case 0x120:    
            convertedErrorCode = 7; /* OC bit 5*/
            break;

        case 0x102:
            convertedErrorCode = 9; /* OV bit 1*/
            break;

        case 0x101:
            convertedErrorCode = 8; /* UV bit 0*/
            break;

        case 0x240:
            convertedErrorCode = 2; /* GE bit 6*/
            break;

        case 0x280:
            convertedErrorCode = 1; /* SC bit 7*/
            break;

        case 0x304:
            convertedErrorCode = 5; /* WT bit 2*/
            break;
		
        case 0x208:
            convertedErrorCode = 10; /* abnormal bit 3*/
            break;
		
        case 0x201:
            convertedErrorCode = 11; /* PO bit 1*/
            break;
		
		case 0x310:
            convertedErrorCode = 12; /* WC bit 4*/
            break;

        default: 
			/* Intentionally Empty */
            break;
    }

    return convertedErrorCode;
}

static void ManageNVMConvertErrorCode(void)
{
    uint16_t Nvm_warning_errorCode = 0;
    uint16_t Nvm_fault_errorCode = 0;
    uint16_t Nvm_failure_fault_errorCode = 0;
    
    CheckPrevValueNVMPEEWP();
    
    Nvm_warning_errorCode =  (NVM_compareErrorCodeResult != 0u) ? (NVM_convertWarningActualCodeResult) : 0u;
    Nvm_fault_errorCode =  (NVM_compareErrorCodeResult != 0u) ? (NVM_convertFaultActualCodeResult) : 0u;
    Nvm_failure_fault_errorCode =  (NVM_compareErrorCodeResult != 0u) ? (NVM_convertFailureActualCodeResult) : 0u;
    
    if(Nvm_warning_errorCode != 0u)
    {
        ManageNVMEWP(Nvm_warning_errorCode);
    }
    if(Nvm_fault_errorCode != 0u)
    {
        ManageNVMEWP(Nvm_fault_errorCode);
    }
    if(Nvm_failure_fault_errorCode != 0u)
    {
        ManageNVMEWP(Nvm_failure_fault_errorCode);
    }

}


static void CheckPrevValueNVMPEEWP(void)
{
    static uint16_t NVM_convertFaultCodeResult_Prev = 0x100u;
    static uint16_t NVM_convertWarningCodeResult_Prev = 0x300u;
    static uint16_t NVM_convertFailureCodeResult_Prev = 0x200u;
    uint16_t NVM_convertFault = 0x100u;
    uint16_t NVM_convertWarning = 0x300u;
    uint16_t NVM_convertFailure = 0x200u;
    
    if(NVM_result_FaultCode != 0u)
    {
        NVM_convertFault = (NVM_fault_merged_convert & ~(NVM_convertFaultCodeResult_Prev)) + 0x100u;
        NVM_convertFaultActualCodeResult = convertErrorCode(NVM_convertFault);
        NVM_convertFaultCodeResult_Prev = NVM_convertFault;
    }
    else
    {
        NVM_convertFaultActualCodeResult = 0u;
    }
    
    if(NVM_result_FailureCode != 0u)
    {
        NVM_convertFailure = (NVM_failure_merged_convert & ~(NVM_convertFailureCodeResult_Prev)) + 0x200u;
        NVM_convertFailureActualCodeResult = convertErrorCode(NVM_convertFailure);
        NVM_convertFailureCodeResult_Prev = NVM_convertFailure;
    }
    else
    {
        NVM_convertFailureActualCodeResult = 0u;
    }
    
    if(NVM_result_WarningCode != 0u)
    {
        NVM_convertWarning = (NVM_warning_merged_convert & ~(NVM_convertWarningCodeResult_Prev)) + 0x300u;
        NVM_convertWarningActualCodeResult = convertErrorCode(NVM_convertWarning);
        NVM_convertWarningCodeResult_Prev = NVM_convertWarning;
    }
    else
    {
        NVM_convertWarningActualCodeResult = 0u;
    }
    
}

static void ManageNVMEWP(uint16_t Nvm_convert_errorCode)
{
    uint8_t nvmRepeatCnt = 0;

	if (Nvm_errorCodeIndex >= errorCodeIndexMax)
	{
		for (nvmRepeatCnt = 0; nvmRepeatCnt < (FlashPageSize - 1u); nvmRepeatCnt++)
		{
			IRV_ptrErrorCodePage[nvmRepeatCnt] = IRV_ptrErrorCodePage[nvmRepeatCnt + 1u];
		}
		IRV_ptrErrorCodePage[127u] = (uint8_t)((uint16)Nvm_convert_errorCode + Nvm_drivingCycleIndicator);
	}
	else
	{
		IRV_ptrErrorCodePage[Nvm_errorCodeIndex] = (uint8_t)((uint16)Nvm_convert_errorCode + Nvm_drivingCycleIndicator);
		Nvm_errorCodeIndex = countErrorCodeIndex(IRV_ptrErrorCodePage);
	}

}


