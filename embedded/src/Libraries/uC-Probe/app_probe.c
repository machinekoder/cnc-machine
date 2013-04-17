/*
*********************************************************************************************************
*                                              EXAMPLE CODE
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
* Filename      : app_probe.c
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/            


#define  APP_PROBE_MODULE
#include <app_probe.h>


/*
*********************************************************************************************************
*                                                 ENABLE
*********************************************************************************************************
*/

#if (APP_CFG_PROBE_COM_EN == DEF_ENABLED)

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                    LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         App_ProbeInit()
*
* Description : Initialize all uC/Probe modules.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : none
*********************************************************************************************************
*/


void  App_ProbeInit (void)
{
#if (PROBE_COM_CFG_SERIAL_EN == DEF_ENABLED)
    SERIAL_IF_CFG  uart_cfg;
#endif    
    
    
    APP_TRACE_INFO(("Initializing uC/Probe ... \n\r"));    
    ProbeCom_Init();                                            /* Initialize the uC/Probe communications module            */

#if (PROBE_COM_CFG_RS232_EN == DEF_ENABLED)
    ProbeRS232_Init(115200);
    ProbeRS232_RxIntEn();
#endif

#if (PROBE_COM_CFG_SERIAL_EN == DEF_ENABLED)
    uart_cfg.Baudrate = SERIAL_BAUDRATE_57600;
    uart_cfg.DataBits = SERIAL_DATABITS_8;
    uart_cfg.StopBits = SERIAL_STOPBITS_1;
    uart_cfg.Parity   = SERIAL_PARITY_NONE;
    uart_cfg.FlowCtrl = SERIAL_FLOW_CTRL_NONE;

    ProbeSerial_Init((CPU_CHAR      *)APP_CFG_PROBE_SERIAL_PORT_NAME,
                     (SERIAL_IF_CFG *)&uart_cfg);
#endif
    
    
#if (PROBE_COM_CFG_TCPIP_EN == DEF_ENABLED)
    ProbeTCPIP_Init();
#endif
}

#endif
