#include "pincon.h"

void Pin_setFunction(uint8 port, uint8 pin, Pin_Function function)
{
    uint8 functionBits;
    
    switch (function)
    {
        case Pin_Function_Primary: functionBits = PINCON_PRIMARY_FUNCTION;
             break;
        case Pin_Function_FirstAlternate: functionBits = PINCON_FIRST_ALTERNATE_FUNCTION;
             break;
        case Pin_Function_SecondAlternate: functionBits = PINCON_SECOND_ALTERNATE_FUNCTION;
             break;
        case Pin_Function_ThirdAlternate: functionBits = PINCON_THIRD_ALTERNATE_FUNCTION;
             break;
        default: break;
    }
    
    switch (port)
    {
        case 0: PINSEL_CLEAR_PORT0(pin,functionBits);
                PINSEL_SET_PORT0(pin,functionBits);
            break;
        case 1: PINSEL_CLEAR_PORT1(pin,functionBits);
                PINSEL_SET_PORT1(pin,functionBits);
            break;
        case 2: PINSEL_CLEAR_PORT2(pin,functionBits);
                PINSEL_SET_PORT2(pin,functionBits);
            break;
        case 3: PINSEL_CLEAR_PORT3(pin,functionBits);
                PINSEL_SET_PORT3(pin,functionBits);
            break;
        default: break;
    }
    
    return;
}

void Pin_setMode(uint8 port, uint8 pin, PinMode mode)
{
    uint8 functionBits;
    
    switch (mode)
    {
        case Pin_Mode_PullUp: functionBits = PINCON_MODE_PULL_UP;
             break;
        case Pin_Mode_Repeater: functionBits = PINCON_MODE_REPEATER;
             break;
        case Pin_Mode_NoPullUpDown: functionBits = PINCON_MODE_NO_PULL_UP_DOWN;
             break;
        case Pin_Mode_PullDown: functionBits = PINCON_MODE_PULL_DOWN;
             break;
        default: break;
    }
    
    switch (port)
    {
        case 0: PINMODE_CLEAR_PORT0(pin,functionBits);
                PINMODE_SET_PORT0(pin,functionBits);
            break;
        case 1: PINMODE_CLEAR_PORT1(pin,functionBits);
                PINMODE_SET_PORT1(pin,functionBits);
            break;
        case 2: PINMODE_CLEAR_PORT2(pin,functionBits);
                PINMODE_SET_PORT2(pin,functionBits);
            break;
        case 3: PINMODE_CLEAR_PORT3(pin,functionBits);
                PINMODE_SET_PORT3(pin,functionBits);
            break;
        default: break;
    }
    
    return;
}

void Pin_setOpenDrainMode(uint8 port, uint8 pin, Pin_OpenDrain mode)
{
    switch (port)
    {
        case 0: PINODMODE_CLEAR_PORT0(pin);
                PINODMODE_SET_PORT0(pin,mode);
            break;
        case 1: PINODMODE_CLEAR_PORT1(pin);
                PINODMODE_SET_PORT1(pin,mode);
            break;
        case 2: PINODMODE_CLEAR_PORT2(pin);
                PINODMODE_SET_PORT2(pin,mode);
            break;
        case 3: PINODMODE_CLEAR_PORT3(pin);
                PINODMODE_SET_PORT3(pin,mode);
            break;
        default: break;
    }
    
    return;
}
