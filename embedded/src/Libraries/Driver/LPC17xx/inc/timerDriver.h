#ifndef _TIMERDRIVER_H_
#define _TIMERDRIVER_H_

#define SYSTEMCLK SystemCoreClock
#define OPTIMIZE_PRESCALER_US()         (SYSTEMCLK/1E6)-1
#define OPTIMIZE_PRESCALER_MS()         (SYSTEMCLK/1E3)-1

#define PROZESSOR_SLEEP()               LPC_SC->PCON = 0x00;
#define PROZESSOR_WAIT_FOR_IRQ()        __WFI()

#define LOW_IRQ_PRIORITY                1
#define HIGH_IRQ_PRIORITY               0

#define TIMER0_ENABLE_POWER()           LPC_SC->PCONP |= (1 << 1);
#define TIMER0_SET_CORE_CLK_DIVISOR_1() LPC_SC->PCLKSEL0 &= ~(0b11 << 2); LPC_SC->PCLKSEL0 |= (0b01 << 2);
#define TIMER0_SET_CORE_CLK_DIVISOR_2() LPC_SC->PCLKSEL0 &= ~(0b11 << 2); LPC_SC->PCLKSEL0 |= (0b10 << 2);
#define TIMER0_SET_CORE_CLK_DIVISOR_4() LPC_SC->PCLKSEL0 &= ~(0b11 << 2); 
#define TIMER0_SET_CORE_CLK_DIVISOR_8() LPC_SC->PCLKSEL0 |= (0b11 << 2);
#define TIMER0_RESET()                  LPC_TIM0->TCR = 0x02
#define TIMER0_START()                  LPC_TIM0->TCR = 0x01
#define TIMER0_STOP()                   LPC_TIM0->TCR = 0x00
#define TIMER0_RUNNING()                (LPC_TIM0->TCR & 0x01)  // Here the PCONP should not be checked, but why???
#define TIMER0_SET_PRESCALER(x)         LPC_TIM0->PR = (uint32)(x)
#define TIMER0_GET_PRESCALER()          LPC_TIM0->PR
#define TIMER0_SET_KHZ(x)               TIMER0_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E3))-1)
#define TIMER0_SET_MHZ(x)               TIMER0_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E6))-1)
#define TIMER0_RESET_IRQS()             LPC_TIM0->IR  = 0xff
#define TIMER0_SET_INTERVAL_MS(x)       LPC_TIM0->MR0 = (uint32)(x*(SYSTEMCLK/1000/(LPC_TIM0->PR+1)))-1
#define TIMER0_SET_INTERVAL_US(x)       LPC_TIM0->MR0 = (uint32)(x*(SYSTEMCLK/1000000/(LPC_TIM0->PR+1)))-1
#define TIMER0_SET_MATCH_REGISTER_0(x)  LPC_TIM0->MR0 = x
#define TIMER0_ENABLE_IRQ()             NVIC_EnableIRQ(TIMER0_IRQn)
#define TIMER0_DISABLE_IRQ()            NVIC_DisableIRQ(TIMER0_IRQn)
#define TIMER0_SET_IRQ_PRIORITY(x)                NVIC_SetPriority(TIMER0_IRQn,x)
#define TIMER0_RESET_AND_IRQ_ON_MATCH()           LPC_TIM0->MCR = (1 << 0) | (1 << 1)
#define TIMER0_RESET_STOP_AND_IRQ_ON_MATCH()      LPC_TIM0->MCR = (1 << 0) | (1 << 1) | (1 << 2)
#define TIMER0_STOP_AND_IRQ_ON_MATCH()            LPC_TIM0->MCR = (1 << 0) | (1 << 2)
#define TIMER0_IRQHANDLER()             TIMER0_IRQHandler(void)
#define TIMER0_COUNTER_VALUE()          LPC_TIM0->TC

#define TIMER1_ENABLE_POWER()           LPC_SC->PCONP |= (1 << 2);
#define TIMER1_SET_CORE_CLK_DIVISOR_1() LPC_SC->PCLKSEL0 &= ~(0b11 << 4); LPC_SC->PCLKSEL0 |= (0b01 << 4);
#define TIMER1_SET_CORE_CLK_DIVISOR_2() LPC_SC->PCLKSEL0 &= ~(0b11 << 4); LPC_SC->PCLKSEL0 |= (0b10 << 4);
#define TIMER1_SET_CORE_CLK_DIVISOR_4() LPC_SC->PCLKSEL0 &= ~(0b11 << 4); 
#define TIMER1_SET_CORE_CLK_DIVISOR_8() LPC_SC->PCLKSEL0 |= (0b11 << 4);
#define TIMER1_RESET()                  LPC_TIM1->TCR = 0x02
#define TIMER1_START()                  LPC_TIM1->TCR = 0x01
#define TIMER1_STOP()                   LPC_TIM1->TCR = 0x00
#define TIMER1_RUNNING()                (LPC_TIM1->TCR & 0x01)
#define TIMER1_SET_PRESCALER(x)         LPC_TIM1->PR = (uint32)(x)
#define TIMER1_GET_PRESCALER()          LPC_TIM1->PR
#define TIMER1_SET_KHZ(x)               TIMER1_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E3))-1)
#define TIMER1_SET_MHZ(x)               TIMER1_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E6))-1)
#define TIMER1_RESET_IRQS()             LPC_TIM1->IR  = 0xff
#define TIMER1_SET_INTERVAL_MS(x)       LPC_TIM1->MR0 = (uint32)(x*(SYSTEMCLK/1000/(LPC_TIM1->PR+1)))-1
#define TIMER1_SET_INTERVAL_US(x)       LPC_TIM1->MR0 = (uint32)(x*(SYSTEMCLK/1000000/(LPC_TIM1->PR+1)))-1
#define TIMER1_SET_MATCH_REGISTER_0(x)  LPC_TIM1->MR0 = x
#define TIMER1_ENABLE_IRQ()             NVIC_EnableIRQ(TIMER1_IRQn)
#define TIMER1_DISABLE_IRQ()            NVIC_DisableIRQ(TIMER1_IRQn)
#define TIMER1_SET_IRQ_PRIORITY(x)                NVIC_SetPriority(TIMER1_IRQn,x)
#define TIMER1_RESET_AND_IRQ_ON_MATCH()           LPC_TIM1->MCR = (1 << 0) | (1 << 1)
#define TIMER1_RESET_STOP_AND_IRQ_ON_MATCH()      LPC_TIM1->MCR = (1 << 0) | (1 << 1) | (1 << 2)
#define TIMER1_STOP_AND_IRQ_ON_MATCH()            LPC_TIM1->MCR = (1 << 0) | (1 << 2)
#define TIMER1_IRQHANDLER()             TIMER1_IRQHandler(void)
#define TIMER1_COUNTER_VALUE()          LPC_TIM1->TC

#define TIMER2_ENABLE_POWER()           LPC_SC->PCONP |= (1 << 22);
#define TIMER2_SET_CORE_CLK_DIVISOR_1() LPC_SC->PCLKSEL1 &= ~(0b11 << 12); LPC_SC->PCLKSEL1 |= (0b01 << 12);
#define TIMER2_SET_CORE_CLK_DIVISOR_2() LPC_SC->PCLKSEL1 &= ~(0b11 << 12); LPC_SC->PCLKSEL1 |= (0b10 << 12);
#define TIMER2_SET_CORE_CLK_DIVISOR_4() LPC_SC->PCLKSEL1 &= ~(0b11 << 12); 
#define TIMER2_SET_CORE_CLK_DIVISOR_8() LPC_SC->PCLKSEL1 |= (0b11 << 12);
#define TIMER2_RESET()                  LPC_TIM2->TCR = 0x02
#define TIMER2_START()                  LPC_TIM2->TCR = 0x01
#define TIMER2_STOP()                   LPC_TIM2->TCR = 0x00
#define TIMER2_RUNNING()                (LPC_SC->PCONP & (1 << 22)) && (LPC_TIM2->TCR & 0x01)   // Here the PCONP must be checked, but why???
#define TIMER2_SET_PRESCALER(x)         LPC_TIM2->PR = (uint32)(x)
#define TIMER2_GET_PRESCALER()          LPC_TIM2->PR
#define TIMER2_SET_KHZ(x)               TIMER2_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E3))-1)
#define TIMER2_SET_MHZ(x)               TIMER2_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E6))-1)
#define TIMER2_SET_MATCH_REGISTER_0(x)  LPC_TIM2->MR0 = x
#define TIMER2_RESET_IRQS()             LPC_TIM2->IR  = 0xff
#define TIMER2_SET_INTERVAL_MS(x)       LPC_TIM2->MR0 = (uint32)(x*(SYSTEMCLK/1000/(LPC_TIM2->PR+1)))-1
#define TIMER2_SET_INTERVAL_US(x)       LPC_TIM2->MR0 = (uint32)(x*(SYSTEMCLK/1000000/(LPC_TIM2->PR+1)))-1
#define TIMER2_ENABLE_IRQ()             NVIC_EnableIRQ(TIMER2_IRQn)
#define TIMER2_DISABLE_IRQ()            NVIC_DisableIRQ(TIMER2_IRQn)
#define TIMER2_SET_IRQ_PRIORITY(x)                NVIC_SetPriority(TIMER2_IRQn,x)
#define TIMER2_RESET_AND_IRQ_ON_MATCH()           LPC_TIM2->MCR = (1 << 0) | (1 << 1)
#define TIMER2_RESET_STOP_AND_IRQ_ON_MATCH()      LPC_TIM2->MCR = (1 << 0) | (1 << 1) | (1 << 2)
#define TIMER2_STOP_AND_IRQ_ON_MATCH()            LPC_TIM2->MCR = (1 << 0) | (1 << 2)
#define TIMER2_IRQHANDLER()             TIMER2_IRQHandler(void)
#define TIMER2_COUNTER_VALUE()          LPC_TIM2->TC

#define TIMER3_ENABLE_POWER()           LPC_SC->PCONP |= (1 << 23);
#define TIMER3_SET_CORE_CLK_DIVISOR_1() LPC_SC->PCLKSEL1 &= ~(0b11 << 14); LPC_SC->PCLKSEL1 |= (0b01 << 14);
#define TIMER3_SET_CORE_CLK_DIVISOR_2() LPC_SC->PCLKSEL1 &= ~(0b11 << 14); LPC_SC->PCLKSEL1 |= (0b10 << 14);
#define TIMER3_SET_CORE_CLK_DIVISOR_4() LPC_SC->PCLKSEL1 &= ~(0b11 << 14); 
#define TIMER3_SET_CORE_CLK_DIVISOR_8() LPC_SC->PCLKSEL1 |= (0b11 << 14);
#define TIMER3_RESET()                  LPC_TIM3->TCR = 0x02
#define TIMER3_START()                  LPC_TIM3->TCR = 0x01
#define TIMER3_STOP()                   LPC_TIM3->TCR = 0x00
#define TIMER3_RUNNING()                (LPC_SC->PCONP & (1 << 23)) && (LPC_TIM3->TCR & 0x01)
#define TIMER3_SET_PRESCALER(x)         LPC_TIM3->PR = (uint32)(x)
#define TIMER3_GET_PRESCALER()          LPC_TIM3->PR
#define TIMER3_SET_KHZ(x)               TIMER3_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E3))-1)
#define TIMER3_SET_MHZ(x)               TIMER3_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E6))-1)
#define TIMER3_SET_MATCH_REGISTER_0(x)  LPC_TIM3->MR0 = x
#define TIMER3_RESET_IRQS()             LPC_TIM3->IR  = 0xff
#define TIMER3_SET_INTERVAL_MS(x)       LPC_TIM3->MR0 = (uint32)(x*(SYSTEMCLK/1000/(LPC_TIM3->PR+1)))-1
#define TIMER3_SET_INTERVAL_US(x)       LPC_TIM3->MR0 = (uint32)(x*(SYSTEMCLK/1000000/(LPC_TIM3->PR+1)) )-1
#define TIMER3_ENABLE_IRQ()             NVIC_EnableIRQ(TIMER3_IRQn)
#define TIMER3_DISABLE_IRQ()            NVIC_DisableIRQ(TIMER3_IRQn)
#define TIMER3_SET_IRQ_PRIORITY(x)                NVIC_SetPriority(TIMER3_IRQn,x)
#define TIMER3_RESET_AND_IRQ_ON_MATCH()           LPC_TIM3->MCR = (1 << 0) | (1 << 1)
#define TIMER3_RESET_STOP_AND_IRQ_ON_MATCH()      LPC_TIM3->MCR = (1 << 0) | (1 << 1) | (1 << 2)
#define TIMER3_RESET_ON_MATCH()                   LPC_TIM3->MCR = (1 << 1);
#define TIMER3_STOP_AND_IRQ_ON_MATCH()            LPC_TIM3->MCR = (1 << 0) | (1 << 2)
#define TIMER3_IRQHANDLER()             TIMER3_IRQHandler(void)
#define TIMER3_COUNTER_VALUE()          LPC_TIM3->TC

#define CAPCOM_ENABLE_PIN0()   LPC_PINCON->PINSEL3  |= (0b11 << 20) // ; LPC_PINCON->PINMODE3 |= (0b10 << 20)  // CAP0.0   P1.26
#define CAPCOM_ENABLE_PIN1()   LPC_PINCON->PINSEL3  |= (0b11 <<  4) // ; LPC_PINCON->PINMODE3 |= (0b10 <<  4)  // CAP1.0   P1.18
#define CAPCOM_ENABLE_PIN2()   LPC_PINCON->PINSEL0  |= (0b11 <<  8) // ; LPC_PINCON->PINMODE0 |= (0b10 <<  8)  // CAP2.0   P0.4
#define CAPCOM_ENABLE_PIN3()   LPC_PINCON->PINSEL1  |= (0b11 << 14) // ; LPC_PINCON->PINMODE1 |= (0b10 << 14)  // CAP3.0   P0.23
#define CAPCOM_ENABLE_PIN4()   LPC_PINCON->PINSEL3  |= (0b11 << 22) // ; LPC_PINCON->PINMODE3 |= (0b10 << 22)  // CAP0.1   P1.27
#define CAPCOM_ENABLE_PIN5()   LPC_PINCON->PINSEL3  |= (0b11 <<  6) // ; LPC_PINCON->PINMODE3 |= (0b10 <<  6)  // CAP1.1   P1.19
#define CAPCOM_ENABLE_PIN6()   LPC_PINCON->PINSEL0  |= (0b11 << 10) // ; LPC_PINCON->PINMODE0 |= (0b10 << 10)  // CAP2.1   P0.5
#define CAPCOM_ENABLE_PIN7()   LPC_PINCON->PINSEL1  |= (0b11 << 16) // ; LPC_PINCON->PINMODE1 |= (0b10 << 16)  // CAP3.1   P0.24

#define CAPCOM0_ENABLE_CAP0()  LPC_TIM0->CCR |= (1 << 0) | (1 << 1) | (1 << 2)   //CAP0 on rising and falling edge and interrupt
#define CAPCOM0_ENABLE_CAP1()  LPC_TIM0->CCR |= (1 << 3) | (1 << 4) | (1 << 5)   //CAP1 on rising and falling edge and interrupt

#define CAPCOM1_ENABLE_CAP0()  LPC_TIM1->CCR |= (1 << 0) | (1 << 1) | (1 << 2)   //CAP0 on rising and falling edge and interrupt
#define CAPCOM1_ENABLE_CAP1()  LPC_TIM1->CCR |= (1 << 3) | (1 << 4) | (1 << 5)   //CAP1 on rising and falling edge and interrupt

#define CAPCOM2_ENABLE_CAP0()  LPC_TIM2->CCR |= (1 << 0) | (1 << 1) | (1 << 2)   //CAP0 on rising and falling edge and interrupt
#define CAPCOM2_ENABLE_CAP1()  LPC_TIM2->CCR |= (1 << 3) | (1 << 4) | (1 << 5)   //CAP1 on rising and falling edge and interrupt

#define CAPCOM3_ENABLE_CAP0()  LPC_TIM3->CCR |= (1 << 0) | (1 << 1) | (1 << 2)   //CAP0 on rising and falling edge and interrupt
#define CAPCOM3_ENABLE_CAP1()  LPC_TIM3->CCR |= (1 << 3) | (1 << 4) | (1 << 5)   //CAP1 on rising and falling edge and interrupt

#include <LPC17xx.h>

#endif
