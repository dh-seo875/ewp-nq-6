/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : Variant_cfg.h
* Project         : 
* Created         : YYYY/MM/DD
* Description     : 
* 
* 
************************************************************************************************************************
* Revison History:
* YYYY/MM/DD             Version              Description                      Author
* 2018/01/09             v1.0                 Draft first version
*
************************************************************************************************************************
*/


#ifndef VARIANT_CFG_H
#define VARIANT_CFG_H
/*
************************************************************************************************************************
*                                                   Defines and macros
************************************************************************************************************************
*/
#define PEWP_TYPE2_0X37                (0)
#define PEWP_TYPE2_0X38                (1)
#define IEWP_TYPE2_0X37                (2)
#define AEWP_TYPE2_0X37                (3)
#define AEWP_TYPE2_0X37_WITHOUTHOTSPOT (4)
#define PEWP_TYPE3_0X37                (5)
#define PEWP_TYPE3_0X38                (6)
#define IEWP_TYPE3_0X37                (7)
#define BEWP_TYPE3_0X37                (8)
#define BEWP_TYPE3_0X38                (0x0Au)

/* #define EWP_TYPE (PEWP_TYPE2_0X37) */
/* #define EWP_TYPE (PEWP_TYPE2_0X38) */
/* #define EWP_TYPE (IEWP_TYPE2_0X37) */
/* #define EWP_TYPE (AEWP_TYPE2_0X37) */
/* #define EWP_TYPE (AEWP_TYPE2_0X37_WITHOUTHOTSPOT) */
/* #define EWP_TYPE (PEWP_TYPE3_0X37) */
/* #define EWP_TYPE (PEWP_TYPE3_0X38) */
/* #define EWP_TYPE (IEWP_TYPE3_0X37) */
/* #define EWP_TYPE (BEWP_TYPE3_0X37) */
 #define EWP_TYPE (BEWP_TYPE3_0X38) 

/* Version information */
/* 1 : DN8 Proto Sample */
/* 2 : DN8 P1 Sample */
/* 3 : DN8 PP Sample */
/* 4 : R-i4 M Sample, lambda3 theta3 M Sample, DN8 PEWP SOP, DN8 AEWP robust Ver.1 , NE EV BEWP1,2 P1 */
/* 5 : R-i4,R-i6 SOP, DN8 AEWP robust Ver.2, lambda3 theta3 SOP, NE EV BEWP2 P2, NE EV BEWP SOP */
/* 6 : NE EV BEWP2 SOP */


#define VERSION (1u)

/*
************************************************************************************************************************
*                                                   Platform Typedef
************************************************************************************************************************
*/


#endif    /* VARIANT_CFG_H */

