#pragma once
#include "app.h"
#include <led.h>

#define APP_LED_STACK_SIZE 50u

static OS_TCB App_TaskLedTCB;
static CPU_STK App_TaskLedStk[APP_LED_STACK_SIZE];

void App_TaskLed (void *p_arg);
