/**************************************************************
WinFilter version 0.8
http://www.winfilter.20m.com
akundert@hotmail.com

Filter type: Low Pass
Filter model: Butterworth
Filter order: 1
Sampling Frequency: 200 Hz
Cut Frequency: 20.000000 Hz
Coefficents Quantization: 16-bit

Z domain Zeros
z = -1.000000 + j 0.000000

Z domain Poles
z = 0.509521 + j -0.000000
***************************************************************/
#include "Std_Types.h"

#define NCoef 1
#define DCgain 4

sint16_t iirVdc(sint16_t NewSample) {
    const sint32_t ACoef[NCoef+1] = {
        32143,
        32143
    };

    const sint32_t BCoef[NCoef+1] = {
        32768,
        -16696
    };

    static sint32_t y[NCoef+1]; //output samples
    //Warning!!!!!! This variable should be signed (input sample width + Coefs width + 1 )-bit width to avoid saturation.

    static sint32_t x[NCoef+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef; n>0; n--) {
       x[n] = x[n-1];
       y[n] = y[n-1];
    }

    //Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for(n=1; n<=NCoef; n++)
    {
        y[0] += ((ACoef[n] * x[n]) - (BCoef[n] * y[n]));
    }
    y[0] /= BCoef[0];
    
    return (sint16_t)(y[0] / DCgain);
}

sint16_t iirIdc(sint16_t NewSample) {
    const sint32_t ACoef[NCoef+1] = {
        32143,
        32143
    };

    const sint32_t BCoef[NCoef+1] = {
        32768,
        -16696
    };

    static sint32_t y[NCoef+1]; //output samples
    //Warning!!!!!! This variable should be signed (input sample width + Coefs width + 1 )-bit width to avoid saturation.

    static sint32_t x[NCoef+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef; n>0; n--) {
       x[n] = x[n-1];
       y[n] = y[n-1];
    }

    //Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for(n=1; n<=NCoef; n++)
    {
        y[0] += ((ACoef[n] * x[n]) - (BCoef[n] * y[n]));
    }
    y[0] /= BCoef[0];
    
    return (sint16_t)(y[0] / DCgain);
}

sint16_t iirIq(sint16_t NewSample) {
    const sint32_t ACoef[NCoef+1] = {
        32143,
        32143
    };

    const sint32_t BCoef[NCoef+1] = {
        32768,
        -16696
    };

    static sint32_t y[NCoef+1]; //output samples
    //Warning!!!!!! This variable should be signed (input sample width + Coefs width + 1 )-bit width to avoid saturation.

    static sint32_t x[NCoef+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef; n>0; n--) {
       x[n] = x[n-1];
       y[n] = y[n-1];
    }

    //Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for(n=1; n<=NCoef; n++)
    {
        y[0] += ((ACoef[n] * x[n]) - (BCoef[n] * y[n]));
    }

    y[0] /= BCoef[0];
    
    return (sint16_t)(y[0] / DCgain);
}

sint16_t iirSpeed(sint16_t NewSample) {
    const sint32_t ACoef[NCoef+1] = {
        32143,
        32143
    };

    const sint32_t BCoef[NCoef+1] = {
        32768,
        -16696
    };

    static sint32_t y[NCoef+1]; //output samples
    //Warning!!!!!! This variable should be signed (input sample width + Coefs width + 1 )-bit width to avoid saturation.

    static sint32_t x[NCoef+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef; n>0; n--) {
       x[n] = x[n-1];
       y[n] = y[n-1];
    }

    //Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for(n=1; n<=NCoef; n++)
    {
        y[0] += ((ACoef[n] * x[n]) - (BCoef[n] * y[n]));
    }

    y[0] /= BCoef[0];
    
    return (sint16_t)(y[0] / DCgain);
}

sint16_t iirTemperature(sint16_t NewSample) {
    const sint32_t ACoef[NCoef+1] = {
        32143,
        32143
    };

    const sint32_t BCoef[NCoef+1] = {
        32768,
        -16696
    };

    static sint32_t y[NCoef+1]; //output samples
    //Warning!!!!!! This variable should be signed (input sample width + Coefs width + 1 )-bit width to avoid saturation.

    static sint32_t x[NCoef+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef; n>0; n--) {
       x[n] = x[n-1];
       y[n] = y[n-1];
    }

    //Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for(n=1; n<=NCoef; n++)
    {
        y[0] += ((ACoef[n] * x[n]) - (BCoef[n] * y[n]));
    }
	
    y[0] /= BCoef[0];
    
    return (sint16_t)(y[0] / DCgain);
}

sint16_t iirIgn(sint16_t NewSample) {
    const sint32_t ACoef[NCoef+1] = {
        32143,
        32143
    };

    const sint32_t BCoef[NCoef+1] = {
        32768,
        -16696
    };

    static sint32_t y[NCoef+1]; //output samples
    //Warning!!!!!! This variable should be signed (input sample width + Coefs width + 1 )-bit width to avoid saturation.

    static sint32_t x[NCoef+1]; //input samples
    int n;

    //shift the old samples
    for(n=NCoef; n>0; n--) {
       x[n] = x[n-1];
       y[n] = y[n-1];
    }

    //Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for(n=1; n<=NCoef; n++)
    {
        y[0] += ((ACoef[n] * x[n]) - (BCoef[n] * y[n]));
    }

    y[0] /= BCoef[0];
    
    return (sint16_t)(y[0] / DCgain);
}


/**************************************************************
WinFilter version 0.8
http://www.winfilter.20m.com
akundert@hotmail.com

Filter type: Low Pass
Filter model: Butterworth
Filter order: 2
Sampling Frequency: 200 Hz
Cut Frequency: 1.000000 Hz
Coefficents Quantization: 16-bit

Z domain Zeros
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000

Z domain Poles
z = 0.977789 + j -0.021728
z = 0.977789 + j 0.021728
***************************************************************/
//#define Ntap 31

//#define DCgainFir 524288

/*sint16_t firFluxAbsValue(sint16_t NewSample) {
    const sint16_t FIRCoef[Ntap] = { 
        13409,
        13856,
        14312,
        14776,
        15248,
        15728,
        16217,
        16714,
        17218,
        17730,
        18250,
        18777,
        19311,
        19853,
        20401,
        20676,
        20401,
        19853,
        19311,
        18777,
        18250,
        17730,
        17218,
        16714,
        16217,
        15728,
        15248,
        14776,
        14312,
        13856,
        13409
    };

    static sint16_t x[Ntap]; input samples
    sint32_t y=0;           output sample
    int n;

    shift the old samples
    for(n=Ntap-1; n>0; n--)
    {
       x[n] = x[n-1];
    }

    Calculate the new output
    x[0] = NewSample;
    for(n=0; n<Ntap; n++)
    {
        y += FIRCoef[n] * x[n];
    }
    
    return (sint16_t)(y / DCgainFir);
}*/

