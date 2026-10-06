#ifdef uds_c

#include "def_lin.h"
//void uds_state_check(void);
//void uds_frame_parser(void);
void erase_app_flash(void);
void prog_test_flash(void);
//void uds_Init(void);
void print_bp(void);

#else 

//extern void uds_state_check(void);
//extern void uds_frame_parser(void);
extern void erase_app_flash(void);
extern void prog_test_flash(void);
//extern void uds_Init(void);
extern void print_bp(void);
//extern void LIN_IRQHandler (uint8_t c);
extern void disable_all_interrupt(void);

extern T_UDS_ uds;

#endif 

