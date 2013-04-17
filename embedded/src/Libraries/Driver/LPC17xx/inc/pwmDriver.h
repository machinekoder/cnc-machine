#pragma once

#include <LPC17xx.h>

#define PCONP_PWM           (1 << 6)            //set pwm1/clock control bit
#define PWM_PDN             (1 << 21)           // 1 PWM1 on, 0 PWM1 off
#define PCLK_PWM            (1 << 24)           // 0b00 CCLK/4, 0b01 CCLK, 0b10 CCLK/2, 0b11 CCLK/8
#define PWM_SET_CORE_CLK()  LPC_SC->PCLKSEL0 |= (0b00<< 12); //set 12:13 for PWM1
#define SYSTEM_CLK          SystemCoreClock
#define PWM_CLK             SYSTEM_CLK/4
#define PWM_PR(n)           LPC_PWM1->PR = n

#define PWM_ENABLE_POWER()  LPC_SC->PCONP |= PCONP_PWM                  //enable PCONP for PWM
#define PWM_DISABLE_POWER() LPC_SC->PCONP &= ~PCONP_PWM                 //set PCLKSEL for PWM

#define PWM_ENABLE_PWM1_1()   LPC_PINCON->PINSEL3  |= (0b10 << 4); LPC_PINCON->PINMODE3 |= (0b00 << 4)  // P1.18 pull-up Resistor
#define PWM_ENABLE_PWM1_2()   LPC_PINCON->PINSEL3  |= (0b10 << 8); LPC_PINCON->PINMODE3 |= (0b00 << 8)  // P1.20 pull-up Resistor
#define PWM_ENABLE_PWM1_3()   LPC_PINCON->PINSEL3  |= (0b10 << 10); LPC_PINCON->PINMODE3 |= (0b00 << 10)  // P1.21 pull-up Resistor
#define PWM_ENABLE_PWM1_4()   LPC_PINCON->PINSEL3  |= (0b10 << 14); LPC_PINCON->PINMODE3 |= (0b00 << 14)  // P1.23 pull-up Resistor
#define PWM_ENABLE_PWM1_5()   LPC_PINCON->PINSEL3  |= (0b10 << 16); LPC_PINCON->PINMODE3 |= (0b00 << 16)  // P1.24 pull-up Resistor
#define PWM_ENABLE_PWM1_6()   LPC_PINCON->PINSEL3  |= (0b10 << 20); LPC_PINCON->PINMODE3 |= (0b00 << 20)// P1.26 pull-up Resistor

#define PWM1_ENABLE_LATCH(mx)      LPC_PWM1->LER |= (1<<mx)                                     //Latch enable for Match mx
#define PWM1_SET_MR0(freq)         LPC_PWM1->MR0 = (uint32)(PWM_CLK/freq);                      //match for main edge
#define PWM1_SET_MR1(freq)         LPC_PWM1->MR1 = (uint32)(duty*(PWM_CLK/freq));               //match for midle edge
#define PWM1_START                 LPC_PWM1->TCR |= (1<<3) | (1 << 0)                           //start PWM mode and counter
#define PWM1_ENABLE_OUTPUT(ch)     LPC_PWM1->PCR |= (1<<(ch+9))                                 //Output enabled for PWM1-5
#define PWM1_DISABLE_OUTPUT(ch)    LPC_PWM1->PCR &= ~(1<<(ch+9))                                // Output disabled for PWM1-5
#define PWM1_SET_0                 LPC_PWM1->TC = LPC_PWM1->MR1-1                               // Nasty but awesome workaround to set the output to 0
#define PWM1_TOGGLE_OUTPUT(ch)     (LPC_PWM1->PCR & (1<<(ch+9))) ? Pwm_stop(ch) : Pwm_start(ch);  // Toggles output for PWM1
