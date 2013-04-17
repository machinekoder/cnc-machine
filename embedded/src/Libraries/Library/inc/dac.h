/**
 * @file dac.h
 * @author Alexander Rössler
 * @brief DAC Library
 * @date 20-11-2012
 */
/** @defgroup DAC DAC (Digital to Analog Converter)
 * @ingroup Peripheral
 * @{
 */
#pragma once

#include <types.h>
#include <dacDriver.h>

/** Initializes the DAC */
void Dac_initialize();
/** Sets the DAC to a specific value 
 *  @param value Value the DAC should be set to, must be between 0 and DAC_STEPS-1
 */
void Dac_setValue(uint16 value);
/** Sets the DAC to specific value given in Volts
 *  @param mV Value in millivolts, must be between VREFN and VREFP
 */
void Dac_setVolt(uint16 mV);

/**
 * @}
 */
