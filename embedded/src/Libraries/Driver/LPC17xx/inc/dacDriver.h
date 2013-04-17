#ifndef _DAC_DRIVER_H_
#define _DAC_DRIVER_H_

#define PCONP_PCADC (1 << 12)
#define PCLK_DAC0   (1 << 22)
#define PCLK_DAC1   (1 << 23)
#define PCLK_ADC0   (1 << 24)
#define PCLK_ADC1   (1 << 25)

#define DACCTRL_INT_DMA_REQ (1 << 0)    // 0 cleared by write, 1 cleared by timeout
#define DACCTRL_DBLBUF_ENA  (1 << 1)    // DACR double buffer
#define DACCTRL_CNT_ENA     (1 << 2)    // Timout conter
#define DACCTRL_DMA_ENA     (1 << 3)    // DMA access

#define DAC_BIAS            (1 << 16)   // 0 settling time 1us, current 700uA; 1 settling time 2.5us, current 350uA

#define VREFP       3300                // Maximum output voltage in mV, normally VCC
#define VREFN       0                   // Minimum output voltage in mV, normally GND
#define DAC_STEPS   1024                // Steps the DAC has, 2^Bits


#define DAC_SET_CLK()                           LPC_SC->PCLKSEL0 &= ~(0b11 << 22)
#define DAC_SET_PIN()                           LPC_PINCON->PINSEL1 &= ~(0b11 << 20); LPC_PINCON->PINSEL1 |= (0b10 << 20)
#define DAC_SET_COUNT_VALUE(x)                  LPC_DAC->DACCNTVAL = x
#define DAC_ENABLE_DOUBLE_BUFFER_AND_TIMEOUT()  LPC_DAC->DACCTRL = DACCTRL_DBLBUF_ENA | DACCTRL_CNT_ENA
#define DAC_SET_VALUE(x)                        LPC_DAC->DACR = (x << 6) | DAC_BIAS;

#include <LPC17xx.h>

#endif
