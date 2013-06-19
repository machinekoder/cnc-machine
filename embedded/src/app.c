/*
 *  uC/OS-III - Getting Started Demo Application
 *
 **/
#include "app.h"

#define printfData(x) Debug_printf(Debug_Level_1,x)
#define printfData2(x,y) Debug_printf(Debug_Level_1,x,y)

#define TIMER_FREQ 35000u
#define HOMING_AMOUNT -1E9     //52500
#define CALIBRATION_AMOUNT 1E4
#define CALIBRATION_AMOUNT_Z 5E3

#define CNC_QUEUE_BUFFER_SIZE 10u

const uint32 commandDelay = 15u;

ApplicationState applicationState = ApplicationState_Idle;

/*
************************************************************************************************
*                                            LOCAL VARIABLES
************************************************************************************************
*/
int32 xCalibration = 3;//25;//25/10;
int32 yCalibration = 3;//25;//25/10;
int32 zCalibration = 3;//25;//25/10;

int32 currentX = 0;     // current X pos in um
int32 currentY = 0;     // current Y pos in um
int32 currentZ = 0;     // current Z pos in um
int32 targetX = 0;     // current X pos in um
int32 targetY = 0;     // current Y pos in um
int32 targetZ = 0;     // current Z pos in um
uint32 currentFeed = MAX_FEED_RATE; // current Feed rate in mm/s

QueueItem cncQueueBufferData[CNC_QUEUE_BUFFER_SIZE];
CircularBuffer cncQueueBuffer;

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

int8 enqueueCncCommand(int32 newXum, int32 newYum, int32 newZum, uint32 feed);
int8 dequeueCncCommand(QueueItem *item);

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



int main (void)
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
    
    CSP_TmrInit();
    CSP_TmrCfg (CSP_TMR_NBR_00,TIMER_FREQ);
    CSP_TmrCfg (CSP_TMR_NBR_01,TIMER_FREQ);
    CSP_TmrCfg (CSP_TMR_NBR_02,TIMER_FREQ);
    
    USBInit();                                               /* USB Initialization */
    /* register descriptors */
    USBRegisterDescriptors(abDescriptors);                   /* USB Descriptor Initialization */

    /* register endpoint handlers */
    USBHwRegisterEPIntHandler(BULK_IN_EP, BulkIn);           /* register BulkIn Handler for EP */
    USBHwRegisterEPIntHandler(BULK_OUT_EP, BulkOut);         /* register BulkOut Handler for EP */
    USBHwRegisterEPIntHandler(INT_IN_EP, NULL);

    USBHwEPConfig(BULK_IN_EP, MAX_PACKET_SIZE);   /* Configure Packet Size for outgoing Transfer */
    USBHwEPConfig(BULK_OUT_EP, MAX_PACKET_SIZE);  /* Configure Packet Size for incoming Transfer */

    /* enable bulk-in interrupts on NAKs */
    USBHwNakIntEnable(INACK_BI);

    Cb_initialize(&cncQueueBuffer, CNC_QUEUE_BUFFER_SIZE, sizeof(QueueItem), (void*)&cncQueueBufferData);
    
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

bool App_putIntoCommandPuffer (int32 newXum, int32 newYum, int32 newZum, uint32 feed)
{
    enqueueCncCommand(newXum, newYum, newZum, feed);
    App_processCncCommands();

    return TRUE;
}

int8 enqueueCncCommand(int32 newXum, int32 newYum, int32 newZum, uint32 feed)
{
    QueueItem item;
    item.targetX = newXum;
    item.targetY = newYum;
    item.targetZ = newZum;
    item.feed    = feed;
    
    return Cb_put(&cncQueueBuffer, (void*)&item);
}

int8 dequeueCncCommand(QueueItem *item)
{
    return Cb_get(&cncQueueBuffer, (void*)item);
}

void App_processCncCommands()
{
    QueueItem item;
    
    if (applicationState == ApplicationState_Working)    // we are already working
    {
        return;
    }
    
    if (dequeueCncCommand(&item) != (int8)(-1))     // we have a command
    {
        targetX = item.targetX;
        targetY = item.targetY;
        targetZ = item.targetZ;
        if ((item.feed != 0u) && (item.feed != 6u))
        {
            currentFeed = item.feed;
        }

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
        
        applicationState = ApplicationState_Working;
    }
    else
    {
        applicationState = ApplicationState_Idle;
    }
}

bool App_getSteps(CommandBufferItem *item)
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
    
    if ((stepsX != 0) || (stepsY != 0) || (stepsZ != 0))
    {
        item->stepsX = 0;
        item->stepsY = 0;
        item->stepsZ = 0;
        CSP_TmrStart(CSP_TMR_NBR_00);   // Workaround
        CSP_TmrStart(CSP_TMR_NBR_01);
        CSP_TmrStart(CSP_TMR_NBR_02);
        return TRUE;
    }

    xOffset = targetX - currentX;
    yOffset = targetY - currentY;
    zOffset = targetZ - currentZ;

    stepTime = commandDelay;
    calls = (int32)(1E3/stepTime);
    feedSteps = (int32)((int32)(currentFeed*1E2)/calls);

    if (!((xOffset != 0) || (yOffset != 0) || (zOffset != 0)))
    {
        return FALSE;
    }

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
    
    return TRUE;
}

bool App_setXDirectionUM(int32 um)
{
    currentX += um;
    return App_setXDirection(um * xCalibration);
}

bool App_setYDirectionUM(int32 um)
{
    currentY += um;
    return App_setYDirection(um * yCalibration);
}

bool App_setZDirectionUM(int32 um)
{
    currentZ += um;
    return App_setZDirection(um * zCalibration);
}

void App_homeX()
{
    OS_ERR       err;

    Debug_printf(Debug_Level_2, "Start homing x axis\n");
    applicationState = ApplicationState_Homing;

    App_setXDirection(HOMING_AMOUNT);
    while(stepsX != 0)
    {
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &err);
    }
    currentX = 0;
    targetX = 0;

    Debug_printf(Debug_Level_2, "Homing x axis finished\n");
    applicationState = ApplicationState_Idle;
}

void App_homeY()
{
    OS_ERR       err;

    Debug_printf(Debug_Level_2, "Start homing y axis\n");
    applicationState = ApplicationState_Homing;

    App_setYDirection(HOMING_AMOUNT);
    while(stepsY != 0)
    {
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &err);
    }
    currentY = 0;
    targetY = 0;

    Debug_printf(Debug_Level_2, "Homing y axis finished\n");
    applicationState = ApplicationState_Idle;
}

void App_homeZ()
{
    OS_ERR       err;

    Debug_printf(Debug_Level_2, "Start homing z axis\n");
    applicationState = ApplicationState_Homing;

    App_setZDirection(HOMING_AMOUNT);
    while(stepsZ != 0)
    {
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &err);
    }
    currentZ = 0;
    targetZ = 0;

    Debug_printf(Debug_Level_2, "Homing z axis finished\n");
    applicationState = ApplicationState_Idle;
}

void App_homeAll()
{
    OS_ERR       err;

    Debug_printf(Debug_Level_2, "Start homing all axes\n");
    applicationState = ApplicationState_Homing;

    App_setXDirection(HOMING_AMOUNT);
    App_setYDirection(HOMING_AMOUNT);
    
    while((stepsX != 0) || (stepsY != 0))
    {
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &err);
    }
    
    App_setZDirection(HOMING_AMOUNT);
    while (stepsZ != 0)
    {
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &err);
    }
    currentX = 0;
    currentY = 0;
    currentZ = 0;
    targetX = 0;
    targetY = 0;
    targetZ = 0;

    Debug_printf(Debug_Level_2, "Homing all axes finished\n");
    applicationState = ApplicationState_Idle;
}

void App_stopMachine()
{
    targetX = currentX;
    targetY = currentY;
    targetZ = currentZ;
    
    testing = false;
    applicationState = ApplicationState_Idle;
}

bool App_cncCalibrateZentool (double measuredDistanceX, double measuredDistanceY, double measuredDistanceZ)
{
    OS_ERR       err;
    
    Debug_printf(Debug_Level_2, "Starting calibration\n");

    xCalibration = (int32)(((double)(CALIBRATION_AMOUNT) / (measuredDistanceX*100.0)) * (double)xCalibration);
    yCalibration = (int32)(((double)(CALIBRATION_AMOUNT) / (measuredDistanceY*100.0)) * (double)yCalibration);
    zCalibration = (int32)(((double)(CALIBRATION_AMOUNT_Z) / (measuredDistanceZ*100.0)) * (double)zCalibration);
    
    App_homeAll();                              // home all axes
    App_setXDirection(CALIBRATION_AMOUNT * xCalibration);      // move all axes a defined amount
    while (stepsX > 0) { 
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &err);
    }
    App_setYDirection(CALIBRATION_AMOUNT * yCalibration);
    while (stepsY > 0) { 
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &err);
    }
    App_setZDirection(CALIBRATION_AMOUNT_Z * zCalibration);
    while (stepsZ > 0) { 
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &err);
    }

    App_homeAll();
    // now the user has to measure the distance;

    Debug_printf(Debug_Level_2, "Distance should be 100mm in x+ und y+ \n if not Pleas measure Distances and run Again \n");
    Debug_printf(Debug_Level_2, "values now: x: %d Steps/mm y: %d Steps/mm z: %d Steps/mm \n", xCalibration, yCalibration, zCalibration);
    Debug_printf(Debug_Level_2, "Calibration finished\n");

    return TRUE;
}
/*! EOF */
