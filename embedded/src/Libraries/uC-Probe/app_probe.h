/*
*********************************************************************************************************
*                                               uC/PROBE
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
*                                           uC/Probe
*                                      Application Code
*
* Filename      : app_probe.h
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/

#ifndef  APP_PROBE_MODULE_PRESENT
#define  APP_PROBE_MODULE_PRESENT

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include  <app_cfg.h>
#include  <probe_com.h>

/*
*********************************************************************************************************
*                                        DEFAULT CONFIGURATION
*********************************************************************************************************
*/


#ifndef  APP_CFG_PROBE_COM_EN
#define  APP_CFG_PROBE_COM_EN                   DEF_DISABLED
#endif

#ifndef  PROBE_COM_CFG_RS232_EN
#define  PROBE_COM_CFG_RS232_EN                 DEF_DISABLED
#endif

#ifndef  PROBE_COM_CFG_SERIAL_EN
#define  PROBE_COM_CFG_SERIAL_EN                DEF_DISABLED
#endif

#ifndef  PROBE_COM_CFG_TCPIP_EN
#define  PROBE_COM_CFG_TCPIP_EN                 DEF_DISABLED
#endif

/*
*********************************************************************************************************
*                                      CONDITIONAL INCLUDE FILES
*********************************************************************************************************
*/

#if (APP_CFG_PROBE_COM_EN == DEF_ENABLED)
#include    <probe_com.h>

#if (PROBE_COM_CFG_RS232_EN == DEF_ENABLED)
#include    <probe_rs232.h>
#endif

#if (PROBE_COM_CFG_SERIAL_EN == DEF_ENABLED)
#include    <probe_serial.h>
#endif

#if (PROBE_COM_CFG_TCPIP_EN == DEF_ENABLED)
#include    <probe_tcpip.h>
#endif
#endif


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   APP_PROBE_MODULE
#define  APP_PROBE_MODULE_EXT
#else
#define  APP_PROBE_MODULE_EXT  extern
#endif


/*
*********************************************************************************************************
*                                                 DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#if (APP_CFG_PROBE_COM_EN == DEF_ENABLED)
void  App_ProbeInit  (void);
#endif

/*
*********************************************************************************************************
*                                          CONFIGURATION ERRORS
*********************************************************************************************************
*/

#if (APP_CFG_PROBE_COM_EN    == DEF_ENABLED) && \
    (PROBE_COM_CFG_SERIAL_EN == DEF_ENABLED)
#ifndef  APP_CFG_PROBE_SERIAL_PORT_NAME
#error  "APP_CFG_PROBE_SERIAL_PORT_NAME                 not #define'd in 'app_cfg.h'     "
#error  "                                   [MUST be  DEF_DISABLED]                      "
#error  "                                   [     ||  DEF_ENABLED ]                      "
#endif
#endif

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif
