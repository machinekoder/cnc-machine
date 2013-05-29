#pragma once

/*
************************************************************************************************
*                                             INCLUDE FILES
************************************************************************************************
*/
#define BUTTON_Xplus 0
#define BUTTON_Xminus 1
#define BUTTON_Yplus 2
#define BUTTON_Yminus 3
#define BUTTON_Zplus 4
#define BUTTON_Zminus 5
#define BUTTON_OK 6

#define ENDSCHALTER_Xplus 7
#define ENDSCHALTER_Xminus 8
#define ENDSCHALTER_Yplus 9
#define ENDSCHALTER_Yminus 10
#define ENDSCHALTER_Zplus 11
#define ENDSCHALTER_Zminus 12

#define TASTER_Xplus_PORT          2
#define TASTER_Xmius_PORT          2
#define TASTER_Yplus_PORT          2
#define TASTER_Yminus_PORT         2
#define TASTER_Zplus_PORT          0
#define TASTER_Zminus_PORT         2
#define TASTER_OK_PORT             2
#define TASTER_OK_PIN              0
#define TASTER_Xplus_PIN           1
#define TASTER_Xmius_PIN           2
#define TASTER_Yplus_PIN           3
#define TASTER_Yminus_PIN          4
#define TASTER_Zplus_PIN           26
#define TASTER_Zminus_PIN          7

#define ENDSCHALTER_Xplus_PORT          0
#define ENDSCHALTER_Xmius_PORT          0
#define ENDSCHALTER_Yplus_PORT          2
#define ENDSCHALTER_Yminus_PORT         2
#define ENDSCHALTER_Zplus_PORT          0
#define ENDSCHALTER_Zminus_PORT         0
#define ENDSCHALTER_Xplus_PIN           8
#define ENDSCHALTER_Xmius_PIN           9
#define ENDSCHALTER_Yplus_PIN           6
#define ENDSCHALTER_Yminus_PIN          8
#define ENDSCHALTER_Zplus_PIN           17
#define ENDSCHALTER_Zminus_PIN          22

#include <defines.h>
#include <includes.h>
#include <sys/stat.h>
#include <timer.h>
#include "taskMachine.h"
#include "zentoolworksDriver.h"
#include "debug.h"

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

/** starts moving in Direction X.
 *  @param stepsX are the steps >0= +  <0= -
 */
void moveXDirection ();
void moveYDirection ();
void moveZDirection ();
bool setXDirection (int32 stepsX_local);
bool setYDirection (int32 stepsY_local);
bool setZDirection (int32 stepsZ_local);
bool setXDirectionMM (int32 mm);
bool setYDirectionMM (int32 mm);
bool setZDirectionMM (int32 mm);


void buttonInit();
void homeX();
void homeY();
void homeZ();
void homeAll();
bool cncCalibrateZentool (uint8_t measuredDistanceX, uint8_t measuredDistanceY, uint8_t measuredDistanceZ);

bool testButtons(void);
bool testEndstops(void);

