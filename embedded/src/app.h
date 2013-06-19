#pragma once

/*
************************************************************************************************
*                                             INCLUDE FILES
************************************************************************************************
*/

#include <defines.h>
#include <includes.h>
#include <sys/stat.h>
#include <zentoolworksDriver.h>
#include "taskStart.h"
#include "taskButton.h"
#include "taskLed.h"
#include "taskMotorSteuerung.h"
#include "taskUsbConnection.h"

/*
************************************************************************************************
*                                             GLOBAL DEFINES
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

#define MAX_FEED_RATE 500u
#define MIN_FEED_RATE 1u

typedef enum {
    ApplicationState_Working  = 1u,
    ApplicationState_Idle     = 2u,
    ApplicationState_Test     = 3u,
    ApplicationState_Homing   = 4u
} ApplicationState;

typedef struct {
    int16 stepsX;
    int16 stepsY;
    int16 stepsZ;
} CommandBufferItem;

typedef struct {
    int32 targetX;
    int32 targetY;
    int32 targetZ;
    uint32 feed;
} QueueItem;

extern ApplicationState applicationState;
extern bool testing;
extern const uint32 commandDelay;
extern int32 currentX;     // current X pos in um
extern int32 currentY;     // current Y pos in um
extern int32 currentZ;     // current Z pos in um
extern int32 targetX;     // current X pos in um
extern int32 targetY;     // current Y pos in um
extern int32 targetZ;     // current Z pos in um

/*
************************************************************************************************
*                                         FUNCTION PROTOTYPES
************************************************************************************************
*/
bool App_putIntoCommandPuffer (int32 newXum, int32 newYum, int32 newZum, uint32 feed);
bool App_cncCalibrateZentool (double measuredDistanceX, double measuredDistanceY, double measuredDistanceZ);

/** starts moving in Direction X.
 *  @param stepsX are the steps >0= +  <0= -
 */
void App_homeX();
void App_homeY();
void App_homeZ();
void App_homeAll();
bool App_setXDirectionUM (int32 mmm);
bool App_setYDirectionUM (int32 mmm);
bool App_setZDirectionUM (int32 mmm);
void App_stopMachine();

bool App_getSteps(CommandBufferItem *item);
void App_processCncCommands();
