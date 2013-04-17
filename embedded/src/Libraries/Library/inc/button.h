/**
 * @file button.h
 * @author Johannes Wimmer & Alexander Rössler
 * @brief Library for controlling the IAP functions
 * @date 25-01-2013
 */

/** @defgroup Button Button Library
 * @ingroup Device
 * @{
 */
#pragma once

#define BUTTON_BUFFER_SIZE 20

#include <types.h>
#include <timer.h>
#include <gpio.h>
#include <pincon.h>
#include <circularbuffer.h>

typedef enum {
    ButtonTypeHighActive = 0,
    ButtonTypeLowActive = 1
} ButtonType;

typedef struct {
    uint8 id;
    uint8 count;
} ButtonValue;

/** Initizes the Button Pin und Ports.
 *@param Mhz Is the frequency in wich values are checked.
 *@param ID  Stands for the name of the Buttons(B1,B2,B3,..).
 *@param Port Is the port of the button.
 *@param Pin  Is the pin of the button.
 *@param Type set "0" for low_active, or" 1" for high_active
 */
uint8 Button_initialize(uint32 khz, uint32 sampleInterval, uint32 timeoutInterval);

void Button_initializeButton(uint8 id, uint8 port, uint8 pin, ButtonType type);

int8 Button_getPress(ButtonValue *value);

/**
 * @}
 */
