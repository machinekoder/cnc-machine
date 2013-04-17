#ifndef _ADC_DRIVER_H_
#define _ADC_DRIVER_H_

#define PCONP_PCADC     (1 << 12)
#define ADCR_PDN        (1 << 21)     // 1 ADC on, 0 ADC off
#define ADCR_BURST      (1 << 16)     // 1 Enables burst mode

#define PCLK_ADC        (1 << 24)     // 0b00 CCLK/4, 0b01 CCLK, 0b10 CCLK/2, 0b11 CCLK/8
#define SYSTEM_CLK      SystemCoreClock
#define ADC_CLK         SYSTEM_CLK/4

#define ADC_DONE            0x80000000
#define ADC_OVERRUN         0x40000000


#define ADC_ENABLE_POWER()  LPC_SC->PCONP |= PCONP_PCADC
#define ADC_DISABLE_POWER() LPC_SC->PCONP &= ~PCONP_PCADC
#define ADC_SET_CORE_CLK()  LPC_SC->PCLKSEL0 &= ~(0b11 << 24); LPC_SC->PCLKSEL0 |= (0b00 << 24)

#define ADC_CLEAR_PINSEL()  LPC_ADC->ADCR &= ~0xFF
#define ADC_ENABLE_PIN0()   LPC_PINCON->PINSEL1  |= (0b01 << 14); LPC_PINCON->PINMODE1 |= (0b10 << 14)  // AD0.0   P0.23
#define ADC_ENABLE_PIN1()   LPC_PINCON->PINSEL1  |= (0b01 << 16); LPC_PINCON->PINMODE1 |= (0b10 << 16)  // AD0.1   P0.24
#define ADC_ENABLE_PIN2()   LPC_PINCON->PINSEL1  |= (0b01 << 18); LPC_PINCON->PINMODE1 |= (0b10 << 18)  // AD0.2   P0.25
#define ADC_ENABLE_PIN3()   LPC_PINCON->PINSEL1  |= (0b01 << 20); LPC_PINCON->PINMODE1 |= (0b10 << 20)  // AD0.3   P0.26
#define ADC_ENABLE_PIN4()   LPC_PINCON->PINSEL3  |= (0b11 << 28); LPC_PINCON->PINMODE3 |= (0b10 << 28)  // AD0.4   P1.30
#define ADC_ENABLE_PIN5()   LPC_PINCON->PINSEL3  |= (0b11 << 30); LPC_PINCON->PINMODE3 |= (0b10 << 30)  // AD0.5   P1.31
#define ADC_ENABLE_PIN6()   LPC_PINCON->PINSEL0  |= (0b10 << 6);  LPC_PINCON->PINMODE0 |= (0b10 << 6)   // AD0.6   P0.3
#define ADC_ENABLE_PIN7()   LPC_PINCON->PINSEL0  |= (0b10 << 4);  LPC_PINCON->PINMODE0 |= (0b10 << 4);   // AD0.7   P0.2
#define ADC_SET_PIN(x)      LPC_ADC->ADCR        |= (1 << x)
#define ADC_CLEAR_PIN(x)    LPC_ADC->ADCR        &= ~(1 << x)
#define ADC_READ_PIN0()     LPC_ADC->ADDR0
#define ADC_READ_PIN1()     LPC_ADC->ADDR1
#define ADC_READ_PIN2()     LPC_ADC->ADDR2
#define ADC_READ_PIN3()     LPC_ADC->ADDR3
#define ADC_READ_PIN4()     LPC_ADC->ADDR4
#define ADC_READ_PIN5()     LPC_ADC->ADDR5
#define ADC_READ_PIN6()     LPC_ADC->ADDR6
#define ADC_READ_PIN7()     LPC_ADC->ADDR7

#define ADC_SET_CLK(x)      LPC_ADC->ADCR |= (((uint16)(ADC_CLK/x) - 1) << 8);
#define ADC_CONFIGURE()     LPC_ADC->ADCR |= (0 << 24); LPC_ADC->ADCR |= (0 << 27); // No start and CAP/MAT rising edge trigger
#define ADC_ENABLE()        LPC_ADC->ADCR |= ADCR_PDN
#define ADC_DISABLE()       LPC_ADC->ADCR &= ~ADCR_PDN
#define ADC_SET_BURSTMODE() LPC_ADC->ADCR |= ADCR_BURST 

#define ADC_ENABLE_IRQ()            NVIC_EnableIRQ(ADC_IRQn)
#define ADC_DISABLE_IRQ()           NVIC_DisableIRQ(ADC_IRQn)
#define ADC_SET_ALL_IRQS_BURST()    LPC_ADC->ADINTEN = 0xFF
#define ADC_SET_ALL_IRQS()          LPC_ADC->ADINTEN = 0x1FF

#define ADC_START()         LPC_ADC->ADCR |= (1 << 24)
#define ADC_STOP()          LPC_ADC->ADCR &= ~(0x7 << 24)

// Working with the ADC output register
#define ADC_HAS_OVERRUN(x)  x & ADC_OVERRUN
#define ADC_GET_VALUE(x)    (x >> 4) & 0xFFFF;

#include <LPC17xx.h>

#endif
