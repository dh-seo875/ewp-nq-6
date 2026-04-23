/*
************************************************************************************************************************
*                                                Myunghwa IND. CO., LTD
*
*                                           (c) Copyright 2018; Myunghwa IND.
*                                                  ALL RIGHTS RESERVED.
*
* Filename        : schduler.c
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
* 2018/02/08             v2.0                 Modify format                    Alex.kim
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                                   Include header files
************************************************************************************************************************
*/
#include "scheduler.h"
#include "types.h"

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
static struct task TaskList[TASKLIST_SIZE];
static unsigned long SchedulerTimer = 0;

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
static void schedulerReset( void );	    /* scheduler timer reset */
//static unsigned char schedulerRemoveTask( unsigned char taskhandle);


/*
************************************************************************************************************************
*                                                    Exported functions
************************************************************************************************************************
*/
void SystemTimer_ISR( void )
{
    SchedulerTimer++;
}

void schedulerInit(void)
{
    unsigned char i;
    
    /* reset task list */
    for(i=0; i<TASKLIST_SIZE; i++)
    {
        /* initialize task list */
        TaskList[i].runtime = 0;
        TaskList[i].funcptr = NULL_PTR;
    }
}


static void schedulerReset( void )
{
    unsigned char i;
    
    /* systime init */
    SchedulerTimer = 0;
    /* locate the task */
    for(i=0; i<TASKLIST_SIZE; i++)
    {
        /* on match, reset the task runtime */
        TaskList[i].runtime = 0;
    }
}


unsigned long schedulerGetTick( void )
{
    return SchedulerTimer;
}



signed char schedulerAddTask(unsigned char Priority, unsigned long runtime, unsigned long nrepeat, unsigned long interval, taskfuncptr taskfunc)
{
    unsigned long i;
    signed char returnValue = SCH_NOT_OK;  // 기본값을 SCH_NOT_OK로 설정

    if (Priority >= Priority_Random)
    {
        /* locate empty scheduler slot */
        for (i = 0; i < TASKLIST_SIZE; i++)
        {
            CRITICAL_SECTION_BEGIN;
            if (TaskList[i].funcptr == NULL_PTR)
            {
                /* setup entry */
                TaskList[i].funcptr = taskfunc;
                TaskList[i].runtime = runtime;
                TaskList[i].nrepeat = nrepeat;
                TaskList[i].interval = interval;
                CRITICAL_SECTION_END;
                returnValue = SCH_OK;  // 빈 슬롯을 찾았으면 SCH_OK로 설정
                break;  // 슬롯을 찾았으면 더 이상 반복할 필요 없음
            }
            CRITICAL_SECTION_END;
        }
    }
    else
    {
        CRITICAL_SECTION_BEGIN;
        if (TaskList[Priority].funcptr == NULL_PTR)
        {
            /* setup entry */
            TaskList[Priority].funcptr = taskfunc;
            TaskList[Priority].runtime = runtime;
            TaskList[Priority].nrepeat = nrepeat;
            TaskList[Priority].interval = interval;
            CRITICAL_SECTION_END;
            returnValue = SCH_OK;  // 빈 슬롯을 찾았으면 SCH_OK로 설정
        }
        CRITICAL_SECTION_END;
    }

    return returnValue;  // 함수 끝에서 한 번만 반환
}



//static unsigned char schedulerRemoveTask( unsigned char taskhandle)
//{
//    TaskList[taskhandle].runtime = 0;
//    TaskList[taskhandle].interval = 0;
//    TaskList[taskhandle].nrepeat = 0;
//    TaskList[taskhandle].funcptr = NULL_PTR;
//    return taskhandle;
//}



void schedulerRun(unsigned long systime)
{
    unsigned char i;
    
    /* locate active task slots */
    for(i=0; i<TASKLIST_SIZE; i++)
    {
        if((TaskList[i].funcptr != NULL_PTR))
        {
            /* check systime against runtime */
            if(systime >= TaskList[i].runtime)
            {
                /* run task, then update it */
                TaskList[i].funcptr();
                //if((TaskList[i].nrepeat != 0UL))
                //{
                //    TaskList[i].runtime += TaskList[i].interval;
                //    TaskList[i].nrepeat--;
                    /* task has expired, remove it now */
                //    if(TaskList[i].nrepeat == 0UL){
                //        (void)schedulerRemoveTask(i); 
                //    }
                //}
                //else
                //{
                    /* nrepeat==0 means run forever */
                    TaskList[i].runtime += TaskList[i].interval;
                //}
            
            }
        }
    }
    
    if(SchedulerTimer >= SCH_MAX_TIME){
        schedulerReset();
    }
}

/*
************************************************************************************************************************
*                                                      Local functions
************************************************************************************************************************
*/




