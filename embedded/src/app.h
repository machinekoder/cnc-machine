#pragma once

/*
************************************************************************************************
*                                             INCLUDE FILES
************************************************************************************************
*/
#define BUTTON_X+ 1
#define BUTTON_X- 2
#define BUTTON_Y+ 3
#define BUTTON_Y- 4
#define BUTTON_Z+ 5
#define BUTTON_Z- 6
#define BUTTON_OK 7

#define ENDSCHALTER_X+ 8
#define ENDSCHALTER_X- 9
#define ENDSCHALTER_Y+ 10
#define ENDSCHALTER_Y- 11
#define ENDSCHALTER_Z+ 12
#define ENDSCHALTER_Z- 13







#include <defines.h>
#include <includes.h>
#include <sys/stat.h>
#include "taskMachine.h"

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
int8 Timer_initialize(Timer timer, uint32 khz, uint32 intervalUs);
void Timer_connectFunction(Timer id, void (* func)(void));
uint32 Timer_running(Timer id);

/** starts moving in Direction X.
 *  @param stepsX are the steps >0= +  <0= -
 */
bool setXDirection (CPU_INT32S stepsX);
bool setYDirection (CPU_INT32S stepsY);
void moveXDirection ();
