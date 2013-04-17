/**
 * @file gpio.h
 * @author Alexander Rössler
 * @brief GPIO Library
 * @date 08-12-2012
 */
/** @defgroup GPIO GPIO (General Purpose Input and Output)
 * @ingroup Peripheral
 * @{
 */
#pragma once

/** Set this constant to 0 if you want to write your own interrupt function */
#ifndef GPIO_IRQ_ENABLED
#define GPIO_IRQ_ENABLED 1
#endif

#include <gpioDriver.h>
#include <types.h>

typedef enum { 
    Gpio_Direction_Input = 0u, 
    Gpio_Direction_Output = 1u
} Gpio_Direction;
                     
typedef enum { 
    Gpio_Interrupt_RisingEdge = 0u,
    Gpio_Interrupt_FallingEdge = 1u,
    Gpio_Interrupt_FallingAndRisingEdge = 2u
} Gpio_Interrupt;
                         
typedef struct {
    uint8 port;
    uint8 pin;
} Gpio_Pair;
                      
/** Set the direction of the GPIO pin
 *  @param port Port of the pin.
 *  @param pin Pin number.
 *  @param direction Input or output.
 */
inline void Gpio_setDirection(uint8 port, uint8 pin, Gpio_Direction direction);
/** Reads data from the  GPIO pin
 *  @param port Port of the pin.
 *  @param pin Pin number.
 *  @return Value that has been read from the pin.
 */
inline uint8 Gpio_read(uint8 port, uint8 pin);
/** Write data to the GPIO pin
 *  @param port Port of the pin.
 *  @param pin Pin number.
 *  @param value Value that should be written to the pin.
 */
inline void Gpio_write(uint8 port, uint8 pin, uint8 value);
/** Set the GPIO pin
 *  @param port Port of the pin.
 *  @param pin Pin number.
 */
inline void Gpio_set(uint8 port, uint8 pin);
/** Clear the GPIO pin
 *  @param port Port of the pin.
 *  @param pin Pin number.
 */
inline void Gpio_clear(uint8 port, uint8 pin);
/** Toggle the GPIO pin
 *  @param port Port of the pin.
 *  @param pin Pin number.
 */
inline void Gpio_toggle(uint8 port, uint8 pin);
/** Enable interrupts on the GPIO pin
 *  @param port Port of the pin.
 *  @param pin Pin number.
 *  @param type Rising edge, falling edge or both.
 *  @param func Function that should be triggered if an interrupt happens.
 */
void Gpio_enableInterrupt(uint8 port, uint8 pin, Gpio_Interrupt type, void (* func)(void));
/** Disable interrupts on the GPIO pin
 *  @param port Port of the pin.
 *  @param pin Pin number.
*/
void Gpio_disableInterrupt(uint8 port, uint8 pin);

/**
 * @}
 */
