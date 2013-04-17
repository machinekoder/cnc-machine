#pragma once
#include "app.h"

typedef enum {
    App_TaskMachine_State_Production = 0,
    App_TaskMachine_State_Service = 1
} App_TaskMachine_State;

OS_TCB   App_TaskMachineTCB[8];
CPU_STK  App_TaskMachineStk[8][APP_STACK_SIZE];
uint8    taskId[8];



void App_TaskMachine (void *p_arg);
