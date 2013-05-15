#pragma once

/*
************************************************************************************************
*                                             INCLUDE FILES
************************************************************************************************
*/
#define BUTTON_X_PLUS   1
#define BUTTON_X_MINUS  2
#define BUTTON_Y_PLUS   3
#define BUTTON_Y_MINUS  4
#define BUTTON_Z_PLUS   5
#define BUTTON_Z_MINUS  6
#define BUTTON_OK       7

#define ENDSCHALTER_X_PLUS  8
#define ENDSCHALTER_X_MINUS 9
#define ENDSCHALTER_Y_PLUS  10
#define ENDSCHALTER_Y_MINUS 11
#define ENDSCHALTER_Z_PLUS  12
#define ENDSCHALTER_Z_MINUS 13

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

/** starts moving in Direction X.
 *  @param stepsX are the steps >0= +  <0= -
 */
void moveXDirection ();
void moveYDirection ();
bool setXDirection (int32 stepsX_local);
bool setYDirection (int32 stepsY);

bool cncCalibrateZentool (uint32 steps, int16 difference);
