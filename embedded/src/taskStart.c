#include "taskStart.h"
#include "taskMachine.h"
#include "taskSupervisor.h"
#include "taskBlink.h"
#include "taskButton.h"
/*
************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
************************************************************************************************
*/

void App_TaskStart (void *p_arg)
{
  OS_ERR       err;
  int 		   i;

  (void)p_arg;                                                    /* Prevent Compiler Warning */
  CPU_Init();                                               /* Initialize the uC/CPU Services */
  OS_CSP_TickInit();                                        /* Initialize the Tick Interrupt. */

#if (OS_CFG_STAT_TASK_EN > 0u)
  OSStatTaskCPUUsageInit(&err);
#endif


#ifdef CPU_CFG_INT_DIS_MEAS_EN
  CPU_IntDisMeasMaxCurReset();
#endif

    for (i = 0; i < 8; ++i)
    {
        taskId[i] = i;
        OSTaskCreate((OS_TCB     *)&(App_TaskMachineTCB[i]),
                    (CPU_CHAR   *)"Machine",
                    (OS_TASK_PTR )App_TaskMachine,
                    (void       *)(&(taskId[i])),
                    (OS_PRIO     )5,
                    (CPU_STK    *)App_TaskMachineStk[i],
                    (CPU_STK_SIZE)0,
                    (CPU_STK_SIZE)APP_STACK_SIZE,
                    (OS_MSG_QTY  )1,
                    (OS_TICK     )0,
                    (void       *)0,
                    (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                    (OS_ERR     *)&err);
  }   


  OSTaskCreate((OS_TCB     *)&App_TaskSupervisorTCB,
               (CPU_CHAR   *)"Supervisor",
               (OS_TASK_PTR )App_TaskSupervisor,
               (void       *)0,
               (OS_PRIO     )6,
               (CPU_STK    *)App_TaskSupervisorStk,
               (CPU_STK_SIZE)0,
               (CPU_STK_SIZE)APP_STACK_SIZE,
               (OS_MSG_QTY  )10,
               (OS_TICK     )0,
               (void       *)0,
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR     *)&err);   
  
    OSTaskCreate((OS_TCB     *)&App_TaskBlinkTCB,
               (CPU_CHAR   *)"Blink",
               (OS_TASK_PTR )App_TaskBlink,
               (void       *)0,
               (OS_PRIO     )10,
               (CPU_STK    *)App_TaskBlinkStk,
               (CPU_STK_SIZE)0,
               (CPU_STK_SIZE)APP_STACK_SIZE,
               (OS_MSG_QTY  )1,
               (OS_TICK     )0,
               (void       *)0,
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR     *)&err); 
    
      OSTaskCreate((OS_TCB     *)&App_TaskButtonTCB,
               (CPU_CHAR   *)"Button",
               (OS_TASK_PTR )App_TaskButton,
               (void       *)0,
               (OS_PRIO     )6,
               (CPU_STK    *)App_TaskButtonStk,
               (CPU_STK_SIZE)0,
               (CPU_STK_SIZE)APP_STACK_SIZE,
               (OS_MSG_QTY  )0,
               (OS_TICK     )0,
               (void       *)0,
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR     *)&err);   

  while (DEF_TRUE) {
      OSTimeDlyHMSM(0u, 0u, 20u, 0u, OS_OPT_TIME_HMSM_STRICT, &err);
  }
}
