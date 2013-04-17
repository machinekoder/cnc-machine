#ifndef _PINCONDRIVER_H_
#define _PINCONDRIVER_H_

#define PINCON_PRIMARY_FUNCTION 0b00
#define PINCON_FIRST_ALTERNATE_FUNCTION 0b01
#define PINCON_SECOND_ALTERNATE_FUNCTION 0b10
#define PINCON_THIRD_ALTERNATE_FUNCTION 0b11

#define PINCON_MODE_PULL_UP     0b00
#define PINCON_MODE_REPEATER    0b01
#define PINCON_MODE_NO_PULL_UP_DOWN 0b10
#define PINCON_MODE_PULL_DOWN   0b11

#define PINSEL_CLEAR_PORT0(pin,functionBits)   (pin < 16)? (LPC_PINCON->PINSEL0 &= ~(0b11 << (pin*2))) : (LPC_PINCON->PINSEL1 &= ~(0b11 << ((pin-16)*2)))
#define PINSEL_SET_PORT0(pin,functionBits)     (pin < 16)? (LPC_PINCON->PINSEL0 |=  (functionBits << (pin*2))) : (LPC_PINCON->PINSEL1 |= (functionBits << ((pin-16)*2)))
#define PINSEL_CLEAR_PORT1(pin,functionBits)   (pin < 16)? (LPC_PINCON->PINSEL2 &= ~(0b11 << (pin*2))) : (LPC_PINCON->PINSEL3 &= ~(0b11 << ((pin-16)*2)))
#define PINSEL_SET_PORT1(pin,functionBits)     (pin < 16)? (LPC_PINCON->PINSEL2 |=  (functionBits << (pin*2))) : (LPC_PINCON->PINSEL3 |= (functionBits << ((pin-16)*2)))
#define PINSEL_CLEAR_PORT2(pin,functionBits)   (pin < 16)? (LPC_PINCON->PINSEL4 &= ~(0b11 << (pin*2))) : (LPC_PINCON->PINSEL5 &= ~(0b11 << ((pin-16)*2)))
#define PINSEL_SET_PORT2(pin,functionBits)     (pin < 16)? (LPC_PINCON->PINSEL4 |=  (functionBits << (pin*2))) : (LPC_PINCON->PINSEL5 |= (functionBits << ((pin-16)*2)))
#define PINSEL_CLEAR_PORT3(pin,functionBits)   (pin < 16)? (LPC_PINCON->PINSEL6 &= ~(0b11 << (pin*2))) : (LPC_PINCON->PINSEL7 &= ~(0b11 << ((pin-16)*2)))
#define PINSEL_SET_PORT3(pin,functionBits)     (pin < 16)? (LPC_PINCON->PINSEL6 |=  (functionBits << (pin*2))) : (LPC_PINCON->PINSEL7 |= (functionBits << ((pin-16)*2)))
   
#define PINMODE_CLEAR_PORT0(pin,functionBits)   (pin < 16)? (LPC_PINCON->PINMODE0 &= ~(0b11 << (pin*2))) : (LPC_PINCON->PINMODE1 &= ~(0b11 << ((pin-16)*2)))
#define PINMODE_SET_PORT0(pin,functionBits)     (pin < 16)? (LPC_PINCON->PINMODE0 |=  (functionBits << (pin*2))) : (LPC_PINCON->PINMODE1 |= (functionBits << ((pin-16)*2)))
#define PINMODE_CLEAR_PORT1(pin,functionBits)   (pin < 16)? (LPC_PINCON->PINMODE2 &= ~(0b11 << (pin*2))) : (LPC_PINCON->PINMODE3 &= ~(0b11 << ((pin-16)*2)))
#define PINMODE_SET_PORT1(pin,functionBits)     (pin < 16)? (LPC_PINCON->PINMODE2 |=  (functionBits << (pin*2))) : (LPC_PINCON->PINMODE3 |= (functionBits << ((pin-16)*2)))
#define PINMODE_CLEAR_PORT2(pin,functionBits)   (pin < 16)? (LPC_PINCON->PINMODE4 &= ~(0b11 << (pin*2))) : (LPC_PINCON->PINMODE5 &= ~(0b11 << ((pin-16)*2)))
#define PINMODE_SET_PORT2(pin,functionBits)     (pin < 16)? (LPC_PINCON->PINMODE4 |=  (functionBits << (pin*2))) : (LPC_PINCON->PINMODE5 |= (functionBits << ((pin-16)*2)))
#define PINMODE_CLEAR_PORT3(pin,functionBits)   (pin < 16)? (LPC_PINCON->PINMODE6 &= ~(0b11 << (pin*2))) : (LPC_PINCON->PINMODE7 &= ~(0b11 << ((pin-16)*2)))
#define PINMODE_SET_PORT3(pin,functionBits)     (pin < 16)? (LPC_PINCON->PINMODE6 |=  (functionBits << (pin*2))) : (LPC_PINCON->PINMODE7 |= (functionBits << ((pin-16)*2)))

#define PINODMODE_CLEAR_PORT0(pin)              LPC_PINCON->PINMODE_OD0 &= (1 << pin)
#define PINODMODE_SET_PORT0(pin,mode)           LPC_PINCON->PINMODE_OD0 |= (mode << pin)
#define PINODMODE_CLEAR_PORT1(pin)              LPC_PINCON->PINMODE_OD1 &= (1 << pin)
#define PINODMODE_SET_PORT1(pin,mode)           LPC_PINCON->PINMODE_OD1 |= (mode << pin)
#define PINODMODE_CLEAR_PORT2(pin)              LPC_PINCON->PINMODE_OD2 &= (1 << pin)
#define PINODMODE_SET_PORT2(pin,mode)           LPC_PINCON->PINMODE_OD2 |= (mode << pin)
#define PINODMODE_CLEAR_PORT3(pin)              LPC_PINCON->PINMODE_OD3 &= (1 << pin)
#define PINODMODE_SET_PORT3(pin,mode)           LPC_PINCON->PINMODE_OD3 |= (mode << pin)


#include <LPC17xx.h>
#endif
