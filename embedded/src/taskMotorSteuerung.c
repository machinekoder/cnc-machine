#include "taskMotorSteuerung.h"
#include "app.h"

int32 stepsX;
int32 stepsY;
int32 stepsZ;

void moveXDirection ();
void moveYDirection ();
void moveZDirection ();

void App_TaskMotorSteuerung (void *p_arg)
{
    OS_ERR       err;
    (void)p_arg;                                             /* Prevent Compiler Warning */
    
    cnc_initialize();
    // init command buffer
    
    while(DEF_TRUE)
    {
        static CommandBufferItem item;

        if (applicationState == ApplicationState_Working)
        {
            if (getSteps(&item) != FALSE)
            {
                setXDirectionUM(item.stepsX);
                setYDirectionUM(item.stepsY);
                setZDirectionUM(item.stepsZ);
            }
            else    // we finished a command
            {
                USB_printf("ok\n");
                applicationState = ApplicationState_Idle;
                processCncCommands();
            }
        }
        OSTimeDlyHMSM(0u, 0u, 0u, commandDelay, OS_OPT_TIME_HMSM_STRICT, &err);

        if (testing == TRUE)
        {
            App_testButtons();
            if (testing == TRUE)
            {
                App_testEndstops();
            }
            testing = FALSE;
        }
    }
}

void App_TMR0_IntHandler (void *p_arg)
{

    moveXDirection();

    CSP_TmrIntClr(CSP_TMR_NBR_00);      /* Clear TMR0 interrupt.                                */
    CPU_IntSrcEn(CPU_INT_SYSTICK);     /* Enable the SYSTICK interrupt. Resume OS operation.   */
}

void App_TMR1_IntHandler (void *p_arg)
{

    moveYDirection();

    CSP_TmrIntClr(CSP_TMR_NBR_01);      /* Clear TMR1 interrupt.                                */
    CPU_IntSrcEn(CPU_INT_SYSTICK);     /* Enable the SYSTICK interrupt. Resume OS operation.   */
}

void App_TMR2_IntHandler (void *p_arg)
{

    moveZDirection();

    CSP_TmrIntClr(CSP_TMR_NBR_02);                /* Clear TMR2 interrupt.                                                                */
    CPU_IntSrcEn(CPU_INT_SYSTICK);     /* Enable the SYSTICK interrupt. Resume OS operation.   */
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

bool App_setXDirection (int32 stepsX_local)
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

bool App_setYDirection (int32 stepsY_local)
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

bool App_setZDirection (int32 stepsZ_local)
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
