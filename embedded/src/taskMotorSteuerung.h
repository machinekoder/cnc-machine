#pragma once
#include "app.h"

static OS_TCB  App_TaskMotorSteuerungTCB;
static CPU_STK App_TaskMotorSteuerungStk[APP_STACK_SIZE];

extern int32 stepsX;
extern int32 stepsY;
extern int32 stepsZ;

void App_TaskMotorSteuerung (void *p_arg);
void App_TMR0_IntHandler(void *p_arg);
void App_TMR1_IntHandler (void *p_arg);
void App_TMR2_IntHandler (void *p_arg);

bool App_setXDirection (int32 stepsX_local);
bool App_setYDirection (int32 stepsY_local);
bool App_setZDirection (int32 stepsZ_local);
