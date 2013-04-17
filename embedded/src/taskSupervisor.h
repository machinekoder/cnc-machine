#pragma once
#include "app.h"

OS_TCB App_TaskSupervisorTCB;
CPU_STK App_TaskSupervisorStk[APP_STACK_SIZE];

void App_TaskSupervisor (void *p_arg);
