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
*                                             NXP LPCxxxx
*
* Filename      : serial_drv_lpcxxxx.c
* Version       : V2.00
* Programmer(s) : FT
*********************************************************************************************************
* Note(s)       : (1) With an appropriate BSP, this device driver will support the UARTs on the NXP LPC
*                     MCUs & MPUs, including :
*
*                         NXP LPC23xx series.
*                         NXP LPC24xx series.
*                         NXP LPC17xx series.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               SERIAL_DRV_ATMEL present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef   SERIAL_DRV_LPCXXXX_PRESENT
#define   SERIAL_DRV_LPCXXXX_PRESENT


/*
*********************************************************************************************************
*                                      SERIAL DEVICE DRIVER API
*********************************************************************************************************
*/

extern  SERIAL_DRV_API  SerialDrv_LPCxxxx_API;
extern  SERIAL_DRV_API  SerialDrv_LPC3xxx_API;

/*$PAGE*/
/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of SERIAL_DRV_LPCXXXX module include.            */
