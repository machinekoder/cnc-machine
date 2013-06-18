#pragma once
#include "app.h"

static OS_TCB App_TaskUsbConnectionTCB;
static CPU_STK App_TaskUsbConnectionStk[APP_STACK_SIZE];

void App_TaskUsbConnection (void *p_arg);
