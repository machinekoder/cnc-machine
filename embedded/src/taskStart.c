#include "app.h"
#include "taskStart.h"
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

    (void)p_arg;                                                    /* Prevent Compiler Warning */
    CPU_Init();                                               /* Initialize the uC/CPU Services */
    OS_CSP_TickInit();                                        /* Initialize the Tick Interrupt. */


#if (OS_CFG_STAT_TASK_EN > 0u)
    OSStatTaskCPUUsageInit(&err);
#endif


#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    OSTaskCreate((OS_TCB     *)&App_TaskButtonTCB,
                 (CPU_CHAR   *)"Button",
                 (OS_TASK_PTR )App_TaskButton,
                 (void       *)0,
                 (OS_PRIO     )4,
                 (CPU_STK    *)App_TaskButtonStk,
                 (CPU_STK_SIZE)0,
                 (CPU_STK_SIZE)APP_STACK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&App_TaskMotorSteuerungTCB,
                 (CPU_CHAR   *)"MotorSteuerung",
                 (OS_TASK_PTR )App_TaskMotorSteuerung,
                 (void       *)0,
                 (OS_PRIO     )2,
                 (CPU_STK    *)App_TaskMotorSteuerungStk,
                 (CPU_STK_SIZE)0,
                 (CPU_STK_SIZE)APP_STACK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&App_TaskUsbConnectionTCB,
                 (CPU_CHAR   *)"UsbConnection",
                 (OS_TASK_PTR )App_TaskUsbConnection,
                 (void       *)0,
                 (OS_PRIO     )3,
                 (CPU_STK    *)App_TaskUsbConnectionStk,
                 (CPU_STK_SIZE)0,
                 (CPU_STK_SIZE)APP_STACK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
    OSTaskCreate((OS_TCB     *)&App_TaskLedTCB,
                 (CPU_CHAR   *)"Led",
                 (OS_TASK_PTR )App_TaskLed,
                 (void       *)0,
                 (OS_PRIO     )10,
                 (CPU_STK    *)App_TaskLedStk,
                 (CPU_STK_SIZE)0,
                 (CPU_STK_SIZE)APP_LED_STACK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    while (DEF_TRUE) {
        OSTimeDlyHMSM(0u, 0u, 10u, 0u,OS_OPT_TIME_HMSM_STRICT,&err);
    }
}
