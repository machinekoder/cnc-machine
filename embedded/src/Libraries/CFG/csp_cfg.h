/*
*********************************************************************************************************
*                                               uC/CSP
*                                    CPU CONFIGURATION & PORT LAYER
*
*                          (c) Copyright 2004-2009; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/CSP is provided in source form for FREE evaluation, for educational
*               use or peaceful research.  If you plan on using uC/CSP in a commercial
*               product you need to contact Micrium to properly license its use in your
*               product.  We provide ALL the source code for your convenience and to
*               help you experience uC/CSP.  The fact that the source code is provided
*               does NOT mean that you can use it without paying a licensing fee.
*
*               Knowledge of the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                       CSP CONFIGURATION FILE
*
*                                              TEMPLATE
*
* Filename      : csp_cfg.h
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  CSP_CFG_MODULE_PRESENT
#define  CSP_CFG_MODULE_PRESENT

/*refers to lpc17xx hardware library*/
#define _GPIO DEF_ENABLED
#define _EMAC DEF_ENABLED
#define _UART
#define _UART0
#define _UART1
#define _SSP

#define _SERIAL_LITE DEF_ENABLED

/*$PAGE*/
/*
*********************************************************************************************************
*                                            ARGUMENTS CHECKING
*
* Note(s) : (1) Configure 'CSP_CFG_ARG_CHK_EN' to enable/disable arguments checking.
*********************************************************************************************************
*/

#define  CSP_CFG_ARG_CHK_EN                      DEF_ENABLED


/*$PAGE*/
/*
*********************************************************************************************************
*                                  INTERRUPT MODULE CONFIGURATION
*
* Note(s) : (1) Configure CSP_CFG_INT_ISR_EXEC_MEAS_EN to include variables to measure CPU's interrupt 
*               Interrupt Service Routine (ISR) execution time.
*
*               (a) ISR execution time is only accurate only when interrupt nesting 
*                   'CSP_CFG_INT_NESTING_EN' is disabled.
*
*           (2) Configure CSP_INT_ISR_ARG_EN to enable/disable argument passing to ISR handlers.
*
*           (3) Configure CSP_CFG_INT_NESTING_EN to enable/disable interrupt nesting.
*********************************************************************************************************
*/

#define  CSP_CFG_INT_ISR_EXEC_MEAS_EN            DEF_ENABLED
#define  CSP_CFG_INT_ISR_ARG_EN                  DEF_ENABLED
#define  CSP_CFG_INT_NESTING_EN                  DEF_DISABLED


/*$PAGE*/
/*
*********************************************************************************************************
*                               CHIP SUPPORT PACKAGE OSCILLATOR CONFIGURATION
*
* Note(s) : (1) Configure CSP_CFG_PM_MAIN_XTAL_FREQ_HZ  with the frequency in Hertz of the main oscillator.
*
*           (2) Configure CSP_CFG_PM_SLOW_XTAL_FREQ_HZ  with the frequency in Hertz of the slow oscillator.
*
*           (3) Configure CSP_CFG_PM_EXT_CLK_FREQ_HZ    with the frequency on Hertz of the external clock.
*********************************************************************************************************
*/

#define  CSP_CFG_PM_MAIN_XTAL_FREQ_HZ             12000000u
#define  CSP_CFG_PM_SLOW_XTAL_FREQ_HZ                32768u
#define  CSP_CFG_PM_EXT_CLK_FREQ_HZ                      0u


/*$PAGE*/
/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of CPU cfg module include.                       */

