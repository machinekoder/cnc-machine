#pragma once
#include "app.h"

static OS_TCB App_TaskButtonTCB;
static CPU_STK App_TaskButtonStk[APP_STACK_SIZE];

void App_TaskButton (void *p_arg);
bool App_testButtons(void);
bool App_testEndstops(void);
