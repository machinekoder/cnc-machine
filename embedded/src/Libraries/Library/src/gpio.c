#include "gpio.h"

void (* functionPointers0[GPIO0_INT_PIN_COUNT])(void);
void (* functionPointers2[GPIO2_INT_PIN_COUNT])(void);

inline void Gpio_setDirection(uint8 port, uint8 pin, Gpio_Direction direction)
{
    switch (port)
    {
        case 0: GPIO0_CLEAR_DIR(pin);
                GPIO0_SET_DIR(direction,pin);
                return;
        case 1: GPIO1_CLEAR_DIR(pin);
                GPIO1_SET_DIR(direction,pin);
                return;
        case 2: GPIO2_CLEAR_DIR(pin);
                GPIO2_SET_DIR(direction,pin);
                return;
        case 3: GPIO3_CLEAR_DIR(pin);
                GPIO3_SET_DIR(direction,pin);
                return;
        default: return;
    }
    return;
}

inline uint8 Gpio_read(uint8 port, uint8 pin)
{
    switch (port)
    {
        case 0: return GPIO0_READ(pin);
        case 1: return GPIO1_READ(pin);
        case 2: return GPIO2_READ(pin);
        case 3: return GPIO3_READ(pin);
        default: return 0;
    }
    return 0;
}

inline void Gpio_write(uint8 port, uint8 pin, uint8 value)
{
    switch (port)
    {
        case 0: (value ? (GPIO0_SET(pin)) : (GPIO0_CLEAR(pin)));
                return;
        case 1: (value ? (GPIO1_SET(pin)) : (GPIO1_CLEAR(pin)));
                return;
        case 2: (value ? (GPIO2_SET(pin)) : (GPIO2_CLEAR(pin)));
                return;
        case 3: (value ? (GPIO3_SET(pin)) : (GPIO3_CLEAR(pin)));
                return;
        default: return;
    }
    return;
}

inline void Gpio_set(uint8 port, uint8 pin)
{
    switch (port)
    {
        case 0: GPIO0_SET(pin);
                return;
        case 1: GPIO1_SET(pin);
                return;
        case 2: GPIO2_SET(pin);
                return;
        case 3: GPIO3_SET(pin);
                return;
        default: return;
    }
    return;
}

inline void Gpio_clear(uint8 port, uint8 pin)
{
    switch (port)
    {
        case 0: GPIO0_CLEAR(pin);
                return;
        case 1: GPIO1_CLEAR(pin);
                return;
        case 2: GPIO2_CLEAR(pin);
                return;
        case 3: GPIO3_CLEAR(pin);
                return;
        default: return;
    }
    return;
}

inline void Gpio_toggle(uint8 port, uint8 pin)
{
    switch (port)
    {
        case 0: (GPIO0_READ(pin) ? (GPIO0_CLEAR(pin)) : (GPIO0_SET(pin)));
                return;
        case 1: (GPIO1_READ(pin) ? (GPIO1_CLEAR(pin)) : (GPIO1_SET(pin)));
                return;
        case 2: (GPIO2_READ(pin) ? (GPIO2_CLEAR(pin)) : (GPIO2_SET(pin)));
                return;
        case 3: (GPIO3_READ(pin) ? (GPIO3_CLEAR(pin)) : (GPIO3_SET(pin)));
                return;
        default: return;
    }
    return;
}

void Gpio_enableInterrupt(uint8 port, uint8 pin, Gpio_Interrupt type, void (* func)(void))
{
    switch (port)
    {
        case 0: if ((type == Gpio_Interrupt_RisingEdge) || (type == Gpio_Interrupt_FallingAndRisingEdge))
                {
                   GPIO0_ENABLE_IRQ_RISING_EDGE(pin);
                }
                if ((type == Gpio_Interrupt_FallingEdge) || (type == Gpio_Interrupt_FallingAndRisingEdge))
                {
                    GPIO0_ENABLE_IRQ_FALLING_EDGE(pin);
                }
                GPIO0_CLEAR_IRQ(pin);
                functionPointers0[pin] = func;
                break;
        case 2: if ((type == Gpio_Interrupt_RisingEdge) || (type == Gpio_Interrupt_FallingAndRisingEdge))
                {
                    GPIO2_ENABLE_IRQ_RISING_EDGE(pin);
                }
                if ((type == Gpio_Interrupt_FallingEdge) || (type == Gpio_Interrupt_FallingAndRisingEdge))
                {
                    GPIO2_ENABLE_IRQ_FALLING_EDGE(pin);
                }
                GPIO2_CLEAR_IRQ(pin);
                functionPointers2[pin] = func;
                break;
        default: return;
    }
    GPIO_ENABLE_IRQS();
    return;
}

void Gpio_disableInterrupt(uint8 port, uint8 pin)
{
    switch (port)
    {
        case 0: GPIO0_DISABLE_IRQ_RISING_EDGE(pin);
                GPIO0_DISABLE_IRQ_FALLING_EDGE(pin);
                GPIO0_CLEAR_IRQ(pin);
                return;
        case 2: GPIO2_DISABLE_IRQ_RISING_EDGE(pin);
                GPIO2_DISABLE_IRQ_FALLING_EDGE(pin);
                GPIO2_CLEAR_IRQ(pin);
                return;
        default: return;
    }
    return;
}

#if (GPIO_IRQ_ENABLED == 1)
void GPIO_IRQHANDLER()
{
    uint8 i;
    
    if (GPIO0_IRQ_PENDING())
    {
        for (i = 0; i < GPIO0_INT_PIN_COUNT; i++)
        {
            if ((GPIO0_RISING_IRQ_PENDING(i)) || (GPIO0_FALLING_IRQ_PENDING(i)))
            {
                GPIO0_CLEAR_IRQ(i);
                (*functionPointers0[i])();
            }
        }

    }
    else if (GPIO2_IRQ_PENDING())
    {
        for (i = 0; i < GPIO2_INT_PIN_COUNT; i++)
        {
            if ((GPIO2_RISING_IRQ_PENDING(i)) || (GPIO2_FALLING_IRQ_PENDING(i)))
            {
                GPIO2_CLEAR_IRQ(i);
                (*functionPointers2[i])();
            }
        }
    }
}
#endif
