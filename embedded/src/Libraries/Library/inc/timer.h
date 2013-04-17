/**
 * @file timer.h
 * @author Alexander Rössler
 * @brief Timer Library
 * @date 16-10-2012
 */
/** @defgroup Timer Timer
 * @ingroup Peripheral
 * @{
 */
#pragma once

/** Set this constant to 0 if you want to write your own interrupt function */
#ifndef TIMER0_IRQ_ENABLED
#define TIMER0_IRQ_ENABLED 1
#endif
/** Set this constant to 0 if you want to write your own interrupt function */
#ifndef TIMER1_IRQ_ENABLED
#define TIMER1_IRQ_ENABLED 1
#endif
/** Set this constant to 0 if you want to write your own interrupt function */
#ifndef TIMER2_IRQ_ENABLED
#define TIMER2_IRQ_ENABLED 1
#endif
/** Set this constant to 0 if you want to write your own interrupt function */
#ifndef TIMER3_IRQ_ENABLED
#define TIMER3_IRQ_ENABLED 1
#endif

#include <types.h>
#include <timerDriver.h>

typedef enum {
    Timer0 = 0,
    Timer1 = 1,
    Timer2 = 2,
    Timer3 = 3
} Timer;

/** Initializes some hardware timer.
 *  @param timer The timer to initialize.
 *  @param khz The frequency the timer should run with. 
 *  @param intervalUs Tick interval in µs.
 *  @return Returns the id of the timer that was initialized or -1 if all timers are already in use.
 */
extern int8 Timer_initialize(Timer timer, uint32 khz, uint32 intervalUs);
/** Initializes some random hardware timer.
 *  @param khz The frequency the timer should run with. 
 *  @param intervalUs Tick interval in µs.
 *  @return Returns the id of the timer that was initialized or -1 if all timers are already in use.
 */
extern int8 Timer_initializeRandom(uint32 khz, uint32 intervalUs);

/** Deinitializes timer id 
 *  @param id Id of the timer that should be deinitialized.
 *  @return Return 0 on successful or -1 if timer is running.
 */
extern int8 Timer_deinitialize(Timer id);

/** Starts timer id.
 *  @param id Id of the timer that should be started. 0-3
 */
extern void Timer_start(Timer id);

/** Stops timer id.
 *  @param id Id of the timer that should be stopped. 0-3
 */
extern void Timer_stop(Timer id);

/** Resets timer id.
 *  @param id Id of the timer that should be reset. 0-3
 */
extern void Timer_reset(Timer id);

/** Sets tick inteval of timer id in microseconds.
 *  @param id Id of the timer. 0-3
 *  @param us Tick interval in µs.
 */
extern void Timer_setIntervalUs(Timer id, uint32 us);

/** Sets tick inteval of timer id in milliseconds.
 *  @param id Id of the timer. 0-3
 *  @param ms Tick interval in ms.
 */
extern void Timer_setIntervalMs(Timer id, uint32 ms);

/** Sets interrupt priority of the timer id.
 *  @param id Id of the timer. 0-3
 *  @param priority Priority level, smaller value means higher priority. Look at __NVIC_PRIO_BITS to see how many bits can be used.
 */
extern void Timer_setPriority(Timer id, uint8 priority);

/** Returns the counter value of a timer.
 *  @param id Id of the target timer
 *  @return counter value
 */
extern uint32 Timer_counterValue(Timer id);

/** Connects a function to the interrupt of timer id.
 *  @param id Id of the timer. 0-3
 *  @param func A pointer to the function that should be connected. e.g,; &allLedsDown
 */
extern void Timer_connectFunction(Timer id, void (* func)(void));

/** A simple delay function that runs on some timer.
 *  @param us Delay time in milliseconds.
 */
extern int8 Timer_delayUs(uint32 ms);

/** A simple delay function that runs on some timer.
 *  @param ms Delay time in milliseconds.
 *  @return Returns 0 if successful or -1 if timer is already running.
 */
extern int8 Timer_delayMs(uint32 ms);

/** Sets timer id to run only one interval and then call a function.
 *  @param ms Interval in milliseconds.
 *  @param func A pointer to the function that should be connected. e.g,; &allLedsDown
 *  @return Returns the id of the timer that was initialized or -1 if all timers are already in use.
 */
extern int8 Timer_singleShot(uint32 ms, void (* func)(void));

//extern int8 initializeCapCom0(uint32 khz, uint8 pin, void (* func)(void));
//extern int8 initializeCapCom1(uint32 khz, uint8 pin, void (* func)(void));
//extern int8 initializeCapCom2(uint32 khz, uint8 pin, void (* func)(void));
//extern int8 initializeCapCom3(uint32 khz, uint8 pin, void (* func)(void));

/**
 * @}
 */
