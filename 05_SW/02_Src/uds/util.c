
#define util_c
#include "tle_device.h"
#include "Main.h"

#include "linmain.h"
#include "genLinConfig.h"
#include "lin_driver_api.h"
#include "Emo_RAM.h"
#include "def_lin.h"
#include "util.h"
#include "uds.h"


/** ==========================================================================

========================================================================== */
extern void disable_all_interrupt(void);

void goto_reset(void)
{
	uint8_t i,buf[128];
 disable_all_interrupt();
	SCU_ChangeNVMProtection(NVM_DATA_READ, PROTECTION_CLEAR);
	SCU_ChangeNVMProtection(NVM_DATA_WRITE,PROTECTION_CLEAR);
	for(i=0;i<128;i++) buf[i]=i;
	ProgramPage(APP_MESSAGE_ADDRESS, buf, 0, 0, 0);
	Delay_us(1000);
//	p=(void *)APP_MESSAGE_ADDRESS;	
//	p[0]=APP_MESSAGE_SIGN;
	__NVIC_SystemReset();
}

unsigned char D2C(unsigned char c)
{
	if(c<10) return (c+'0');
	return (c+'7');
}

/* ===========================================================================================

=========================================================================================== */  
void OutDev0(uint8_t send)
{
	UART2_Send_Byte(send);
	Delay_us(40);

}

/* ===========================================================================================

=========================================================================================== */  
void OutStr(const char *text)
{
  while (*text != '\0'){
		OutDev0(*text++) ;
  }
}

/** ==========================================================================

========================================================================== */
void put_flag(uint8_t a)				
{							
	if(a) OutDev0('1');	
	else OutDev0('0');	
	OutDev0(',');	
}

void put_int(unsigned int a)				
{							
	unsigned char i;
	i=a>>12;OutDev0(D2C(i));	
	i=a>>8; i&=0x0F;	OutDev0(D2C(i));	
	i=a>>4; i&=0x0F;	OutDev0(D2C(i));	
	i=a&0x0F;	OutDev0(D2C(i));	
	OutDev0(',');	
}
/** ==========================================================================

========================================================================== */
void put_int_dec(uint16_t a)
{
    uint8_t c[5];
    boolean_t digitStarted = FALSE;
    uint16_t k = a;
    uint16_t p;

    p = k / 10000u;
    if (p != 0u)
    {
        c[4] = (uint8_t)(p + (uint16_t)'0');
        digitStarted = TRUE;
    }
    else
    {
        c[4] = (uint8_t)' ';
    }
    k %= 10000u;

    p = k / 1000u;
    if ((p == 0u) && (digitStarted == FALSE))
    {
        c[3] = (uint8_t)' ';
    }
    else
    {
        c[3] = (uint8_t)(p + (uint16_t)'0');
        digitStarted = TRUE;
    }
    k %= 1000u;

    p = k / 100u;
    if ((p == 0u) && (digitStarted == FALSE))
    {
        c[2] = (uint8_t)' ';
    }
    else
    {
        c[2] = (uint8_t)(p + (uint16_t)'0');
        digitStarted = TRUE;
    }
    k %= 100u;

    p = k / 10u;
    if ((p == 0u) && (digitStarted == FALSE))
    {
        c[1] = (uint8_t)' ';
    }
    else
    {
        c[1] = (uint8_t)(p + (uint16_t)'0');
    }

    c[0] = (uint8_t)((k % 10u) + (uint16_t)'0');

    OutDev0(c[4]);
    OutDev0(c[3]);
    OutDev0(c[2]);
    OutDev0(c[1]);
    OutDev0(c[0]);
    OutDev0((uint8_t)',');
}

void put_cr(void)				
{							
	OutDev0('\r');	
	OutDev0('\n');	
}

void put_char(unsigned char a)				
{							
	unsigned char i;
	i=a>>4; i&=0x0F;	OutDev0(D2C(i));	
	i=a&0x0F;	OutDev0(D2C(i));	
	OutDev0(',');	
}
/** ==========================================================================

========================================================================== */
void put_char_dec(uint8_t a)
{
    uint8_t c[3];
    boolean_t digitStarted = FALSE;
    uint16_t k = (uint16_t)a;
    uint16_t p;

    p = k / 100u;

    if ((p == 0u))
    {
        c[2] = (uint8_t)' ';
    }
    else
    {
        c[2] = (uint8_t)(p + (uint16_t)'0');
        digitStarted = TRUE;
    }

    k %= 100u;
    p = k / 10u;

    if ((p == 0u) && (digitStarted == FALSE))
    {
        c[1] = (uint8_t)' ';
    }
    else
    {
        c[1] = (uint8_t)(p + (uint16_t)'0');
    }

    c[0] = (uint8_t)((k % 10u) + (uint16_t)'0');

    OutDev0(c[2]);
    OutDev0(c[1]);
    OutDev0(c[0]);
    OutDev0((uint8_t)',');
}

void put_long(uint32_t w) 
{
	unsigned char c;
	c=w>>28;c&=0x0F;OutDev0(D2C(c));
	c=w>>24;c&=0x0F;OutDev0(D2C(c));
	c=w>>20;c&=0x0F;OutDev0(D2C(c));
	c=w>>16;c&=0x0F;OutDev0(D2C(c));
	c=w>>12;c&=0x0F;OutDev0(D2C(c));
	c=w>>8;c&=0x0F;OutDev0(D2C(c));
	c=w>>4;c&=0x0F;OutDev0(D2C(c));
	c=w;c&=0x0F;OutDev0(D2C(c));
	OutDev0(',');

}

