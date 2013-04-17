#ifndef _GPIODIVER_H_
#define _GPIODIVER_H_

#define GPIO0_CLEAR_DIR(pin)            LPC_GPIO0->FIODIR &= ~(1 << pin)
#define GPIO0_SET_DIR(direction,pin)    LPC_GPIO0->FIODIR |= (direction << pin)    
#define GPIO1_CLEAR_DIR(pin)            LPC_GPIO1->FIODIR &= ~(1 << pin)
#define GPIO1_SET_DIR(direction,pin)    LPC_GPIO1->FIODIR |= (direction << pin)    
#define GPIO2_CLEAR_DIR(pin)            LPC_GPIO2->FIODIR &= ~(1 << pin)
#define GPIO2_SET_DIR(direction,pin)    LPC_GPIO2->FIODIR |= (direction << pin)    
#define GPIO3_CLEAR_DIR(pin)            LPC_GPIO3->FIODIR &= ~(1 << pin)
#define GPIO3_SET_DIR(direction,pin)    LPC_GPIO3->FIODIR |= (direction << pin)    

#define GPIO0_READ(pin)                 ((LPC_GPIO0->FIOPIN & (1 << pin)) > 0)
#define GPIO1_READ(pin)                 ((LPC_GPIO1->FIOPIN & (1 << pin)) > 0)
#define GPIO2_READ(pin)                 ((LPC_GPIO2->FIOPIN & (1 << pin)) > 0)
#define GPIO3_READ(pin)                 ((LPC_GPIO3->FIOPIN & (1 << pin)) > 0)

#define GPIO0_SET(pin)                  LPC_GPIO0->FIOSET |= (1 << pin)
#define GPIO1_SET(pin)                  LPC_GPIO1->FIOSET |= (1 << pin)
#define GPIO2_SET(pin)                  LPC_GPIO2->FIOSET |= (1 << pin)
#define GPIO3_SET(pin)                  LPC_GPIO3->FIOSET |= (1 << pin)

#define GPIO0_CLEAR(pin)                LPC_GPIO0->FIOCLR |= (1 << pin)
#define GPIO1_CLEAR(pin)                LPC_GPIO1->FIOCLR |= (1 << pin)
#define GPIO2_CLEAR(pin)                LPC_GPIO2->FIOCLR |= (1 << pin)
#define GPIO3_CLEAR(pin)                LPC_GPIO3->FIOCLR |= (1 << pin)

#define GPIO0_INT_PIN_COUNT             21
#define GPIO2_INT_PIN_COUNT             14

#define GPIO_IRQHANDLER()               EINT3_IRQHandler()
#define GPIO_ENABLE_IRQS()              NVIC_EnableIRQ(EINT3_IRQn);

#define GPIO0_ENABLE_IRQ_RISING_EDGE(pin)   LPC_GPIOINT->IO0IntEnR |= (1 << pin)
#define GPIO0_DISABLE_IRQ_RISING_EDGE(pin)  LPC_GPIOINT->IO0IntEnR &= ~(1 << pin);
#define GPIO0_ENABLE_IRQ_FALLING_EDGE(pin)  LPC_GPIOINT->IO0IntEnF |= (1 << pin)
#define GPIO0_DISABLE_IRQ_FALLING_EDGE(pin) LPC_GPIOINT->IO0IntEnF &= ~(1 << pin);
#define GPIO0_RISING_IRQ_PENDING(pin)       LPC_GPIOINT->IO0IntStatR & (1 << pin)
#define GPIO0_FALLING_IRQ_PENDING(pin)      LPC_GPIOINT->IO0IntStatF & (1 << pin)
#define GPIO0_CLEAR_IRQ(pin)                LPC_GPIOINT->IO0IntClr |= (1 << pin)
#define GPIO0_IRQ_PENDING()                 LPC_GPIOINT->IntStatus & (1 << 0)

#define GPIO2_ENABLE_IRQ_RISING_EDGE(pin)   LPC_GPIOINT->IO2IntEnR |= (1 << pin)
#define GPIO2_DISABLE_IRQ_RISING_EDGE(pin)  LPC_GPIOINT->IO2IntEnR &= ~(1 << pin);
#define GPIO2_ENABLE_IRQ_FALLING_EDGE(pin)  LPC_GPIOINT->IO2IntEnF |= (1 << pin)
#define GPIO2_DISABLE_IRQ_FALLING_EDGE(pin) LPC_GPIOINT->IO2IntEnF &= ~(1 << pin);
#define GPIO2_RISING_IRQ_PENDING(pin)       LPC_GPIOINT->IO2IntStatR & (1 << pin)
#define GPIO2_FALLING_IRQ_PENDING(pin)      LPC_GPIOINT->IO2IntStatF & (1 << pin)
#define GPIO2_CLEAR_IRQ(pin)                LPC_GPIOINT->IO2IntClr |= (1 << pin)
#define GPIO2_IRQ_PENDING()                 LPC_GPIOINT->IntStatus & (1 << 2)

#include <LPC17xx.h>

#endif
