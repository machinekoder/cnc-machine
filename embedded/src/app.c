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

   /* Enable Timer2 Interrupt.*/
CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN,
                   (CSP_DEV_NBR   )CSP_INT_SRC_NBR_TMR_02,
                   (CPU_FNCT_PTR  )App_TMR2_IntHandler,
                   (void         *)0);
CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_TMR_02);


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
      Button_task();  
        testValue = Gpio_read(testPort,testPin);
      OSTimeDlyHMSM(0u, 0u, 0u, 10u, OS_OPT_TIME_HMSM_STRICT, &err);
      if(count==10)
      {
          //for(i=0;i<=20;i++)
          //{
              if (Button_getPress(&value) != -1)
              {
                if (value.id == BUTTON_Xplus)
                {
                    setXDirection(value.count * 5000);
                }
                else if (value.id == BUTTON_Xminus)
                {
                    setXDirection(-value.count * 5000);
                }
                if (value.id == BUTTON_Yplus)
                {
                    setYDirection(value.count * 5000);
                }
                else if (value.id == BUTTON_Yminus)
                {
                    setYDirection(-value.count * 5000);
                } 
                if (value.id == BUTTON_Zplus)
                {
                    setZDirection(value.count * 5000);
                }
                else if (value.id == BUTTON_Zminus)
                {
                    setZDirection(-value.count * 5000);
                }  
                //cncButtons[0] = value;
              }
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
   // setXDirection(20000);
   // setYDirection(20000);
    OSTimeDlyHMSM(0u, 0u, 3u, 0u, OS_OPT_TIME_HMSM_STRICT, &err);
   // setXDirection(-20000);
   // setYDirection(-20000);
   // OSTimeDlyHMSM(0u, 0u, 3u, 0u, OS_OPT_TIME_HMSM_STRICT, &err);  
 }
 
}

void moveXDirection ()
{         
  uint8 value_p;
  uint8 value_m;
  
  value_p = (stepsX > 0) && !(Button_read(ENDSCHALTER_Xplus));
  value_m = (stepsX < 0) && !(Button_read(ENDSCHALTER_Xminus));
  
  if (value_p)
  {
    stepsX--;
    Gpio_toggle(MOTOR_X_CLK_PORT,MOTOR_X_CLK_PIN);        //CLK X
  }
  else if(value_m)
  {
      stepsX++;
      Gpio_toggle(MOTOR_X_CLK_PORT,MOTOR_X_CLK_PIN);
  }
  else
  {
      CSP_TmrStop(CSP_TMR_NBR_00);
  }

}

void moveYDirection ()
{
  uint8 value_p;
  uint8 value_m;
  
  value_p = (stepsY > 0) && !(Button_read(ENDSCHALTER_Yplus));
  value_m = (stepsY < 0) && !(Button_read(ENDSCHALTER_Yminus));
  
  if (value_p)
  {
    stepsY--;
    Gpio_toggle(MOTOR_Y_CLK_PORT,MOTOR_Y_CLK_PIN);        //CLK X
  }
  else if(value_m)
  {
      stepsY++;
      Gpio_toggle(MOTOR_Y_CLK_PORT,MOTOR_Y_CLK_PIN);
  }
  else
  {
      CSP_TmrStop(CSP_TMR_NBR_01);
      
  }
}

void moveZDirection ()
{
  uint8 value_p;
  uint8 value_m;
  
  value_p = (stepsZ > 0) && !(Button_read(ENDSCHALTER_Zplus));
  value_m = (stepsZ < 0) && !(Button_read(ENDSCHALTER_Zminus));
  
  if (value_p)
  {
    stepsZ--;
    Gpio_toggle(MOTOR_Z_CLK_PORT,MOTOR_Z_CLK_PIN);        //CLK X
  }
  else if(value_m)
  {
      stepsZ++;
      Gpio_toggle(MOTOR_Y_CLK_PORT,MOTOR_Y_CLK_PIN);
  }
  else
  {
      CSP_TmrStop(CSP_TMR_NBR_02);
      
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
    
    stepsX = stepsX_local * 2; // one cock has rising and falling edge
 
    if (stepsX_local > 0)
    {
      Gpio_set(MOTOR_X_DIR_PORT,MOTOR_X_DIR_PIN);       // directionX
    }
    else
    {
      Gpio_clear(MOTOR_X_DIR_PORT,MOTOR_X_DIR_PIN);     // directionX
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
    
    stepsY = stepsY_local * 2; // one cock has rising and falling edge
 
    if (stepsY_local > 0)
    {
      Gpio_set(MOTOR_Y_DIR_PORT,MOTOR_Y_DIR_PIN); // directionY
    }
    else
    {
      Gpio_clear(MOTOR_Y_DIR_PORT,MOTOR_Y_DIR_PIN); // directionY
    }
    
    CSP_TmrStart(CSP_TMR_NBR_01); 
    
    return TRUE;
}

bool setZDirection (int32 stepsZ_local)
{
  /*  
  if (Timer_running(Timer1)) 
    {
      return FALSE;
    }
    */    
    if (stepsZ_local == 0)
    {
      return FALSE;
    }
    
    stepsY = stepsZ_local * 2; // one cock has rising and falling edge
 
    if (stepsZ_local < 0)
    {
      Gpio_set(MOTOR_Z_DIR_PORT,MOTOR_Z_DIR_PIN); // directionY
    }
    else
    {
      Gpio_clear(MOTOR_Z_DIR_PORT,MOTOR_Z_DIR_PIN); // directionY
    }
    
    CSP_TmrStart(CSP_TMR_NBR_02); 
    
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
    Button_initialize2(10000, 30000);
        //+++++++++++++++++++++++++++++++++++++++++TASTER++++++++++++++++++++++++++++++++++++++++++++++++++
    //Taster x+
    Button_initializeButton(BUTTON_Xplus, TASTER_Xplus_PORT, TASTER_Xplus_PIN, ButtonTypeLowActive);
    Button_initializeButton(BUTTON_Xminus, TASTER_Xmius_PORT, TASTER_Xmius_PIN, ButtonTypeLowActive);
    Button_initializeButton(BUTTON_Yplus, TASTER_Yplus_PORT, TASTER_Yplus_PIN, ButtonTypeLowActive);
    Button_initializeButton(BUTTON_Yminus, TASTER_Yminus_PORT, TASTER_Yminus_PIN, ButtonTypeLowActive);
    Button_initializeButton(BUTTON_Zplus, TASTER_Zplus_PORT, TASTER_Zplus_PIN, ButtonTypeLowActive);
    Button_initializeButton(BUTTON_Zminus, TASTER_Zminus_PORT, TASTER_Zminus_PIN, ButtonTypeLowActive);
    Button_initializeButton(BUTTON_OK,TASTER_OK_PORT,TASTER_OK_PIN, ButtonTypeLowActive);
    Button_initializeButton(ENDSCHALTER_Xplus, ENDSCHALTER_Xplus_PORT, ENDSCHALTER_Xplus_PIN, ButtonTypeLowActive);
    Button_initializeButton(ENDSCHALTER_Xminus, ENDSCHALTER_Xmius_PORT,  ENDSCHALTER_Xmius_PIN, ButtonTypeLowActive);
    Button_initializeButton(ENDSCHALTER_Yplus, ENDSCHALTER_Yplus_PORT, ENDSCHALTER_Yplus_PIN, ButtonTypeLowActive);
    Button_initializeButton(ENDSCHALTER_Yminus, ENDSCHALTER_Yminus_PORT,  ENDSCHALTER_Yminus_PIN, ButtonTypeLowActive);
    Button_initializeButton(ENDSCHALTER_Zplus, ENDSCHALTER_Zplus_PORT, ENDSCHALTER_Zplus_PIN, ButtonTypeLowActive);
    Button_initializeButton(ENDSCHALTER_Zminus, ENDSCHALTER_Zminus_PORT,  ENDSCHALTER_Zminus_PIN, ButtonTypeLowActive);
  
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
  
  CSP_TmrIntClr(CSP_TMR_NBR_01);		/* Clear TMR1 interrupt.								*/
  CPU_IntSrcEn(CPU_INT_SYSTICK);     /* Enable the SYSTICK interrupt. Resume OS operation.   */
}

static void App_TMR2_IntHandler (void *p_arg)
{
  
  moveZDirection();
  
  CSP_TmrIntClr(CSP_TMR_NBR_02);                /* Clear TMR2 interrupt.                                                                */
  CPU_IntSrcEn(CPU_INT_SYSTICK);     /* Enable the SYSTICK interrupt. Resume OS operation.   */
}
/*! EOF */
