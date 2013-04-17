/*
*********************************************************************************************************
*                                     SERIAL (BYTE) COMMUNICATION
*
*                         (c) Copyright 2007-2009; Micrium, Inc.; Weston, FL
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
*                                     SERIAL (BYTE) COMMUNICATION
*                                            DEVICE DRIVER
*
*                            LPC17xx BSP FOR THE NXP LPCXXXX SERIAL DRIVER
*
* Filename      : serial_bsp_lpc17xx.h
* Version       : V2.00
* Programmer(s) : FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               SERIAL_BSP_AT91SAM9261 present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef   SERIAL_BSP_LPC17XX_PRESENT
#define   SERIAL_BSP_LPC17XX_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <serial.h>
#include  <serial_drv_lpcxxxx.h>


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           SERIAL DEVICES
*********************************************************************************************************
*/

extern  SERIAL_DEV_CFG  SerialDev_LPCxxxx_UART0;

extern  SERIAL_DEV_CFG  SerialDev_LPCxxxx_UART1;
extern  SERIAL_DEV_CFG  SerialDev_LPCxxxx_UART1_Alt;

extern  SERIAL_DEV_CFG  SerialDev_LPCxxxx_UART2;
extern  SERIAL_DEV_CFG  SerialDev_LPCxxxx_UART2_Alt;

extern  SERIAL_DEV_CFG  SerialDev_LPCxxxx_UART3;
extern  SERIAL_DEV_CFG  SerialDev_LPCxxxx_UART3_Alt0;
extern  SERIAL_DEV_CFG  SerialDev_LPCxxxx_UART3_Alt1;


/*$PAGE*/
/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/


#endif                                                          /* End of SERIAL_BSP_AT91SAM9261 module include.           */
