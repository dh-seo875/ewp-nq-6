/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : Sensor.c
* Project         : JKa_EREV
* Created         : YYYY/MM/DD
* Description     : sensor
* 
* 
************************************************************************************************************************
* Revison History:
* YYYY/MM/DD             Version              Description                      Author
* 2018/02/08             v1.0                 Draft first version
*
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                                   Include header files
************************************************************************************************************************
*/
#include "Sensor.h"
#include "RteStd.h"
#include "Diag.h"
#include "Manage.h"
#include "Variant_cfg.h"

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

/* via Rte */
static sint16_t    sensor_idcFiltered		  = 0;	



/* Only used Sensor.c */



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


/*
************************************************************************************************************************
*                                                 Local function prototypes
************************************************************************************************************************
*/
static void CheckShortCircuit(void);
static void CheckGateDrvError (void);
static void MeasVdc (void);
static void MeasIdc (void);
static void MeasIq (void);
static void MeasId (void);
static void MeasSpeed (void);
static void MeasTemperature(void);
//static void MeasFluxAbsValue(void);
static void CheckLinError (void);
static void CheckIgn (void);
static void CheckOpenPhase (void);
static void ConvertDutyToRpm (void);
static void ConvertRpmToDuty (void);
static void MeasFluxAbsValue(void);
static void SensorSWComponent1(void);
static void SensorSWComponent2(void);
static void SensorSWComponent3(void);
static void SensorSWComponent4(void);


/*
************************************************************************************************************************
*                                                    Exported functions
************************************************************************************************************************
*/

static void SensorSWComponent1(void)
{
	ConvertDutyToRpm();
	CheckOpenPhase();
	MeasSpeed();
	ConvertRpmToDuty();
}

static void SensorSWComponent2(void)
{
	MeasVdc();
	MeasIdc();
	MeasIq();
}

static void SensorSWComponent3(void)
{
	CheckLinError();	
	CheckIgn();
}

static void SensorSWComponent4(void)
{
	CheckShortCircuit();
	CheckGateDrvError();
	MeasTemperature();
	MeasFluxAbsValue();
}



void SensorSWComponentPack(void)
{
    static uint8_t SensorTask_Cnt = 0;
	if(SensorTask_Cnt == 0u)  //16us
	{
        SensorSWComponent1();
		SensorTask_Cnt++;
	}
	else if(SensorTask_Cnt == 1u) //17us
	{
        SensorSWComponent2();
		SensorTask_Cnt++;
	}
	else if(SensorTask_Cnt == 2u) //10us
	{
	    SensorSWComponent3();
		SensorTask_Cnt++;
	}
	else  //13us
	{
	    SensorSWComponent4();
		SensorTask_Cnt = 0u;
	}
}
static void ConvertDutyToRpm(void)
{
	static uint16_t rpm = 0U;

	uint8_t sensor_LinRequireDuty = 0U;    
	uint32_t tmp;

	(void)ReadLinRequireDuty(&sensor_LinRequireDuty);

	if (sensor_LinRequireDuty < LOWER_DUTY) // 0-9 duty
	{
	    rpm = 0U;
	}
	else if ((sensor_LinRequireDuty <= UPPER_DUTY)) //10-90 duty
	{
	    /* tmp = (duty - 10) * 5000 */
	    tmp = (uint32_t)((uint32_t)sensor_LinRequireDuty - (uint32_t)LOWER_DUTY) * (uint32_t)RPM_RANGE;

	    /* rpm = 1000 + tmp / 80 */
	    rpm = (uint16_t)((uint32_t)MIN_RPM + (tmp / (uint32_t)DUTY_RANGE));
	}
	else if((sensor_LinRequireDuty <= 100U)) //91-100
	{
    	rpm = MAX_RPM;
	}
	else // 101-255
	{
         /* Hold last valid RPM */
	}

	(void)WriteConvertRPM((sint16_t)rpm);
}
static void ConvertRpmToDuty(void)
{
	uint8_t duty;
	sint16_t sensor_ActRPM;
	uint16_t sensor_diffRPM;
	uint16_t duty_calc;
	
	(void)ReadSpeedFiltered(&sensor_ActRPM);
	
	if (sensor_ActRPM <= 0)
	{
		/* 0 RPM �� 0% duty */
		duty = 0U;
	}
	else if (sensor_ActRPM < (sint16_t)MIN_RPM)  // 0 - 10 Duty area
	{
		/* Below mapping range �� lower bound */
		duty = (uint8_t)((uint16_t)sensor_ActRPM/100u);
	}
	else if (sensor_ActRPM >= (sint16_t)MAX_RPM) // 90 - 100 Duty area
	{
		/* Above mapping range �� upper bound */
		duty = UPPER_DUTY;
	}
	else // 10 - 89 Duty area
	{
		/* Linear mapping (pure integer math) */
		sensor_diffRPM = (uint16_t)sensor_ActRPM - (uint16_t)MIN_RPM;	
		duty_calc = (DUTY_RANGE * sensor_diffRPM) / (uint16_t)RPM_RANGE;
	
		duty = (uint8_t)((uint16_t)LOWER_DUTY + duty_calc);
	}
	
	(void)WriteConvertActDuty(duty);

}
static void CheckShortCircuit(void)
{
	uint8_t     sensor_hwShortCircuitCheck = 0;
	DiagState_t sensor_hwShortCircuitState = NORMAL;
    (void)GetOverCurrent(&sensor_hwShortCircuitCheck);

    if(sensor_hwShortCircuitCheck == 0u) 
    {
        sensor_hwShortCircuitState = NORMAL;
    }
    else
    {
        sensor_hwShortCircuitState = FAULT;
    }
    

    (void)WriteHwShortCircuitState(sensor_hwShortCircuitState);
}


static void CheckGateDrvError (void)
{
	uint8_t     sensor_hwGateDrvErrorCheck = 0;
	DiagState_t sensor_hwGateDrvErrorState = NORMAL;
    (void)GetGateDrvError(&sensor_hwGateDrvErrorCheck);

    if(sensor_hwGateDrvErrorCheck == 0u)
    {
        sensor_hwGateDrvErrorState = NORMAL;
    }
    else
    {
        sensor_hwGateDrvErrorState = FAULT;
    }
    

    (void)WriteHwGateDrvErrorState(sensor_hwGateDrvErrorState);
}


static void MeasVdc (void)
{    
	static const sint16_t CAL_VdcConversionFactor   = VDC_CONVERSION_FACTOR;
	static const sint16_t CAL_VdcCompensationFactor = VDC_COMPENSATION_FACTOR;
	sint16_t    sensor_vdcMeasured		  = 0;
	sint16_t    sensor_vdcFiltered		  = 0;

    (void)GetVdcMeasured(&sensor_vdcMeasured);
	if(sensor_vdcMeasured > 0)
    {
        sensor_vdcMeasured = (sensor_vdcMeasured / CAL_VdcConversionFactor); /*0.01V from mV*/
    }
    sensor_vdcMeasured = (sensor_vdcMeasured + CAL_VdcCompensationFactor); /*Compensate voltage Drop depend on D3*/

    if(sensor_idcFiltered > 200)
    {
        sensor_vdcMeasured = (sensor_vdcMeasured + (sensor_idcFiltered * (sint16_t)6 / (sint16_t)100));
    }
    else
    {
    }
    
    sensor_vdcFiltered = iirVdc(sensor_vdcMeasured);

    //(void)WriteVdcMeasured(sensor_vdcMeasured);
    (void)WriteVdcFiltered(sensor_vdcFiltered);
}


static void MeasIdc (void)
{    
	static const sint16_t CAL_IdcConversionFactor   = IDC_CONVERSION_FACTOR;
    sint16_t    sensor_idcMeasured         = 0;

    (void)GetIdcMeasured(&sensor_idcMeasured);

    sensor_idcMeasured = (sensor_idcMeasured / CAL_IdcConversionFactor); /*0.01A from mA*/
    sensor_idcFiltered = iirIdc(sensor_idcMeasured);

//    (void)WriteIdcMeasured(sensor_idcMeasured);
    (void)WriteIdcFiltered(sensor_idcFiltered);
}

static void MeasIq (void)
{
//	sint16_t    sensor_idMeasured		  = 0;
	sint16_t    sensor_iqMeasured		  = 0;
	sint16_t    sensor_iqFiltered		  = 0;

    (void)GetIqMeasured(&sensor_iqMeasured);
//    (void)GetIdMeasured(&sensor_idMeasured);

    sensor_iqMeasured = (sint16_t)((sint32_t)sensor_iqMeasured * 100 / 197); /*1A 83.33A from 32767*/
//    sensor_idMeasured = (sint16_t)((sint32_t)sensor_idMeasured * 100 / 393);
    sensor_iqFiltered = iirIq(sensor_iqMeasured);

//    (void)WriteIqMeasured(sensor_iqMeasured);
//    (void)WriteIdMeasured(sensor_idMeasured);
    (void)WriteIqFiltered(sensor_iqFiltered);
}


static void MeasSpeed (void)
{
	sint16_t    sensor_speedMeasured 	  = 0;
	sint16_t    sensor_speedFiltered 	  = 0;
    DiagState_t	   sensor_hwOpenPhaseCheck	  = NORMAL;

    (void)GetSpeedMeasured(&sensor_speedMeasured);
    (void)ReadHwOpenPhaseState(&sensor_hwOpenPhaseCheck);

    if (sensor_hwOpenPhaseCheck == FAULT) /* over 2 phase open */
    {
//        sensor_speedMeasured = 0;
        sensor_speedFiltered = 0;
    }
    else /* less than 1 phase open */
    {
        sensor_speedFiltered = iirSpeed(sensor_speedMeasured);
    }

    
//    (void)WriteSpeedMeasured(sensor_speedMeasured);
    (void)WriteSpeedFiltered(sensor_speedFiltered);
}


static void MeasTemperature (void)
{
	sint16_t    sensor_temperatureMeasured = 0;
	sint16_t    sensor_temperatureFiltered = 0;

    (void)GetTemperatureMeasured(&sensor_temperatureMeasured);

    sensor_temperatureFiltered = iirTemperature(sensor_temperatureMeasured);

//    (void)WriteTemperatureMeasured(sensor_temperatureMeasured);
    (void)WriteTemperatureFiltered(sensor_temperatureFiltered);
}


static void CheckLinError (void)
{
    uint8_t linErrorCheck = 0;
	uint8_t     sensor_linErrorCheck       = 0;
    DiagState_t sensor_linErrorState       = NORMAL;
	
    (void)ReadLinTimeOutFlag(&sensor_linErrorCheck);

    linErrorCheck = sensor_linErrorCheck;

    if(linErrorCheck == 0u)
    {
        sensor_linErrorState = NORMAL;
    }
    else
    {
        sensor_linErrorState = FAULT;
    }
    

    (void)WriteLinTimeOutErrorState(sensor_linErrorState);
}

static void CheckIgn(void)
{
	static const sint16_t CAL_IgnConversionFactor   = IGN_CONVERSION_FACTOR;
	static const sint16_t CAL_IgnOnTh               = IGN_ON_TH;
	static const sint16_t CAL_IgnOffTh              = IGN_OFF_TH;
	static const sint16_t CAL_IgnFaultCntMax        = IGN_FAULT_CNT_MAX;
	static const sint16_t CAL_IgnNormalCntMax	   = IGN_NORMAL_CNT_MAX;

	sint16_t    sensor_ignMeasured		  = 0;
	sint16_t    sensor_ignFiltered		  = 0;
	static sint16_t    ignFaultCnt		   = 0;
	static sint16_t    ignNormalCnt 	   = 0;
    static sint8_t sensor_ignState 	  = IGN_ON;
	
    (void)GetIgnMeasured(&sensor_ignMeasured);

    sensor_ignMeasured = (sensor_ignMeasured / CAL_IgnConversionFactor); /*0.01V from mV*/
    sensor_ignFiltered = iirIgn(sensor_ignMeasured);

    switch(sensor_ignState)
    {
        case IGN_OFF:
            if (sensor_ignFiltered > CAL_IgnOnTh)
            {
                ignNormalCnt++;
            }
            else
            {
                ignNormalCnt--;
                if (ignNormalCnt < 0)  /* zero saturation */
                {
                    ignNormalCnt = 0;
                }
            }


            if (ignNormalCnt >= CAL_IgnNormalCntMax)
            {
                ignNormalCnt = 0;
                sensor_ignState = IGN_ON;
            }
            else
            {
            }
            break;
        
		default: // Case IGN_ON
            if (sensor_ignFiltered < CAL_IgnOffTh)
            {
                ignFaultCnt++;
            }
            else
            {
                ignFaultCnt--;
                if (ignFaultCnt < 0)  /* zero saturation */
                {
                    ignFaultCnt = 0;
                }
            }
			
	        if (ignFaultCnt >= CAL_IgnFaultCntMax)
            {
                ignFaultCnt = 0;
                sensor_ignState = IGN_OFF;
            }
            else
            {
            }			
			break;
    }

//    (void)WriteIgnMeasured(sensor_ignMeasured);
//    (void)WriteIgnFiltered(sensor_ignFiltered);
    (void)WriteIgnFlag(sensor_ignState);
}

static void CheckOpenPhase(void)
{
	DiagState_t sensor_hwOpenPhaseState	  = NORMAL;
	sint16_t sensor_Phas_A = 0;
	sint16_t sensor_Phas_B = 0;
	sint16_t sensor_Phas_C = 0;
	
	static sint16_t ErrCnt_Phs_A = 0;
    static sint16_t ErrCnt_Phs_B = 0;
    static sint16_t ErrCnt_Phs_C = 0;
	uint8_t sensor_MotorState = 0;
	uint8_t sensor_Confirm_OPflag_sum = 0;
	static uint8_t Confirm_OPflag_A = 0;
	static uint8_t Confirm_OPflag_B = 0;
	static uint8_t Confirm_OPflag_C = 0;
	
	(void)GetPhaseCurrA(&sensor_Phas_A);
	(void)GetPhaseCurrB(&sensor_Phas_B);	
	(void)GetPhaseCurrC(&sensor_Phas_C);
	(void)GetMotorState(&sensor_MotorState);	


    /* Closed Loop*/
	if(sensor_MotorState == 3u)
	{
		/******************************* Check A,B and C Phase *******************************/
		ErrCnt_Phs_A = ((sensor_Phas_A < UPPER_LIMIT) && (sensor_Phas_A > LOWER_LIMIT)) ? (ErrCnt_Phs_A+1) : (ErrCnt_Phs_A-1);
		
		ErrCnt_Phs_B = ((sensor_Phas_B < UPPER_LIMIT) && (sensor_Phas_B > LOWER_LIMIT)) ? (ErrCnt_Phs_B+1) : (ErrCnt_Phs_B-1);
			
		ErrCnt_Phs_C = ((sensor_Phas_C < UPPER_LIMIT) && (sensor_Phas_C > LOWER_LIMIT)) ? (ErrCnt_Phs_C+1) : (ErrCnt_Phs_C-1);

		/*****************************************************************************************/
		
		/**************************** Saturation OpenPhase Error Count ***************************/

		ErrCnt_Phs_A = (ErrCnt_Phs_A >= UPPER_COUNTER_LIMIT) ? UPPER_COUNTER_LIMIT : ErrCnt_Phs_A;
        ErrCnt_Phs_A = (ErrCnt_Phs_A < LOWER_COUNTER_LIMIT) ? LOWER_COUNTER_LIMIT : ErrCnt_Phs_A;

		ErrCnt_Phs_B = (ErrCnt_Phs_B >= UPPER_COUNTER_LIMIT) ? UPPER_COUNTER_LIMIT : ErrCnt_Phs_B;
        ErrCnt_Phs_B = (ErrCnt_Phs_B < LOWER_COUNTER_LIMIT) ? LOWER_COUNTER_LIMIT : ErrCnt_Phs_B;

		ErrCnt_Phs_C = (ErrCnt_Phs_C >= UPPER_COUNTER_LIMIT) ? UPPER_COUNTER_LIMIT : ErrCnt_Phs_C;
        ErrCnt_Phs_C = (ErrCnt_Phs_C < LOWER_COUNTER_LIMIT) ? LOWER_COUNTER_LIMIT : ErrCnt_Phs_C;		

		/************************************************************************************/

				/************************** OpenPhase A,B and C Hysteresis************************/
		Confirm_OPflag_A = (ErrCnt_Phs_A >= CONFIRM_OP_COUNT) ? 1u : Confirm_OPflag_A;
		Confirm_OPflag_A = (ErrCnt_Phs_A <= UNCONFIRM_OP_COUNT) ? 0u : Confirm_OPflag_A;

		Confirm_OPflag_B = (ErrCnt_Phs_B >= CONFIRM_OP_COUNT) ? 1u : Confirm_OPflag_B;
		Confirm_OPflag_B = (ErrCnt_Phs_B <= UNCONFIRM_OP_COUNT) ? 0u : Confirm_OPflag_B;		

		Confirm_OPflag_C = (ErrCnt_Phs_C >= CONFIRM_OP_COUNT) ? 1u : Confirm_OPflag_C;
		Confirm_OPflag_C = (ErrCnt_Phs_C <= UNCONFIRM_OP_COUNT) ? 0u : Confirm_OPflag_C;		

		/*********************************************************************************/
		
		/************************************ Other **************************************/
		sensor_Confirm_OPflag_sum  = Confirm_OPflag_A + Confirm_OPflag_B + Confirm_OPflag_C;
	}
	/* Motor Stop*/
	else if(sensor_MotorState == 1u) 
	{
		ErrCnt_Phs_A = 0;
		ErrCnt_Phs_B = 0;
		ErrCnt_Phs_C = 0;
		sensor_Confirm_OPflag_sum = 0u;
	}
	else
    {
        /* no action */
	}
    
    if (sensor_Confirm_OPflag_sum >= 0x02u)
    {
        sensor_hwOpenPhaseState = FAULT;
    }
    else if (sensor_Confirm_OPflag_sum == 0x01u)
    {
        sensor_hwOpenPhaseState = WARNING;
    }
    else
    {
        sensor_hwOpenPhaseState = NORMAL;
    }
    
    (void)WriteHwOpenPhaseState(sensor_hwOpenPhaseState);
}

static void MeasFluxAbsValue (void)
{
	static sint16_t sensor_fluxAbsValueMeasured = 0;
    static uint16_t sensor_fluxAbsFiltered = 0u;
    static uint16_t medianBuffer[3] = {0u, 0u, 0u};
    static uint32_t filterAccumulator = 0u;
    static uint8_t medianIndex = 0u;
    static uint8_t initialized = 0u;
    uint16_t rawFlux;
    uint16_t medianFlux;
    uint16_t a;
    uint16_t b;
    uint16_t c;

    (void)GetFluxAbsValue(&sensor_fluxAbsValueMeasured);

    /* Flux magnitude is clamped at zero if an invalid signed value is received. */
    rawFlux = (sensor_fluxAbsValueMeasured > 0) ? (uint16_t)sensor_fluxAbsValueMeasured : 0u;

    if (initialized == 0u)
    {
        medianBuffer[0] = rawFlux;
        medianBuffer[1] = rawFlux;
        medianBuffer[2] = rawFlux;
        filterAccumulator = (uint32_t)rawFlux * FLUX_DIAG_IIR_DIVISOR;
        initialized = 1u;
    }
    else
    {
        medianBuffer[medianIndex] = rawFlux;
        medianIndex++;
        if (medianIndex >= 3u)
        {
            medianIndex = 0u;
        }
    }

    a = medianBuffer[0];
    b = medianBuffer[1];
    c = medianBuffer[2];
    if (((a >= b) && (a <= c)) || ((a >= c) && (a <= b)))
    {
        medianFlux = a;
    }
    else if (((b >= a) && (b <= c)) || ((b >= c) && (b <= a)))
    {
        medianFlux = b;
    }
    else
    {
        medianFlux = c;
    }
    /* First-order IIR: y[n] = y[n-1] + (x[n] - y[n-1]) / 20.
       The scaled accumulator avoids a 20-sample buffer and integer deadband. */
    filterAccumulator = filterAccumulator -
                        (filterAccumulator / FLUX_DIAG_IIR_DIVISOR) +
                        (uint32_t)medianFlux;
    sensor_fluxAbsFiltered = (uint16_t)(filterAccumulator / FLUX_DIAG_IIR_DIVISOR);
    (void)WriteFluxAbsValueMeasured(sensor_fluxAbsFiltered);
}
//static void CheckErrorCodeIndex (void)
//{    
//	uint8_t	   sensor_errorCodeIndex	  = 0;

//    (void)GetErrorCodeIndex(&sensor_errorCodeIndex);

//    (void)WriteErrorCodeIndex(sensor_errorCodeIndex);
//}


//static void CheckPowerLimit (void)
//{    
//	uint8_t	   sensor_Power_Limit	  = 0;

//    (void)GetPowerLimitState(&sensor_Power_Limit);

//    (void)WritePowerLimit(sensor_Power_Limit);
//}


/*
************************************************************************************************************************
*                                                      Local functions
************************************************************************************************************************
*/






