/*
*********************************************************************************************************
*                                              uC/OS-II
*                                        The Real-Time Kernel
*
*                          (c) Copyright 2004-2008; Micrium, Inc.; Weston, FL               
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/OS-II is provided in source form for FREE evaluation, for educational
*               use or peaceful research.  If you plan on using uC/OS-II in a commercial    
*               product you need to contact Micrium to properly license its use in your     
*               product.  We provide ALL the source code for your convenience and to        
*               help you experience uC/OS-II.  The fact that the source code is provided
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
*                                           MASTER INCLUDES
*
*                                    KEIL MCB1700 DEVELOPMENT KIT                                    
*
* Filename      : includes.h
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/

#ifndef  INCLUDES_MODULES_PRESENT
#define  INCLUDES_MODULES_PRESENT

/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include <stdint.h>
#include  <math.h>


/*
*********************************************************************************************************
*                                              LIBRARIES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <lib_str.h>


/*
*********************************************************************************************************
*                                              APP / BSP
*********************************************************************************************************
*/

#include <app_cfg.h>
#include <bsp.h>
#include <os_serial_lite.h>
//#include <lpc17xx_ssp.h>
#include <lpc17xx_dac.h>
//#include <uart.h>
#include <led.h>
#include <dac.h>
#include <gpio.h>
#include <pincon.h>

/*
*********************************************************************************************************
*                                                 OS
*********************************************************************************************************
*/

#include  <os.h>
#include  <os_csp.h>


/*
*********************************************************************************************************
*                                                 CSP
*********************************************************************************************************
*/

#include  <csp.h>

/*
*********************************************************************************************************
*                                                  PROBE
*********************************************************************************************************
*/

#include  <app_probe.h>


/*
*********************************************************************************************************
*                                                  SERIAL
*********************************************************************************************************
*/

#include  <app_serial.h>

/*
*********************************************************************************************************
*                                                  OWN
*********************************************************************************************************
*/

#include "taskStart.h"
#include "timer.h"
#include "gpio.h"
#include "button.h"

/*
*********************************************************************************************************
*                                            INCLUDES END
*********************************************************************************************************
*/

#endif
