#pragma once

/*
************************************************************************************************
*                                             INCLUDE FILES
************************************************************************************************
*/
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
