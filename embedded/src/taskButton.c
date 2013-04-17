#include "taskButton.h"

void App_TaskButton(void* p_arg)
{
    OS_ERR       err;
    CPU_TS       ts;
    OS_MSG_SIZE  messageSize;
    
    (void)p_arg;
    
    Gpio_setDirection(2, 10, Gpio_Direction_Input);
    Pin_setMode(2, 10, Pin_Mode_PullDown);
    
    while (DEF_TRUE)
    {
        if (Gpio_read(2, 10) == 0)
        {
            OSSemPost(&ButtonSem,
                OS_OPT_POST_1,
                &err
            );
            OSTimeDlyHMSM(0u, 0u, 0u, 300u, OS_OPT_TIME_HMSM_STRICT, &err);
        }
        OSTimeDlyHMSM(0u, 0u, 0u, 50u, OS_OPT_TIME_HMSM_STRICT, &err);
    }
}

void App_TaskButton_waitForPress()
{
    OS_ERR       err;
    CPU_TS       ts;
    OSSemPend(&ButtonSem,
              0, 
              OS_OPT_PEND_BLOCKING,
              &ts, 
              &err);
}
