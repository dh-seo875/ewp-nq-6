#ifdef uart_c

void OutDev0(uint8_t send);
void OutStr(const char *text);
void put_int(unsigned int a);			
void put_int_dec(uint16_t a);			
void put_int_dec_sign(int16_t a);				
void put_cr(void)	;			
void put_char_dec(uint8_t a);				
void put_char(unsigned char a);				
void put_long(uint32_t w); 

#else 



extern void goto_reset(void);

extern void OutDev0(uint8_t send);
extern void OutDev0(uint8_t send);
extern void OutStr(const char *text);
extern void put_int(unsigned int a);			
extern void put_int_dec(uint16_t a);			
extern void put_int_dec_sign(int16_t a);				
extern void put_cr(void)	;			
extern void put_char_dec(uint8_t a);				
extern void put_char(unsigned char a);				
extern void put_long(uint32_t w); 



#endif 
