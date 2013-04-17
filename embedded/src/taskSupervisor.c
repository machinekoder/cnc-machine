#include "taskSupervisor.h"
#include "taskMachine.h"
#include "taskBlink.h"
#include "taskButton.h"

void App_TaskSupervisor(void* p_arg)
{
    OS_ERR       err;
    CPU_TS       ts;
    OS_MSG_SIZE  messageSize;
    
    uint32       producedGoods = 0;
    char         outputBuffer[100];
    
    uint32*      tmpRawMaterial;
    App_TaskMachine_ServiceRequest* serviceRequest;
    

    (void)p_arg;
    os_serial_lite_init(0, NULL);
    
    while (DEF_TRUE)
    {
        serviceRequest = (App_TaskMachine_ServiceRequest*)OSTaskQPend(0,                       // Wait for a message
                                                            OS_OPT_PEND_BLOCKING,
                                                            &messageSize, 
                                                            &ts,
                                                            &err);
        
        // Request input from user
        snprintf(outputBuffer, 100, "Machine %u requests new goods!\n", serviceRequest->taskId);
        os_serial_sync_send(0, outputBuffer, strlen(outputBuffer));
        
        //App_TaskBlink_startBlink();
        //App_TaskButton_waitForPress();
        //App_TaskBlink_stopBlink();
        
        producedGoods += serviceRequest->producedGoods;
        
        tmpRawMaterial = OSMemGet(&RawMaterialMemory, &err);
        *tmpRawMaterial = 500;
        
        OSTaskQPost(&App_TaskMachineTCB[serviceRequest->taskId],
                    (void*)tmpRawMaterial,
                    (OS_MSG_SIZE)sizeof(uint32*),
                    OS_OPT_POST_FIFO,
                    &err);
        
        OSMemPut(&ServiceRequestMemory, (void*) serviceRequest, &err);       // Deallocate the memory
    }
}
