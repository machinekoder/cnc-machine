#include "taskLed.h"
#include "app.h"

void App_TaskLed (void *p_arg)
{
    OS_ERR       err;
    (void)p_arg;                                             /* Prevent Compiler Warning */
    
    Led_initialize(1,29,Led_LowActive_Yes); // onboard LED Led1
    
    while (DEF_TRUE)
    {
        if (applicationState == ApplicationState_Idle)
        {
            Led_set(Led1);
            OSTimeDlyHMSM(0u, 0u, 1u, 0u,OS_OPT_TIME_HMSM_STRICT,&err);
            Led_clear(Led1);
            OSTimeDlyHMSM(0u, 0u, 1u, 0u,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        else if (applicationState == ApplicationState_Homing)
        {
            Led_set(Led1);
            OSTimeDlyHMSM(0u, 0u, 0u, 300u,OS_OPT_TIME_HMSM_STRICT,&err);
            Led_clear(Led1);
            OSTimeDlyHMSM(0u, 0u, 0u, 300u,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        else if (applicationState == ApplicationState_Test)
        {
            Led_set(Led1);
            OSTimeDlyHMSM(0u, 0u, 0u, 100u,OS_OPT_TIME_HMSM_STRICT,&err);
            Led_clear(Led1);
            OSTimeDlyHMSM(0u, 0u, 0u, 100u,OS_OPT_TIME_HMSM_STRICT,&err);
            Led_set(Led1);
            OSTimeDlyHMSM(0u, 0u, 0u, 100u,OS_OPT_TIME_HMSM_STRICT,&err);
            Led_clear(Led1);
            OSTimeDlyHMSM(0u, 0u, 0u, 100u,OS_OPT_TIME_HMSM_STRICT,&err);
            Led_set(Led1);
            OSTimeDlyHMSM(0u, 0u, 0u, 100u,OS_OPT_TIME_HMSM_STRICT,&err);
            Led_clear(Led1);
            OSTimeDlyHMSM(0u, 0u, 1u, 0u,OS_OPT_TIME_HMSM_STRICT,&err);
        }
        else if (applicationState == ApplicationState_Working)
        {
            Led_set(Led1);
            OSTimeDlyHMSM(0u, 0u, 0u, 100u,OS_OPT_TIME_HMSM_STRICT,&err);
            Led_clear(Led1);
            OSTimeDlyHMSM(0u, 0u, 0u, 100u,OS_OPT_TIME_HMSM_STRICT,&err);
        }
    }
}
