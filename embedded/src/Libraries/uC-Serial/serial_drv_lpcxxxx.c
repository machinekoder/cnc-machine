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
*                         NXP LPC23xx     series.
*                         NXP LPC24xx     series.
*                         NXP LPC17xx     series.
*                         NXP LPC3180/81  series. (see note #2)
*                         NXP LPC32x0     series. (see note #2)
*
*                 (2) The LPC32x0 series and the LPC3180/81 use the 'SerialDrv_LPC3xxx_API' API, this 
*                     is because there are small differences in the register definition.
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define   SERIAL_DRV_LPCXXXX_MODULE
#include  <serial.h>
#include  <serial_drv_lpcxxxx.h>


/*
*********************************************************************************************************
*                                        BAUD RATE CALCULATION ALGORITHM CONSTANST
*********************************************************************************************************
*/

#define  SERIAL_DRV_LPCXXXX_VAL_MULVAL                       15u
#define  SERIAL_DRV_LPCXXXX_VAL_DIVAL_MAX_IX                 14u


/*
*********************************************************************************************************
*                                                   REGISTER BIT DEFINES
*********************************************************************************************************
*/
                                                                /* --------- LINE CONTROL REGISTER BIT DEFINES -------- */
#define  SERIAL_DRV_LPCXXXX_BIT_LCR_WLS_05                DEF_BIT_NONE
#define  SERIAL_DRV_LPCXXXX_BIT_LCR_WLS_06                DEF_BIT_MASK(1u, 0u)
#define  SERIAL_DRV_LPCXXXX_BIT_LCR_WLS_07                DEF_BIT_MASK(2u, 0u)
#define  SERIAL_DRV_LPCXXXX_BIT_LCR_WLS_08                DEF_BIT_MASK(3u, 0u)
#define  SERIAL_DRV_LPCXXXX_BIT_LCR_STOP_BIT              DEF_BIT_02
#define  SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_EN                DEF_BIT_03
#define  SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_ODD               DEF_BIT_NONE
#define  SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_EVEN              DEF_BIT_MASK(1u, 4)
#define  SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_FORCED_ONE        DEF_BIT_MASK(2u, 4)
#define  SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_FORCED_ZERO       DEF_BIT_MASK(3u, 4)
#define  SERIAL_DRV_LPCXXXX_BIT_LCR_DLAB                  DEF_BIT_07

                                                                /* ------------- FIFO REGISTER BIT DEFINES ------------ */
#define  SERIAL_DRV_LPCXXXX_BIT_FCR_FIFO_EN               DEF_BIT_00
#define  SERIAL_DRV_LPCXXXX_BIT_FCR_RX_FIFO_RST           DEF_BIT_01
#define  SERIAL_DRV_LPCXXXX_BIT_FCR_TX_FIFO_RST           DEF_BIT_02
#define  SERIAL_DRV_LPCXXXX_BIT_FCR_FIFO_CTRL             DEF_BIT_03
#define  SERIAL_DRV_LPCXXXX_BIT_FCR_RX_TRIG_LEVEL_01      DEF_BIT_NONE
#define  SERIAL_DRV_LPCXXXX_BIT_FCR_RX_TRIG_LEVEL_04      DEF_BIT_MASK(1u, 6)
#define  SERIAL_DRV_LPCXXXX_BIT_FCR_RX_TRIG_LEVEL_08      DEF_BIT_MASK(2u, 6)
#define  SERIAL_DRV_LPCXXXX_BIT_FCR_RX_TRIG_LEVEL_14      DEF_BIT_MASK(3u, 6)

                                                                /* ------ INTERRUPT ENABLE REGISTER BIT DEFINES ------- */
#define  SERIAL_DRV_LPCXXXX_BIT_IER_RBR                   DEF_BIT_00
#define  SERIAL_DRV_LPCXXXX_BIT_IER_THRE                  DEF_BIT_01
#define  SERIAL_DRV_LPCXXXX_BIT_IER_RX_LINE               DEF_BIT_02

                                                                /* ---------- LINE STATUS REGISTER BIT DEFINES -------- */
#define  SERIAL_DRV_LPCXXXX_BIT_LSR_RDR                   DEF_BIT_00
#define  SERIAL_DRV_LPCXXXX_BIT_LSR_OE                    DEF_BIT_01
#define  SERIAL_DRV_LPCXXXX_BIT_LSR_PE                    DEF_BIT_02
#define  SERIAL_DRV_LPCXXXX_BIT_LSR_FE                    DEF_BIT_03
#define  SERIAL_DRV_LPCXXXX_BIT_LSR_RX_ERR               (SERIAL_DRV_LPCXXXX_BIT_LSR_OE | \
                                                          SERIAL_DRV_LPCXXXX_BIT_LSR_PE | \
                                                          SERIAL_DRV_LPCXXXX_BIT_LSR_FE)
#define  SERIAL_DRV_LPCXXXX_BIT_LSR_BI                    DEF_BIT_04
#define  SERIAL_DRV_LPCXXXX_BIT_LSR_THRE                  DEF_BIT_05
#define  SERIAL_DRV_LPCXXXX_BIT_LSR_TEMT                  DEF_BIT_06
#define  SERIAL_DRV_LPCXXXX_BIT_LSR_RXFE                  DEF_BIT_07

                                                                /* -- INTERRUPT IDENTIFICATION REGISTER BIT DEFINES  -- */
#define  SERIAL_DRV_LPCXXXX_BIT_IIR_INT_STAT              DEF_BIT_00
#define  SERIAL_DRV_LPCXXXX_MSK_IIR_INT_ID                DEF_BIT_FIELD(3u, 1u)
#define  SERIAL_DRV_LPCXXXX_BIT_IIR_INT_ID_RLS            3u    /* Receive Line Status (RLS)                            */
#define  SERIAL_DRV_LPCXXXX_BIT_IIR_INT_ID_RDA            2u    /* Receive Data Available (RDA)                         */
#define  SERIAL_DRV_LPCXXXX_BIT_IIR_INT_ID_THRE           1u    /* Transmit Holding Register Interrupt                  */
#define  SERIAL_DRV_LPCXXXX_BIT_IIR_INT_ID_CTI            6u    /* Character Time-Out indicator                         */

                                                                /* ------- TRANSMIT ENABLE REGISTER BIT DEFINES ------- */
#define  SERIAL_DRV_LPCXXXX_BIT_TER_TXEN                  DEF_BIT_07


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/

typedef  struct  serial_drv_reg {
    CPU_REG32  RBR;                                             /* Receive buffer     Register.                         */
    CPU_REG32  DLM;                                             /* Divisor Latch MSB  Register.                         */
    CPU_REG32  IIR;                                             /* Interrupt ID       Register.                         */
    CPU_REG32  LCR;                                             /* Line Control       Register.                         */
    CPU_REG32  MCR;                                             /* Modem Control      Register.                         */
    CPU_REG32  LSR;                                             /* Line Status        Register.                         */
    CPU_REG32  MSR;                                             /* Modem Status       Register.                         */
    CPU_REG32  SCR;                                             /* Scratch Pad        Register.                         */
    CPU_REG32  ACR;                                             /* Auto-baud control  Register.                         */
    CPU_REG32  RESERVED0;
    CPU_REG32  FDR;                                             /* Fractional Divider Register.                         */
    CPU_REG32  TER;                                             /* Transmit Enable    Register.                         */
    CPU_REG32  RESERVED1[9u];
    CPU_REG32  FIFOLVL;                                         /* FIFO Level         Register.                         */
} SERIAL_DRV_REG;


typedef  struct  serial_drv_reg_lpc3xxx {
    CPU_REG32  RBR;                                             /* Receive buffer     Register.                         */
    CPU_REG32  DLM;                                             /* Divisor Latch MSB  Register.                         */
    CPU_REG32  IIR;                                             /* Interrupt ID       Register.                         */
    CPU_REG32  LCR;                                             /* Line Control       Register.                         */
    CPU_REG32  MCR;                                             /* Modem Control      Register.                         */
    CPU_REG32  LSR;                                             /* Line Status        Register.                         */
    CPU_REG32  MSR;                                             /* Modem Status       Register.                         */
    CPU_REG32  FIFOLVL;                                         /* FIFO Level         Register.                         */
} SERIAL_DRV_REG_LPC3xxx;


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
                                                                /* Driver initialization.                               */
static  void        SerialDrv_Init            (SERIAL_ERR     *perr);

                                                                /* Driver open.                                         */
static  void        SerialDrv_Open            (SERIAL_DEV     *pdev,
                                               SERIAL_IF_CFG  *pcfg,
                                               SERIAL_ERR     *perr);

static  void        SerialDrv_LPC3xxx_Open    (SERIAL_DEV     *pdev,
                                               SERIAL_IF_CFG  *pcfg,
                                               SERIAL_ERR     *perr);

                                                                /* Driver close.                                        */
static  void        SerialDrv_Close           (SERIAL_DEV     *pdev,
                                               SERIAL_ERR     *perr);

static  void        SerialDrv_LPC3xxx_Close   (SERIAL_DEV     *pdev,
                                               SERIAL_ERR     *perr);


                                                                /* Driver receiver start.                               */
static  void        SerialDrv_RxStart          (SERIAL_DEV     *pdev,
                                                SERIAL_ERR     *perr);

                                                                /* Driver receiver stop.                                */
static  void        SerialDrv_RxStop           (SERIAL_DEV     *pdev,
                                                SERIAL_ERR     *perr);

                                                                /* Driver octet receive.                                */
static  void        SerialDrv_RxOctet          (SERIAL_DEV     *pdev,
                                                CPU_INT08U     *pdatum,
                                                SERIAL_ERR     *perr);

                                                                /* Driver transmitter start.                            */
static  void        SerialDrv_TxStart          (SERIAL_DEV     *pdev,
                                                SERIAL_ERR     *perr);

static  void        SerialDrv_LPC3xxx_TxStart  (SERIAL_DEV     *pdev,
                                                SERIAL_ERR     *perr);

                                                                /* Driver transmitter stop.                             */
static  void        SerialDrv_TxStop           (SERIAL_DEV     *pdev,
                                                SERIAL_ERR     *perr);

static  void        SerialDrv_LPC3xxx_TxStop   (SERIAL_DEV     *pdev,
                                                SERIAL_ERR     *perr);

                                                                /* Driver octet transmit.                               */
static  void        SerialDrv_TxOctet          (SERIAL_DEV     *pdev,
                                                CPU_INT08U      datum,
                                                SERIAL_ERR     *perr);

                                                                /* Driver ISR handler.                                  */
static  void        SerialDrv_ISR_Handler      (SERIAL_DEV     *pdev,
                                                CPU_INT08U      type);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                 SERIAL INTERFACE DEVICE DRIVER API
*********************************************************************************************************
*********************************************************************************************************
*/

SERIAL_DRV_API  SerialDrv_LPCxxxx_API = {
    SerialDrv_Init,
    SerialDrv_Open,
    SerialDrv_Close,
    SerialDrv_RxStart,
    SerialDrv_RxStop,
    SerialDrv_RxOctet,
    SerialDrv_TxStart,
    SerialDrv_TxStop,
    SerialDrv_TxOctet,
    SerialDrv_ISR_Handler
};

SERIAL_DRV_API  SerialDrv_LPC3xxx_API = {
    SerialDrv_Init,
    SerialDrv_LPC3xxx_Open,
    SerialDrv_LPC3xxx_Close,
    SerialDrv_RxStart,
    SerialDrv_RxStop,
    SerialDrv_RxOctet,
    SerialDrv_LPC3xxx_TxStart,
    SerialDrv_LPC3xxx_TxStop,
    SerialDrv_TxOctet,
    SerialDrv_ISR_Handler
};


/*$PAGE*/
/*
*********************************************************************************************************
*********************************************************************************************************
*                                     DRIVER INTERFACE FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          SerialDrv_Init()
*
* Description : Initialize serial device driver.
*
* Argument(s) : perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Driver initialized.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_DrvAdd() via 'pdrv_api->Init()'.
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  void  SerialDrv_Init (SERIAL_ERR  *perr)
{
   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                          SerialDrv_Open()
*                                       SerialDrv_LPC3xxx_Open()
*
* Description : Open a serial device for communication.
*
* Argument(s) : pdev        Pointer to device.
*
*               pcfg        Pointer to interface configuration.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device              opened.
*                               SERIAL_ERR_DRV_OPEN             Device could NOT be opened.
*                               SERIAL_ERR_DRV_INVALID          Device configuration invalid.
*                               SERIAL_ERR_MEM_ALLOC            Memory could NOT be allocated for device
*                                                                   internal data.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Open() via 'pdev->Drv_API->Open()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*
*               (2) The LPCxxx UART baud rate can be calculated as:
*
*                                             PCLK
*                   BAUD_RATE  =  ---------------------------------
*                                                      DivAddval
*                                  16 x  BRD  x ( 1 + ----------- )
*                                                       MulVal 
*
*                  where PCLK      = UART Peripheral Clock.
*                        BRD       = Baud rate divider (integer)
*                        DivAddVal = Fractional Baud rate divider.
*                        MulVal    = Fractional Baud rate multiplier.
*
*              (3) The LPC3xxx UART baud rate can be calculated as:
*
*                                         Source Clock
*                   BAUD_RATE  =  ---------------------------------
*                                           16 x  BRD  
*                                             
*                  where Source Clock  = Is either HCLK or PCLK divided with the preescaler.
*                        BRD           = Baud rate divider (integer)
*********************************************************************************************************
*/

static  void  SerialDrv_Open (SERIAL_DEV     *pdev,
                              SERIAL_IF_CFG  *pcfg,
                              SERIAL_ERR     *perr)
{

    CPU_INT32U       br_div_num;                                /* Baud Rate divider numerator.                         */
    CPU_INT32U       br_div_den;                                /* Baud Rate divider denominator.                       */   
    CPU_INT32U       br_div;                                    /* Baud Rate divider.                                   */
    CPU_INT32U       br_div_tmp;                                /* Baud Rate divider temporal.                          */    
    CPU_INT32U       br_val_err;                                /* Baud Rate value error.                               */
    CPU_INT32U       br_val_err_max;                            /* Baud Rate Maximum error.                             */    
    CPU_INT32U       br_val;                                    /* Baud Rate Maximum error.                             */    
    CPU_BOOLEAN      div_found;
    CPU_INT08U       fp_div;                                    /* Fractional part multiplier.                          */
    CPU_INT08U       fp_div_ix;                                 /* Fractional part multiplier index.                    */
    CPU_BOOLEAN      flow_ctrl;
    CPU_INT32U       per_clk_freq;                              /* Peripheral Clock frequency.                          */
    CPU_INT32U       reg_val;
    SERIAL_DRV_REG  *p_reg;
    SERIAL_DEV_CFG  *p_cfg;
    
    
    p_cfg         = pdev->Dev_Cfg;
                                                                /* -------------- BAUD RATE CALCULATION --------------- */
    per_clk_freq  = p_cfg->BSP_API->ClkFreqGet();               /* Get the UART peripheral clock                        */

    if (pcfg->Baudrate > per_clk_freq) {                        /* If baud rate higher than peripheral clock?           */
        *perr =  SERIAL_ERR_DRV_INVALID;
    }
    
    br_div_num     = (per_clk_freq);
    br_div_den     = (pcfg->Baudrate * 16u);
    div_found      = DEF_NO;
    br_val_err     = DEF_INT_32U_MAX_VAL;
    br_div_tmp     = (br_div_num / br_div_den); 
    br_div         = 0u;
    br_val_err_max = DEF_INT_32U_MAX_VAL;
    fp_div         = 0u;

    if ((br_div_tmp != 0u             ) &&                     /* If the baud rate divider is valid.                   */
        (br_div_tmp <= DEF_INT_16_MASK)) {
        br_div    = br_div_tmp;                                /* ... store the result.                                */
        fp_div    = 0u; 
        div_found = DEF_YES;
        br_val    = (br_div_num / (16u * br_div));
        if (br_val > pcfg->Baudrate) {
            br_val_err_max = br_val - pcfg->Baudrate;
        } else {
            br_val_err_max = pcfg->Baudrate - br_val;        
        }
    }
    
    if ((br_val_err_max != 0u     ) ||
        (div_found      != DEF_YES)) {
    
        br_div_num *= SERIAL_DRV_LPCXXXX_VAL_MULVAL;            /* Pre-calculate the numerator                          */
                                                                /* Try from divider 1 to maximum divider                */
        for (fp_div_ix = 1u; fp_div_ix <= SERIAL_DRV_LPCXXXX_VAL_DIVAL_MAX_IX; fp_div_ix++) {
            br_div_den = (pcfg->Baudrate * 16u)
                       * (SERIAL_DRV_LPCXXXX_VAL_MULVAL + (CPU_INT32U)fp_div_ix);
            br_div_tmp = (br_div_num / br_div_den);             /* Calculate the baud rate divider                      */
            
            if ((br_div_tmp != 0u             ) &&              /* Check if the baud rate is valid                      */
                (br_div_tmp <= DEF_INT_16_MASK)) {
                                                                /* Calculate the remainer                               */
                br_div_den = ((br_div_tmp * 16u)
                           * ((CPU_INT32U)fp_div_ix + SERIAL_DRV_LPCXXXX_VAL_MULVAL));
                br_val     = (br_div_num / br_div_den);
                                                                /* Calculate the error between the found value and ...  */
                                                                /* ... and the expected value.                          */                
                if (br_val > pcfg->Baudrate) {
                    br_val_err = br_val - pcfg->Baudrate;
                } else {
                    br_val_err = pcfg->Baudrate - br_val;        
                }
                
                if (br_val_err < br_val_err_max) {              /* If the value found reduces the error ...             */
                    br_div         = br_div_tmp;                /* ... Store the result to be written to the registers  */
                    fp_div         = fp_div_ix;
                    div_found      = DEF_YES;
                    br_val_err_max = br_val_err;
                }
            }
        }                
    }
    
    if (div_found == DEF_NO) {
       *perr = SERIAL_ERR_DRV_INVALID;
        return;
    }
 
                                                                /* ---------------- UART LINE CONFIGUARTION ----------- */
    reg_val = DEF_BIT_NONE;
    
    switch (pcfg->DataBits) {                                   /* ... World Length Configuration.                      */
        case SERIAL_DATABITS_5:
             reg_val = SERIAL_DRV_LPCXXXX_BIT_LCR_WLS_05;
             break;

        case SERIAL_DATABITS_6:
             reg_val =SERIAL_DRV_LPCXXXX_BIT_LCR_WLS_06;
             break;
             
        case SERIAL_DATABITS_7:
             reg_val = SERIAL_DRV_LPCXXXX_BIT_LCR_WLS_07;
             break;

        case SERIAL_DATABITS_8:
             reg_val = SERIAL_DRV_LPCXXXX_BIT_LCR_WLS_08;
             break;
       
        default:
             *perr = SERIAL_ERR_DRV_INVALID;
             return;             
    }
    
    switch (pcfg->StopBits) {                                   /* ... Stop Bits configuration.                         */
        case SERIAL_STOPBITS_1:
             break;

        case SERIAL_STOPBITS_1_5:
             if (pcfg->DataBits == (CPU_INT08U)SERIAL_DATABITS_5) {
                 DEF_BIT_SET(reg_val, SERIAL_DRV_LPCXXXX_BIT_LCR_STOP_BIT);
             } else {             
                 *perr = SERIAL_ERR_DRV_INVALID;
                  return;
             }
             break;

        case SERIAL_STOPBITS_2:
             DEF_BIT_SET(reg_val, SERIAL_DRV_LPCXXXX_BIT_LCR_STOP_BIT);
             break;

        default:
            *perr = SERIAL_ERR_DRV_INVALID;
            return; 
    }
    
    switch (pcfg->Parity) {
        case SERIAL_PARITY_ODD:
             DEF_BIT_SET(reg_val, SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_EN | 
                                  SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_ODD);
             break;


        case SERIAL_PARITY_EVEN:
             DEF_BIT_SET(reg_val, SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_EN | 
                                  SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_ODD);
             break;


        case SERIAL_PARITY_MARK:
             DEF_BIT_SET(reg_val, SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_EN | 
                                  SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_FORCED_ONE);
             break;

        case SERIAL_PARITY_SPACE:
             DEF_BIT_SET(reg_val, SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_EN | 
                                  SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_FORCED_ZERO);
             break;

        case SERIAL_PARITY_NONE:
             break;
             
        default:
            *perr = SERIAL_ERR_DRV_INVALID;
             return; 
    }

    p_reg = (SERIAL_DRV_REG *)p_cfg->BaseAddr;

    p_cfg->BSP_API->ClkEn(perr);                                /* En dev-specific HW clk.                              */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }
        
    p_reg->LCR = DEF_BIT_NONE;                                  /* Disable Interrupts.                                  */
    p_reg->DLM = DEF_BIT_NONE;

    p_reg->LCR = reg_val;                                       /* Configure the Line control register.                 */
             
                                                                /* Baud Rate configuration ...                          */
    DEF_BIT_SET(p_reg->LCR, SERIAL_DRV_LPCXXXX_BIT_LCR_DLAB);   /* ... Enable DLAB bit (access Divisor Latch register ) */
    p_reg->RBR   = (br_div      ) & DEF_INT_08_MASK;            /* ... Set LSB Divisor Latch                            */
    p_reg->DLM   = (br_div >> 8u) & DEF_INT_08_MASK;            /* ... Set MSB Divisor Latch                            */
    DEF_BIT_CLR(p_reg->LCR, SERIAL_DRV_LPCXXXX_BIT_LCR_DLAB);   /* ... Disable DLAB bit                                 */    
    p_reg->FDR   = SERIAL_DRV_LPCXXXX_VAL_MULVAL << 4u          /* ... Set the fractional divider                       */
                 | fp_div;

                                                                /* --------------- FIFOs CONFIGURATION  --------------- */
    p_reg->IIR   = SERIAL_DRV_LPCXXXX_BIT_FCR_RX_FIFO_RST       /* Reset the TX and RX FIFOs. FIFO is disabled          */
                 | SERIAL_DRV_LPCXXXX_BIT_FCR_TX_FIFO_RST;
    
    p_reg->IIR   = SERIAL_DRV_LPCXXXX_BIT_FCR_FIFO_EN           /* Enable the FIFO & Set the Rx trigger level to 1      */
                 | SERIAL_DRV_LPCXXXX_BIT_FCR_RX_TRIG_LEVEL_01;      
    
                   
    switch (pcfg->FlowCtrl) {
        case SERIAL_FLOW_CTRL_HARDWARE:
             flow_ctrl  = DEF_ENABLED;
             break;

        case SERIAL_FLOW_CTRL_NONE:
        default:
             flow_ctrl  = DEF_DISABLED;
             break;
    }

    p_cfg->BSP_API->CfgGPIO(flow_ctrl, perr);                   /* Cfg dev-specific GPIO.                               */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }
    p_cfg->BSP_API->CfgInt(pdev, perr);                         /* Cfg dev-specific int.                                */
}



static  void  SerialDrv_LPC3xxx_Open (SERIAL_DEV     *pdev,
                                      SERIAL_IF_CFG  *pcfg,
                                      SERIAL_ERR    *perr)
{

    CPU_INT32U       br_div;                                    /* Baud Rate divider.                                   */
    CPU_BOOLEAN      flow_ctrl;
    CPU_INT32U       per_clk_freq;                              /* Peripheral Clock frequency.                          */
    CPU_INT32U       reg_val;
    SERIAL_DRV_REG  *p_reg;
    SERIAL_DEV_CFG  *p_cfg;
    
    
    p_cfg  = pdev->Dev_Cfg;        
                                                                /* -------------- BAUD RATE CALCULATION --------------- */
    per_clk_freq  = p_cfg->BSP_API->ClkFreqGet();               /* Get the UART peripheral clock                        */

    if (pcfg->Baudrate > per_clk_freq) {                        /* If baud rate higher than peripheral clock?           */
        *perr =  SERIAL_ERR_DRV_INVALID;
    }

    br_div         =  (((per_clk_freq) / (8u * pcfg->Baudrate)) + 1u) / 2u;
                                                                /* ---------------- UART LINE CONFIGUARTION ----------- */
    reg_val = DEF_BIT_NONE;
    
    switch (pcfg->DataBits) {                                   /* ... World Length Configuration.                      */
        case SERIAL_DATABITS_5:
             reg_val = SERIAL_DRV_LPCXXXX_BIT_LCR_WLS_05;
             break;

        case SERIAL_DATABITS_6:
             reg_val =SERIAL_DRV_LPCXXXX_BIT_LCR_WLS_06;
             break;
             
        case SERIAL_DATABITS_7:
             reg_val = SERIAL_DRV_LPCXXXX_BIT_LCR_WLS_07;
             break;

        case SERIAL_DATABITS_8:
             reg_val = SERIAL_DRV_LPCXXXX_BIT_LCR_WLS_08;
             break;
       
        default:
             *perr = SERIAL_ERR_DRV_INVALID;
             return;             
    }
    
    switch (pcfg->StopBits) {                                   /* ... Stop Bits configuration.                         */
        case SERIAL_STOPBITS_1:
             break;

        case SERIAL_STOPBITS_1_5:
             if (pcfg->DataBits == (CPU_INT08U)SERIAL_DATABITS_5) {
                 DEF_BIT_SET(reg_val, SERIAL_DRV_LPCXXXX_BIT_LCR_STOP_BIT);
             } else {             
                 *perr = SERIAL_ERR_DRV_INVALID;
                  return;
             }
             break;

        case SERIAL_STOPBITS_2:
             DEF_BIT_SET(reg_val, SERIAL_DRV_LPCXXXX_BIT_LCR_STOP_BIT);
             break;

        default:
            *perr = SERIAL_ERR_DRV_INVALID;
            return; 
    }
    
    switch (pcfg->Parity) {
        case SERIAL_PARITY_ODD:
             DEF_BIT_SET(reg_val, SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_EN | 
                                  SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_ODD);
             break;


        case SERIAL_PARITY_EVEN:
             DEF_BIT_SET(reg_val, SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_EN | 
                                  SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_ODD);
             break;


        case SERIAL_PARITY_MARK:
             DEF_BIT_SET(reg_val, SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_EN | 
                                  SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_FORCED_ONE);
             break;

        case SERIAL_PARITY_SPACE:
             DEF_BIT_SET(reg_val, SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_EN | 
                                  SERIAL_DRV_LPCXXXX_BIT_LCR_PAR_FORCED_ZERO);
             break;

        case SERIAL_PARITY_NONE:
             break;
             
        default:
            *perr = SERIAL_ERR_DRV_INVALID;
             return; 
    }

    p_reg = (SERIAL_DRV_REG *)p_cfg->BaseAddr;

    p_cfg->BSP_API->ClkEn(perr);                                /* En dev-specific HW clk.                              */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }
        
    p_reg->LCR = DEF_BIT_NONE;                                  /* Disable Interrupts.                                  */
    p_reg->DLM = DEF_BIT_NONE;

    p_reg->LCR = reg_val;                                       /* Configure the Line control register.                 */
             
                                                                /* Baud Rate configuration ...                          */
    DEF_BIT_SET(p_reg->LCR, SERIAL_DRV_LPCXXXX_BIT_LCR_DLAB);   /* ... Enable DLAB bit (access Divisor Latch register ) */
    p_reg->RBR   = (br_div      ) & DEF_INT_08_MASK;            /* ... Set LSB Divisor Latch                            */
    p_reg->DLM   = (br_div >> 8u) & DEF_INT_08_MASK;            /* ... Set MSB Divisor Latch                            */
    DEF_BIT_CLR(p_reg->LCR, SERIAL_DRV_LPCXXXX_BIT_LCR_DLAB);   /* ... Disable DLAB bit                                 */    
    
                                                                /* --------------- FIFOs CONFIGURATION  --------------- */
    p_reg->IIR   = SERIAL_DRV_LPCXXXX_BIT_FCR_RX_FIFO_RST       /* Reset the TX and RX FIFOs. FIFO is disabled          */
                 | SERIAL_DRV_LPCXXXX_BIT_FCR_TX_FIFO_RST;
        
                   
    switch (pcfg->FlowCtrl) {
        case SERIAL_FLOW_CTRL_HARDWARE:
             flow_ctrl  = DEF_ENABLED;
             break;

        case SERIAL_FLOW_CTRL_NONE:
        default:
             flow_ctrl  = DEF_DISABLED;
             break;
    }

    p_cfg->BSP_API->CfgGPIO(flow_ctrl, perr);                   /* Cfg dev-specific GPIO.                               */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }
    p_cfg->BSP_API->CfgInt(pdev, perr);                         /* Cfg dev-specific int.                                */
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                          SerialDrv_Close()
*                                      SerialDrv_Close_LPC3xxx()
*
* Description : Close a serial device for communication.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device closed.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Close() via 'pdev->Drv_API->Close()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_Close (SERIAL_DEV  *pdev,
                               SERIAL_ERR  *perr)
{
    SERIAL_DRV_REG  *p_reg;
    SERIAL_DEV_CFG  *p_cfg;

    
    p_cfg = pdev->Dev_Cfg;
    p_reg = (SERIAL_DRV_REG *)p_cfg->BaseAddr;
    
    DEF_BIT_CLR(p_reg->LCR, SERIAL_DRV_LPCXXXX_BIT_LCR_DLAB);   /* ... Disable DLAB bit.                                */
    p_reg->DLM = DEF_BIT_NONE;
    p_reg->IIR = DEF_BIT_NONE;
    p_reg->LCR = DEF_BIT_NONE;
    p_reg->FDR = DEF_BIT_NONE;

    p_cfg->BSP_API->ClkDis(perr);                               /* Dis dev-specific HW clk.                             */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }

   *perr = SERIAL_ERR_NONE;
}


static  void  SerialDrv_LPC3xxx_Close (SERIAL_DEV  *pdev,
                                       SERIAL_ERR  *perr)
{
    SERIAL_DRV_REG  *p_reg;
    SERIAL_DEV_CFG  *p_cfg;
    
    
    p_cfg  = pdev->Dev_Cfg;   
    p_reg  = (SERIAL_DRV_REG *)p_cfg->BaseAddr;

    DEF_BIT_CLR(p_reg->LCR, SERIAL_DRV_LPCXXXX_BIT_LCR_DLAB);   /* ... Disable DLAB bit.                                */
    p_reg->DLM = DEF_BIT_NONE;
    p_reg->IIR = DEF_BIT_NONE;
    p_reg->LCR = DEF_BIT_NONE;

    p_cfg->BSP_API->ClkDis(perr);                               /* Dis dev-specific HW clk.                             */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }

   *perr = SERIAL_ERR_NONE;
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_RxStart()
*
* Description : Start receiving data & getting data received interrupts.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device reception started.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Open(),
*               Serial_IF_RxStart() via 'pdev->Drv_API->RxStart()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_RxStart (SERIAL_DEV  *pdev,
                                 SERIAL_ERR  *perr)
{
    SERIAL_DRV_REG  *p_reg;
    SERIAL_DEV_CFG  *p_cfg;
    
    
    p_cfg = pdev->Dev_Cfg;           
    p_reg = (SERIAL_DRV_REG *)p_cfg->BaseAddr;    
                                                                /* Enable the Receive Data available & RX line Status.  */
    DEF_BIT_SET(p_reg->DLM, SERIAL_DRV_LPCXXXX_BIT_IER_RBR | SERIAL_DRV_LPCXXXX_BIT_IER_RX_LINE);

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_RxStop()
*
* Description : Stop receiving data and data received interrupts.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device reception stopped.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Close(),
*               Serial_IF_Rx(),
*               Serial_IF_RxStop() via 'pdev->Drv_API->RxStop()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_RxStop (SERIAL_DEV  *pdev,
                                SERIAL_ERR  *perr)
{
    SERIAL_DRV_REG  *p_reg;
    SERIAL_DEV_CFG  *p_cfg;
    
    
    p_cfg = pdev->Dev_Cfg;           
    p_reg = (SERIAL_DRV_REG *)p_cfg->BaseAddr;
        
                                                                /* Disable the Receive Data available & RX line Status.  */
    DEF_BIT_CLR(p_reg->DLM, SERIAL_DRV_LPCXXXX_BIT_IER_RBR | SERIAL_DRV_LPCXXXX_BIT_IER_RX_LINE);

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_RxOctet()
*
* Description : Receive data octet.
*
* Argument(s) : pdev        Pointer to device.
*
*               pdatum      Pointer to variable that will receive the received data octet.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device octet     transmitted,
*                               SERIAL_ERR_DRV_IO_ERR           Device octet NOT transmitted.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_IF_Rx() via 'pdrv_api->RxOctet()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_RxOctet (SERIAL_DEV  *pdev,
                                 CPU_INT08U  *pdatum,
                                 SERIAL_ERR  *perr)
{
    CPU_INT32U       reg_val;
    SERIAL_DRV_REG  *p_reg;
    SERIAL_DEV_CFG  *p_cfg;
    
    
    p_cfg   = pdev->Dev_Cfg;           
    p_reg   = (SERIAL_DRV_REG *)p_cfg->BaseAddr;
    reg_val = p_reg->LSR;
    
                                                                /* If the receive data doesn't contain data ...         */
    if (DEF_BIT_IS_CLR(reg_val, SERIAL_DRV_LPCXXXX_BIT_LSR_RDR)) {
        *perr = SERIAL_ERR_DRV_IO_ERR;
        return;    
    }
                                                                /* Check for Rx errors.                                 */
    if (DEF_BIT_IS_SET_ANY(reg_val, SERIAL_DRV_LPCXXXX_BIT_LSR_RX_ERR)) {
        *perr = SERIAL_ERR_DRV_IO_ERR;
        return;    
    }
    
    *pdatum = (CPU_INT08U)(p_reg->RBR & DEF_INT_08_MASK);
    *perr  = SERIAL_ERR_NONE;    
}



/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_TxStart()
*                                     SerialDrv_LPC3xxx_TxStart()
*
* Description : Start transmitting data & getting data transmitted interrupts.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device transmission started.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_IF_Tx(),
*               Serial_WrStart() via 'pdev->Drv_API->TxStart()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_TxStart (SERIAL_DEV  *pdev,
                                 SERIAL_ERR  *perr)
{
    SERIAL_DRV_REG  *p_reg;
    SERIAL_DEV_CFG  *p_cfg;
    
    
    p_cfg   = pdev->Dev_Cfg;           
    p_reg   = (SERIAL_DRV_REG *)p_cfg->BaseAddr;
     
                                                                /* Enable the transmit holding register empty interrupt */
    DEF_BIT_SET(p_reg->DLM, SERIAL_DRV_LPCXXXX_BIT_IER_THRE);
    DEF_BIT_SET(p_reg->TER, SERIAL_DRV_LPCXXXX_BIT_TER_TXEN);
   *perr = SERIAL_ERR_NONE;
}


static  void  SerialDrv_LPC3xxx_TxStart (SERIAL_DEV  *pdev,
                                         SERIAL_ERR  *perr)
{
    SERIAL_DRV_REG  *p_reg;
    SERIAL_DEV_CFG  *p_cfg;
    
    
    p_cfg   = pdev->Dev_Cfg;           
    p_reg   = (SERIAL_DRV_REG *)p_cfg->BaseAddr;
     
                                                                /* Enable the transmit holding register empty interrupt */
    DEF_BIT_SET(p_reg->DLM, SERIAL_DRV_LPCXXXX_BIT_IER_THRE);
   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_TxStop()
*                                     SerialDrv_LPC3xxx_TxStop()
*
* Description : Stop transmitting data & getting data transmitted interrupts.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device transmission stopped.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Close(),
*               Serial_IF_Tx(),
*               Serial_WrStartNext() via 'pdev->Drv_API->TxStop()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_TxStop (SERIAL_DEV  *pdev,
                                SERIAL_ERR  *perr)
{
    SERIAL_DRV_REG  *p_reg;
    SERIAL_DEV_CFG  *p_cfg;
    
    
    p_cfg   = pdev->Dev_Cfg;           
    p_reg   = (SERIAL_DRV_REG *)p_cfg->BaseAddr;
     
                                                                /* Disable transmit holding register empty interrupt.   */
    DEF_BIT_CLR(p_reg->DLM, SERIAL_DRV_LPCXXXX_BIT_IER_THRE);
    DEF_BIT_CLR(p_reg->TER, SERIAL_DRV_LPCXXXX_BIT_TER_TXEN);

   *perr = SERIAL_ERR_NONE;
}


static  void  SerialDrv_LPC3xxx_TxStop (SERIAL_DEV  *pdev,
                                        SERIAL_ERR  *perr)
{
    SERIAL_DRV_REG  *p_reg;
    SERIAL_DEV_CFG  *p_cfg;
    
    
    p_cfg   = pdev->Dev_Cfg;           
    p_reg   = (SERIAL_DRV_REG *)p_cfg->BaseAddr;
     
                                                                /* Disable transmit holding register empty interrupt.   */
    DEF_BIT_CLR(p_reg->DLM, SERIAL_DRV_LPCXXXX_BIT_IER_THRE);

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_TxOctet()
*
* Description : Transmit data octet.
*
* Argument(s) : pdev        Pointer to device.
*
*               datum       Data octet to transmit.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device octet     transmitted,
*                               SERIAL_ERR_DRV_IO_ERR           Device octet NOT transmitted.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_IF_Tx()   via      'pdrv_api->TxOctet()',
*               Serial_WrStart() via 'pdev->Drv_API->TxOctet()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_TxOctet (SERIAL_DEV  *pdev,
                                 CPU_INT08U   datum,
                                 SERIAL_ERR  *perr)
{
    CPU_INT08U       reg_val;
    SERIAL_DRV_REG  *p_reg;
    SERIAL_DEV_CFG  *p_cfg;    
    
    p_cfg   = pdev->Dev_Cfg;           
    p_reg   = (SERIAL_DRV_REG *)p_cfg->BaseAddr;
    reg_val = p_reg->LSR;
    
    if (DEF_BIT_IS_CLR(reg_val, SERIAL_DRV_LPCXXXX_BIT_LSR_THRE)) {
        *perr = SERIAL_ERR_DRV_IO_ERR;
        return;    
    }
    
    p_reg->RBR = datum;

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                       SerialDrv_ISR_Handler()
*
* Description : Handle serial interface's device interrupt service routine (ISR) function(s).
*
* Argument(s) : pdev        Pointer to device.
*
*               type        Device  interrupt type(s) to handle :
*
*                               SERIAL_ISR_TYPE_UNKNOWN         Handle unknown device           ISR(s).
*                               SERIAL_ISR_TYPE_RX              Handle device receive           ISR(s).
*                               SERIAL_ISR_TYPE_RX_OVERRUN      Handle device receive  overrun  ISR(s).
*                               SERIAL_ISR_TYPE_TX_RDY          Handle device transmit ready    ISR(s).
*                               SERIAL_ISR_TYPE_TX_COMPLETE     Handle device transmit complete ISR(s).
*                               SERIAL_ISR_TYPE_ERR             Handle device error             ISR(s).
*
* Return(s)   : None.
*
* Caller(s)   : Device driver(s)' Board Support Package (BSP) Interrupt Service Routine (ISR) handler(s).
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  void  SerialDrv_ISR_Handler (SERIAL_DEV  *pdev,
                                     CPU_INT08U   type)
{
    CPU_INT08U       datum;
    CPU_INT32U       lsr;
    CPU_INT32U       iir;
    CPU_INT08U       iir_int_id;
    SERIAL_DRV_REG  *p_reg;
    SERIAL_DEV_CFG  *p_cfg;
    

    (void)&type;
    p_cfg = pdev->Dev_Cfg;           
    p_reg = (SERIAL_DRV_REG *)p_cfg->BaseAddr;
    iir   = p_reg->IIR;                                         /* Read te interrupt indentification register           */
                                                                /* If at least one interrupt is pending ...             */
    while (DEF_BIT_IS_CLR(iir, SERIAL_DRV_LPCXXXX_BIT_IIR_INT_STAT)) {
        iir_int_id = (CPU_INT08U)((iir & SERIAL_DRV_LPCXXXX_MSK_IIR_INT_ID) >> 1u);
        switch (iir_int_id) {
            case SERIAL_DRV_LPCXXXX_BIT_IIR_INT_ID_RLS:         /*Rx Line Status                                        */
                 lsr = p_reg->LSR;                 
                 break;
           
            case SERIAL_DRV_LPCXXXX_BIT_IIR_INT_ID_CTI:         /* Character time-out                                   */
            case SERIAL_DRV_LPCXXXX_BIT_IIR_INT_ID_RDA:         /* Rx data available                                    */
                 lsr = p_reg->LSR;                             
                 if (DEF_BIT_IS_CLR(lsr, SERIAL_DRV_LPCXXXX_BIT_LSR_RX_ERR)) {
                     datum = (CPU_INT08U)(p_reg->RBR & DEF_INT_08_MASK);    
                     SerialIF_Rx(pdev, datum);                 
                 }            
                 break;
                 
            case SERIAL_DRV_LPCXXXX_BIT_IIR_INT_ID_THRE:        /* Transmit Holding register                            */
                 SerialIF_Tx(pdev);                             /* Inform serial core of wr completion.                 */
                 break;
                 
            default:
                break;
        }
        iir = p_reg->IIR;
    }
}
