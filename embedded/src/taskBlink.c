#include "taskBlink.h"

void App_TaskBlink(void* p_arg)
{
    OS_ERR       err;
    CPU_TS       ts;
    OS_MSG_SIZE  messageSize;
    
    App_TaskBlink_State taskState = App_TaskBlink_State_Idle;
    
    App_TaskBlink_State tmpState;
    
    (void)p_arg;
    
    Led_initialize(1, 29, Led_LowActive_Yes);                           // Init
    
    while (DEF_TRUE)
    {
        if (taskState == App_TaskBlink_State_Idle)
        {
        }
        else if (taskState == App_TaskBlink_State_Blink)
        {
            Led_toggle(Led1);
        }
        
        tmpState = (App_TaskBlink_State)OSTaskQPend(10, 
                                                    OS_OPT_PEND_BLOCKING, 
                                                    &messageSize, 
                                                    &ts, 
                                                    &err);
        
        if (err != OS_ERR_TIMEOUT)  // we have received something
        {
            taskState = tmpState;
            if (taskState == App_TaskBlink_State_Idle)
            {
                Led_clear(Led1);
            }
        }
        
        OSTimeDlyHMSM(0u, 0u, 0u, 300u, OS_OPT_TIME_HMSM_STRICT, &err);
    }
}

void App_TaskBlink_startBlink()
{
    OS_ERR       err;
    OSTaskQPost(&App_TaskBlinkTCB,
                (void*)App_TaskBlink_State_Blink,
                sizeof(void*),
                OS_OPT_POST_FIFO,
                &err);
}

void App_TaskBlink_stopBlink()
{
    OS_ERR       err;
    OSTaskQPost(&App_TaskBlinkTCB,
                (void*)App_TaskBlink_State_Idle,
                sizeof(void*),
                OS_OPT_POST_FIFO,
                &err);
}
