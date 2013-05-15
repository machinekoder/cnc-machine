#pragma once

/*
************************************************************************************************
*                                             INCLUDE FILES
************************************************************************************************
*/
#define BUTTON_Xplus 1
#define BUTTON_Xminus 2
#define BUTTON_Yplus 3
#define BUTTON_Yminus 4
#define BUTTON_Zplus 5
#define BUTTON_Zminus 6
#define BUTTON_OK 7

#define ENDSCHALTER_Xplus 8
#define ENDSCHALTER_Xminus 9
#define ENDSCHALTER_Yplus 10
#define ENDSCHALTER_Yminus 11
#define ENDSCHALTER_Zplus 12
#define ENDSCHALTER_Zminus 13

#include <defines.h>
#include <includes.h>
#include <sys/stat.h>
#include <timer.h>
#include "taskMachine.h"
#include "zentoolworksDriver.h"

/*
************************************************************************************************
*                                             GLOBAL DEFINES
************************************************************************************************
*/

#define OUTPUT_MEMORY_SIZE 50

typedef struct {
    uint8  taskId;
    uint32 producedGoods;
} App_TaskMachine_ServiceRequest;

char g_TWBuffer[0xFF];
char g_TRBuffer[0xFF];
char testBuffer[100];

OS_SEM UartSem;
OS_SEM DacSem;
OS_SEM ButtonSem;

OS_MEM OutputMemory;
OS_MEM ServiceRequestMemory;
OS_MEM RawMaterialMemory;

 char                           OutputMemoryStorage[10][OUTPUT_MEMORY_SIZE];
 App_TaskMachine_ServiceRequest ServiceRequestMemoryStorage[20];
 uint32                         RawMaterialMemoryStorage[20];

//uint8 wakeup = 0;

/*
************************************************************************************************
*                                         FUNCTION PROTOTYPES
************************************************************************************************
*/
void DAC_WriteValue(uint32 dac_value);
static void App_TMR0_IntHandler(void *p_arg);
static void App_TMR1_IntHandler (void *p_arg);
//static void App_TMR2_IntHandler (void *p_arg);

/** starts moving in Direction X.
 *  @param stepsX are the steps >0= +  <0= -
 */
void moveXDirection ();
void moveYDirection ();
bool setXDirection (int32 stepsX_local);
bool setYDirection (int32 stepsY);

bool cncCalibrateZentool (uint32 steps, int16 difference);

