/*
*********************************************************************************************************
*                                     MICRIUM BOARD SUPPORT SUPPORT
*
*                          (c) Copyright 2003-2009; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*
*                                      BOARD SUPPORT PACKAGE (BSP)
*
*                                    KEIL MCB1700 DEVELOPMENT KIT
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define   BSP_MODULE
#include  <bsp.h>
/*hardware interrupt controller reference*/
#include <lpc17xx_nvic.h>


/*
*********************************************************************************************************
*                                             REGISTER & BIT DEFINES
*********************************************************************************************************
*/

                                                                /* - SYSTEM CONTROL AND STATUS REGISTER & BIT DEFINES  -*/
#define  BSP_REG_SCS                             (*(CPU_REG32 *)(0x400FC1A0))
#define  BSP_BIT_SCS_OSCRANGE                    DEF_BIT_04
#define  BSP_BIT_SCS_OSCEN                       DEF_BIT_05
#define  BSP_BIT_SCS_OSCSTAT                     DEF_BIT_06

                                                                /* ------- CLOCK DIVIDERS REGISTER & BIT DEFINES ------ */
#define  BSP_REG_CCLKCFG                         (*(CPU_REG32 *)(0x400FC104))
#define  BSP_REG_CLKSRCSEL                       (*(CPU_REG32 *)(0x400FC10C))
#define  BSP_REG_PCLKSEL0                        (*(CPU_REG32 *)(0x400FC1A8))
#define  BSP_REG_PCLKSEL1                        (*(CPU_REG32 *)(0x400FC1AC))

                                                                /* ------- CLOCK DIVIDERS REGISTER & BIT DEFINES ------ */

#define  BSP_REG_AD0CR                           (*(CPU_REG32 *)(0x40034000))
#define  BSP_REG_AD0DR2                          (*(CPU_REG32 *)(0x40034018))

                                                                /* ------------ PLL REGISTER & BIT DEFINES ------------ */
#define  BSP_ADDR_PLL                              (CPU_INT32U )0x400FC080u

#define  BSP_REG_PLLCTRL(pll_id)                 (*(CPU_REG32 *)(BSP_ADDR_PLL + (0x20u * (pll_id)) + 0x00u))
#define  BSP_REG_PLLCFG(pll_id)                  (*(CPU_REG32 *)(BSP_ADDR_PLL + (0x20u * (pll_id)) + 0x04u))
#define  BSP_REG_PLLSTAT(pll_id)                 (*(CPU_REG32 *)(BSP_ADDR_PLL + (0x20u * (pll_id)) + 0x08u))
#define  BSP_REG_PLLFEED(pll_id)                 (*(CPU_REG32 *)(BSP_ADDR_PLL + (0x20u * (pll_id)) + 0x0Cu))

#define  BSP_MSK_PLLCFG0_MSEL                    0x00007FFFu
#define  BSP_MSK_PLLCFG0_NSEL                    0x00FF0000u

#define  BSP_MSK_PLLCFG1_MSEL                    0x0000001Fu
#define  BSP_MSK_PLLCFG1_NSEL                    0x00000060u

#define  BSP_BIT_PLLCTRL_PLLE                    DEF_BIT_00     /* PLL enable                                           */
#define  BSP_BIT_PLLCTRL_PLLC                    DEF_BIT_01     /* PLL connect                                          */

#define  BSP_BIT_PLLSTAT_PLLE0_STAT              DEF_BIT_24     /* Read-back for the PLL enable bit                     */
#define  BSP_BIT_PLLSTAT_PLLC0_STAT              DEF_BIT_25     /* Read-back for the PLL connect bit                    */
#define  BSP_BIT_PLLSTAT_PLOCK0                  DEF_BIT_26     /* Reflect the PLL lock status                          */

#define  BSP_BIT_PLLSTAT_PLLE1_STAT              DEF_BIT_08     /* Read-back for the PLL enable bit                     */
#define  BSP_BIT_PLLSTAT_PLLC1_STAT              DEF_BIT_09     /* Read-back for the PLL connect bit                    */
#define  BSP_BIT_PLLSTAT_PLOCK1                  DEF_BIT_10     /* Reflect the PLL lock status                          */

#define  BSP_BIT_PLLFEED_VAL0                    0x000000AAu
#define  BSP_BIT_PLLFEED_VAL1                    0x00000055u

#define  BSP_BIT_CLKSRCSEL_RC                    DEF_BIT_NONE
#define  BSP_BIT_CLKSRCSEL_MAIN                  DEF_BIT_00
#define  BSP_BIT_CLKSRCSEL_RTC                   DEF_BIT_01

                                                                /* --- FLASH ACCELERATOR CFG REGISTER & BIT DEFINES --- */
#define  BSP_REG_FLASHCFG                        (*(CPU_REG32 *)(0x400FC000u))

#define  BSP_MSK_FLASHCFG_CLK_1                  DEF_BIT_MASK(0u, 12u)
#define  BSP_MSK_FLASHCFG_CLK_2                  DEF_BIT_MASK(1u, 12u)
#define  BSP_MSK_FLASHCFG_CLK_3                  DEF_BIT_MASK(2u, 12u)
#define  BSP_MSK_FLASHCFG_CLK_4                  DEF_BIT_MASK(3u, 12u)
#define  BSP_MSK_FLASHCFG_CLK_5                  DEF_BIT_MASK(4u, 12u)
#define  BSP_MSK_FLASHCFG_CLK_6                  DEF_BIT_MASK(5u, 12u)

#define  BSP_BIT_FLASHCFG_FETCHCFG_ALL           DEF_BIT_MASK(2u, 0u)
#define  BSP_BIT_FLASHCFG_DATACFG_ALL            DEF_BIT_MASK(2u, 2u)
#define  BSP_BIT_FLASHCFG_ACCEL_EN               DEF_BIT_04
#define  BSP_BIT_FLASHCFG_PREFETCH_ALL           DEF_BIT_05


/*
*********************************************************************************************************
*                                             GPIO DEFINES
*********************************************************************************************************
*/

#define  BSP_GPIO_PORT1_LED_01                  DEF_BIT_28
#define  BSP_GPIO_PORT1_LED_02                  DEF_BIT_29
#define  BSP_GPIO_PORT1_LED_03                  DEF_BIT_31
#define  BSP_GPIO_PORT1_LED_GRP                (BSP_GPIO_PORT1_LED_01 | \
                                                BSP_GPIO_PORT1_LED_02 | \
                                                BSP_GPIO_PORT1_LED_03)

#define  BSP_GPIO_PORT2_LED_04                  DEF_BIT_02
#define  BSP_GPIO_PORT2_LED_05                  DEF_BIT_03
#define  BSP_GPIO_PORT2_LED_06                  DEF_BIT_04
#define  BSP_GPIO_PORT2_LED_07                  DEF_BIT_05
#define  BSP_GPIO_PORT2_LED_08                  DEF_BIT_06

#define  BSP_GPIO_PORT2_LED_GRP                (BSP_GPIO_PORT2_LED_04 | \
                                                BSP_GPIO_PORT2_LED_05 | \
                                                BSP_GPIO_PORT2_LED_06 | \
                                                BSP_GPIO_PORT2_LED_07 | \
                                                BSP_GPIO_PORT2_LED_08)

#define  BSP_GPIO_PORT1_JOY_UP                  DEF_BIT_24
#define  BSP_GPIO_PORT1_JOY_DOWN                DEF_BIT_26
#define  BSP_GPIO_PORT1_JOY_LEFT                DEF_BIT_23
#define  BSP_GPIO_PORT1_JOY_RIGHT               DEF_BIT_25
#define  BSP_GPIO_PORT1_JOY_CENTER              DEF_BIT_20

#define  BSP_GPIO_PORT1_JOY_GRP                (BSP_GPIO_PORT1_JOY_UP     | \
                                                BSP_GPIO_PORT1_JOY_DOWN   | \
                                                BSP_GPIO_PORT1_JOY_LEFT   | \
                                                BSP_GPIO_PORT1_JOY_RIGHT  | \
                                                BSP_GPIO_PORT1_JOY_CENTER)

#define  BSP_GPIO_PORT2_PB                      DEF_BIT_10

#define  BSP_GPIO_PORT4_SD_CARD_PRESENT         DEF_BIT_29


/*
*********************************************************************************************************
*                                             BSP CONSTANS VALUES
*********************************************************************************************************
*/
                                                               /* Maximum register timeout                            */
#define  BSP_VAL_MAX_TO                         (CPU_INT16U)(0xFFFF)
                                                               /* Analog to Digital sample rate.                      */
#define  BSP_VAL_ADC_POT_SAMPLE_FREQ                      500000


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/

#define  BSP_PLL_FEED_SEQ(pll_nbr)              {    CPU_CRITICAL_ENTER();                               \
                                                                                                         \
                                                     BSP_REG_PLLFEED((pll_nbr)) = BSP_BIT_PLLFEED_VAL0;  \
                                                     BSP_REG_PLLFEED((pll_nbr)) = BSP_BIT_PLLFEED_VAL1;  \
                                                                                                         \
                                                     CPU_CRITICAL_EXIT();                                \
                                                }

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  CPU_BOOLEAN  BSP_CPU_Init  (void);


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             BSP_LowLevelInit()
*
* Description : System Low Level Init (PLLs, External Memories, Boot loaders, etc).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Startup Code.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LowLevelInit (void)
{

}


/*
*********************************************************************************************************
*                                             BSP_PreInit()
*
* Description : System Pre-Initialization. Initializes all peripherals that don't require OS services (LEDs, PBs)
*               or modules than need to be initialized before the OS (External memory controller).
*
* Argument(s) : main().
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_PreInit (void)
{
    BSP_CPU_Init();                                             /* Initialize CPU Clks & PLLs                           */

    CPU_IntDis();                                               /* Disable CPU interrupts.                              */
    NVICInit();													/* Initialize hardware interrupt controller*/
    CSP_IntInit();                                              /* Initialize software interrupt controller module.              */

}


/*
*********************************************************************************************************
*                                             BSP_PostInit()
*
* Description : Initialize all the peripherals that required OS services (OS initialized)
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_PostInit (void)
{

}


/*
*********************************************************************************************************
*                                             BSP_CPU_Init()
*
* Description : Initialize the CPU clks, memory map, etc.
*               (1) Initialize the main oscillator.
*               (2) Configure PLL0.
*               (3) Configure CPU frequency.
*               (3) Configure PLL1.
*               (4) Configure peripherals clocks.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_PreInit()
*
* Note(s)     : (1) The PLL0 Frequency is determined by:
*
*                       Fcco = (2 x M x Fin) / N
*
*                       where M   = PLL0 Multipler
*                             N   = PLL0 Pre-divider
*                            Fin  = PLL0 Input Frequency (Main oscillator).
*
*               (2) PLL0 settings must meet the following:
*                       Fin is in the range of 32Khz to 50 Mhz.
*                       Fcco is in the range of 275 Mhz to 550 Mhz
*
*               (3) The LPC17xx CPU frequency is determined by:
*
*                       CPU_freq = PLL0_fcc0 / CPU_div
*
*                       where CPU_div is the CPU clock divider.
*
*               (4) The USB clock frequency is determined by:
*
*                       USB_Clk = M x Fosc x P, or USB_clk = Fcco / (2 x P)
*
*                       Fcco    =  Fosc x M x 2 x P
*
*                       where Fcco = PLL1 output frequency.
*                             M    = PLL1 Multiplier.
*                             P    = PLL1 pre-divider.
*                             Fosc = Oscillator frequency.
*
*               (5) The PLL1 inputs and settings must meet the following criteria:
*                   Fosc    is in the range of 10 Mhz to 25 Mhz.
*                   USBClk  is 48 Mhz
*                   Fcco    is in the range of 156 Mhz to 320 Mhz
*
*               (6) In this example the LPC1768 operates:
*
*                       CPU Frequency:
*                           PLL0_Fcco = 2 x  M x Fin / 2
*                                     = 2 x 25 x  12 / 2
*                                     = 300 Mhz.
*
*                           CPU_freq  = PLL0_fcc0 / CPU_div
*                           CPU_freq  = 300  / 3
*                                     = 100 Mhz
*
*
*                       USB Frequency:
*                           PLL1_Fcc0 = Fosc x M x 2 x P
*                                     = 12   x M x 2 x P
*                                     = 192 Mhz
*
*                           USB_Clk   = M   x Fosc
*                                     = 4   x 12
*                                     = 48 Mhz
*********************************************************************************************************
*/

static  CPU_BOOLEAN  BSP_CPU_Init (void)
{
    CPU_INT16U    reg_to;
    CPU_INT32U    reg_val;
    CPU_SR_ALLOC();


    BSP_REG_FLASHCFG = (CPU_INT32U)BSP_MSK_FLASHCFG_CLK_6;      /* Set 6 cycles to acces the Flash memory (Safe setting)*/

                                                                /* ----------- MAIN OSCILLATOR INITIALIZATION --------- */
    DEF_BIT_CLR(BSP_REG_SCS, BSP_BIT_SCS_OSCRANGE);             /* Set the main oscillator range                        */


    reg_to = BSP_VAL_MAX_TO;

    DEF_BIT_SET(BSP_REG_SCS, BSP_BIT_SCS_OSCEN);                /* Enable the Main Oscillator                           */

                                                                /* Wait until the main oscillator is enabled.           */
    while (DEF_BIT_IS_CLR(BSP_REG_SCS, BSP_BIT_SCS_OSCSTAT) &&
          (reg_to > 0u)) {
        reg_to--;
    }

    if (reg_to == 0u) {                                         /* Configuration fail                                   */
        return (DEF_FAIL);
    }

    BSP_REG_PCLKSEL0 = DEF_BIT_NONE;                            /* All peripheral clock runs at CPU_Clk / 4 = 25 Mhz.   */
    BSP_REG_PCLKSEL1 = DEF_BIT_NONE;

                                                                /* ------------------ PLL0 CONFIGURATION -------------- */

    reg_val  = (((25u - 1u) <<  0u) & BSP_MSK_PLLCFG0_MSEL)     /* PLL0 values M = 25 & N = 2 (see note #6)             */
             | ((( 2u - 1u) << 16u) & BSP_MSK_PLLCFG0_NSEL);
                                                                /* 1. Disconnect PLL0 with one feed sequence if PLL ... */
                                                                /* ... already connected.                               */
    if (DEF_BIT_IS_SET(BSP_REG_PLLSTAT(0u), BSP_BIT_PLLSTAT_PLLC0_STAT)) {
        DEF_BIT_CLR(BSP_REG_PLLCTRL(0u), BSP_BIT_PLLCTRL_PLLC);
        BSP_PLL_FEED_SEQ(0u);
    }

    DEF_BIT_CLR(BSP_REG_PLLCTRL(0u), BSP_BIT_PLLCTRL_PLLE);     /* 2. Disable PLL0 with one feed sequence               */
    BSP_PLL_FEED_SEQ(0u);

    BSP_REG_CCLKCFG   = (1u - 1u);                              /* 3. Change the CPU clock divider setting to speed ... */
                                                                /* ... operation without PLL0                           */

    BSP_REG_CLKSRCSEL = BSP_BIT_CLKSRCSEL_MAIN;                 /* 4. Select the main osc. as the PLL0 clock source     */

    BSP_REG_PLLCFG(0u) = reg_val;                               /* 5. Write to the PLLCFG and make it effective with... */
    BSP_PLL_FEED_SEQ(0u)                                        /* ... one one feed sequence                            */

    DEF_BIT_SET(BSP_REG_PLLCTRL(0u), BSP_BIT_PLLCTRL_PLLE);     /* 6. Enable PLL0 with one feed sequence                */
    BSP_PLL_FEED_SEQ(0u);

    BSP_REG_CCLKCFG   = (3u - 1u);                              /* 7. Change the CPU clock divider setting for ...      */
                                                                /* ... operation with PLL0                              */

    reg_to = BSP_VAL_MAX_TO;                                    /* 8. Wait for PLL0 to achieve lock by monitoring ...   */
                                                                /* ... the PLOCK0 bit in the PLL0STAT                   */
    while (DEF_BIT_IS_CLR(BSP_REG_PLLSTAT(0u), BSP_BIT_PLLSTAT_PLOCK0) &&
          (reg_to > 0u)) {
        reg_to--;
    }

    if (reg_to == 0u) {
        return (DEF_FAIL);
    }

    DEF_BIT_SET(BSP_REG_PLLCTRL(0u), BSP_BIT_PLLCTRL_PLLC);     /* 9. Connect PLL0 with one feed sequence.              */
    BSP_PLL_FEED_SEQ(0u);
                                                                /* ------------------ PLL1 CONFIGURATION -------------- */
    reg_val  = (((4u - 1u)  <<  0u) & BSP_MSK_PLLCFG1_MSEL)     /* PLL1 values M = 4; P = 2 coded as '01' (see note #6) */
             | (((0x01u   ) <<  5u) & BSP_MSK_PLLCFG1_NSEL);

    DEF_BIT_CLR(BSP_REG_PLLCTRL(1u), BSP_BIT_PLLCTRL_PLLC);     /* 1. Disconnect PLL1 with one feed sequence            */
    BSP_PLL_FEED_SEQ(1u);

    DEF_BIT_CLR(BSP_REG_PLLCTRL(1u), BSP_BIT_PLLCTRL_PLLE);     /* 2. Disable PLL1 with one feed sequence               */
    BSP_PLL_FEED_SEQ(1u);

    BSP_REG_PLLCFG(1u) = reg_val;                               /* 3. Write to the PLLCFG and make it effective with... */
    BSP_PLL_FEED_SEQ(1u);                                       /* ... one one feed sequence                            */

    DEF_BIT_SET(BSP_REG_PLLCTRL(1u), BSP_BIT_PLLCTRL_PLLE);     /* 4. Enable PLL1 with one feed sequence                */
    BSP_PLL_FEED_SEQ(1u);

    reg_to = BSP_VAL_MAX_TO;                                    /* 5. Wait for PLL1 to achieve lock by monitoring ...   */
                                                                /* ... the PLOCK1 bit in the PLL1STAT                   */
    while (DEF_BIT_IS_CLR(BSP_REG_PLLSTAT(1u), BSP_BIT_PLLSTAT_PLOCK1) &&
          (reg_to > 0u)) {
        reg_to--;
    }

    if (reg_to == 0u) {
        return (DEF_FAIL);
    }


    DEF_BIT_SET(BSP_REG_PLLCTRL(1u), BSP_BIT_PLLCTRL_PLLC);     /* 6. Connect PLL1 with one feed sequence               */
    BSP_PLL_FEED_SEQ(1u);

    BSP_REG_FLASHCFG = BSP_MSK_FLASHCFG_CLK_5                   /* Set 5 cycles to access the Flash memory.             */
                     | BSP_BIT_FLASHCFG_FETCHCFG_ALL
                     | BSP_BIT_FLASHCFG_DATACFG_ALL
                     | BSP_BIT_FLASHCFG_ACCEL_EN
                     | BSP_BIT_FLASHCFG_PREFETCH_ALL;

    return (DEF_OK);
}
