/*
 * usb_app.c
 *
 *  Created on: Apr 24, 2013
 *      Author: Roman
 */

/*
************************************************************************************************
*                                             INCLUDE FILES
************************************************************************************************
*/
#include <includes.h>

/*
************************************************************************************************
*                                             LOCAL DEFINES
************************************************************************************************
*/
#define APP_STACK_SIZE 200

/*
************************************************************************************************
*                                            LOCAL VARIABLES
************************************************************************************************
*/
static OS_TCB App_TaskStartTCB;               /* Application Startup Task Control Block (TCB) */
static OS_TCB App_TaskATCB;

static CPU_STK App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE];             /* Start Task Stack */
static CPU_STK App_TaskAStk[APP_STACK_SIZE];
/*
************************************************************************************************
*                                         FUNCTION PROTOTYPES
************************************************************************************************
*/
static void App_TaskStart (void  *p_arg);
static void App_TaskA (void *p_arg);
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

static  void
App_TaskStart (void *p_arg)
{
  OS_ERR       err;

  (void)p_arg;                                              /* Prevent Compiler Warning */
  CPU_Init();                               		  /* Initialize the uC/CPU Services */
  OS_CSP_TickInit();                                  /* Initialize the Tick Interrupt. */

  USBInit();                               				          /* USB Initialization */

  /* register descriptors */
  USBRegisterDescriptors(abDescriptors);			   /* USB Descriptor Initialization */

  /* register endpoint handlers */
  USBHwRegisterEPIntHandler(BULK_IN_EP, BulkIn);	  /* register BulkIn Handler for EP */
  USBHwRegisterEPIntHandler(BULK_OUT_EP, BulkOut);	 /* register BulkOut Handler for EP */
  USBHwRegisterEPIntHandler(INT_IN_EP, NULL);

  USBHwEPConfig(BULK_IN_EP, MAX_PACKET_SIZE);	/* Configure Packet Size for outgoing Transfer */
  USBHwEPConfig(BULK_OUT_EP, MAX_PACKET_SIZE);  /* Configure Packet Size for incoming Transfer */

  /* enable bulk-in interrupts on NAKs */
  USBHwNakIntEnable(INACK_BI);

  if(CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN,
                 (CSP_DEV_NBR   )CSP_INT_SRC_NBR_USB_00,
                 (CPU_FNCT_PTR  )USB_IRQHandler,
                 (void         *)0) != DEF_OK){
	  while(DEF_TRUE);
  }												/* register Interrupt Handler in RTOS */

  CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_USB_00);   /* Enable USB Interrupt. */

  USBHwConnect(TRUE);                        				  /* USB Connect */

  #if (OS_CFG_STAT_TASK_EN > 0u)
  	  OSStatTaskCPUUsageInit(&err);
  #endif


  #ifdef CPU_CFG_INT_DIS_MEAS_EN
  	  CPU_IntDisMeasMaxCurReset();
  #endif

  OSTaskCreate((OS_TCB     *)&App_TaskATCB,
               (CPU_CHAR   *)"TaskA",
               (OS_TASK_PTR )App_TaskA,
               (void       *)0,
               (OS_PRIO     )5,
               (CPU_STK    *)App_TaskAStk,
               (CPU_STK_SIZE)0,
               (CPU_STK_SIZE)APP_STACK_SIZE,
               (OS_MSG_QTY  )0,
               (OS_TICK     )0,
               (void       *)0,
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR     *)&err);

    GPIO_SetDir(1,(1<<29),1);
    GPIO_ClearValue(1,(1<<29));
    while (DEF_TRUE) {
    	GPIO_SetValue(1,(1<<29));
    	OSTimeDlyHMSM(0u, 0u, 1u, 0u,OS_OPT_TIME_HMSM_STRICT,&err);
    	GPIO_ClearValue(1,(1<<29));
    	OSTimeDlyHMSM(0u, 0u, 1u, 0u,OS_OPT_TIME_HMSM_STRICT,&err);

    }
}

/*
************************************************************************************************
*                                          Application TASK
*
* Description : This is an example of a simple application task that communicates based on USB.
*
* Arguments   : p_arg   is the argument passed to 'App_TaskA()' by 'OSTaskCreate()'.
*
* Returns     : none
*
************************************************************************************************
*/
static void
App_TaskA (void *p_arg)
{
  OS_ERR       err;

  uint8_t str[] = "I'm a LPC1758\n";						/* Setup string for transmitting */

  (void)p_arg;                                              /* Prevent Compiler Warning */
  while(DEF_TRUE) {
	  if(usbReceiveBufferSize > 0){									/* if a Message was received */
		  usbSendBufferSize = strlen((char *)str);					/* calculate string length of outgoing data */
		  usbSendBuffer[0]=0x00ff&((usbSendBufferSize+1)>>8);		/* Highbyte */
		  usbSendBuffer[1]=0x00ff&(usbSendBufferSize+1);			    /* Lowbyte  */
		  sprintf((char *)&usbSendBuffer[2],"%s",str);		/* write data to output buffer */
		  usbSendBufferSize += 3;									/* HB + LB + \0 */
		  usbReceiveBufferSize = 0;									/* reset the Message length of the incoming buffer */
	  }
	  OSTimeDlyHMSM(0u, 0u, 0u, 500u, OS_OPT_TIME_HMSM_STRICT, &err);
  }
}
/*! EOF */
