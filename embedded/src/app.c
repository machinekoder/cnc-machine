/*
 *  uC/OS-III - Getting Started Demo Application
 *
 **/
#include "app.h"



#define printfData(x) Debug_printf(Debug_Level_1,x)
#define printfData2(x,y) Debug_printf(Debug_Level_1,x,y)

#define TIMER_FREQ 35000u
#define HOMING_AMOUNT -1E9     //52500
#define CALIBRATION_AMOUNT 100
#define BUTTON_STEP_UM 5000

#define COMMAND_BUFFER_SIZE 200u
#define CNC_MM_COMMAND_BUFFER_SIZE 100u

#define MAX_FEED_RATE 100u
#define MIN_FEED_RATE 10u
#define COMMAND_DELAY 50u

typedef enum {
    ApplicationState_Movement = 0u,
    ApplicationState_Working  = 1u,
    ApplicationState_Test     = 2u,
} ApplicationState;

typedef struct {
    int16 stepsX;
    int16 stepsY;
    int16 stepsZ;
} CommandBufferItem;

/*
************************************************************************************************
*                                            LOCAL VARIABLES
************************************************************************************************
*/
static OS_TCB App_TaskStartTCB;               /* Application Startup Task Control Block (TCB) */
static OS_TCB App_ButtonTCB;
static OS_TCB App_MotorSteuerungTCB;
static OS_TCB App_USB_ConnectionTCB;

static CPU_STK App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE];             /* Start Task Stack */
static CPU_STK App_ButtonStk[APP_STACK_SIZE];
static CPU_STK App_MotorSteuerungStk[APP_STACK_SIZE];
static CPU_STK App_USB_ConnectionStk[APP_STACK_SIZE];

int32 stepsX;
int32 stepsY;
int32 stepsZ;

uint32 xCalibration = 25/10;
uint32 yCalibration = 25/10;
uint32 zCalibration = 25/10;

int32 currentX = 0;     // current X pos in um
int32 currentY = 0;     // current Y pos in um
int32 currentZ = 0;     // current Z pos in um
int32 targetX = 0;     // current X pos in um
int32 targetY = 0;     // current Y pos in um
int32 targetZ = 0;     // current Z pos in um
uint32 currentFeed = 5; // current Feed rate in mm/s

CommandBufferItem cncCommandBufferData[CNC_MM_COMMAND_BUFFER_SIZE];
CircularBuffer cncCommandPuffer;

ApplicationState applicationState = ApplicationState_Movement;

bool buttonXPlusTest  = FALSE;
bool buttonXMinusTest = FALSE;
bool buttonYPlusTest  = FALSE;
bool buttonYMinusTest = FALSE;
bool buttonZPlusTest  = FALSE;
bool buttonZMinusTest = FALSE;
bool buttonOkTest     = FALSE;
bool endschalterXPlusTest  = FALSE;
bool endschalterXMinusTest = FALSE;
bool endschalterYPlusTest  = FALSE;
bool endschalterYMinusTest = FALSE;
bool endschalterZPlusTest  = FALSE;
bool endschalterZMinusTest = FALSE;

// For testing
int8 testValue = 2;
uint8 testPort = 2;
uint8 testPin = 2;
bool testing = FALSE;

/*
************************************************************************************************
*                                         FUNCTION PROTOTYPES
************************************************************************************************
*/
static void App_TMR0_IntHandler(void *p_arg);
static void App_TMR1_IntHandler (void *p_arg);
static void App_TMR2_IntHandler (void *p_arg);

static void App_TaskStart (void  *p_arg);
static void App_Button (void *p_arg);
static void App_MotorSteuerung (void *p_arg);
static void App_USBConnection (void *p_arg);

void commandSplitter(char* data, uint32 length);
void processCommand(char *buffer);

bool getSteps(CommandBufferItem *item);

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


//#include "Libraries/uC-CSP/csp.h"

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

    Debug_printf(Debug_Level_1, "Starting init");

    cnc_initialize();
    buttonInit();
    Led_initialize(1,29,Led_LowActive_Yes); // onboard LED Led1

    // init command buffer
    Cb_initialize(&cncCommandPuffer, CNC_MM_COMMAND_BUFFER_SIZE, sizeof(CommandBufferItem), (void*)&cncCommandBufferData);
    // DAC_Init(LPC_DAC);
    //  Led_initialize(1,29, Led>_LowActive_Yes);
    CSP_TmrInit();
    CSP_TmrCfg (CSP_TMR_NBR_00,TIMER_FREQ);
    CSP_TmrCfg (CSP_TMR_NBR_01,TIMER_FREQ);
    CSP_TmrCfg (CSP_TMR_NBR_02,TIMER_FREQ);


    Debug_printf(Debug_Level_1, "Init finished");

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

    USBInit();                                                                  /* USB Initialization */

    /* register descriptors */
    USBRegisterDescriptors(abDescriptors);                           /* USB Descriptor Initialization */

    /* register endpoint handlers */
    USBHwRegisterEPIntHandler(BULK_IN_EP, BulkIn);          /* register BulkIn Handler for EP */
    USBHwRegisterEPIntHandler(BULK_OUT_EP, BulkOut);       /* register BulkOut Handler for EP */
    USBHwRegisterEPIntHandler(INT_IN_EP, NULL);

    USBHwEPConfig(BULK_IN_EP, MAX_PACKET_SIZE);   /* Configure Packet Size for outgoing Transfer */
    USBHwEPConfig(BULK_OUT_EP, MAX_PACKET_SIZE);  /* Configure Packet Size for incoming Transfer */


    /* enable bulk-in interrupts on NAKs */
    USBHwNakIntEnable(INACK_BI);

    if(CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN,
                      (CSP_DEV_NBR   )CSP_INT_SRC_NBR_USB_00,
                      (CPU_FNCT_PTR  )USB_IRQHandler,
                      (void         *)0) != DEF_OK) {
        while(DEF_TRUE);
    }                                                                                             /* register Interrupt Handler in RTOS */

    CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_USB_00);   /* Enable USB Interrupt. */

    USBHwConnect(TRUE);

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

    OSTaskCreate((OS_TCB     *)&App_USB_ConnectionTCB,
                 (CPU_CHAR   *)"USB_Connection",
                 (OS_TASK_PTR )App_USBConnection,
                 (void       *)0,
                 (OS_PRIO     )2,
                 (CPU_STK    *)App_USB_ConnectionStk,
                 (CPU_STK_SIZE)0,
                 (CPU_STK_SIZE)APP_STACK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    while (DEF_TRUE) {
        Led_set(Led1);
        OSTimeDlyHMSM(0u, 0u, 1u, 0u,OS_OPT_TIME_HMSM_STRICT,&err);
        Led_clear(Led1);
        OSTimeDlyHMSM(0u, 0u, 1u, 0u,OS_OPT_TIME_HMSM_STRICT,&err);



    }
}

static void App_Button (void *p_arg)
{
    OS_ERR       err;
    CPU_INT08U count;
    ButtonValue value;
    (void)p_arg;                                                    /* Prevent Compiler Warning */

    while(DEF_TRUE) {
        for(count=0; count<=10; count++)
        {
            Button_task();        // reads the button values
            //testValue = Gpio_read(testPort,testPin);
            OSTimeDlyHMSM(0u, 0u, 0u, 10u, OS_OPT_TIME_HMSM_STRICT, &err);
            if(count==10)
            {
                while (Button_getPress(&value) != (int8)(-1))
                {
                    if (applicationState == ApplicationState_Movement)
                    {
                        int32 movement;
                        if (value.id == BUTTON_Xplus)
                        {
                            movement = value.count * BUTTON_STEP_UM;
                            setXDirectionUM(movement);
                            Debug_printf(Debug_Level_2, "X: %imm\n", movement);
                        }
                        else if (value.id == BUTTON_Xminus)
                        {
                            movement = -value.count * BUTTON_STEP_UM;
                            setXDirectionUM(movement);
                            Debug_printf(Debug_Level_2, "X: %imm\n", movement);
                        }
                        if (value.id == BUTTON_Yplus)
                        {
                            movement = value.count * BUTTON_STEP_UM;
                            setYDirectionUM(movement);
                            Debug_printf(Debug_Level_2, "Y: %imm\n", movement);
                        }
                        else if (value.id == BUTTON_Yminus)
                        {
                            movement = -value.count * BUTTON_STEP_UM;
                            setYDirectionUM(movement);
                            Debug_printf(Debug_Level_2, "Y: %imm\n", movement);
                        }
                        if (value.id == BUTTON_Zplus)
                        {
                            movement = value.count * BUTTON_STEP_UM;
                            setZDirectionUM(movement);
                            Debug_printf(Debug_Level_2, "Z: %imm\n", movement);
                        }
                        else if (value.id == BUTTON_Zminus)
                        {
                            movement = -value.count * BUTTON_STEP_UM;
                            setZDirectionUM(movement);
                            Debug_printf(Debug_Level_2, "Z: %imm\n", movement);
                        }
                        else if (value.id == BUTTON_OK)
                        {
                            Debug_printf(Debug_Level_2, "OK pressed\n");
                            testing = TRUE;
                        }
                    }
                    else if (applicationState == ApplicationState_Test)
                    {
                        if (value.id == BUTTON_Xplus)
                        {
                            buttonXPlusTest = TRUE;
                            Debug_printf(Debug_Level_2, "X+ pressed\n");
                        }
                        else if (value.id == BUTTON_Xminus)
                        {
                            buttonXMinusTest = TRUE;
                            Debug_printf(Debug_Level_2, "X- pressed\n");
                        }
                        if (value.id == BUTTON_Yplus)
                        {
                            buttonYPlusTest = TRUE;
                            Debug_printf(Debug_Level_2, "Y+ pressed\n");
                        }
                        else if (value.id == BUTTON_Yminus)
                        {
                            buttonYMinusTest = TRUE;
                            Debug_printf(Debug_Level_2, "Y- pressed\n");
                        }
                        if (value.id == BUTTON_Zplus)
                        {
                            buttonZPlusTest = TRUE;
                            Debug_printf(Debug_Level_2, "Z+ pressed\n");
                        }
                        else if (value.id == BUTTON_Zminus)
                        {
                            buttonZMinusTest = TRUE;
                            Debug_printf(Debug_Level_2, "Z- pressed\n");
                        }
                        else if (value.id == BUTTON_OK)
                        {
                            buttonOkTest = TRUE;
                            Debug_printf(Debug_Level_2, "OK pressed\n");
                        }
                        else if (value.id == ENDSCHALTER_Xplus)
                        {
                            endschalterXPlusTest = TRUE;
                            Debug_printf(Debug_Level_2, "Endschalter X+\n");
                        }
                        else if (value.id == ENDSCHALTER_Xminus)
                        {
                            endschalterXMinusTest = TRUE;
                            Debug_printf(Debug_Level_2, "Endschalter X-\n");
                        }
                        else if (value.id == ENDSCHALTER_Yplus)
                        {
                            endschalterYPlusTest = TRUE;
                            Debug_printf(Debug_Level_2, "Endschalter Y+\n");
                        }
                        else if (value.id == ENDSCHALTER_Yminus)
                        {
                            endschalterYMinusTest = TRUE;
                            Debug_printf(Debug_Level_2, "Endschalter Y-\n");
                        }
                        else if (value.id == ENDSCHALTER_Zplus)
                        {
                            endschalterZPlusTest = TRUE;
                            Debug_printf(Debug_Level_2, "Endschalter Z+\n");
                        }
                        else if (value.id == ENDSCHALTER_Zminus)
                        {
                            endschalterZMinusTest = TRUE;
                            Debug_printf(Debug_Level_2, "Endschalter Z-\n");
                        }
                    }
                }
            }
        }
    }
}

static void App_USBConnection (void *p_arg)
{
    OS_ERR       err;
    (void)p_arg;                                             /* Prevent Compiler Warning */

    //uint8_t str[] = "I'm a LPC1758\n";                        /* Setup string for transmitting */

    (void)p_arg;                                              /* Prevent Compiler Warning */
    while(DEF_TRUE)
    {
        if(usbReceiveBufferSize > 0)
        {   /* if a Message was received */
            USB_printf("I'm a LPC1758 %i\n",5);
            commandSplitter((char*)&usbReceiveBuffer[2], usbReceiveBufferSize);
            usbReceiveBufferSize = 0;                                    /* reset the Message length of the incoming buffer */
#if 0
            BulkInSize = strlen((char *)str);                   /* calculate string length of outgoing data */
            abBulkInBuf[0]=0x00ff&((BulkInSize+1)>>8);          /* Highbyte */
            abBulkInBuf[1]=0x00ff&(BulkInSize+1);               /* Lowbyte  */

            sprintf((char *)&abBulkInBuf[2],"%s",str);               /* write data to output buffer */
            BulkInSize += 3;                                    /* HB + LB + \0 */
            BulkOutSize = 0;                                    /* reset the Message length of the incoming buffer */
#endif
        }
        OSTimeDlyHMSM(0u, 0u, 0u, 500u, OS_OPT_TIME_HMSM_STRICT, &err);

    }
}

static void App_MotorSteuerung (void *p_arg)
{
    OS_ERR       err;
    (void)p_arg;                                             /* Prevent Compiler Warning */

    //uint16 test=-1000;
    //uint8 time=10;
    //uint8_t str[] = "I'm a LPC1758\n";                        /* Setup string for transmitting */

    (void)p_arg;                                              /* Prevent Compiler Warning */
    while(DEF_TRUE)
    {
        static CommandBufferItem item;

        if (getSteps(&item) != FALSE)
            //if (Cb_get(&cncCommandPuffer, (void*)&item) != (int8)(-1))
        {
            setXDirectionUM(item.stepsX);
            setYDirectionUM(item.stepsY);
            setZDirectionUM(item.stepsZ);
        }
        OSTimeDlyHMSM(0u, 0u, 0u, COMMAND_DELAY, OS_OPT_TIME_HMSM_STRICT, &err);

        if (testing == TRUE)
        {
            testButtons();
            testEndstops();
            testing = FALSE;
        }
    }
}

bool putIntoCommandPuffer (int32 newXum, int32 newYum, int32 newZum, uint32 feed)
{

    targetX = newXum;
    targetY = newYum;
    targetZ = newZum;
    currentFeed = feed;

    if (targetX < 0)
    {
        targetX = 0;
    }
    if (targetY < 0)
    {
        targetY = 0;
    }
    if (targetZ < 0)
    {
        targetZ = 0;
    }

    return TRUE;
}

bool getSteps(CommandBufferItem *item)
{
    int32 xOffset;
    int32 yOffset;
    int32 zOffset;
    int32 stepTime;
    int32 calls;
    int32 feedSteps;

    int32 xFeed = 0;
    int32 yFeed = 0;
    int32 zFeed = 0;

    xOffset = targetX - currentX;
    yOffset = targetY - currentY;
    zOffset = targetZ - currentZ;

    stepTime = COMMAND_DELAY;
    calls = 1E3/stepTime;
    feedSteps = (currentFeed*1E3)/calls;

    if (!((xOffset != 0) || (yOffset != 0) || (zOffset != 0)))
    {
        return FALSE;
    }

    //while ((xOffset != 0) || (yOffset != 0) || (zOffset != 0))
    //{
    //xOffset = xOffset - xFeed;
    //yOffset = yOffset - yFeed;
    //zOffset = zOffset - zFeed;

    xFeed = (int32)(xOffset*feedSteps/(abs(xOffset)+abs(yOffset)+abs(zOffset)));
    yFeed = (int32)(yOffset*feedSteps/(abs(xOffset)+abs(yOffset)+abs(zOffset)));
    zFeed = (feedSteps - abs(xFeed) - abs(yFeed)) * ((zOffset)/abs(zOffset));

    if (abs(xFeed) > abs(xOffset))
    {
        xFeed = xOffset;
    }
    if (abs(yFeed) > abs(yOffset))
    {
        yFeed = yOffset;
    }
    if (abs(zFeed) > abs(zOffset))
    {
        zFeed = zOffset;
    }

    item->stepsX = (int16)xFeed;
    item->stepsY = (int16)yFeed;
    item->stepsZ = (int16)zFeed;

    //Cb_put(&cncCommandPuffer, (void*)&item);
    //}

    return TRUE;
}

bool setXDirectionUM(int32 mmm)
{
    currentX += mmm;
    return setXDirection(mmm * xCalibration);
}

bool setYDirectionUM(int32 mmm)
{
    currentY += mmm;
    return setYDirection(mmm * yCalibration);
}

bool setZDirectionUM(int32 mmm)
{
    currentZ += mmm;
    return setZDirection(mmm * zCalibration);
}

void commandSplitter(char* data, uint32 length)
{
    uint32 i;
    uint32 commandBufferPos = 0;
    static char commandBuffer[COMMAND_BUFFER_SIZE];

    for (i = 0; i < length; ++i)
    {
        if (data[i] == '\n')
        {
            processCommand(commandBuffer);
            commandBufferPos = 0;
        }
        else
        {
            commandBuffer[commandBufferPos] = data[i];
            commandBufferPos++;
        }
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
        stepsX = 0;
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
        stepsY = 0;
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
        Gpio_toggle(MOTOR_Z_CLK_PORT,MOTOR_Z_CLK_PIN);
    }
    else
    {
        CSP_TmrStop(CSP_TMR_NBR_02);
        stepsZ = 0;
    }
}

bool setXDirection (int32 stepsX_local)
{
    if (stepsX_local == 0)
    {
        return FALSE;
    }

    if (stepsX_local > 0)
    {
        Gpio_set(MOTOR_X_DIR_PORT,MOTOR_X_DIR_PIN);       // directionX
    }
    else
    {
        Gpio_clear(MOTOR_X_DIR_PORT,MOTOR_X_DIR_PIN);     // directionX
    }

    if (stepsX == 0)
    {
        stepsX = stepsX_local * 2; // one cock has rising and falling edge
        CSP_TmrStart(CSP_TMR_NBR_00);
    }
    else
    {
        stepsX += stepsX_local * 2; // one cock has rising and falling edge
    }

    return TRUE;
}

bool setYDirection (int32 stepsY_local)
{
    if (stepsY_local == 0)
    {
        return FALSE;
    }

    if (stepsY_local > 0)
    {
        Gpio_set(MOTOR_Y_DIR_PORT,MOTOR_Y_DIR_PIN); // directionY
    }
    else
    {
        Gpio_clear(MOTOR_Y_DIR_PORT,MOTOR_Y_DIR_PIN); // directionY
    }

    if (stepsY == 0)
    {
        stepsY = stepsY_local * 2; // one cock has rising and falling edge
        CSP_TmrStart(CSP_TMR_NBR_01);
    }
    else
    {
        stepsY += stepsY_local * 2; // one cock has rising and falling edge
    }

    return TRUE;
}

bool setZDirection (int32 stepsZ_local)
{
    if (stepsZ_local == 0)
    {
        return FALSE;
    }

    if (stepsZ_local < 0)
    {
        Gpio_clear(MOTOR_Z_DIR_PORT,MOTOR_Z_DIR_PIN); // directionY
    }
    else
    {
        Gpio_set(MOTOR_Z_DIR_PORT,MOTOR_Z_DIR_PIN); // directionY
    }

    if (stepsZ == 0)
    {
        stepsZ = stepsZ_local * 2; // one cock has rising and falling edge
        CSP_TmrStart(CSP_TMR_NBR_02);
    }
    else
    {
        stepsZ += stepsZ_local * 2; // one cock has rising and falling edge
    }

    return TRUE;
}



void homeX()
{
    OS_ERR       err;

    Debug_printf(Debug_Level_2, "Start homing x axis\n");

    setXDirection(HOMING_AMOUNT);
    while(stepsX != 0)
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &err);
    currentX = 0;
    targetX = 0;

    Debug_printf(Debug_Level_2, "Homing x axis finished\n");
}

void homeY()
{
    OS_ERR       err;

    Debug_printf(Debug_Level_2, "Start homing y axis\n");

    setYDirection(HOMING_AMOUNT);
    while(stepsY != 0)
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &err);
    currentY = 0;
    targetY = 0;

    Debug_printf(Debug_Level_2, "Homing y axis finished\n");
}

void homeZ()
{
    OS_ERR       err;

    Debug_printf(Debug_Level_2, "Start homing z axis\n");

    setZDirection(HOMING_AMOUNT);
    while(stepsZ != 0)
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &err);
    currentZ = 0;
    targetZ = 0;

    Debug_printf(Debug_Level_2, "Homing z axis finished\n");
}

void homeAll()
{
    OS_ERR       err;

    Debug_printf(Debug_Level_2, "Start homing all axes\n");

    setXDirection(HOMING_AMOUNT);
    setYDirection(HOMING_AMOUNT);
    setZDirection(HOMING_AMOUNT);
    while((stepsX != 0) || (stepsY != 0) || (stepsZ != 0))
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &err);
    currentX = 0;
    currentY = 0;
    currentZ = 0;
    targetX = 0;
    targetY = 0;
    targetZ = 0;

    Debug_printf(Debug_Level_2, "Homing all axes finished\n");
}

bool cncCalibrateZentool (uint8_t measuredDistanceX, uint8_t measuredDistanceY, uint8_t measuredDistanceZ)
{
    Debug_printf(Debug_Level_2, "Starting calibration\n");

    // homeAll();                              // home all axes
    setXDirection(CALIBRATION_AMOUNT * xCalibration);      // move all axes a defined amount
    while (stepsX > 0) { }
    setYDirection(CALIBRATION_AMOUNT * yCalibration);
    while (stepsY > 0) { }
    setZDirection(measuredDistanceZ * zCalibration);
    while (stepsZ > 0) { }

    homeAll();
    // now the user has to measure the distance
    xCalibration = (CALIBRATION_AMOUNT / measuredDistanceX) * xCalibration;
    yCalibration = (CALIBRATION_AMOUNT / measuredDistanceX) * yCalibration;
    zCalibration = (CALIBRATION_AMOUNT / measuredDistanceZ) * zCalibration;

    Debug_printf(Debug_Level_2, "Distance should be 100mm in x+ und y+ \n if not Pleas measure Distances and run Again \n");
    Debug_printf(Debug_Level_2, "values now: x: %d Steps/mm y: %d Steps/mm z: %d Steps/mm \n", xCalibration, yCalibration, zCalibration);
    Debug_printf(Debug_Level_2, "Calibration finished\n");

    return TRUE;
}


bool testButtons(void )
{
    OS_ERR       err;

    Debug_printf(Debug_Level_1, "Press all buttons\n");

    applicationState = ApplicationState_Test;

    buttonXPlusTest  = FALSE;
    buttonXMinusTest = FALSE;
    buttonYPlusTest  = FALSE;
    buttonYMinusTest = FALSE;
    buttonZPlusTest  = FALSE;
    buttonZMinusTest = FALSE;
    buttonOkTest     = FALSE;

    while(!(buttonXPlusTest && buttonXMinusTest
            && buttonYPlusTest && buttonYMinusTest
            && buttonZPlusTest && buttonZMinusTest
            && buttonOkTest
           ))
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &err);

    applicationState = ApplicationState_Movement;

    Debug_printf(Debug_Level_1, "All buttons successfully tested\n");

    return TRUE;
}

bool testEndstops(void )
{
    OS_ERR       err;

    Debug_printf(Debug_Level_1, "Test all endstops\n");

    applicationState = ApplicationState_Test;

    endschalterXPlusTest  = FALSE;
    endschalterXMinusTest = FALSE;
    endschalterYPlusTest  = FALSE;
    endschalterYMinusTest = FALSE;
    endschalterZPlusTest  = FALSE;
    endschalterZMinusTest = FALSE;

    while(!(endschalterXPlusTest && endschalterXMinusTest
            && endschalterYPlusTest && endschalterYMinusTest
            && endschalterZPlusTest && endschalterZMinusTest
           ))
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &err);

    applicationState = ApplicationState_Movement;

    Debug_printf(Debug_Level_1, "All endstops successfully tested\n");

    return TRUE;
}


void buttonInit ()
{
    Button_initialize2(1E4, 1E5);
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

void printUnknownCommand(void)
{
    USB_printf("CMD?\r");
    Led_clear(2);
}

void printParameterMissing(void)
{
    USB_printf("Missing parameter.\r");
    Led_clear(2);
}

void printAcknowledgement(void)
{
    USB_printf("ACK\r");
    Led_clear(2);
}

void printError(char *message)
{
    USB_printf("ERR: %s\r", message);
}

void printAliveMessage(void)
{
    USB_printf("yes\r");
    Led_clear(2);
}

bool compareBaseCommand(char *original, char *received)
{
    return (strcmp(original,received) == 0);
}

bool compareExtendedCommand(char *original, char *received)
{
    return (((strlen(received) == 1) && (strncmp(original,received,1) == 0)) ||
            (strcmp(original,received) == 0));
}

bool parseParameter(char *rawData, int32 *x, int32 *y, int32 *z, uint32 *f)
{
    switch (rawData[0])
    {
    case 'X':
        *x = (int32)(atof(&rawData[1])*1000.0);
        return TRUE;
    case 'Y':
        *y = (int32)(atof(&rawData[1])*1000.0);
        return TRUE;
    case 'Z':
        *z = (int32)(atof(&rawData[1])*1000.0);
        return TRUE;
    case 'F':
        *f = (uint32)(atoi(&rawData[1]));
        return TRUE;
    default:
        return FALSE;
    }

    return FALSE;
}

void processCommand(char *buffer)
{
    char *dataPointer;
    char *dataPointer1;
    char *dataPointer2;
    char *dataPointer3;
    char *savePointer;

    Led_set(Led1);  // set the yellow led to indicate incoming data status

    dataPointer = strtok_r(buffer, " ", &savePointer);

    if (compareBaseCommand("G00", dataPointer))
    {
        uint8 commandCount = 0;
        int32 x = currentX;
        int32 y = currentY;
        int32 z = currentZ;

        if ((dataPointer = strtok_r(NULL, " ", &savePointer)) == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if ((dataPointer1 = strtok_r(NULL, " ", &savePointer)) == NULL)
        {
            commandCount = 1;
        }
        else if ((dataPointer2 = strtok_r(NULL, " ", &savePointer)) == NULL)
        {
            commandCount = 2;
        }
        else if ((dataPointer3 = strtok_r(NULL, " ", &savePointer)) == NULL)
        {
            commandCount = 3;
        }
        else
        {
            printUnknownCommand();
            return;
        }

        parseParameter(dataPointer, &x, &y, &z, NULL);
        if (commandCount > 1)
        {
            parseParameter(dataPointer1, &x, &y, &z, NULL);
        }
        if (commandCount > 2)
        {
            parseParameter(dataPointer2, &x, &y, &z, NULL);
        }

        putIntoCommandPuffer(x, y, z, currentFeed);

        return;
    }
    else if (compareBaseCommand("G00", dataPointer))
    {
        uint8 commandCount = 0;
        int32 x = currentX;
        int32 y = currentY;
        int32 z = currentZ;
        uint32 feed = currentFeed;

        if ((dataPointer = strtok_r(NULL, " ", &savePointer)) == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if ((dataPointer1 = strtok_r(NULL, " ", &savePointer)) == NULL)
        {
            commandCount = 1;
        }
        else if ((dataPointer2 = strtok_r(NULL, " ", &savePointer)) == NULL)
        {
            commandCount = 2;
        }
        else if ((dataPointer3 = strtok_r(NULL, " ", &savePointer)) == NULL)
        {
            commandCount = 3;
        }
        else
        {
            commandCount = 4;
            return;
        }

        parseParameter(dataPointer, &x, &y, &z, &feed);
        if (commandCount > 1)
        {
            parseParameter(dataPointer1, &x, &y, &z, &feed);
        }
        if (commandCount > 2)
        {
            parseParameter(dataPointer2, &x, &y, &z, &feed);
        }
        if (commandCount > 3)
        {
            parseParameter(dataPointer3, &x, &y, &z, &feed);
        }

        putIntoCommandPuffer(x, y, z, feed);

        return;
    }
    else if (compareBaseCommand("move", dataPointer))
    {
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (compareExtendedCommand("x",dataPointer))
        {
            // We have a set command
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer != NULL)
            {
                targetX += (int32)(atof(dataPointer)*1000.0);
                if (targetX < 0)
                {
                    targetX = 0;
                }
            }
            else
            {
                printUnknownCommand();
            }
            return;
        }
        else if (compareExtendedCommand("y",dataPointer))
        {
            // We have a set command
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer != NULL)
            {
                targetY += (int32)(atof(dataPointer)*1000.0);
                if (targetY < 0)
                {
                    targetY = 0;
                }
            }
            else
            {
                printUnknownCommand();
            }
            return;
        }
        else if (compareExtendedCommand("z",dataPointer))
        {
            // We have a set command
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer != NULL)
            {
                targetZ += (int32)(atof(dataPointer)*1000.0);
                if (targetZ < 0)
                {
                    targetZ = 0;
                }
            }
            else
            {
                printUnknownCommand();
            }
            return;
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else if (compareBaseCommand("set", dataPointer))
    {
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (compareExtendedCommand("x",dataPointer))
        {
            // We have a set command
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer != NULL)
            {
                targetX = (int32)(atof(dataPointer)*1000.0);
                if (targetX < 0)
                {
                    targetX = 0;
                }
            }
            else
            {
                printUnknownCommand();
            }
            return;
        }
        else if (compareExtendedCommand("y",dataPointer))
        {
            // We have a set command
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer != NULL)
            {
                targetY = (int32)(atof(dataPointer)*1000.0);
                if (targetY < 0)
                {
                    targetY = 0;
                }
            }
            else
            {
                printUnknownCommand();
            }
            return;
        }
        else if (compareExtendedCommand("z",dataPointer))
        {
            // We have a set command
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer != NULL)
            {
                targetZ = (int32)(atof(dataPointer)*1000.0);
                if (targetZ < 0)
                {
                    targetZ = 0;
                }
            }
            else
            {
                printUnknownCommand();
            }
            return;
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else if (compareBaseCommand("home", dataPointer))
    {
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (compareExtendedCommand("x",dataPointer))
        {
            homeX();
            return;
        }
        if (compareExtendedCommand("y",dataPointer))
        {
            homeY();
            return;
        }
        if (compareExtendedCommand("z",dataPointer))
        {
            homeZ();
            return;
        }
        if (compareExtendedCommand("all",dataPointer))
        {
            homeAll();
            return;
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else if (compareBaseCommand("calibrate", dataPointer))
    {
        if (((dataPointer = strtok_r(NULL, " ", &savePointer)) == NULL) ||
                ((dataPointer1 = strtok_r(NULL, " ", &savePointer)) == NULL) ||
                ((dataPointer2 = strtok_r(NULL, " ", &savePointer)) == NULL) )
            return;

        cncCalibrateZentool(atoi(dataPointer), atoi(dataPointer1), atoi(dataPointer2));

        return;
    }
    else
    {
        printUnknownCommand();
    }
}
/*! EOF */
