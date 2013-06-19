#pragma once
#include <led.h>
#include "app.h"

#define APP_LED_STACK_SIZE 10

static OS_TCB App_TaskLedTCB;
static CPU_STK App_TaskLedStk[APP_LED_STACK_SIZE];

void App_TaskLed (void *p_arg);
