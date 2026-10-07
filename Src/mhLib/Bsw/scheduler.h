/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : schduler.h
* Project         : iEWP
* Created         : 2006.05.24
* Description     : 
* 
* This code is distributed under the GNU Public License
*	which can be found at http://www.gnu.org/licenses/gpl.txt
*
************************************************************************************************************************
* Revison History:
* YYYY/MM/DD             Version              Description                      Author
* 2006/05/24             v1.0                 Draft first version              Pascal Stang - Copyright (C) 2006
* 2006/05/28             v1.1                 Add priority                     Alex.kim
* 2018/02/08             v2.0                 modify format                    Alex.kim
************************************************************************************************************************
*/

#ifndef SCHEDULER_H
#define SCHEDULER_H

/*
************************************************************************************************************************
*                                                  Include header files
************************************************************************************************************************
*/

/*
************************************************************************************************************************
*                                                   Defines and macros
************************************************************************************************************************
*/
#define TASKLIST_SIZE      (6u)
#define TASKLIST_RANDOM    (6u)
#define INFINITE_LOOP     	(0u)
#define SCH_MAX_TIME       (2000000000u)


#define    Priority_1  0u
#define    Priority_2  1u
#define    Priority_3  2u
#define    Priority_4  3u
#define    Priority_5  4u
#define    Priority_6  5u
//#define    Priority_7  6
//#define    Priority_LOW	   		(TASKLIST_SIZE-1),
#define    Priority_Random			(TASKLIST_RANDOM)


#define SCH_OK 					   (SCH_ReturnType)0
#define SCH_NOT_OK 				(SCH_ReturnType)1

#define CRITICAL_SECTION_BEGIN    
#define CRITICAL_SECTION_END      

/*
************************************************************************************************************************
*                                                         Typedefs
************************************************************************************************************************
*/
typedef void (*taskfuncptr)(void);

struct task
{
    unsigned long runtime;
    unsigned long nrepeat;
    unsigned long interval;
    taskfuncptr funcptr;
};

typedef signed char SCH_ReturnType;


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
void schedulerInit(void);           /* initialize scheduler  */
void schedulerTick( void );       /* increament tick       */
unsigned long schedulerGetTick( void );   /* get system tick */
extern void SystemTimer_ISR( void );


/*
* run scheduler
*	this function should be called at the system clock tick rate
*	the system time must be passed in systime
*/
void schedulerRun(unsigned long systime);

/*
* schedule a task
*	returns a handle to the task (or -1 for failure)
*/
signed char schedulerAddTask( unsigned char Priority, unsigned long runtime, unsigned long nrepeat, unsigned long interval, taskfuncptr taskfunc );


/* 
* remove a scheduled task(s) by function reference
* NOTE: this will remove ALL tasks referencing the specified function
*/

#endif    /* SCHEDULER_H */

