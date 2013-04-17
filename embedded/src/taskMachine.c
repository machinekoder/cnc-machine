#include "taskMachine.h"
#include "taskSupervisor.h"

void App_TaskMachine (void *p_arg)
{
    OS_ERR          err;
    CPU_TS          ts;
    OS_MSG_SIZE     messageSize;
    
    uint8                           taskId          = *((uint8*)p_arg);
    uint32                          producedGoods   = 0;
    uint32                          rawMaterial     = 100;
    App_TaskMachine_State           taskState       = App_TaskMachine_State_Production;
    App_TaskMachine_ServiceRequest  *serviceRequest;
    
    while (DEF_TRUE) {
        
        if (taskState == App_TaskMachine_State_Production)
        {
            rawMaterial--;
            producedGoods++;
            
            if (rawMaterial == 0)       // oh noez now material!!!
            {
                serviceRequest = (App_TaskMachine_ServiceRequest*)OSMemGet(&ServiceRequestMemory, &err);
                serviceRequest->taskId = taskId;
                serviceRequest->producedGoods = producedGoods;
                
                OSTaskQPost(&App_TaskSupervisorTCB,
                            (void*)serviceRequest,
                            sizeof(void*),
                            OS_OPT_POST_FIFO,
                            &err);
                
                taskState = App_TaskMachine_State_Service;
            }
            else 
            {
                OSTimeDlyHMSM(0u, 0u, 0u, 10u, OS_OPT_TIME_HMSM_STRICT, &err);
            }
        }
        else if (taskState == App_TaskMachine_State_Service)
        {
            uint32 *tmpRawMaterial;
            tmpRawMaterial = (uint32*)OSTaskQPend(0,                       // Wait for a message
                                                OS_OPT_PEND_BLOCKING,
                                                &messageSize, 
                                                &ts,
                                                &err);
            
            rawMaterial     = *tmpRawMaterial;
            producedGoods   = 0;
            taskState       = App_TaskMachine_State_Production;
            
            OSMemPut(&RawMaterialMemory, (void*)tmpRawMaterial, &err);           // Deallocate the memory
        }
    }
}
