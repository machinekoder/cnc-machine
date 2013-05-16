/*
 *  uC/OS-III - Getting Started Demo Application
 *
 **/
#include "app.h"
#include "Libraries/uC-CSP/csp.h"



/*
************************************************************************************************
*                                            LOCAL VARIABLES
************************************************************************************************
*/
static OS_TCB App_TaskStartTCB;               /* Application Startup Task Control Block (TCB) */
static OS_TCB App_ButtonTCB;
static OS_TCB App_MotorSteuerungTCB;

static CPU_STK App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE];             /* Start Task Stack */
static CPU_STK App_ButtonStk[APP_STACK_SIZE];
static CPU_STK App_MotorSteuerungStk[APP_STACK_SIZE];


ButtonValue cncButtons[20]; // TODO
int8 testValue = 2;
uint8 testPort = 2;
uint8 testPin = 2;

/*
************************************************************************************************
*                                         FUNCTION PROTOTYPES
************************************************************************************************
*/
static void App_TaskStart (void  *p_arg);
static void App_Button (void *p_arg);
static void App_MotorSteuerung (void *p_arg);

void buttonInit();


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
#include "Libraries/uC-CSP/csp.h"


uint32 stepsX;
uint32 stepsY;
uint32 stepsZ;
uint32 mySteps;
uint32 endx_p=0;
uint32 endx_m=0;
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
   
     cnc_initialize();
     buttonInit();
    // DAC_Init(LPC_DAC);
     //  Led_initialize(1,29, Led>_LowActive_Yes);
     CSP_TmrInit();
     CSP_TmrCfg (CSP_TMR_NBR_00,50000u);
     CSP_TmrCfg (CSP_TMR_NBR_01,40000u);
     CSP_TmrCfg (CSP_TMR_NBR_02,40000u);
     
    if(os_err != OS_ERR_NONE)
      for(;;);

    OSTaskCreate((OS_TCB      *)&App_TaskStartTCB,                  /* Create the Start Task */
                 (CPU_CHAR    *)"Start",
                 (OS_TASK_PTR  )App_TaskStart,
                 (void        *)0,
                 (OS_PRIO      )4,                          
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

static  void App_TaskStart (void *p_arg)
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
  
   /* Enable Timer0 Interrupt.*/
CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN,
                   (CSP_DEV_NBR   )CSP_INT_SRC_NBR_TMR_00,
                   (CPU_FNCT_PTR  )App_TMR0_IntHandler,
                   (void         *)0);
CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_TMR_00);  

   /* Enable Timer1 Interrupt.*/
CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN,
                   (CSP_DEV_NBR   )CSP_INT_SRC_NBR_TMR_01,
                   (CPU_FNCT_PTR  )App_TMR1_IntHandler,
                   (void         *)0);
CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_TMR_01);

   /* Enable Timer2 Interrupt.
CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN,
                   (CSP_DEV_NBR   )CSP_INT_SRC_NBR_TMR_02,
                   (CPU_FNCT_PTR  )App_TMR2_IntHandler,
                   (void         *)0);
CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_TMR_02);
*/

  OSTaskCreate((OS_TCB     *)&App_ButtonTCB,
               (CPU_CHAR   *)"LED",
               (OS_TASK_PTR )App_Button,
               (void       *)0,
               (OS_PRIO     )2,
               (CPU_STK    *)App_ButtonStk,
               (CPU_STK_SIZE)0,
               (CPU_STK_SIZE)APP_STACK_SIZE,
               (OS_MSG_QTY  )0,
               (OS_TICK     )0,
               (void       *)0,
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR     *)&err);
  
    OSTaskCreate((OS_TCB     *)&App_MotorSteuerungTCB,
               (CPU_CHAR   *)"MotorSteuerung",
               (OS_TASK_PTR )App_MotorSteuerung,
               (void       *)0,
               (OS_PRIO     )2,
               (CPU_STK    *)App_MotorSteuerungStk,
               (CPU_STK_SIZE)0,
               (CPU_STK_SIZE)APP_STACK_SIZE,
               (OS_MSG_QTY  )0,
               (OS_TICK     )0,
               (void       *)0,
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR     *)&err);

  while (DEF_TRUE) {
    OSTimeDlyHMSM(0u, 0u, 1u, 0u, OS_OPT_TIME_HMSM_STRICT, &err);
  }
}

static void App_Button (void *p_arg) 
{
  OS_ERR       err;
    CPU_INT08U count;
    CPU_INT08U i;
  ButtonValue value;
  (void)p_arg;                                                    /* Prevent Compiler Warning */
  
  while(DEF_TRUE) {
    for(count=0;count<=10;count++)
    {
      //Button_task();  
        testValue = Gpio_read(testPort,testPin);
      OSTimeDlyHMSM(0u, 0u, 0u, 10u, OS_OPT_TIME_HMSM_STRICT, &err);
      if(count==10)
      {
          //for(i=0;i<=20;i++)
          //{
              //if (Button_getPress(&value) != -1)
              //{
              //  cncButtons[0] = value;
              //}
              //i = i;
          //}
      }
    }
  }
}

static void App_MotorSteuerung (void *p_arg) 
{
OS_ERR       err;
  

  (void)p_arg;                                                    /* Prevent Compiler Warning */
 
 for(;;) 
 {
 //  setXDirection(-3000);
    setXDirection(-1000);
    OSTimeDlyHMSM(0u, 0u, 1u, 0u, OS_OPT_TIME_HMSM_STRICT, &err);
 //   setXDirection(1000);
    setXDirection(3000);
    OSTimeDlyHMSM(0u, 0u, 1u, 0u, OS_OPT_TIME_HMSM_STRICT, &err);   
 }
 
}

void moveXDirection ()
{         
  
  
  endx_m = Gpio_read(0,9);
  endx_p = CSP_GPIO_Rd(0) & !(1<<9);
  if ((stepsX > 0) && (endx_m == 1))
  {
    stepsX--;
    Gpio_toggle(0,11);        //CLK X
  }
  else
  {
      CSP_TmrStop(CSP_TMR_NBR_00);
  }

}

void moveYDirection ()
{
  stepsY--;
  Gpio_toggle(1,23);       //clk Y
  
   if ((stepsX > 0) && !(Gpio_read(2,6) || Gpio_read(2,8)))
  {
    CSP_TmrStop(CSP_TMR_NBR_01);
  }
}

bool setXDirection (int32 stepsX_local)
{               
/*
    bool running = Timer_running(Timer0);

    if (running) 
    {
      return FALSE;
    }
  */  
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
    
    CSP_TmrStart(CSP_TMR_NBR_00);
    
    return TRUE;
}

bool setYDirection (int32 stepsY_local)
{
  /*  
  if (Timer_running(Timer1)) 
    {
      return FALSE;
    }
    */    
    if (stepsY_local == 0)
    {
      return FALSE;
    }
    
    stepsY = abs(stepsY_local) * 2; // one cock has rising and falling edge
 
    if (stepsY_local < 0)
    {
      Gpio_set(1,20); // directionY
    }
    else
    {
      Gpio_clear(1,20); // directionY
    }
    
    CSP_TmrStart(CSP_TMR_NBR_01); 
    
    return TRUE;
}

bool cncCalibrateZentool (uint32 steps, int16 difference)
{
  /*
  int32 val;
  val=steps+difference;
  
   moveYDirection(1000);
   moveXDirection(steps);
   moveXDirection(val);
   moveXDirection(1000);
   
   return TRUE;
*/
  return TRUE;
}

void buttonInit ()
{
        //+++++++++++++++++++++++++++++++++++++++++TASTER++++++++++++++++++++++++++++++++++++++++++++++++++
    //Taster x+
    Button_initializeButton(1,2,1,ButtonTypeLowActive);

    //Taster x-
    Button_initializeButton(2,2,2,ButtonTypeLowActive);

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
#define ENDSCHALTER_XP  

}

static void App_TMR0_IntHandler (void *p_arg)
{
  
  moveXDirection();
  
  CSP_TmrIntClr(CSP_TMR_NBR_00);		/* Clear TMR0 interrupt.								*/
  CPU_IntSrcEn(CPU_INT_SYSTICK);     /* Enable the SYSTICK interrupt. Resume OS operation.   */
}

static void App_TMR1_IntHandler (void *p_arg)
{
  
  moveYDirection();
  
  CSP_TmrIntClr(CSP_TMR_NBR_01);		/* Clear TMR0 interrupt.								*/
  CPU_IntSrcEn(CPU_INT_SYSTICK);     /* Enable the SYSTICK interrupt. Resume OS operation.   */
}
/*! EOF */
