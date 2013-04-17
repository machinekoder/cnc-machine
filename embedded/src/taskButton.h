#pragma once
#include "app.h"

OS_TCB App_TaskButtonTCB;
CPU_STK App_TaskButtonStk[APP_STACK_SIZE];

void App_TaskButton (void *p_arg);

void App_TaskButton_waitForPress();
