/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : ExampleCode.c
* Project         : iEWP
* Created         : 2018.09.27
* Description     : 
* 
*
************************************************************************************************************************
* Revison History:
* YYYY/MM/DD             Version              Description                      Author
* 2018/09/27             v1.0                 Draft                            Alex.kim
*
*
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                                   Include header files
************************************************************************************************************************
*/
#include "ExampleCode.h"


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
Diag_FaultCodeType FailureCode[128] =
{
    FAULT_E_SHORTCIRCUIT,      /* 001 */
    FAULT_E_OVERVOLTAGE,       /* 002 */    
    FAULT_E_SHORTCIRCUIT,      /* 003 */
    FAULT_E_GATEERROR,         /* 004 */
    FAULT_E_SHORTCIRCUIT,      /* 005 */    
    FAULT_E_SHORTCIRCUIT,      /* 006 */
    FAULT_E_UNDERVOLTAGE,      /* 007 */
    FAULT_E_SHORTCIRCUIT,      /* 008 */    
    FAULT_E_OVERVOLTAGE,       /* 009 */
    FAULT_E_UNDERVOLTAGE,      /* 010 */
    FAULT_E_SHORTCIRCUIT,      /* 011 */    
    FAULT_E_UNDERVOLTAGE,      /* 012 */
    FAULT_E_SHORTCIRCUIT,      /* 013 */
    FAULT_E_SHORTCIRCUIT,      /* 014 */    
    FAULT_E_LINERROR,          /* 015 */
    FAULT_E_SHORTCIRCUIT,      /* 016 */
    FAULT_E_SHORTCIRCUIT,      /* 017 */    
    FAULT_E_LINERROR,          /* 018 */
    FAULT_E_SHORTCIRCUIT,      /* 019 */    
    FAULT_E_OVERTEMPERATURE,   /* 020 */
    FAULT_E_SHORTCIRCUIT,      /* 021 */
    FAULT_E_SPEEDERROR,        /* 022 */    
    FAULT_E_SHORTCIRCUIT,      /* 023 */
    FAULT_E_GATEERROR,         /* 024 */
    FAULT_E_SHORTCIRCUIT,      /* 025 */    
    FAULT_E_OVERTEMPERATURE,   /* 026 */
    FAULT_E_WARNTEMPERATURE,   /* 027 */
    FAULT_E_SHORTCIRCUIT,      /* 028 */    
    FAULT_E_OVERTEMPERATURE,   /* 029 */
    FAULT_E_WARNTEMPERATURE,   /* 030 */
    FAULT_E_SHORTCIRCUIT,      /* 031 */
    FAULT_E_OVERVOLTAGE,       /* 032 */    
    FAULT_E_SHORTCIRCUIT,      /* 033 */
    FAULT_E_GATEERROR,         /* 034 */
    FAULT_E_SHORTCIRCUIT,      /* 035 */    
    FAULT_E_SHORTCIRCUIT,      /* 036 */
    FAULT_E_UNDERVOLTAGE,      /* 037 */
    FAULT_E_SHORTCIRCUIT,      /* 038 */    
    FAULT_E_OVERVOLTAGE,       /* 039 */
    FAULT_E_UNDERVOLTAGE,      /* 040 */
    FAULT_E_SHORTCIRCUIT,      /* 041 */    
    FAULT_E_UNDERVOLTAGE,      /* 042 */
    FAULT_E_SHORTCIRCUIT,      /* 043 */
    FAULT_E_SHORTCIRCUIT,      /* 044 */    
    FAULT_E_LINERROR,          /* 045 */
    FAULT_E_SHORTCIRCUIT,      /* 046 */
    FAULT_E_SHORTCIRCUIT,      /* 047 */    
    FAULT_E_LINERROR,          /* 048 */
    FAULT_E_SHORTCIRCUIT,      /* 049 */    
    FAULT_E_OVERTEMPERATURE,   /* 050 */
    FAULT_E_SHORTCIRCUIT,      /* 051 */
    FAULT_E_SPEEDERROR,        /* 052 */    
    FAULT_E_SHORTCIRCUIT,      /* 053 */
    FAULT_E_GATEERROR,         /* 054 */
    FAULT_E_SHORTCIRCUIT,      /* 055 */    
    FAULT_E_OVERTEMPERATURE,   /* 056 */
    FAULT_E_WARNTEMPERATURE,   /* 057 */
    FAULT_E_SHORTCIRCUIT,      /* 058 */    
    FAULT_E_OVERTEMPERATURE,   /* 059 */
    FAULT_E_WARNTEMPERATURE,   /* 060 */
    0,0,0,0,0,0,0,0,0,0,       /* 61~70 */
    0,0,0,0,0,0,0,0,0,0,       /* 71~80 */
    0,0,0,0,0,0,0,0,0,0,       /* 81~90 */
    0,0,0,0,0,0,0,0,0,0,       /* 91~100 */
    0,0,0,0,0,0,0,0,0,0,       /* 101~110 */
    0,0,0,0,0,0,0,0,0,0,       /* 111~120 */
    0,0,0,0,0,0,0,0            /* 121~128 */    
};

uint8_t ItemVersion[16] = 
{
    'M',    /* 01 */
    'Y',    /* 02 */
    'U',    /* 03 */
    'N',    /* 04 */
    'G',    /* 05 */
    '_',    /* 06 */
    'H',    /* 07 */
    'W',    /* 08 */
    'A',    /* 09 */
    '_',    /* 10 */
    'S',    /* 11 */
    'W',    /* 12 */
    '_',    /* 13 */
    '0',    /* 14 */
    '0',    /* 15 */
    '1'     /* 16 */    
};


uint8_t HWVersion[3] = 
{
    'H',    /* 01 */
    'W',    /* 02 */
    '1'     /* 03 */
};


uint8_t BOOTVersion[3] = 
{
    'B',    /* 01 */
    'T',    /* 02 */
    '1'     /* 03 */
};


uint8_t DateInfo[10] = 
{
    '2',    /* 01 */
    '0',    /* 02 */
    '1',    /* 03 */
    '8',    /* 04 */
    '/',    /* 05 */
    '0',    /* 06 */
    '9',    /* 07 */
    '/',    /* 08 */
    '2',    /* 09 */
    '1'     /* 10 */
};

uint8_t OnLineCheckSum[4] = 
{
    0,    /* 01 */
    0,    /* 02 */
    0,    /* 03 */
    0     /* 04 */
};


uint8_t CommandOfEeprom[3] = 
{
    'D',    /* 01 */
    'E',    /* 02 */
    'L'     /* 03 */    
};


uint8_t CalibrationData[24] = 
{
    0,      /* 001 */
    0,      /* 002 */    
    0,      /* 003 */
    0,      /* 004 */
    0,      /* 005 */    
    0,      /* 006 */
    0,      /* 007 */
    0,      /* 008 */    
    0,      /* 009 */
    0,      /* 010 */
    0,      /* 011 */    
    0,      /* 012 */
    0,      /* 013 */
    0,      /* 014 */    
    0,      /* 015 */
    0,      /* 016 */
    0,      /* 017 */    
    0,      /* 018 */
    0,      /* 019 */    
    0,      /* 020 */
    0,      /* 021 */
    0,      /* 022 */    
    0,      /* 023 */
    0       /* 024 */
};

uint8_t NvmEraseFlag = 0;


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
uint8_t CallBack_BootSoftwareIdentifier (uint8_t* mData, uint16_t mDataLength)
{
    boolean_t retValue = TRUE;

    return retValue;
}

uint8_t CallBack_ApplicationSoftwareIdentifier (uint8_t* mData, uint16_t mDataLength)
{
    boolean_t retValue = TRUE;

    return retValue;
}

uint8_t CallBack_VehicleManufacturer (uint8_t* mData, uint16_t mDataLength)
{
    boolean_t retValue = TRUE;

    return retValue;
}

uint8_t CallBack_ReportTesterSerialNumber (uint8_t* mData, uint16_t mDataLength)
{
    boolean_t retValue = TRUE;

    return retValue;
}

uint8_t CallBack_ReportProgrammingDate (uint8_t* mData, uint16_t mDataLength)
{
    boolean_t retValue = TRUE;

    return retValue;
}

uint8_t CallBack_ReportEcuCheckSum (uint8_t* mData, uint16_t mDataLength)
{
    boolean_t retValue = TRUE;

    return retValue;
}

uint8_t CallBack_ReportEraseEeprom (uint8_t* mData, uint16_t mDataLength)
{
    boolean_t retValue = TRUE;

    uint8_t mCheckBuf[3] = {0, 0, 0};

    if (mDataLength != 3)
    {
        retValue = FALSE;
    }
    else
    {
        mCheckBuf[0] = mData[0];
        mCheckBuf[1] = mData[1];
        mCheckBuf[2] = mData[2];
    }

    if ((mCheckBuf[0] != CommandOfEeprom[0]) || (mCheckBuf[1] != CommandOfEeprom[1]) || (mCheckBuf[2] != CommandOfEeprom[2]))
    {
        retValue = FALSE;
    }
    else
    {
    }

    NvmEraseFlag = 1;
    
    return retValue;
}

uint8_t CallBack_ReportCalibratingData (uint8_t* mData, uint16_t mDataLength)
{
    boolean_t retValue = TRUE;
    uint8_t i = 0;

    if (mDataLength > 24)
    {
        retValue = FALSE;
    }
    else
    {
        for (i = 0; i < mDataLength; i++)
        {
            CalibrationData[i] = mData[i];
        }
    }

    return retValue;
}


uint8_t CallBack_ReportDataCalibrated (uint8_t* mData, uint16_t mDataLength)
{
    boolean_t retValue = TRUE;

    return retValue;
}



void MakeCheckSum (void)
{
    /* ************************************************************************** */
    /* address of program area */
    uint32_t applicationStartAddr = 0x11002200; 
    uint32_t applicationEndAddr   = 0x1100DF7F;
    uint32_t i = 0;
    uint32_t* mFlashValue = 0;
    uint32_t mCheckSum = 0xFFFFFFFF;


    for(i = applicationStartAddr; i < applicationEndAddr; i=i+4) {
        mFlashValue = (uint32_t*)i;
        mCheckSum ^= *mFlashValue;
    }

    OnLineCheckSum[0] = (uint8_t)((mCheckSum & 0xFF000000) >> 24);
    OnLineCheckSum[1] = (uint8_t)((mCheckSum & 0x00FF0000) >> 16);
    OnLineCheckSum[2] = (uint8_t)((mCheckSum & 0x0000FF00) >>  8);
    OnLineCheckSum[3] = (uint8_t)((mCheckSum & 0x000000FF)      );    
}


/*
************************************************************************************************************************
*                                                      Local functions
************************************************************************************************************************
*/


