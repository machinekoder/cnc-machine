#include <lpc17xx_nvic.h>

void __cs3_stack();
void __cs3_reset();
/* Core interrupts */
void NMI_Handler();
void HardFault_Handler();
void MemManage_Handler();
void BusFault_Handler();
void UsageFault_Handler();
void SVC_Handler();
void DebugMon_Handler();
void PendSV_Handler();
void SysTick_Handler();
/* External peripheral interrupts */
void WDT_IRQHandler();				/* 16: WDT							*/
void TIMER0_IRQHandler();			/* 17: Timer0						*/
void TIMER1_IRQHandler();           /* 18: Timer1                       */
void TIMER2_IRQHandler();           /* 19: Timer2                       */
void TIMER3_IRQHandler();           /* 20: Timer3                       */
void UART0_IRQHandler();            /* 21: UART0                        */
void UART1_IRQHandler();            /* 22: UART1                        */
void UART2_IRQHandler();            /* 23: UART2                        */
void UART3_IRQHandler();            /* 24: UART3                        */
void PWM1_IRQHandler();             /* 25: PWM1                         */
void I2C0_IRQHandler();             /* 26: I2C0                         */
void I2C1_IRQHandler();             /* 27: I2C1                         */
void I2C2_IRQHandler();             /* 28: I2C2                         */
void SPI_IRQHandler();              /* 29: SPI                          */
void SSP0_IRQHandler();             /* 30: SSP0                         */
void SSP1_IRQHandler();             /* 31: SSP1                         */
void PLL0_IRQHandler();             /* 32: PLL0 Lock (Main PLL)         */
void RTC_IRQHandler();              /* 33: Real Time Clock              */
void EINT0_IRQHandler();            /* 34: External Interrupt 0         */
void EINT1_IRQHandler();            /* 35: External Interrupt 1         */
void EINT2_IRQHandler();            /* 36: External Interrupt 2         */
void EINT3_IRQHandler();            /* 37: External Interrupt 3         */
void ADC_IRQHandler();              /* 38: A/D Converter                */
void BOD_IRQHandler();              /* 39: Brown-Out Detect             */
void USB_IRQHandler();              /* 40: USB                          */
void CAN_IRQHandler();              /* 41: CAN                          */
void DMA_IRQHandler();              /* 42: General Purpose DMA          */
void I2S_IRQHandler();              /* 43: I2S                          */
void ENET_IRQHandler();             /* 44: Ethernet                     */
void RIT_IRQHandler();              /* 45: Repetitive Interrupt Timer   */
void MCPWM_IRQHandler();            /* 46: Motor Control PWM            */
void QEI_IRQHandler();              /* 47: Quadrature Encoder Interface */
void PLL1_IRQHandler();             /* 48: PLL1 Lock (USB PLL)          */
void USBActivity_IRQHandler(); 	    /* 49: USB Activity 				*/
void CANActivity_IRQHandler();      /* 50: CAN Activity					*/


/* [1] On page 768 recommends a table allignment of 256 word beeing 1024 bytes allowing for 128 interrupts. */
static void (* const IntVectorTable[])(void) __attribute__ ((aligned (1024))) =
{
      /* Core interrupts */
      __cs3_stack,
      __cs3_reset,
      NMI_Handler,
      HardFault_Handler,
      MemManage_Handler,
      BusFault_Handler,
      UsageFault_Handler,
      0,
      0,
      0,
      0,
      SVC_Handler,
      DebugMon_Handler,
      0,
      PendSV_Handler,
      SysTick_Handler,

      /* External peripheral interrupts */
      WDT_IRQHandler,   	       /* 16: Watchdog Timer               */
      TIMER0_IRQHandler,    	   /* 17: Timer0                       */
      TIMER1_IRQHandler,           /* 18: Timer1                       */
      TIMER2_IRQHandler,            /* 19: Timer2                       */
      TIMER3_IRQHandler,           /* 20: Timer3                       */
      UART0_IRQHandler,            /* 21: UART0                        */
      UART1_IRQHandler,            /* 22: UART1                        */
      UART2_IRQHandler,            /* 23: UART2                        */
      UART3_IRQHandler,            /* 24: UART3                        */
      PWM1_IRQHandler,             /* 25: PWM1                         */
      I2C0_IRQHandler,             /* 26: I2C0                         */
      I2C1_IRQHandler,             /* 27: I2C1                         */
      I2C2_IRQHandler,             /* 28: I2C2                         */
      SPI_IRQHandler,              /* 29: SPI                          */
      SSP0_IRQHandler,             /* 30: SSP0                         */
      SSP1_IRQHandler,             /* 31: SSP1                         */
      PLL0_IRQHandler,             /* 32: PLL0 Lock (Main PLL)         */
      RTC_IRQHandler,              /* 33: Real Time Clock              */
      EINT0_IRQHandler,            /* 34: External Interrupt 0         */
      EINT1_IRQHandler,            /* 35: External Interrupt 1         */
      EINT2_IRQHandler,            /* 36: External Interrupt 2         */
      EINT3_IRQHandler,            /* 37: External Interrupt 3         */
      ADC_IRQHandler,              /* 38: A/D Converter                */
      BOD_IRQHandler,              /* 39: Brown-Out Detect             */
      USB_IRQHandler,              /* 40: USB                          */
      CAN_IRQHandler,              /* 41: CAN                          */
      DMA_IRQHandler,              /* 42: General Purpose DMA          */
      I2S_IRQHandler,              /* 43: I2S                          */
      ENET_IRQHandler,             /* 44: Ethernet                     */
      RIT_IRQHandler,              /* 45: Repetitive Interrupt Timer   */
      MCPWM_IRQHandler,            /* 46: Motor Control PWM            */
      QEI_IRQHandler,              /* 47: Quadrature Encoder Interface */
      PLL1_IRQHandler,             /* 48: PLL1 Lock (USB PLL)          */
      USBActivity_IRQHandler,	   /* 49: USB Activity 				*/
      CANActivity_IRQHandler	   /* 50: CAN Activity					*/

};

/**************************************************************************
 * global external functions
 *************************************************************************/

/**************************************************************************
 * local functions
 *************************************************************************/

/**************************************************************************
 * global functions
 *************************************************************************/

/**
 * @name  AppInit
 * @brief Set the VTOR register of the cortex m3 cpu.
 *
 * Resets all interrupt pending status flags and sets the VTOR
 * register of the cortex m3 cpu to point to our interrupt vector table.
 *
 */
void NVICInit(void)
{
   /* DeInit NVIC and SCBNVIC */
   NVIC_DeInit();
   NVIC_SCBDeInit();
   /* Configure the NVIC Preemption Priority Bits:
    * two (2) bits of preemption priority, six (6) bits of sub-priority.
    * Since the Number of Bits used for Priority Levels is five (5), so the
    * actual bit number of sub-priority is three (3)
    */
   NVIC_SetPriorityGrouping(0x05);
   NVIC_SetVTOR((uint32_t)IntVectorTable);
}

