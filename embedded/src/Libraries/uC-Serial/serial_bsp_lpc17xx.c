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
*
* Filename      : serial_bsp_lpc17xx.c
* Version       : V2.00
* Programmer(s) : FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <serial_bsp_lpc17xx.h>
#include  <csp.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  SERIAL_BSP_LPC17xx_UART0_BASE        ((void *)0x4000C000)
#define  SERIAL_BSP_LPC17xx_UART1_BASE        ((void *)0x40010000)
#define  SERIAL_BSP_LPC17xx_UART2_BASE        ((void *)0x40098000)
#define  SERIAL_BSP_LPC17xx_UART3_BASE        ((void *)0x4009C000)


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

                                                                /* -------------------- UART0 BSP API ----------------- */
static  void        SerialBSP_LPC17xx_UART0_ClkEn        (SERIAL_ERR   *perr);
static  void        SerialBSP_LPC17xx_UART0_ClkDis       (SERIAL_ERR   *perr);
static  void        SerialBSP_LPC17xx_UART0_CfgGPIO      (CPU_BOOLEAN   flow_ctrl,
                                                          SERIAL_ERR   *perr);
static  void        SerialBSP_LPC17xx_UART0_CfgInt       (SERIAL_DEV   *pdev,
                                                          SERIAL_ERR   *perr);
static  CPU_INT32U  SerialBSP_LPC17xx_UART0_ClkFreqGet   (void);

                                                                /* -------------------- UART1 BSP API ----------------- */
static  void        SerialBSP_LPC17xx_UART1_ClkEn         (SERIAL_ERR   *perr);
static  void        SerialBSP_LPC17xx_UART1_ClkDis        (SERIAL_ERR   *perr);
static  void        SerialBSP_LPC17xx_UART1_CfgGPIO       (CPU_BOOLEAN   flow_ctrl,
                                                           SERIAL_ERR   *perr);
static  void        SerialBSP_LPC17xx_UART1_CfgGPIO_Alt   (CPU_BOOLEAN   flow_ctrl,
                                                           SERIAL_ERR   *perr);

static  void        SerialBSP_LPC17xx_UART1_CfgInt        (SERIAL_DEV   *pdev,
                                                           SERIAL_ERR   *perr);
static  CPU_INT32U  SerialBSP_LPC17xx_UART1_ClkFreqGet    (void);

                                                                /* -------------------- UART2 BSP API ----------------- */
static  void        SerialBSP_LPC17xx_UART2_ClkEn         (SERIAL_ERR   *perr);
static  void        SerialBSP_LPC17xx_UART2_ClkDis        (SERIAL_ERR   *perr);
static  void        SerialBSP_LPC17xx_UART2_CfgGPIO       (CPU_BOOLEAN   flow_ctrl,
                                                           SERIAL_ERR   *perr);

static  void        SerialBSP_LPC17xx_UART2_CfgGPIO_Alt   (CPU_BOOLEAN   flow_ctrl,
                                                           SERIAL_ERR   *perr);
static  void        SerialBSP_LPC17xx_UART2_CfgInt        (SERIAL_DEV   *pdev,
                                                           SERIAL_ERR   *perr);
static  CPU_INT32U  SerialBSP_LPC17xx_UART2_ClkFreqGet    (void);

                                                                /* -------------------- UART3 BSP API ----------------- */
static  void        SerialBSP_LPC17xx_UART3_ClkEn         (SERIAL_ERR   *perr);
static  void        SerialBSP_LPC17xx_UART3_ClkDis        (SERIAL_ERR   *perr);
static  void        SerialBSP_LPC17xx_UART3_CfgGPIO       (CPU_BOOLEAN   flow_ctrl,
                                                           SERIAL_ERR   *perr);
static  void        SerialBSP_LPC17xx_UART3_CfgGPIO_Alt0  (CPU_BOOLEAN   flow_ctrl,
                                                           SERIAL_ERR   *perr);
static  void        SerialBSP_LPC17xx_UART3_CfgGPIO_Alt1  (CPU_BOOLEAN   flow_ctrl,
                                                           SERIAL_ERR   *perr);

static  void        SerialBSP_LPC17xx_UART3_CfgInt        (SERIAL_DEV   *pdev,
                                                           SERIAL_ERR   *perr);
static  CPU_INT32U  SerialBSP_LPC17xx_UART3_ClkFreqGet    (void);

#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)    
static  void        SerialBSP_LPC17xx_UARTx_IntHandler    (void *p_arg);
#else
static  void        SerialBSP_LPC17xx_UART0_IntHandler    (void *p_arg);
static  void        SerialBSP_LPC17xx_UART1_IntHandler    (void *p_arg);
static  void        SerialBSP_LPC17xx_UART2_IntHandler    (void *p_arg);
static  void        SerialBSP_LPC17xx_UART3_IntHandler    (void *p_arg);
#endif


/*
*********************************************************************************************************
*                                   SERIAL INTERFACE DEVICE BSP API
*********************************************************************************************************
*/

static  SERIAL_BSP_API  SerialBSP_LPC17xx_UART0 = {
    SerialBSP_LPC17xx_UART0_ClkEn,
    SerialBSP_LPC17xx_UART0_ClkDis,
    SerialBSP_LPC17xx_UART0_CfgGPIO,
    SerialBSP_LPC17xx_UART0_CfgInt,
    SerialBSP_LPC17xx_UART0_ClkFreqGet
};

static  SERIAL_BSP_API  SerialBSP_LPC17xx_UART1 = {
    SerialBSP_LPC17xx_UART1_ClkEn,
    SerialBSP_LPC17xx_UART1_ClkDis,
    SerialBSP_LPC17xx_UART1_CfgGPIO,
    SerialBSP_LPC17xx_UART1_CfgInt,
    SerialBSP_LPC17xx_UART1_ClkFreqGet
};

static  SERIAL_BSP_API  SerialBSP_LPC17xx_UART1_Alt = {
    SerialBSP_LPC17xx_UART1_ClkEn,
    SerialBSP_LPC17xx_UART1_ClkDis,
    SerialBSP_LPC17xx_UART1_CfgGPIO_Alt,
    SerialBSP_LPC17xx_UART1_CfgInt,
    SerialBSP_LPC17xx_UART1_ClkFreqGet
};

static  SERIAL_BSP_API  SerialBSP_LPC17xx_UART2 = {
    SerialBSP_LPC17xx_UART2_ClkEn,
    SerialBSP_LPC17xx_UART2_ClkDis,
    SerialBSP_LPC17xx_UART2_CfgGPIO,
    SerialBSP_LPC17xx_UART2_CfgInt,
    SerialBSP_LPC17xx_UART2_ClkFreqGet
};

static  SERIAL_BSP_API  SerialBSP_LPC17xx_UART2_Alt = {
    SerialBSP_LPC17xx_UART2_ClkEn,
    SerialBSP_LPC17xx_UART2_ClkDis,
    SerialBSP_LPC17xx_UART2_CfgGPIO_Alt,
    SerialBSP_LPC17xx_UART2_CfgInt,
    SerialBSP_LPC17xx_UART2_ClkFreqGet
};

static  SERIAL_BSP_API  SerialBSP_LPC17xx_UART3 = {
    SerialBSP_LPC17xx_UART3_ClkEn,
    SerialBSP_LPC17xx_UART3_ClkDis,
    SerialBSP_LPC17xx_UART3_CfgGPIO,
    SerialBSP_LPC17xx_UART3_CfgInt,
    SerialBSP_LPC17xx_UART3_ClkFreqGet
};

static  SERIAL_BSP_API  SerialBSP_LPC17xx_UART3_Alt0 = {
    SerialBSP_LPC17xx_UART3_ClkEn,
    SerialBSP_LPC17xx_UART3_ClkDis,
    SerialBSP_LPC17xx_UART3_CfgGPIO_Alt0,
    SerialBSP_LPC17xx_UART3_CfgInt,
    SerialBSP_LPC17xx_UART3_ClkFreqGet
};

static  SERIAL_BSP_API  SerialBSP_LPC17xx_UART3_Alt1 = {
    SerialBSP_LPC17xx_UART3_ClkEn,
    SerialBSP_LPC17xx_UART3_ClkDis,
    SerialBSP_LPC17xx_UART3_CfgGPIO_Alt1,
    SerialBSP_LPC17xx_UART3_CfgInt,
    SerialBSP_LPC17xx_UART3_ClkFreqGet
};


/*
*********************************************************************************************************
*********************************************************************************************************
*                                      SERIAL INTERFACE DEVICES
*********************************************************************************************************
*********************************************************************************************************
*/

SERIAL_DEV_CFG  SerialDev_LPCxxxx_UART0 = {
     SERIAL_BSP_LPC17xx_UART0_BASE,
    &SerialDrv_LPCxxxx_API,
    &SerialBSP_LPC17xx_UART0
};

SERIAL_DEV_CFG  SerialDev_LPCxxxx_UART1 = {
     SERIAL_BSP_LPC17xx_UART1_BASE,
    &SerialDrv_LPCxxxx_API,
    &SerialBSP_LPC17xx_UART1
};

SERIAL_DEV_CFG  SerialDev_LPCxxxx_UART1_Alt = {
     SERIAL_BSP_LPC17xx_UART1_BASE,
    &SerialDrv_LPCxxxx_API,
    &SerialBSP_LPC17xx_UART1_Alt
};

SERIAL_DEV_CFG  SerialDev_LPCxxxx_UART2 = {
     SERIAL_BSP_LPC17xx_UART2_BASE,
    &SerialDrv_LPCxxxx_API,
    &SerialBSP_LPC17xx_UART2
};

SERIAL_DEV_CFG  SerialDev_LPCxxxx_UART2_Alt = {
     SERIAL_BSP_LPC17xx_UART2_BASE,
    &SerialDrv_LPCxxxx_API,
    &SerialBSP_LPC17xx_UART2_Alt
};

SERIAL_DEV_CFG  SerialDev_LPCxxxx_UART3 = {
     SERIAL_BSP_LPC17xx_UART3_BASE,
    &SerialDrv_LPCxxxx_API,
    &SerialBSP_LPC17xx_UART3
};

SERIAL_DEV_CFG  SerialDev_LPCxxxx_UART3_Alt0 = {
     SERIAL_BSP_LPC17xx_UART3_BASE,
    &SerialDrv_LPCxxxx_API,
    &SerialBSP_LPC17xx_UART3_Alt0
};


SERIAL_DEV_CFG  SerialDev_LPCxxxx_UART3_Alt1 = {
     SERIAL_BSP_LPC17xx_UART3_BASE,
    &SerialDrv_LPCxxxx_API,
    &SerialBSP_LPC17xx_UART3_Alt1
};


/*
*********************************************************************************************************
*********************************************************************************************************
*                                      LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*********************************************************************************************************
*/

#if (CSP_CFG_INT_ISR_ARG_EN == DEF_DISABLED)    
static  SERIAL_DEV  *SerialBSP_LPC17xx_USART0_DevPtr;
static  SERIAL_DEV  *SerialBSP_LPC17xx_USART1_DevPtr;
static  SERIAL_DEV  *SerialBSP_LPC17xx_USART2_DevPtr;
static  SERIAL_DEV  *SerialBSP_LPC17xx_USART3_DevPtr;
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                          SerialBSP_LPC17xx_UARTx_ClkEn()
*
* Description : Enable device/peripheral clock.
*
* Argument(s) : perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device/peripheral clock enabled successfully.
*
* Return(s)   : none.
*
* Caller(s)   : SerialDrv_Open() via 'pdev->BSP_API->ClkEn()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  SerialBSP_LPC17xx_UART0_ClkEn (SERIAL_ERR  *perr)
{
   CSP_IntDis(CSP_INT_CTRL_NBR_MAIN,
              CSP_INT_SRC_NBR_UART_00);
   
   CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_UART_00);
    
   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_LPC17xx_UART1_ClkEn (SERIAL_ERR  *perr)
{
   CSP_IntDis(CSP_INT_CTRL_NBR_MAIN,
              CSP_INT_SRC_NBR_UART_01);
   
   CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_UART_01);

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_LPC17xx_UART2_ClkEn(SERIAL_ERR  *perr)
{
   CSP_IntDis(CSP_INT_CTRL_NBR_MAIN,
              CSP_INT_SRC_NBR_UART_02);
   
   CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_UART_01);

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_LPC17xx_UART3_ClkEn (SERIAL_ERR  *perr)
{
   CSP_IntDis(CSP_INT_CTRL_NBR_MAIN,
              CSP_INT_SRC_NBR_UART_03);
   CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_UART_03);

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialBSP_ClkDis()
*
* Description : Disable device/peripheral clock.
*
* Argument(s) : perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device/peripheral clock disabled successfully.
*
* Return(s)   : none.
*
* Caller(s)   : SerialDrv_Close() via 'pdev->BSP_API->ClkDis()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  SerialBSP_LPC17xx_UART0_ClkDis (SERIAL_ERR  *perr)
{
   CSP_IntDis(CSP_INT_CTRL_NBR_MAIN,
              CSP_INT_SRC_NBR_UART_00);
   
   CSP_PM_PerClkDis(CSP_PM_PER_CLK_NBR_UART_00);

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_LPC17xx_UART1_ClkDis (SERIAL_ERR  *perr)
{
   CSP_IntDis(CSP_INT_CTRL_NBR_MAIN,
              CSP_INT_SRC_NBR_UART_01);
   
   CSP_PM_PerClkDis(CSP_PM_PER_CLK_NBR_UART_01);

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_LPC17xx_UART2_ClkDis (SERIAL_ERR  *perr)
{
   CSP_IntDis(CSP_INT_CTRL_NBR_MAIN,
              CSP_INT_SRC_NBR_UART_02);
   
   CSP_PM_PerClkDis(CSP_PM_PER_CLK_NBR_UART_01);

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_LPC17xx_UART3_ClkDis (SERIAL_ERR  *perr)
{
   CSP_IntDis(CSP_INT_CTRL_NBR_MAIN,
              CSP_INT_SRC_NBR_UART_03);
   
   CSP_PM_PerClkDis(CSP_PM_PER_CLK_NBR_UART_03);

   *perr = SERIAL_ERR_NONE;
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                   SerialBSP_LPC17xx_UARTx_CfgGPIO()
*
* Description : Configure device/peripheral input/output pins.
*
* Argument(s) : flow_ctrl   Flow control enabled.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device/peripheral GPIO configured successfully.
*                               SERIAL_ERR_DRV_INVALID          Device configuration invalid.
*
* Return(s)   : none.
*
* Caller(s)   : SerialDrv_Open() via 'pdev->BSP_API->CfgGPIO()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  SerialBSP_LPC17xx_UART0_CfgGPIO (CPU_BOOLEAN   flow_ctrl,
                                               SERIAL_ERR   *perr)
{
    if (flow_ctrl == DEF_ENABLED) {
        *perr = SERIAL_ERR_DRV_INVALID;
         return;
    }
                                                               /* ------------- UART 0 GPIO CONFIGURATION ------------- */
    CSP_GPIO_Cfg((CSP_DEV_NBR    )CSP_GPIO_PORT_NBR_00,        /* ... Port 0.                                           */
                 (CSP_GPIO_MSK   )(DEF_BIT_02 | DEF_BIT_03),   /* ... P0.2 = TXD0 ; P0.3 RXD0                           */
                 (CPU_BOOLEAN    )0u,                          /* ... IN/OUT not used.                                  */
                 (CSP_OPT_FLAGS  )CSP_GPIO_FLAG_MODE_NONE,     /* ... Neither Pull-up nor Pull-down                     */
                 (CPU_BOOLEAN    )DEF_NO,                      /* ... Pin interrupt disable                             */ 
                 (CSP_OPT        )0u,
                 (CSP_OPT        )CSP_GPIO_FNCT_01);            /* ... Function 01 = UART peripheral                     */

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_LPC17xx_UART1_CfgGPIO (CPU_BOOLEAN   flow_ctrl,
                                               SERIAL_ERR   *perr)
{
    if (flow_ctrl == DEF_ENABLED) {
                                                                /* ------------- UART 1 GPIO CONFIGURATION ------------ */
        CSP_GPIO_Cfg((CSP_DEV_NBR    )CSP_GPIO_PORT_NBR_00,     /* ... Port 0.                                          */
                     (CSP_GPIO_MSK   )(DEF_BIT_15 | DEF_BIT_16 | DEF_BIT_17 | DEF_BIT_18),                               
                     (CPU_BOOLEAN    )0u,                       /* ... IN/OUT not used.                                 */
                     (CSP_OPT_FLAGS  )CSP_GPIO_FLAG_MODE_NONE,  /* ... Neither Pull-up nor Pull-down                    */
                     (CPU_BOOLEAN    )DEF_NO,                   /* ... Pin interrupt disable                            */ 
                     (CSP_OPT        )0u,
                     (CSP_OPT        )CSP_GPIO_FNCT_01);        /* ... Function 01 = UART peripheral                    */                     
    } else {
        CSP_GPIO_Cfg((CSP_DEV_NBR    )CSP_GPIO_PORT_NBR_00,     /* ... Port 0.                                          */
                     (CSP_GPIO_MSK   )(DEF_BIT_15 | DEF_BIT_16),                               
                     (CPU_BOOLEAN    )0u,                       /* ... IN/OUT not used.                                 */
                     (CSP_OPT_FLAGS  )CSP_GPIO_FLAG_MODE_NONE,  /* ... Neither Pull-up nor Pull-down                    */
                     (CPU_BOOLEAN    )DEF_NO,                   /* ... Pin interrupt disable                            */ 
                     (CSP_OPT        )0u,                                      
                     (CSP_OPT        )CSP_GPIO_FNCT_01);        /* ... Function 01 = UART peripheral                    */

    }

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_LPC17xx_UART1_CfgGPIO_Alt (CPU_BOOLEAN   flow_ctrl,
                                                   SERIAL_ERR   *perr)
{
    if (flow_ctrl == DEF_ENABLED) {
                                                                /* ------------- UART 1 GPIO CONFIGURATION ------------ */
        CSP_GPIO_Cfg((CSP_DEV_NBR    )CSP_GPIO_PORT_NBR_02,     /* ... Port 2.                                          */
                     (CSP_GPIO_MSK   )(DEF_BIT_00 | DEF_BIT_01 | DEF_BIT_02 | DEF_BIT_03),                               
                     (CPU_BOOLEAN    )0u,                       /* ... IN/OUT not used.                                 */
                     (CSP_OPT_FLAGS  )CSP_GPIO_FLAG_MODE_NONE,  /* ... Neither Pull-up nor Pull-down                    */
                     (CPU_BOOLEAN    )DEF_NO,                   /* ... Pin interrupt disable                            */ 
                     (CSP_OPT        )0u,
                     (CSP_OPT        )CSP_GPIO_FNCT_02);        /* ... Function 02 = UART peripheral                    */

    } else {
        CSP_GPIO_Cfg((CSP_DEV_NBR    )CSP_GPIO_PORT_NBR_02,     /* ... Port 0.                                          */
                     (CSP_GPIO_MSK   )(DEF_BIT_00 | DEF_BIT_01),                               
                     (CPU_BOOLEAN    )0u,                       /* ... IN/OUT not used.                                 */
                     (CSP_OPT_FLAGS  )CSP_GPIO_FLAG_MODE_NONE,  /* ... Neither Pull-up nor Pull-down                    */
                     (CPU_BOOLEAN    )DEF_NO,                   /* ... Pin interrupt disable                            */ 
                     (CSP_OPT        )0u,
                     (CSP_OPT        )CSP_GPIO_FNCT_02);        /* ... Function 02 = UART peripheral                    */
    }

   *perr = SERIAL_ERR_NONE;
}


static  void  SerialBSP_LPC17xx_UART2_CfgGPIO (CPU_BOOLEAN   flow_ctrl,
                                               SERIAL_ERR   *perr)
{
    if (flow_ctrl == DEF_ENABLED) {
        *perr = SERIAL_ERR_DRV_INVALID;
         return;
    }
                                                               /* ------------- UART 0 GPIO CONFIGURATION ------------- */
    CSP_GPIO_Cfg((CSP_DEV_NBR    )CSP_GPIO_PORT_NBR_00,        /* ... Port 0.                                           */
                 (CSP_GPIO_MSK   )(DEF_BIT_10 | DEF_BIT_11),   /* ... P0.10 = TXD0 ; P0.11 RXD0                         */
                 (CPU_BOOLEAN    )0u,                          /* ... IN/OUT not used.                                  */
                 (CSP_OPT_FLAGS  )CSP_GPIO_FLAG_MODE_NONE,     /* ... Neither Pull-up nor Pull-down                     */
                 (CPU_BOOLEAN    )DEF_NO,                      /* ... Pin interrupt disable                             */ 
                 (CSP_OPT        )0u,
                 (CSP_OPT        )CSP_GPIO_FNCT_01);           /* ... Function 01 = UART peripheral                     */

   *perr = SERIAL_ERR_NONE;
}


static  void  SerialBSP_LPC17xx_UART2_CfgGPIO_Alt (CPU_BOOLEAN   flow_ctrl,
                                                   SERIAL_ERR   *perr)
{
    if (flow_ctrl == DEF_ENABLED) {
        *perr = SERIAL_ERR_DRV_INVALID;
         return;
    }
                                                               /* ------------- UART 0 GPIO CONFIGURATION ------------- */
    CSP_GPIO_Cfg((CSP_DEV_NBR    )CSP_GPIO_PORT_NBR_02,        /* ... Port 2.                                           */
                 (CSP_GPIO_MSK   )(DEF_BIT_08 | DEF_BIT_09),   /* ... P2.8 = TXD0 ; P2.9 RXD0                           */
                 (CPU_BOOLEAN    )0u,                          /* ... IN/OUT not used.                                  */
                 (CSP_OPT_FLAGS  )CSP_GPIO_FLAG_MODE_NONE,     /* ... Neither Pull-up nor Pull-down                     */
                 (CPU_BOOLEAN    )DEF_NO,                      /* ... Pin interrupt disable                             */ 
                 (CSP_OPT        )0u,
                 (CSP_OPT        )CSP_GPIO_FNCT_02);           /* ... Function 02 = UART peripheral                     */

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_LPC17xx_UART3_CfgGPIO (CPU_BOOLEAN   flow_ctrl,
                                               SERIAL_ERR   *perr)

{
    if (flow_ctrl == DEF_ENABLED) {
        *perr = SERIAL_ERR_DRV_INVALID;
         return;
    }
                                                               /* ------------- UART 0 GPIO CONFIGURATION ------------- */
    CSP_GPIO_Cfg((CSP_DEV_NBR    )CSP_GPIO_PORT_NBR_00,        /* ... Port 0.                                           */
                 (CSP_GPIO_MSK   )(DEF_BIT_00 | DEF_BIT_01),   /* ... P0.0 = TXD3 ; P0.1 RXD3                           */
                 (CPU_BOOLEAN    )0u,                          /* ... IN/OUT not used.                                  */
                 (CSP_OPT_FLAGS  )CSP_GPIO_FLAG_MODE_NONE,     /* ... Neither Pull-up nor Pull-down                     */
                 (CPU_BOOLEAN    )DEF_NO,                      /* ... Pin interrupt disable                             */ 
                 (CSP_OPT        )0u,
                 (CSP_OPT        )CSP_GPIO_FNCT_02);           /* ... Function 02 = UART peripheral                     */                 

   *perr = SERIAL_ERR_NONE;   
}

static  void  SerialBSP_LPC17xx_UART3_CfgGPIO_Alt0 (CPU_BOOLEAN   flow_ctrl,
                                                     SERIAL_ERR   *perr)

{
    if (flow_ctrl == DEF_ENABLED) {
        *perr = SERIAL_ERR_DRV_INVALID;
         return;
    }
                                                               /* ------------- UART 0 GPIO CONFIGURATION ------------- */
    CSP_GPIO_Cfg((CSP_DEV_NBR    )CSP_GPIO_PORT_NBR_00,        /* ... Port 0.                                           */
                 (CSP_GPIO_MSK   )(DEF_BIT_25 | DEF_BIT_26),   /* ... P0.25 = TXD3 ; P0.26 RXD3                         */
                 (CPU_BOOLEAN    )0u,                          /* ... IN/OUT not used.                                  */
                 (CSP_OPT_FLAGS  )CSP_GPIO_FLAG_MODE_NONE,     /* ... Neither Pull-up nor Pull-down                     */
                 (CPU_BOOLEAN    )DEF_NO,                      /* ... Pin interrupt disable                             */ 
                 (CSP_OPT        )0u,                             
                 (CSP_OPT        )CSP_GPIO_FNCT_03);           /* ... Function 03 = UART peripheral                     */                                        

   *perr = SERIAL_ERR_NONE;   
}


static  void  SerialBSP_LPC17xx_UART3_CfgGPIO_Alt1 (CPU_BOOLEAN   flow_ctrl,
                                                     SERIAL_ERR   *perr)

{
    if (flow_ctrl == DEF_ENABLED) {
        *perr = SERIAL_ERR_DRV_INVALID;
         return;
    }
                                                               /* ------------- UART 0 GPIO CONFIGURATION ------------- */
    CSP_GPIO_Cfg((CSP_DEV_NBR    )CSP_GPIO_PORT_NBR_04,        /* ... Port 4.                                           */
                 (CSP_GPIO_MSK   )(DEF_BIT_28| DEF_BIT_29),    /* ... P4.28 = TXD3 ; P4.29 TXD3                         */
                 (CPU_BOOLEAN    )0u,                          /* ... IN/OUT not used.                                  */
                 (CSP_OPT_FLAGS  )CSP_GPIO_FLAG_MODE_NONE,     /* ... Neither Pull-up nor Pull-down                     */
                 (CPU_BOOLEAN    )DEF_NO,                      /* ... Pin interrupt disable                             */ 
                 (CSP_OPT        )0u,                   
                 (CSP_OPT        )CSP_GPIO_FNCT_03);           /* ... Function 03 = UART peripheral                     */

   *perr = SERIAL_ERR_NONE;   
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialBSP_CfgInt()
*
* Description : Configure device/peripheral interrupt.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device/peripheral interrupt configured successfully.
*
* Return(s)   : none.
*
* Caller(s)   : SerialDrv_Open() via 'pdev->BSP_API->CfgInt()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  SerialBSP_LPC17xx_UART0_CfgInt (SERIAL_DEV  *pdev,
                                              SERIAL_ERR  *perr)
{
#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)        
   CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN,
                  (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_00,
                  (CPU_FNCT_PTR  )SerialBSP_LPC17xx_UARTx_IntHandler,
                  (void         *)pdev);
#else

   SerialBSP_LPC17xx_USART0_DevPtr = pdev;
   
   CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN,
                  (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_00,
                  (CPU_FNCT_PTR  )SerialBSP_LPC17xx_UART0_IntHandler,
                  (void         *)pdev);
#endif   
   
   CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_UART_00);
    
   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_LPC17xx_UART1_CfgInt (SERIAL_DEV  *pdev,
                                              SERIAL_ERR  *perr)
{
#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)
    CSP_IntVectReg((CSP_DEV_NBR  )CSP_INT_CTRL_NBR_MAIN,
                  (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_01,
                  (CPU_FNCT_PTR  )SerialBSP_LPC17xx_UARTx_IntHandler,
                  (CPU_VOID     *)pdev);
#else

    SerialBSP_LPC17xx_USART1_DevPtr = pdev;

    CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN,
                   (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_01,
                   (CPU_FNCT_PTR  )SerialBSP_LPC17xx_UART1_IntHandler,
                   (void         *)pdev);
#endif    
   
   CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_UART_01);
    
   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_LPC17xx_UART2_CfgInt (SERIAL_DEV  *pdev,
                                              SERIAL_ERR  *perr)
{

#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)
    CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN,
                  (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_02,
                  (CPU_FNCT_PTR  )SerialBSP_LPC17xx_UARTx_IntHandler,
                  (void         *)pdev);
#else
    SerialBSP_LPC17xx_USART2_DevPtr = pdev;
    
    CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN,
                  (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_02,
                  (CPU_FNCT_PTR  )SerialBSP_LPC17xx_UART2_IntHandler,
                  (void         *)pdev);

#endif
    
   CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_UART_02);
    
   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_LPC17xx_UART3_CfgInt (SERIAL_DEV  *pdev,
                                              SERIAL_ERR  *perr)
{
#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)
   CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN,
                  (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_03,
                  (CPU_FNCT_PTR  )SerialBSP_LPC17xx_UARTx_IntHandler,
                  (CPU_VOID     *)pdev);
#else
   SerialBSP_LPC17xx_USART3_DevPtr = pdev;

   CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN,
                  (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_03,
                  (CPU_FNCT_PTR  )SerialBSP_LPC17xx_UART3_IntHandler,
                  (void         *)pdev);
#endif
   
   CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_UART_03);
    
   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                SerialBSP_LPC17xx_UARTx_ClkFreqGet()
*
* Description : Get peripheral clock frequency.
*
* Argument(s) : none.
*
* Return(s)   : Peripheral clock frequency, in Hz.
*
* Caller(s)   : SerialDrv_Open() via 'pdev->BSP_API->ClkFreqGet()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_INT32U  SerialBSP_LPC17xx_UART0_ClkFreqGet (void)
{
    CPU_INT32U  clk_freq;


    clk_freq = CSP_PM_PerClkFreqGet(CSP_PM_PER_CLK_NBR_UART_00);
    return (clk_freq);
}


static  CPU_INT32U  SerialBSP_LPC17xx_UART1_ClkFreqGet (void)
{
    CPU_INT32U  clk_freq;


    clk_freq = CSP_PM_PerClkFreqGet(CSP_PM_PER_CLK_NBR_UART_01);
    return (clk_freq);
}


static  CPU_INT32U  SerialBSP_LPC17xx_UART2_ClkFreqGet (void)
{
    CPU_INT32U  clk_freq;


    clk_freq = CSP_PM_PerClkFreqGet(CSP_PM_PER_CLK_NBR_UART_01);
    return (clk_freq);
}


static  CPU_INT32U  SerialBSP_LPC17xx_UART3_ClkFreqGet (void)
{
    CPU_INT32U  clk_freq;


    clk_freq = CSP_PM_PerClkFreqGet(CSP_PM_PER_CLK_NBR_UART_03);
    return (clk_freq);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                       SerialBSP_LPC17xx_UARTx_IntHandler()
*
* Description : Handle serial port interrupt.
*
* Argument(s) : p_arg     Pointer to the Interrupt ISR argument.
*                         
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)
static  void  SerialBSP_LPC17xx_UARTx_IntHandler (void *p_arg)
{
    SERIAL_DEV      *p_dev;    
    SERIAL_DEV_CFG  *p_cfg;

    p_dev  = (SERIAL_DEV *)p_arg;
    p_cfg  = p_dev->Dev_Cfg;    
    p_cfg->Drv_API->ISR_Handler(p_dev, SERIAL_ISR_TYPE_UNKNOWN);
}
#else
static  void  SerialBSP_LPC17xx_UART0_IntHandler  (void *p_arg) 
{
    SERIAL_DEV      *p_dev;
    SERIAL_DEV_CFG  *p_cfg;

    
    p_dev  = (SERIAL_DEV *)SerialBSP_LPC17xx_USART0_DevPtr;
    p_cfg  = p_dev->Dev_Cfg;    
    p_cfg->Drv_API->ISR_Handler(p_dev, SERIAL_ISR_TYPE_UNKNOWN);

}
static  void  SerialBSP_LPC17xx_UART1_IntHandler  (void *p_arg)
{
    SERIAL_DEV      *p_dev;
    SERIAL_DEV_CFG  *p_cfg;

    
    p_dev  = (SERIAL_DEV *)SerialBSP_LPC17xx_USART1_DevPtr;
    p_cfg  = p_dev->Dev_Cfg;    
    p_cfg->Drv_API->ISR_Handler(p_dev, SERIAL_ISR_TYPE_UNKNOWN);

}

static  void  SerialBSP_LPC17xx_UART2_IntHandler  (void *p_arg)
{
    SERIAL_DEV      *p_dev;
    SERIAL_DEV_CFG  *p_cfg;

    
    p_dev  = (SERIAL_DEV *)SerialBSP_LPC17xx_USART2_DevPtr;
    p_cfg  = p_dev->Dev_Cfg;    
    p_cfg->Drv_API->ISR_Handler(p_dev, SERIAL_ISR_TYPE_UNKNOWN);
}
static  void  SerialBSP_LPC17xx_UART3_IntHandler  (void *p_arg)
{
    SERIAL_DEV      *p_dev;
    SERIAL_DEV_CFG  *p_cfg;

    
    p_dev  = (SERIAL_DEV *)SerialBSP_LPC17xx_USART3_DevPtr;
    p_cfg  = p_dev->Dev_Cfg;    
    p_cfg->Drv_API->ISR_Handler(p_dev, SERIAL_ISR_TYPE_UNKNOWN);
}
#endif
