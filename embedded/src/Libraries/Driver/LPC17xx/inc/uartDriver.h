#ifndef _UARTDRIVER_H_
#define _UARTDRIVER_H_

#include <LPC17xx.h>

#define SYSTEMCORECLK SystemCoreClock
#define PCLK SYSTEMCORECLK/4

#define PCUART0_POWERON (1 << 3)
#define PCUART1_POWERON (1 << 4)
#define PCUART2_POWERON (1 << 24)
#define PCUART3_POWERON (1 << 25)

#define PCLK_UART0 6
#define PCLK_UART1 8
#define PCLK_UART2 16
#define PCLK_UART3 18
#define PCLK_UART0_MASK (3 << 6)
#define PCLK_UART1_MASK (3 << 8)
#define PCLK_UART2_MASK (3 << 16)
#define PCLK_UART3_MASK (3 << 18)

#define LCR_WLS0    (1 << 0)    //Word Length Select, 00=5bit,01=6bit,10=7bit,11=8bit
#define LCR_WLS1    (1 << 1)
#define LCR_SBS     (1 << 2)    //Stop Bit Select, 0=1stop bit,1=2 stop bits
#define LCR_PE      (1 << 3)    //Parity Enable
#define LCR_PS0     (1 << 4)    //Parity Select
#define LCR_PS1     (1 << 5)
#define LCR_BC      (1 << 6)    //Break Control
#define LCR_DLAB    (1 << 7)    //Divisor Latch Access Bit

#define LSR_RDR     (1 << 0)    //Receiver Data Ready
#define LSR_OE      (1 << 1)    //Overrun Error
#define LSR_PE      (1 << 2)    //Parity Error
#define LSR_FE      (1 << 3)    //Framing Error
#define LSR_BI      (1 << 4)    //Break Interrupt
#define LSR_THRE    (1 << 5)    //Transmitter Holding Register
#define LSR_TEMT    (1 << 6)    //Transmitter Empty
#define LSR_RXFE    (1 << 7)    //Error in RX FIFO

#define FCR_FE      (1 << 0)    //FIFO Enable
#define FCR_RXFR    (1 << 1)    //RX FIFO Reset
#define FCR_TXFR    (1 << 2)    //TX FIFO Reset

#define IER_RBR     (1 << 0)    //RDR Interrupt Enable
#define IER_THRE    (1 << 1)    //THRE Interrupt Enable
#define IER_RX      (1 << 2)    //RX Line Status Interrupt Enable
#define IER_ABEOIntEn (1 << 8)  //End of AutoBaud Interrupt Enable

#define UART0_POWER_ON()                LPC_SC->PCONP |=  PCUART0_POWERON
#define UART0_ENABLE_CLK()              LPC_SC->PCLKSEL0 &= ~(PCLK_UART0_MASK); LPC_SC->PCLKSEL0 |=  (0 << PCLK_UART0)
#define UART0_ENABLE_PINS()             LPC_PINCON->PINSEL0 &= ~((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7)); LPC_PINCON->PINSEL0 |= ((1 << 4) | (1 << 6)) 
#define UART0_CONFIGURE()               LPC_UART0->LCR = LCR_WLS0 | LCR_WLS1
#define UART0_SET_DLAB()                LPC_UART0->LCR |= LCR_DLAB
#define UART0_CLEAR_DLAB()              LPC_UART0->LCR &= ~LCR_DLAB
#define UART0_SET_BAUDRATE(baudrate)    LPC_UART0->DLM = (uint8)((PCLK / (16*baudrate)) / 256); LPC_UART0->DLL = (uint8)(((PCLK / 16) / baudrate) % 256)
#define UART0_ENABLE_AND_RESET_FIFO()   LPC_UART0->FCR = FCR_FE | FCR_RXFR | FCR_TXFR
#define UART0_THR_EMPTY()               (LPC_UART0->LSR & LSR_THRE) != 0
#define UART0_RBR_EMPTY()               (LPC_UART0->LSR & LSR_RDR) == 0
#define UART0_SET_CHAR(c)               LPC_UART0->THR = c
#define UART0_READ_CHAR()               LPC_UART0->RBR
#define UART0_SET_RBR_AND_THRE_INTERRUPT()       LPC_UART0->IER = IER_RBR | IER_THRE
#define UART0_IRQHANDLER()              UART0_IRQHandler(void)
#define UART0_ENABLE_IRQ()              NVIC_EnableIRQ(UART0_IRQn)
#define UART0_DISABLE_IRQ()             NVIC_DisableIRQ(UART0_IRQn)
#define UART0_RESET_IRQS()              NVIC_ClearPendingIRQ(UART0_IRQn)
#define UART0_SET_IRQS()                NVIC_SetPendingIRQ(UART0_IRQn)
#define UART0_RBR_INTERRUPT_OCCURED()   (LPC_UART0->IIR & 0b1111) == 0b0100
#define UART0_THRE_INTERRUPT_OCCURED()  (LPC_UART0->IIR & 0b1111) == 0b0010

#define UART1_POWER_ON()                LPC_SC->PCONP |=  PCUART1_POWERON
#define UART1_ENABLE_CLK()              LPC_SC->PCLKSEL0 &= ~(PCLK_UART1_MASK); LPC_SC->PCLKSEL0 |=  (0 << PCLK_UART1)
#define UART1_ENABLE_PINS()             LPC_PINCON->PINSEL0 &= ~0xc0000000; LPC_PINCON->PINSEL0 |= (1 << 30); LPC_PINCON->PINSEL1 &= ~0xc; LPC_PINCON->PINSEL1 |= (1 << 0);
#define UART1_CONFIGURE()               LPC_UART1->LCR = LCR_WLS0 | LCR_WLS1
#define UART1_SET_DLAB()                LPC_UART1->LCR |= LCR_DLAB
#define UART1_CLEAR_DLAB()              LPC_UART1->LCR &= ~LCR_DLAB
#define UART1_SET_BAUDRATE(baudrate)    LPC_UART1->DLM = (uint8)((PCLK / (16*baudrate)) / 256); LPC_UART1->DLL = (uint8)(((PCLK / 16) / baudrate) % 256)
#define UART1_ENABLE_AND_RESET_FIFO()   LPC_UART1->FCR = FCR_FE | FCR_RXFR | FCR_TXFR
#define UART1_THR_EMPTY()               (LPC_UART1->LSR & LSR_THRE) != 0
#define UART1_RBR_EMPTY()               (LPC_UART1->LSR & LSR_RDR) == 0
#define UART1_SET_CHAR(c)               LPC_UART1->THR = c
#define UART1_READ_CHAR()               LPC_UART1->RBR
#define UART1_SET_RBR_AND_THRE_INTERRUPT()       LPC_UART1->IER = IER_RBR | IER_THRE
#define UART1_IRQHANDLER()              UART1_IRQHandler(void)
#define UART1_ENABLE_IRQ()              NVIC_EnableIRQ(UART1_IRQn)
#define UART1_DISABLE_IRQ()             NVIC_DisableIRQ(UART1_IRQn)
#define UART1_RESET_IRQS()              NVIC_ClearPendingIRQ(UAR1_IRQn)
#define UART1_SET_IRQS()                NVIC_SetPendingIRQ(UART1_IRQn)
#define UART1_RBR_INTERRUPT_OCCURED()   (LPC_UART1->IIR & 0b1111) == 0b0100
#define UART1_THRE_INTERRUPT_OCCURED()  (LPC_UART1->IIR & 0b1111) == 0b0010

#define UART2_POWER_ON()                LPC_SC->PCONP |=  PCUART2_POWERON
#define UART2_ENABLE_CLK()              LPC_SC->PCLKSEL1 &= ~(PCLK_UART2_MASK); LPC_SC->PCLKSEL1 |=  (0 << PCLK_UART2)
#define UART2_ENABLE_PINS()             LPC_PINCON->PINSEL1 &= LPC_PINCON->PINSEL0 &= ~0xf00000; LPC_PINCON->PINSEL0 |= ((1 << 20) | (1 << 22)); 
#define UART2_CONFIGURE()               LPC_UART2->LCR = LCR_WLS0 | LCR_WLS1
#define UART2_SET_DLAB()                LPC_UART2->LCR |= LCR_DLAB
#define UART2_CLEAR_DLAB()              LPC_UART2->LCR &= ~LCR_DLAB
#define UART2_SET_BAUDRATE(baudrate)    LPC_UART2->DLM = (uint8)((PCLK / (16*baudrate)) / 256); LPC_UART2->DLL = (uint8)(((PCLK / 16) / baudrate) % 256)
#define UART2_ENABLE_AND_RESET_FIFO()   LPC_UART2->FCR = FCR_FE | FCR_RXFR | FCR_TXFR
#define UART2_THR_EMPTY()               (LPC_UART2->LSR & LSR_THRE) != 0
#define UART2_RBR_EMPTY()               (LPC_UART2->LSR & LSR_RDR) == 0
#define UART2_SET_CHAR(c)               LPC_UART2->THR = c
#define UART2_READ_CHAR()               LPC_UART2->RBR
#define UART2_SET_RBR_AND_THRE_INTERRUPT()       LPC_UART2->IER = IER_RBR | IER_THRE
#define UART2_IRQHANDLER()              UART2_IRQHandler(void)
#define UART2_ENABLE_IRQ()              NVIC_EnableIRQ(UART2_IRQn)
#define UART2_DISABLE_IRQ()             NVIC_DisableIRQ(UART2_IRQn)
#define UART2_RESET_IRQS()              NVIC_ClearPendingIRQ(UART2_IRQn)
#define UART2_SET_IRQS()                NVIC_SetPendingIRQ(UART2_IRQn)
#define UART2_RBR_INTERRUPT_OCCURED()   (LPC_UART2->IIR & 0b1111) == 0b0100
#define UART2_THRE_INTERRUPT_OCCURED()  (LPC_UART2->IIR & 0b1111) == 0b0010

#define UART3_POWER_ON()                LPC_SC->PCONP |=  PCUART3_POWERON
#define UART3_ENABLE_CLK()              LPC_SC->PCLKSEL1 &= ~(PCLK_UART3_MASK); LPC_SC->PCLKSEL1 |=  (0 << PCLK_UART3)
#define UART3_ENABLE_PINS()             LPC_PINCON->PINSEL0 &= ~0xf; LPC_PINCON->PINSEL0 |= ((1 << 1) | (1 << 3)); 
#define UART3_CONFIGURE()               LPC_UART3->LCR = LCR_WLS0 | LCR_WLS1
#define UART3_SET_DLAB()                LPC_UART3->LCR |= LCR_DLAB
#define UART3_CLEAR_DLAB()              LPC_UART3->LCR &= ~LCR_DLAB
#define UART3_SET_BAUDRATE(baudrate)    LPC_UART3->DLM = (uint8)((PCLK / (16*baudrate)) / 256); LPC_UART3->DLL = (uint8)(((PCLK / 16) / baudrate) % 256)
#define UART3_ENABLE_AND_RESET_FIFO()   LPC_UART3->FCR = FCR_FE | FCR_RXFR | FCR_TXFR
#define UART3_THR_EMPTY()               (LPC_UART3->LSR & LSR_THRE) != 0
#define UART3_RBR_EMPTY()               (LPC_UART3->LSR & LSR_RDR) == 0
#define UART3_SET_CHAR(c)               LPC_UART3->THR = c
#define UART3_READ_CHAR()               LPC_UART3->RBR
#define UART3_SET_RBR_AND_THRE_INTERRUPT()       LPC_UART3->IER = IER_RBR | IER_THRE
#define UART3_IRQHANDLER()              UART3_IRQHandler(void)
#define UART3_ENABLE_IRQ()              NVIC_EnableIRQ(UART3_IRQn)
#define UART3_DISABLE_IRQ()             NVIC_DisableIRQ(UART3_IRQn)
#define UART3_RESET_IRQS()              NVIC_ClearPendingIRQ(UART3_IRQn)
#define UART3_SET_IRQS()                NVIC_SetPendingIRQ(UART3_IRQn)
#define UART3_RBR_INTERRUPT_OCCURED()   (LPC_UART3->IIR & 0b1111) == 0b0100
#define UART3_THRE_INTERRUPT_OCCURED()  (LPC_UART3->IIR & 0b1111) == 0b0010

#endif
