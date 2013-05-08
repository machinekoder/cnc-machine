/*
 *  uC/OS-III - Getting Started Demo Application
 *
 **/




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

#include "app.h"

int32 stepsX;
int32 stepsY;
int32 stepsZ;
int32 mySteps;

int 
main (void)
{
//    OS_ERR   os_err;
//#if (CPU_CFG_NAME_EN == DEF_ENABLED)
//    CPU_ERR  cpu_err;
//#endif

//    BSP_PreInit();                                 /* initialize basic board support routines */

//#if (CPU_CFG_NAME_EN == DEF_ENABLED)
//    CPU_NameSet((CPU_CHAR *)CSP_DEV_NAME,
//                (CPU_ERR  *)&cpu_err);
//#endif

    //Mem_Init();                                       /* Initialize memory management  module */

    //OSInit(&os_err);                                                        /* Init uC/OS-III */
    //if(os_err != OS_ERR_NONE)
    //  for(;;);


    //  Dac_initialize();                             // Init DAC
    //DAC_Init(LPC_DAC);


    //OSSemCreate(&UartSem, "UART Semaphore", 1, &os_err);
    //OSSemCreate(&DacSem, "DAC Semaphore", 1, &os_err);
    //OSSemCreate(&ButtonSem, "Button Semaphore", 0, &os_err);

    /*OSMemCreate((OS_MEM     *)&OutputMemory,
                (CPU_CHAR   *)"OutputMemory",
                (void       *)&OutputMemoryStorage[0][0],
                (OS_MEM_QTY  ) 10,
                (OS_MEM_SIZE ) OUTPUT_MEMORY_SIZE,
                (OS_ERR     *) &os_err);     */
 

    //OSStart(&os_err);                                                   /* Start Multitasking */
    //if(os_err != OS_ERR_NONE)                                         /* shall never get here */
    //      for(;;);
          
    //Timer_initialize(Timer0, 270, 3);
    CPU_SR_ALLOC();
    Led_initialize(1,29, Led_LowActive_Yes);
    
    CPU_CRITICAL_ENTER();
    Timer_initialize(Timer0, 250, 30);
    Timer_connectFunction(Timer0, moveXDirection);
    Timer_initialize(Timer1, 250, 30);
    Timer_connectFunction(Timer1, moveYDirection);
    CPU_CRITICAL_EXIT();
    
    cnc_initialize();
    
    mySteps = -2000;
    
    
    for (;;)
    { 
          //setXDirection (2000);
          //setYDirection (mySteps);
          setXDirection (mySteps);
    }

    return 0 ;
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

    
void moveXDirection ()
{                  
  stepsX--;
  Gpio_toggle(0,11);        //CLK X
  
  if (stepsX == 0)
  {
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    Timer_stop(Timer0); 
    CPU_CRITICAL_EXIT();
  }

}

void moveYDirection ()
{
  stepsY--;
  Gpio_toggle(1,23);       //clk Y
  
  if (stepsY == 0)
  {
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    Timer_stop(Timer1); 
    CPU_CRITICAL_EXIT();
  }
}

bool setXDirection (int32 stepsX_local)
{               
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    bool running = Timer_running(Timer0);
    CPU_CRITICAL_EXIT();
    if (running) 
    {
      return FALSE;
    }
    
    if (stepsX_local == 0)
    {
      return FALSE;
    }
    
    stepsX = abs(stepsX_local) * 2; // one cock has rising and falling edge
 
    if (stepsX_local > 0)
    {
      Gpio_set(0,10);       // directionX
    }
    else
    {
      Gpio_clear(0,10);     // directionX
    }
    
    CPU_CRITICAL_ENTER();
    Timer_start(Timer0); 
    CPU_CRITICAL_EXIT();
    
    return TRUE;
}

bool setYDirection (int32 stepsY_local)
{
    if (Timer_running(Timer1)) 
    {
      return FALSE;
    }
        
    if (stepsY_local == 0)
    {
      return FALSE;
    }
    
    stepsY = abs(stepsY_local) * 2; // one cock has rising and falling edge
 
    if (stepsY_local > 0)
    {
      Gpio_set(1,20); // directionY
    }
    else
    {
      Gpio_clear(1,20); // directionY
    }
    
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    Timer_start(Timer1); 
    CPU_CRITICAL_EXIT(); 
    
    return TRUE;
}

bool cncCalibrateZentool (uint32 steps, int16 difference)
{
  int32 val;
  val=steps+difference;
  
   moveYDirection(1000);
   moveXDirection(steps);
   moveXDirection(val);
   moveXDirection(1000);
   
   return TRUE;
}

void buttonInit ()
{
        //+++++++++++++++++++++++++++++++++++++++++TASTER++++++++++++++++++++++++++++++++++++++++++++++++++
    //Taster x+
    Button_initializeButton(1,2,1,ButtonTypeLowActive);

    //Taster x-
    Button_initializeButton(2,2,1,ButtonTypeLowActive);

    //Taster y+
    Button_initializeButton(3,2,3,ButtonTypeLowActive);

    //Taster y-
    Button_initializeButton(4,2,4,ButtonTypeLowActive);

    //Taster z+
    Button_initializeButton(5,0,26,ButtonTypeLowActive);

    //Taster z-
    Button_initializeButton(6,2,7,ButtonTypeLowActive);

    //Taster OK
    Button_initializeButton(7,2,0,ButtonTypeLowActive);

        //+++++++++++++++++++++++++++++++++++++++++ENDSCHALTER++++++++++++++++++++++++++++++++++++++++++++++++++
    //Endschalter x+
    Button_initializeButton(8,0,8,ButtonTypeLowActive);

    //Endschalter x-
    Button_initializeButton(9,0,9,ButtonTypeLowActive);

    //Endschalter y+
    Button_initializeButton(10,2,6,ButtonTypeLowActive);

    //Endschalter y-
    Button_initializeButton(11,2,8,ButtonTypeLowActive);

    //Endschalter z+
    Button_initializeButton(12,0,17,ButtonTypeLowActive);

    //Endschalter z-
    Button_initializeButton(13,0,22,ButtonTypeLowActive);

}

/*! EOF */
