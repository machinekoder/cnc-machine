#pragma once
#include "app.h"
#include "taskLed.h"
#include "taskUsbConnection.h"
#include "taskMotorSteuerung.h"

static OS_TCB App_TaskStartTCB;               /* Application Startup Task Control Block (TCB) */
static CPU_STK App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE];             /* Start Task Stack */

void App_TaskStart (void  *p_arg);
