#include "timeout.h"

#define TIMEOUT_TIMER_KHZ       1
#define TIMEOUT_TIMER_INTERVAL  ~(uint32)0

volatile uint8 globalTimerId;

int8 Timeout_initialize(Timer timerId)
{
    
    if (timerId == -1)
    {
        timerId = Timer_initializeRandom(TIMEOUT_TIMER_KHZ, TIMEOUT_TIMER_INTERVAL);
    }
    else
    {
        if (timerId == 0)
        {
            if (Timer_initialize(Timer0, TIMEOUT_TIMER_KHZ, TIMEOUT_TIMER_INTERVAL) == -1)
            {
                return -1;
            }
        }
        else if (timerId == 1)
        {
            if (Timer_initialize(Timer1, TIMEOUT_TIMER_KHZ, TIMEOUT_TIMER_INTERVAL) == -1)
            {
                return -1;
            }
        }
        else if (timerId == 2)
        {
            if (Timer_initialize(Timer2, TIMEOUT_TIMER_KHZ, TIMEOUT_TIMER_INTERVAL) == -1)
            {
                return -1;
            }
        }
        else if (timerId == 3)
        {
            if (Timer_initialize(Timer3, TIMEOUT_TIMER_KHZ, TIMEOUT_TIMER_INTERVAL) == -1)
            {
                return -1;
            }
        }
        else
        {
            timerId = -1;
        }
    }
    
    if (timerId == -1)
        return -1;
    
    globalTimerId = timerId;
    Timer_start(timerId);
    
    return 0;
}

uint32 Timeout_msecs(void)
{
    return Timer_counterValue(globalTimerId);
}

void Timeout_reset(void)
{
    Timer_reset(globalTimerId);
}
