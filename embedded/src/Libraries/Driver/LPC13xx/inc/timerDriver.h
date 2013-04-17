#ifndef _TIMERDRIVER_H_
#define _TIMERDRIVER_H_

#define SYSTEMCLK SystemCoreClock
#define OPTIMIZE_PRESCALER_US()         (SYSTEMCLK/1E6)-1
#define OPTIMIZE_PRESCALER_MS()         (SYSTEMCLK/1E3)-1

#define PROZESSOR_SLEEP()               LPC_PMU->PCON &= ~(1<<1)
#define PROZESSOR_WAIT_FOR_IRQ()  __WFI()

#define LOW_IRQ_PRIORITY          1
#define HIGH_IRQ_PRIORITY         0

#define TIMER0_ENABLE_CLK()             LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 9)
#define TIMER0_DISABLE_CLK()            LPC_SYSCON->SYSAHBCLKCTRL &= ~(1 << 9)
#define TIMER0_RESET()                  LPC_TMR32B0->TCR = 0x02
#define TIMER0_START()                  LPC_TMR32B0->TCR = 0x01
#define TIMER0_STOP()                   LPC_TMR32B0->TCR = 0x00
#define TIMER0_RUNNING()                LPC_TMR32B0->TCR & 0x01
#define TIMER0_SET_PRESCALER(x)         LPC_TMR32B0->PR = (uint32)(x)
#define TIMER0_SET_KHZ(x)               TIMER0_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E3))-1)
#define TIMER0_SET_MHZ(x)               TIMER0_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E6))-1)
#define TIMER0_RESET_IRQS()             LPC_TMR32B0->IR  = 0xff
#define TIMER0_SET_INTERVAL_MS(x)       LPC_TMR32B0->MR0 = (uint32)x*SYSTEMCLK/(LPC_TMR32B0->PR+1) / 1000
#define TIMER0_SET_INTERVAL_US(x)       LPC_TMR32B0->MR0 = (uint32)x*SYSTEMCLK/(LPC_TMR32B0->PR+1) / 1000000
#define TIMER0_ENABLE_IRQ()             NVIC_EnableIRQ(TIMER_32_0_IRQn)
#define TIMER0_DISABLE_IRQ()            NVIC_DisableIRQ(TIMER_32_0_IRQn)
#define TIMER0_SET_IRQ_PRIORITY(x)                NVIC_SetPriority(TIMER_32_0_IRQn,x)
#define TIMER0_RESET_AND_IRQ_ON_MATCH()           LPC_TMR32B0->MCR = (1 << 0) | (1 << 1)
#define TIMER0_RESET_STOP_AND_IRQ_ON_MATCH()      LPC_TMR32B0->MCR = (1 << 0) | (1 << 1) | (1 << 2)
#define TIMER0_STOP_AND_IRQ_ON_MATCH()            LPC_TMR32B0->MCR = (1 << 0) | (1 << 2)
#define TIMER0_IRQHANDLER()             TIMER32_0_IRQHandler(void)

#define TIMER1_ENABLE_CLK()             LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 10)
#define TIMER1_DISABLE_CLK()            LPC_SYSCON->SYSAHBCLKCTRL &= ~(1 << 10)
#define TIMER1_RESET()                  LPC_TMR32B1->TCR = 0x02
#define TIMER1_START()                  LPC_TMR32B1->TCR = 0x01
#define TIMER1_STOP()                   LPC_TMR32B1->TCR = 0x00
#define TIMER1_RUNNING()                LPC_TMR32B1->TCR & 0x01
#define TIMER1_SET_PRESCALER(x)         LPC_TMR32B1->PR = (uint32)(x)
#define TIMER1_SET_KHZ(x)               TIMER1_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E3))-1)
#define TIMER1_SET_MHZ(x)               TIMER1_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E6))-1)
#define TIMER1_RESET_IRQS()             LPC_TMR32B1->IR  = 0xff
#define TIMER1_SET_INTERVAL_MS(x)       LPC_TMR32B1->MR0 = (uint32)x*SYSTEMCLK/(LPC_TMR32B1->PR+1) / 1000
#define TIMER1_SET_INTERVAL_US(x)       LPC_TMR32B1->MR0 = (uint32)x*SYSTEMCLK/(LPC_TMR32B1->PR+1) / 1000000
#define TIMER1_ENABLE_IRQ()             NVIC_EnableIRQ(TIMER_32_1_IRQn)
#define TIMER1_DISABLE_IRQ()            NVIC_DisableIRQ(TIMER_32_1_IRQn)
#define TIMER1_SET_IRQ_PRIORITY(x)                NVIC_SetPriority(TIMER_32_1_IRQn,x)
#define TIMER1_RESET_AND_IRQ_ON_MATCH()           LPC_TMR32B1->MCR = (1 << 0) | (1 << 1)
#define TIMER1_RESET_STOP_AND_IRQ_ON_MATCH()      LPC_TMR32B1->MCR = (1 << 0) | (1 << 1) | (1 << 2)
#define TIMER1_STOP_AND_IRQ_ON_MATCH()            LPC_TMR32B1->MCR = (1 << 0) | (1 << 2)
#define TIMER1_IRQHANDLER()             TIMER32_1_IRQHandler(void)

#define TIMER2_ENABLE_CLK()             LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 7)
#define TIMER2_DISABLE_CLK()            LPC_SYSCON->SYSAHBCLKCTRL &= ~(1 << 7)
#define TIMER2_RESET()                  LPC_TMR16B0->TCR = 0x02
#define TIMER2_START()                  LPC_TMR16B0->TCR = 0x01
#define TIMER2_STOP()                   LPC_TMR16B0->TCR = 0x00
#define TIMER2_RUNNING()                LPC_TMR16B0->TCR & 0x01
#define TIMER2_SET_PRESCALER(x)         LPC_TMR16B0->PR = (uint32)(x)
#define TIMER2_SET_KHZ(x)               TIMER2_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E3))-1)
#define TIMER2_SET_MHZ(x)               TIMER2_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E6))-1)
#define TIMER2_RESET_IRQS()             LPC_TMR16B0->IR  = 0xff
#define TIMER2_SET_INTERVAL_MS(x)       LPC_TMR16B0->MR0 = (uint32)x*SYSTEMCLK/(LPC_TMR16B0->PR+1) / 1000
#define TIMER2_SET_INTERVAL_US(x)       LPC_TMR16B0->MR0 = (uint32)x*SYSTEMCLK/(LPC_TMR16B0->PR+1) / 1000000
#define TIMER2_ENABLE_IRQ()             NVIC_EnableIRQ(TIMER_16_0_IRQn)
#define TIMER2_DISABLE_IRQ()            NVIC_DisableIRQ(TIMER_16_0_IRQn)
#define TIMER2_SET_IRQ_PRIORITY(x)                NVIC_SetPriority(TIMER_16_0_IRQn,x)
#define TIMER2_RESET_AND_IRQ_ON_MATCH()           LPC_TMR16B0->MCR = (1 << 0) | (1 << 1)
#define TIMER2_RESET_STOP_AND_IRQ_ON_MATCH()      LPC_TMR16B0->MCR = (1 << 0) | (1 << 1) | (1 << 2)
#define TIMER2_STOP_AND_IRQ_ON_MATCH()            LPC_TMR16B0->MCR = (1 << 0) | (1 << 2)
#define TIMER2_IRQHANDLER()             TIMER16_0_IRQHandler(void)

#define TIMER3_ENABLE_CLK()             LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 8)
#define TIMER3_DISABLE_CLK()            LPC_SYSCON->SYSAHBCLKCTRL &= ~(1 << 8)
#define TIMER3_RESET()                  LPC_TMR16B1->TCR = 0x02
#define TIMER3_START()                  LPC_TMR16B1->TCR = 0x01
#define TIMER3_STOP()                   LPC_TMR16B1->TCR = 0x00
#define TIMER3_RUNNING()                LPC_TMR16B1->TCR & 0x01
#define TIMER3_SET_PRESCALER(x)         LPC_TMR16B1->PR = (uint32)(x)
#define TIMER3_SET_KHZ(x)               TIMER3_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E3))-1)
#define TIMER3_SET_MHZ(x)               TIMER3_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E6))-1)
#define TIMER3_RESET_IRQS()             LPC_TMR16B1->IR  = 0xff
#define TIMER3_SET_INTERVAL_MS(x)       LPC_TMR16B1->MR0 = (uint32)x*SYSTEMCLK/(LPC_TMR16B1->PR+1) / 1000
#define TIMER3_SET_INTERVAL_US(x)       LPC_TMR16B1->MR0 = (uint32)x*SYSTEMCLK/(LPC_TMR16B1->PR+1) / 1000000
#define TIMER3_ENABLE_IRQ()             NVIC_EnableIRQ(TIMER_16_1_IRQn)
#define TIMER3_DISABLE_IRQ()            NVIC_DisableIRQ(TIMER_16_1_IRQn)
#define TIMER3_SET_IRQ_PRIORITY(x)                NVIC_SetPriority(TIMER_16_1_IRQn,x)
#define TIMER3_RESET_AND_IRQ_ON_MATCH()           LPC_TMR16B1->MCR = (1 << 0) | (1 << 1)
#define TIMER3_RESET_STOP_AND_IRQ_ON_MATCH()      LPC_TMR16B1->MCR = (1 << 0) | (1 << 1) | (1 << 2)
#define TIMER3_STOP_AND_IRQ_ON_MATCH()            LPC_TMR16B1->MCR = (1 << 0) | (1 << 2)
#define TIMER3_IRQHANDLER()             TIMER16_1_IRQHandler(void)

#include <LPC13xx.h>
#include <types.h>

#endif
