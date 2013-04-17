/**
 * @file timeout.h
 * @author Alexander Rössler
 * @brief Timeout Library
 * @date 05-01-2013
 */
/** @defgroup Timeout Timeout Library
 * @ingroup Functional
 * @{
 */
#pragma once

#include <types.h>
#include <timer.h>

/** Initializes a free timer to work as counter for timeouts.
 *  @return 0 if successful, -1 on error
 */
int8 Timeout_initialize(Timer timerId);
/** Returns current timeout value in msecs
 *  @return timeout value in ms
 */
uint32 Timeout_msecs(void);
/** Resets the timeout timer. */
void Timeout_reset(void);

/**
 * @}
 */
