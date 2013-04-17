 /**
 * @file pincon.h
 * @author Alexander Rössler
 * @brief PINCON Library
 * @date 08-12-2012
 */
/** @defgroup PINCON PINCON (Pin Configuration)
 * @ingroup Peripheral
 * @{
 */
#pragma once

#include <pinconDriver.h>
#include <types.h>

typedef enum { 
    Pin_Function_Primary = 0,
    Pin_Function_FirstAlternate = 1,
    Pin_Function_SecondAlternate = 2,
    Pin_Function_ThirdAlternate = 3
} Pin_Function;

typedef enum { 
    Pin_Mode_PullUp = 0,
    Pin_Mode_Repeater = 1,
    Pin_Mode_NoPullUpDown = 2,
    Pin_Mode_PullDown = 3
} PinMode;

typedef enum { 
    Pin_OpenDrain_Disable = 0,
    Pin_OpenDrain_Enable = 1
} Pin_OpenDrain;

/** Set the function of the selected pin, lookup the functions in the manual (PINSEL)
 *  @param port Port of the pin.
 *  @param pin Pin number.
 *  @param function Function to be selected.
 */
void Pin_setFunction(uint8 port, uint8 pin, Pin_Function function);
/** Set the mode of the selected pin, lookup the modes in the manual (PINMODE)
 *  @param port Port of the pin.
 *  @param pin Pin number.
 *  @param mode Mode that should be set on the pin.
 */
void Pin_setMode(uint8 port, uint8 pin, PinMode mode);
/** Set open drain mode of the selected pin, lookup the open drain modes in the manul (PINMODE_OD)
 *  @param port Port of the pin.
 *  @param pin Pin number.
 *  @param mode Open drain mode that should be set on the pin (enabled or not).
 */
void Pin_setOpenDrainMode(uint8 port, uint8 pin, Pin_OpenDrain mode);

/**
 * @}
 */
