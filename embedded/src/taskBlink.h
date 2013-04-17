#pragma once
#include "app.h"

typedef enum {
    App_TaskBlink_State_Blink = 0,
    App_TaskBlink_State_Idle = 1
} App_TaskBlink_State;

OS_TCB App_TaskBlinkTCB;
CPU_STK App_TaskBlinkStk[APP_STACK_SIZE];

void App_TaskBlink (void *p_arg);

void App_TaskBlink_startBlink();
void App_TaskBlink_stopBlink();
