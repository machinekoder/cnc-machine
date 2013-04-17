#include <lpc17xx_nvic.h>

extern void  OS_CPU_PendSVHandler (void);
extern void  OS_CPU_SysTickHandler(void);

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
/* External peripheral interrupts */
/* kuu: the only IRQ handler that is necessary for peripheral interrupts */
void CSP_IntHandler();


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
      OS_CPU_PendSVHandler,
      OS_CPU_SysTickHandler,

      /* External peripheral interrupts */
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler,
      CSP_IntHandler
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

