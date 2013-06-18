#include "taskButton.h"

#define BUTTON_STEP_UM 5000

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

void buttonInit();

void App_TaskButton (void *p_arg)
{
    OS_ERR       err;
    CPU_INT08U count;
    ButtonValue value;
    (void)p_arg;                                                    /* Prevent Compiler Warning */

    buttonInit();
        
    while(DEF_TRUE) {
        for(count=0; count<=10; count++)
        {
            Button_task();        // reads the button values
            OSTimeDlyHMSM(0u, 0u, 0u, 10u, OS_OPT_TIME_HMSM_STRICT, &err);
            if(count==10)
            {
                while (Button_getPress(&value) != (int8)(-1))
                {
                    if (applicationState == ApplicationState_Idle)
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

bool App_testButtons(void )
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
            && buttonOkTest && (applicationState == ApplicationState_Test)
           ))
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &err);

    applicationState = ApplicationState_Idle;

    Debug_printf(Debug_Level_1, "All buttons successfully tested\n");

    return TRUE;
}

bool App_testEndstops(void )
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
            && (applicationState == ApplicationState_Test)
           ))
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &err);

    applicationState = ApplicationState_Idle;

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
