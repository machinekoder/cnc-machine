/*
 *  uC/OS-III - Getting Started Demo Application
 *
 **/

#include "app.h"
#include "taskStart.h"

/*
************************************************************************************************
*                                            LOCAL VARIABLES
************************************************************************************************
*/


/*
************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code. It is assumed that your code will 
*               call main() once you have performed all necessary initialization.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : Startup Code.
*
* Note(s)     : none.
************************************************************************************************
*/

int 
main (void)
{
    OS_ERR   os_err;
#if (CPU_CFG_NAME_EN == DEF_ENABLED)
    CPU_ERR  cpu_err;
#endif

    BSP_PreInit();                                 /* initialize basic board support routines */

#if (CPU_CFG_NAME_EN == DEF_ENABLED)
    CPU_NameSet((CPU_CHAR *)CSP_DEV_NAME,
                (CPU_ERR  *)&cpu_err);
#endif

    Mem_Init();                                       /* Initialize memory management  module */

    OSInit(&os_err);                                                        /* Init uC/OS-III */
    if(os_err != OS_ERR_NONE)
      for(;;);


    //  Dac_initialize();                             // Init DAC
    //DAC_Init(LPC_DAC);

    OSSemCreate(&UartSem, "UART Semaphore", 1, &os_err);
    OSSemCreate(&DacSem, "DAC Semaphore", 1, &os_err);
    OSSemCreate(&ButtonSem, "Button Semaphore", 0, &os_err);

    OSMemCreate((OS_MEM     *)&OutputMemory,
                (CPU_CHAR   *)"OutputMemory",
                (void       *)&OutputMemoryStorage[0][0],
                (OS_MEM_QTY  ) 10,
                (OS_MEM_SIZE ) OUTPUT_MEMORY_SIZE,
                (OS_ERR     *) &os_err);
    
    OSMemCreate((OS_MEM     *)&ServiceRequestMemory,
                (CPU_CHAR   *)"ServiceRequestMemory",
                (void       *)&ServiceRequestMemoryStorage[0],
                (OS_MEM_QTY  ) 20,
                (OS_MEM_SIZE ) sizeof(App_TaskMachine_ServiceRequest),
                (OS_ERR     *) &os_err);
    
    OSMemCreate((OS_MEM     *)&RawMaterialMemory,
                (CPU_CHAR   *)"RawMaterialMemory",
                (void       *)&RawMaterialMemoryStorage[0],
                (OS_MEM_QTY  ) 20,
                (OS_MEM_SIZE ) sizeof(uint32),
                (OS_ERR     *) &os_err);

    OSTaskCreate((OS_TCB      *)&App_TaskStartTCB,                  /* Create the Start Task */
                 (CPU_CHAR    *)"Start",
                 (OS_TASK_PTR  )App_TaskStart,
                 (void        *)0,
                 (OS_PRIO      )5,                          
                 (CPU_STK     *)App_TaskStartStk,
                 (CPU_STK_SIZE )APP_CFG_TASK_START_STK_SIZE_LIMIT,
                 (CPU_STK_SIZE )APP_CFG_TASK_START_STK_SIZE,
                 (OS_MSG_QTY   )0u,
                 (OS_TICK      )0u,
                 (void        *)0,
                 (OS_OPT       )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR      *)&os_err);

    OSStart(&os_err);                                                   /* Start Multitasking */
    if(os_err != OS_ERR_NONE)                                         /* shall never get here */
          for(;;);
    return (0);
}

void DAC_WriteValue(uint32 dac_value)
{
    OS_ERR		err;
    CPU_TS		ts;
    /*******************************************************************************************
    *                                                  PEND ON SEMAPHORE
    *
    * Description: This function waits for a semaphore.
    *
    * Arguments  : p_sem         is a pointer to the semaphore
    *
    *              timeout       is an optional timeout period (in clock ticks).  If non-zero,
    *                            your task will wait for the resource up to the amount of time
    *                            (in 'ticks') specified by this argument.  If you specify 0,
    *                            however, your task will wait forever at the specified semaphore
    *                            or, until the resource becomes available (or the event occurs).
    *
    *              opt           determines whether the user wants to block if the semaphore is
    *              				 not available or not:
    *
    *                            OS_OPT_PEND_BLOCKING
    *                            OS_OPT_PEND_NON_BLOCKING
    *
    *              p_ts          is a pointer to a variable that will receive the timestamp of
    *              			     when the semaphore was posted or pend aborted or the semaphore
    *              			     deleted.  If you pass a NULL pointer (i.e. (CPU_TS*)0) then you
    *              			     will not get the timestamp.  In other words, passing a
    *              			     NULL pointer is valid and indicates that you don't need the
    *              			     timestamp.
    *
    *              p_err         is a pointer to a variable that will contain an error code
    *              				 returned by this function.
    *
    *                            OS_ERR_NONE               The call was successful and your task
    *                            						   owns the resource or, the event you
    *                            						   are waiting for occurred.
    *                            OS_ERR_OBJ_DEL            If 'p_sem' was deleted
    *                            OS_ERR_OBJ_PTR_NULL       If 'p_sem' is a NULL pointer.
    *                            OS_ERR_OBJ_TYPE           If 'p_sem' is not pointing at a
    *                            						   semaphore
    *                            OS_ERR_OPT_INVALID        If you specified an invalid value
    *                            						   for 'opt'
    *                            OS_ERR_PEND_ABORT         If the pend was aborted by another
    *                            						   task
    *                            OS_ERR_PEND_ISR           If you called this function from an
    *                            						   ISR and the result would lead to a
    *                            						   suspension.
    *                            OS_ERR_PEND_WOULD_BLOCK   If you specified non-blocking but
    *                            						   the semaphore was not available.
    *                            OS_ERR_SCHED_LOCKED       If you called this function when the
    *                            						   scheduler is locked
    *                            OS_ERR_STATUS_INVALID     Pend status is invalid
    *                            OS_ERR_TIMEOUT            The semaphore was not received within
    *                            						   the specified timeout.
    *
    *
    * Returns    : The current value of the semaphore counter or 0 if not available.
    ********************************************************************************************
    */

    OSSemPend(&DacSem,
              10,
              OS_OPT_PEND_BLOCKING,
              &ts,
              &err);

    DAC_UpdateValue(LPC_DAC,dac_value);


    /*******************************************************************************************
    *                                     POST TO A SEMAPHORE
    *
    * Description: This function signals a semaphore
    *
    * Arguments  : p_sem    is a pointer to the semaphore
    *
    *              opt      determines the type of POST performed:
    *
    *                       OS_OPT_POST_1            POST and ready only the highest priority
    *                       						 task waiting on semaphore
    *                                                (if tasks are waiting).
    *                       OS_OPT_POST_ALL          POST to ALL tasks that are waiting on the
    *                       						 semaphore
    *
    *                       OS_OPT_POST_NO_SCHED     Do not call the scheduler
    *
    *                       Note(s): 1) OS_OPT_POST_NO_SCHED can be added with one of the other
    *                       options.
    *
    *              p_err    is a pointer to a variable that will contain an error code returned
    *              			by this function.
    *
    *                       OS_ERR_NONE          The call was successful and the semaphore was
    *                       				     signaled.
    *                       OS_ERR_OBJ_PTR_NULL  If 'p_sem' is a NULL pointer.
    *                       OS_ERR_OBJ_TYPE      If 'p_sem' is not pointing at a semaphore
    *                       OS_ERR_SEM_OVF       If the post would cause the semaphore count
    *                       			         to overflow.
    *
    * Returns    : The current value of the semaphore counter or 0 upon error.
    ********************************************************************************************
    */

    OSSemPost(&DacSem,
              OS_OPT_POST_ALL,
              &err);
}

/*! EOF */
