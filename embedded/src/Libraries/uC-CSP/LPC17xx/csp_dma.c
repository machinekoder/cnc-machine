/*
*********************************************************************************************************
*                                              uC/CSP
*                                        Chip Support Package
*
*                            (c) Copyright 2010; Micrium, Inc.; Weston, FL
*                         (c) Copyright 2003-2010; Micrium, Inc.; Weston, FL
*
*               All rights reserved. Protected by international copyright laws.
*
*               uC/CSP is provided in source form to registered licensees ONLY.  It is
*               illegal to distribute this source code to any third party unless you receive
*               written permission by an authorized Micrium representative.  Knowledge of
*               the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                             DMA CONTROLLER
*                                               NXP LPC17xx 
*
* Filename      : csp_dma.c
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
* Note(s)       : 
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define  CSP_DMA_MODULE
#include  <csp.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  CSP_DMA_CH_MAX_NBR                               8u

#define  CSP_DMA_CH_STATE_FREE                            0u
#define  CSP_DMA_CH_STATE_ALLOC                           1u

/*
*********************************************************************************************************
*                                    REGISTER & BIT-FIELD DEFINITION
*********************************************************************************************************
*/

#define  CSP_ADDR_DMA_REG                          (CPU_ADDR   )(0x50004000)
#define  CSP_DMA_REG_REQ_SEL                   (( *(CPU_REG32 *)0x400FC1C4))


                                                                /* ------ DMA CONFIGRATUIN REGISTER BIT DEFINES ------- */
#define  CSP_DMA_REG_CFG_DMA_EN                 DEF_BIT_00      /* DMA enable.                                          */


                                                                /* ----- DMA CHANNEL CONTROL REGISTER BIT DEFIENS ----- */
#define  CSP_DMA_BIT_CH_CTRL_SI                 DEF_BIT_26     
#define  CSP_DMA_BIT_CH_CTRL_DI                 DEF_BIT_27
#define  CSP_DMA_BIT_CH_CTRL_I                  DEF_BIT_31
#define  CSP_DMA_MSK_CH_CTRL_XFER_SIZE          DEF_BIT_FIELD(12u, 0u)

                                                                /* -- DMA CHANNEL CONFIGURATION REGISTER BIT DEFIENS -- */
#define  CSP_DMA_BIT_CH_CFG_CH_EN               DEF_BIT_00
#define  CSP_DMA_BIT_CH_CFG_XFER_MEM_TO_MEM     DEF_BIT_MASK(0u, 11u) 
#define  CSP_DMA_BIT_CH_CFG_XFER_MEM_TO_PER     DEF_BIT_MASK(1u, 11u)
#define  CSP_DMA_BIT_CH_CFG_XFER_PER_TO_MEM     DEF_BIT_MASK(2u, 11u)
#define  CSP_DMA_BIT_CH_CFG_XFER_PER_TO_PER     DEF_BIT_MASK(3u, 11u)
#define  CSP_DMA_MSK_CH_CFG_PER_REQ             DEF_BIT_FIELD(4u, 0u)
#define  CSP_DMA_BIT_CH_CFG_IE                  DEF_BIT_14
#define  CSP_DMA_BIT_CH_CFG_ITC                 DEF_BIT_15


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                     DMA CONTROLLER  REGISTER
*********************************************************************************************************
*/


typedef  struct csp_dma_ch {
    CPU_INT08U            State;
    CSP_DMA_CALLBACK_PTR  CallBackFnctPtr;
    void                 *CallBackArgPtr;
#if 0
    void                 *Signal;                               /* #### It will added to support synchronous transfers. */
#endif
} CSP_DMA_CH;


/*
*********************************************************************************************************
*                                     DMA CHANNEL STATUS DATA TYPE
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                     DMA CONTROLLER  REGISTER
*********************************************************************************************************
*/

typedef  struct  csp_dma_ch_reg {                               /* ---------------- DMA CHANNEL REGISTERS ------------- */
    CPU_REG32  SrcAddr;                                         /* Source address.                                      */
    CPU_REG32  DestAddr;                                        /* Destination address.                                 */
    CPU_REG32  LLI;                                             /* Link list item register.                             */
    CPU_REG32  Ctrl;                                            /* Control register.                                    */
    CPU_REG32  Cfg;                                             /* Configuration register.                              */                                                           
    CPU_REG32  RSVD0[3];                                        /* Reserved.                                            */
} CSP_DMA_CH_REG;

 
typedef  struct csp_dma_reg {                                   /* --------------- DMA CONTROLLER REGISTER ------------ */
    CPU_REG32       IntStat;                                    /* Interrupt status register.                           */
    CPU_REG32       IntTCStat;                                  /* Interrupt terminal count request status register.    */                                                         
    CPU_REG32       IntTCClr;                                   /* Interrupt terminal count request clear  register.    */
    CPU_REG32       IntErrStat;                                 /* Interrupt error status register.                     */
    CPU_REG32       IntErrClr;                                  /* Interrupt error clear register                       */                                                           
    CPU_REG32       IntRawIntTC_Stat;                           /* RAW interrupt terminal count status register.        */                                                           
    CPU_REG32       IntRawIntErrStat;                           /* RAW interrupt status register.                       */
    CPU_REG32       EnCh;                                       /* Enabled channel register.                            */
    CPU_REG32       SoftBurstReq;                               /* Software burst request register.                     */
    CPU_REG32       SoftSingleReq;                              /* Software single request register.                    */
    CPU_REG32       SoftLastBurstReq;                           /* Software last burst request register.                */
    CPU_REG32       SoftLastSingleReq;                          /* Software last single request register.               */
    CPU_REG32       Cfg;                                        /* Configuration register.                              */
    CPU_REG32       Sync;                                       /* Synchronization register.                            */
    CPU_REG32       RSVD0[50];
    CSP_DMA_CH_REG  CHx[CSP_DMA_CH_MAX_NBR];                    /* DMA channel registers.                               */
} CSP_DMA_REG;


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

static  CSP_DMA_CH  CSP_DMA_ChTbl[CSP_DMA_CH_MAX_NBR];


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL MACRO's
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  CSP_DMA_IntHandler (void *p_arg);


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                        CSP_DMA_Init()
*
* Description : Initialize DMA controller.
* 
*                   (1) Disable DMA controller interrupt. 
*                   (2) Enable  DMA controller clock
*                   (3) Clear all pending interrupts.
*                   (4) Initialize all DMA channels to a default state.
*                   (5) Install DMA controller interrupt handler.
*                   (6) Enable  DMA controller interrupt.
*                   
* Argument(s) : none.
*
* Return(s)   : DEF_OK,    If the DMA engine was initialize correctly, 
*               DEF_FAIL,  Otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_DMA_InitExt (void)                          
{   
    CSP_DMA_REG     *p_dma_reg;
    CSP_DMA_CH_REG  *p_dma_ch_reg;
    CSP_DMA_CH      *p_ch_tbl;     
    CPU_INT08U       ch_nbr;
    
    
    CSP_IntDis(CSP_INT_CTRL_NBR_MAIN,                           /* Disable DMA interrupts                               */
               CSP_INT_SRC_NBR_DMA_00);
    
    CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_DMA_00);                 /* Enable DMA controller clock.                         */
    
    p_dma_reg            = (CSP_DMA_REG *)CSP_ADDR_DMA_REG;
    p_dma_reg->IntTCClr  = DEF_INT_08_MASK;                     /* Clear all pending interrupts.                        */
    p_dma_reg->IntErrClr = DEF_INT_08_MASK;
                                                                /* Initialize DMA channels register & table             */
    for (ch_nbr = 0u; ch_nbr < CSP_DMA_CH_MAX_NBR; ch_nbr++) {
        p_dma_ch_reg              = &(p_dma_reg->CHx[ch_nbr]);
        p_ch_tbl                  = &CSP_DMA_ChTbl[ch_nbr];
        p_ch_tbl->State           = CSP_DMA_CH_STATE_FREE;
        p_ch_tbl->CallBackFnctPtr = (CSP_DMA_CALLBACK_PTR  )0;
        p_ch_tbl->CallBackArgPtr  = (void                 *)0;
        p_dma_ch_reg->SrcAddr     = 0u;
        p_dma_ch_reg->DestAddr    = 0u;
        p_dma_ch_reg->LLI         = 0u;
        p_dma_ch_reg->Ctrl        = DEF_BIT_NONE;
        p_dma_ch_reg->Cfg         = DEF_BIT_NONE;
    }

    p_dma_reg->Cfg = CSP_DMA_REG_CFG_DMA_EN;                    /* Enable DMA controller.                               */

 
    (void)CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN, /* Install global DMA interrupt handler.                */
                         (CSP_DEV_NBR   )CSP_INT_SRC_NBR_DMA_00,
                         (CPU_FNCT_PTR  )CSP_DMA_IntHandler,
                         (void         *)0);
    
    CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, 
              CSP_INT_SRC_NBR_DMA_00);
                            
    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                        CSP_DMA_CH_AllocExt()
*
* Description : Allocate a channel for a new transaction.
*                   
* Argument(s) : prio       Channel priority.
*
*                              CSP_DMA_CH_PRIO_ANY_LOW                  Any channel available. (from low  to high).
*                              CSP_DMA_CH_PRIO_ANY_HIGH                 Any channel available. (from high to low).
*                                         0                             Channel at priority 0. Highest
*                                         1                             Channel at priority 1.
*                                         .
*                                         .
*                                         .
*                                         7                             Channel at priority 7.  Lowest.  
*
*               p_ch_nbr   Pointer to the variable to return the allocated channel number.
*
* Return(s)   : DEF_OK,    If the a channel was allocated, 
*               DEF_FAIL,  Otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_DMA_CH_AllocExt (CSP_OPT      prio,
                                  CSP_DEV_NBR *p_ch_nbr)
{
    CSP_DMA_CH  *p_ch_tbl;        
    CPU_INT08U   ch_ix;
    CPU_INT08U   ch_nbr;
    CPU_INT08U   ch_alloc;
    CPU_BOOLEAN  found;
    
    CPU_SR_ALLOC();

#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)         
    if ((prio  > CSP_DMA_CH_PRIO_LOWEST   ) &&
        (prio != CSP_DMA_CH_PRIO_ANY_LOW  ) &&
        (prio != CSP_DMA_CH_PRIO_ANY_HIGH)) {
        return (DEF_FAIL);    
    }

    if (p_ch_nbr == (CSP_DEV_NBR *)0) {
        return (DEF_FAIL);
    }
#endif

    found    = DEF_NO;
    ch_alloc = 0u;

    if ((prio == CSP_DMA_CH_PRIO_ANY_LOW  ) ||
        (prio == CSP_DMA_CH_PRIO_ANY_HIGH)) {        
        ch_nbr = CSP_DMA_CH_MAX_NBR;
        
        if (prio == CSP_DMA_CH_PRIO_ANY_LOW) {
            ch_ix = CSP_DMA_CH_MAX_NBR - 1u;
        } else {
            ch_ix = 0u;
        }
                
        while ((found  == DEF_NO) &&                            /* Search for the first channel available.              */
               (ch_nbr != 0     )) {
            p_ch_tbl = &CSP_DMA_ChTbl[ch_ix];    
                                                                /* If the channel is available ...                      */
            if (p_ch_tbl->State == CSP_DMA_CH_STATE_FREE) {                
                
                CPU_CRITICAL_ENTER();                           /* ... Mark the channel as 'ALLOC' (allocated) in ...   */                                                                
                p_ch_tbl->State = CSP_DMA_CH_STATE_ALLOC;       /* ... in the channel table.                            */
                CPU_CRITICAL_EXIT();                
                ch_alloc = ch_ix;
                found    = DEF_TRUE;
            }
            ch_nbr--;
            if (prio == CSP_DMA_CH_PRIO_ANY_LOW) {              /* Search from low to hight.                            */
                ch_ix--;
            } else {
                ch_ix++;                                        /* Search from high to low.                             */
            }
        }
    } else {
        p_ch_tbl = &CSP_DMA_ChTbl[prio];
        if (p_ch_tbl->State == CSP_DMA_CH_STATE_FREE) {
            
            CPU_CRITICAL_ENTER();
            p_ch_tbl->State = CSP_DMA_CH_STATE_ALLOC;            
            CPU_CRITICAL_EXIT();
            
            ch_alloc = prio;
            found    = DEF_YES;
        }    
    }

    if (found == DEF_NO) {
        return (DEF_FAIL);
    }

    *p_ch_nbr = (CSP_DEV_NBR)ch_alloc;

    return (DEF_OK);
}

/*
*********************************************************************************************************
*                                        CSP_DMA_CH_Free()
*
* Description : Free a DMA channel.
*                   
* Argument(s) : ch_nbr   Channel number.
*
* Return(s)   : DEF_OK,    If the a channel was freed, 
*               DEF_FAIL,  Otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_DMA_CH_FreeExt (CSP_DEV_NBR  ch_nbr)
                               
{
    CSP_DMA_REG     *p_dma_reg;    
    CSP_DMA_CH_REG  *p_dma_ch_reg;
    CSP_DMA_CH      *p_ch_tbl;
    
    CPU_SR_ALLOC();

#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)         
    if (ch_nbr > CSP_DMA_CH_MAX_NBR - 1u) {
        return (DEF_FAIL);
    }
#endif    

    p_dma_reg    = (CSP_DMA_REG *)CSP_ADDR_DMA_REG;
    p_dma_ch_reg = &(p_dma_reg->CHx[ch_nbr]);
    p_ch_tbl     = &CSP_DMA_ChTbl[ch_nbr];
        
    CPU_CRITICAL_ENTER();
    p_dma_reg->IntTCClr    = DEF_BIT(ch_nbr);                   /* Clear all pending interrupts.                        */
    p_dma_reg->IntErrClr   = DEF_BIT(ch_nbr);    
    p_ch_tbl->State        = CSP_DMA_CH_STATE_FREE;             /* Free the channel.                                    */
    p_dma_ch_reg->SrcAddr  = DEF_BIT_NONE;                      /* Unitialize DMA channel cfg & ctrl registers.         */
    p_dma_ch_reg->DestAddr = DEF_BIT_NONE;
    p_dma_ch_reg->Cfg      = DEF_BIT_NONE;
    p_dma_ch_reg->Ctrl     = DEF_BIT_NONE;       
    CPU_CRITICAL_EXIT();

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                        CSP_DMA_CH_CfgExt()
*
* Description : Configure a channel for a new DMA transmission.
*                   
* Argument(s) : ch_nbr           Channel number.
*
*               xfer_type        DMA transfer type.
*
*                                    CSP_DMA_XFER_TYPE_MEM_TO_MEM    Memory     to memory
*                                    CSP_DMA_XFER_TYPE_PER_TO_MEM    Peripheral to memory.
*                                    CSP_DMA_XFER_TYPE_MEM_TO_PER    Memory     to Peripheral.
*                                    CSP_DMA_XFER_TYPE_PER_TO_PER    Peripheral to Peripheral.
*
*              src_burst_size    Source burst size minus 1 (see note #1, #1a).
*                                              
*              src_width         Source transfer width. 
*                                    CPU_WORD_SIZE_08   byte ( 8-bit)
*                                    CPU_WORD_SIZE_16   byte (16-bit)
*                                    CPU_WORD_SIZE_32   byte (32-bit)
*
*              dest_burst_size   Source burst size minus 1. (see note #1, #1a).
*
*
*              dest_width        Destination transfer width.
*                                    CPU_WORD_SIZE_08   byte ( 8-bit)
*                                    CPU_WORD_SIZE_16   byte (16-bit)
*                                    CPU_WORD_SIZE_32   byte (32-bit)
*
*              src_req           Source request peripheral. 
*                                    CSP_DMA_XFER_PER_REQ_SSP_00_TX 
*                                    CSP_DMA_XFER_PER_REQ_SSP_00_RX
*                                    CSP_DMA_XFER_PER_REQ_SSP_01_TX
*                                    CSP_DMA_XFER_PER_REQ_SSP_01_RX
*                                    CSP_DMA_XFER_PER_REQ_ADC
*                                    CSP_DMA_XFER_PER_REQ_I2S_00
*                                    CSP_DMA_XFER_PER_REQ_I2S_01
*                                    CSP_DMA_XFER_PER_REQ_DAC
*                                    CSP_DMA_XFER_PER_REQ_UART_00_TX
*                                    CSP_DMA_XFER_PER_REQ_UART_00_RX
*                                    CSP_DMA_XFER_PER_REQ_UART_01_TX
*                                    CSP_DMA_XFER_PER_REQ_UART_01_RX
*                                    CSP_DMA_XFER_PER_REQ_UART_02_TX
*                                    CSP_DMA_XFER_PER_REQ_UART_02_RX
*                                    CSP_DMA_XFER_PER_REQ_UART_03_TX
*                                    CSP_DMA_XFER_PER_REQ_UART_03_RX
*                                    CSP_DMA_XFER_PER_REQ_TMR_00_MATCH_0
*                                    CSP_DMA_XFER_PER_REQ_TMR_00_MATCH_1
*                                    CSP_DMA_XFER_PER_REQ_TMR_01_MATCH_0
*                                    CSP_DMA_XFER_PER_REQ_TMR_01_MATCH_1
*                                    CSP_DMA_XFER_PER_REQ_TMR_02_MATCH_0
*                                    CSP_DMA_XFER_PER_REQ_TMR_02_MATCH_1
*                                    CSP_DMA_XFER_PER_REQ_TMR_03_MATCH_0
*                                    CSP_DMA_XFER_PER_REQ_TMR_03_MATCH_1
*
*               dest_req          Destination request peripheral.  (same as 'src_req' argument)
*
*
* Return(s)   : DEF_OK,    If the a channel was configured, 
*               DEF_FAIL,  Otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) The LPC17xx DMA controller only allows the following burst size values:
*                   1, 4, 8, 16, 32, 64, 128, 256.
*
*                   (a) If a burst size values is different from the allowed values, the next power of 2 would 
*                       be selected.
*
*               (2) Only the following values are accepted for source/destination transfer width.
*                   CPU_WORD_SIZE_08, CPU_WORD_SIZE_16, CPU_WORD_SIZE_32.
*
*               (3) The value of the DMA source request peripheral is ignored if the source of the 
*                   transfer is from memory.
*
*               (4) The value of the DMA destination request peripheral is ignored if the destination of the 
*                   transfer is to memory.
*
*               (5) The callback function should be defined as:
*
*                   void   MyCallback (CSP_DEV_NBR  ch_nbr, 
*                                      CPU_INT16U   xfer_size_rem,
*                                      void         *p_arg,
*                                      CPU_BOOLEAN   status)
*
*                   where,
*
*                     ch_nbr           DMA channel number.
*                     xfer_size_rem    Remaining transfer size.
*                     p_arg            Pointer to the callback argument.
*                     status           DEF_OK    if the DMA completes the transfer with no errors.
*                                      DEF_FAIL, otherwise.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_DMA_CH_CfgExt (CSP_DEV_NBR   ch_nbr,
                                CSP_OPT       xfer_type,
                                CSP_OPT       src_burst_size,
                                CSP_OPT       src_width,
                                CSP_OPT       dest_burst_size,
                                CSP_OPT       dest_width,
                                CSP_DEV_NBR   src_req,
                                CSP_DEV_NBR   dest_req)
                                

{   CSP_DMA_REG     *p_dma_reg;
    CSP_DMA_CH_REG  *p_dma_ch_reg;
    CPU_INT32U       reg_cfg;
    CPU_INT32U       reg_ctrl;
    CPU_INT32U       reg_val;
    CPU_SR_ALLOC();
    
                                                                /* ------------------ ARGUMENTS CHECKING -------------- */
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (ch_nbr > CSP_DMA_CH_MAX_NBR - 1u) {                     /* Invalid channel number?                              */
        return (DEF_FAIL);
    }
                                                                /* Channel not available?                               */
    if (CSP_DMA_ChTbl[ch_nbr].State != CSP_DMA_CH_STATE_ALLOC) {
        return (DEF_FAIL);
    }
    
    if ((src_width != CPU_WORD_SIZE_08) &&                      /* Check src width parameter.                           */
        (src_width != CPU_WORD_SIZE_16) &&
        (src_width != CPU_WORD_SIZE_32)) {
        return (DEF_FAIL);
    }

    if ((dest_width != CPU_WORD_SIZE_08) &&                     /* Check src width parameter.                           */
        (dest_width != CPU_WORD_SIZE_16) &&
        (dest_width != CPU_WORD_SIZE_32)) {
        return (DEF_FAIL);
    }
#endif
                                                                /* Channel configuration.                               */
    reg_cfg  = DEF_BIT_NONE; 
    reg_ctrl = DEF_BIT_NONE;

    if (src_burst_size == 255u) {                               /* Set the source/destination burst size.               */
       reg_val   = 7u;
    } else if (src_burst_size >= 4u) {
        reg_val  = (DEF_INT_32_NBR_BITS - CPU_CntLeadZeros(src_burst_size) - 2u);        
    } else {
        reg_val  = DEF_BIT_NONE; 
    }
            
    DEF_BIT_SET(reg_ctrl, reg_val << 12u);

    if (dest_burst_size == 255u) {
       reg_val  = 7u;
    } else if (dest_burst_size >= 4u) {
        reg_val = (DEF_INT_32_NBR_BITS - CPU_CntLeadZeros(dest_burst_size) - 2u);        
    } else {
        reg_val = DEF_BIT_NONE;
    }

    DEF_BIT_SET(reg_ctrl, reg_val           << 15u);
    DEF_BIT_SET(reg_ctrl, (src_width  - 1u) << 18u);
    DEF_BIT_SET(reg_ctrl, (dest_width - 1u) << 21u);
    


                                                                /* Set the transfer type.                               */
    switch (xfer_type) {
        case CSP_DMA_XFER_TYPE_MEM_TO_PER:
             DEF_BIT_SET(reg_cfg, CSP_DMA_BIT_CH_CFG_XFER_MEM_TO_PER);              
             break;

        case CSP_DMA_XFER_TYPE_MEM_TO_MEM:
             DEF_BIT_SET(reg_cfg, CSP_DMA_BIT_CH_CFG_XFER_MEM_TO_MEM);              
             break;

        case CSP_DMA_XFER_TYPE_PER_TO_MEM:
             DEF_BIT_SET(reg_cfg, CSP_DMA_BIT_CH_CFG_XFER_PER_TO_MEM);              
             break;

        case CSP_DMA_XFER_TYPE_PER_TO_PER:
             DEF_BIT_SET(reg_cfg, CSP_DMA_BIT_CH_CFG_XFER_PER_TO_PER);
             break;
   
        default:
             return (DEF_FAIL);
    }
                                                                /* Configure source request signal  (see note #3)       */
                                                                /* ... transfer from peripheral?                        */
    if (((xfer_type == CSP_DMA_XFER_TYPE_PER_TO_MEM       )  || 
         (xfer_type == CSP_DMA_XFER_TYPE_PER_TO_PER       ))) {    
        
        if (src_req < CSP_DMA_XFER_PER_REQ_UART_00_TX) {
            ;    
        } else if (src_req < CSP_DMA_XFER_PER_REQ_TMR_00_MATCH_0) {
            CPU_CRITICAL_ENTER();
            DEF_BIT_CLR(CSP_DMA_REG_REQ_SEL, DEF_BIT(src_req - CSP_DMA_XFER_PER_REQ_UART_00_TX));
            CPU_CRITICAL_EXIT();
        } else {
            src_req -= CSP_DMA_XFER_PER_REQ_UART_00_TX;
   
            CPU_CRITICAL_ENTER();
            DEF_BIT_SET(CSP_DMA_REG_REQ_SEL, DEF_BIT(src_req - CSP_DMA_XFER_PER_REQ_UART_00_TX)); 
            CPU_CRITICAL_EXIT();
        }
        DEF_BIT_SET(reg_cfg, (src_req & CSP_DMA_MSK_CH_CFG_PER_REQ) << 1u);                
    }
    
    
    if (((xfer_type  == CSP_DMA_XFER_TYPE_MEM_TO_PER       )  || 
         (xfer_type  == CSP_DMA_XFER_TYPE_PER_TO_PER       ))) {

        if (dest_req < CSP_DMA_XFER_PER_REQ_UART_00_TX) {
            ;    
        } else if (dest_req < CSP_DMA_XFER_PER_REQ_TMR_00_MATCH_0) {
            CPU_CRITICAL_ENTER();
            DEF_BIT_CLR(CSP_DMA_REG_REQ_SEL, DEF_BIT(dest_req - CSP_DMA_XFER_PER_REQ_UART_00_TX));
            CPU_CRITICAL_EXIT();
        } else {
            dest_req -= CSP_DMA_XFER_PER_REQ_UART_00_TX;

            CPU_CRITICAL_ENTER();            
            DEF_BIT_SET(CSP_DMA_REG_REQ_SEL, DEF_BIT(dest_req - CSP_DMA_XFER_PER_REQ_UART_00_TX)); 
            CPU_CRITICAL_EXIT();
        }
             
        DEF_BIT_SET(reg_cfg, (dest_req & CSP_DMA_MSK_CH_CFG_PER_REQ) << 6u);        
    }

    p_dma_reg    = (CSP_DMA_REG *)CSP_ADDR_DMA_REG;                  
    p_dma_ch_reg = &(p_dma_reg->CHx[ch_nbr]);
    
    CPU_CRITICAL_ENTER();
    p_dma_ch_reg->Cfg      = (CPU_INT32U)reg_cfg;   
    p_dma_ch_reg->Ctrl     = (CPU_INT32U)reg_ctrl;       
    CPU_CRITICAL_EXIT();

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                        CSP_DMA_XferStartExt()
*
* Description : Initialize unattended DMA transfer.
*                   
* Argument(s) : ch_nbr       Channel number.
*
*               p_dest        Pointer to destination memory buffer.
*
*               p_src         Pointer to source      memory buffer.
*
*               xfer_size     Transfer size.
*
*               opt           Transfer pptions flags.
*
*                                 CSP_DMA_OPT_FLAG_XFER_SRC_INC   Source address is incremented.
*                                 CSP_DMA_OPT_FLAG_XFER_DEST_INC  Destination address is incremented.
*
* Return(s)   : DEF_OK,    If the a trasnfer was initialized, 
*               DEF_FAIL,  Otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_DMA_XferStartExt (CSP_DEV_NBR     ch_nbr,
                                   void           *p_dest,
                                   void           *p_src,                                   
                                   CPU_SIZE_T      xfer_size,
                                   CSP_OPT_FLAGS   opt)

{   
    CSP_DMA_REG     *p_dma_reg;
    CSP_DMA_CH_REG  *p_dma_ch_reg;
    CPU_INT32U       reg_ctrl;    
    CPU_SR_ALLOC();

                                                                /* ------------------ ARGUMENTS CHECKING -------------- */
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)             
    if (ch_nbr > CSP_DMA_CH_MAX_NBR - 1u) {                     /* Invalid channel number?                              */
        return (DEF_FAIL);
    }
                                                                /* Channel not available?                               */
    if (CSP_DMA_ChTbl[ch_nbr].State != CSP_DMA_CH_STATE_ALLOC) {
        return (DEF_FAIL);
    }

    if ((p_dest == (void *)0) ||                                /* Null pointers?                                        */
        (p_src  == (void *)0)) {
        return (DEF_FAIL);
    }    
#endif    

    p_dma_reg    = (CSP_DMA_REG *)CSP_ADDR_DMA_REG;                  
    p_dma_ch_reg = &(p_dma_reg->CHx[ch_nbr]);
    reg_ctrl     = p_dma_ch_reg->Ctrl;

    DEF_BIT_CLR(reg_ctrl, CSP_DMA_MSK_CH_CTRL_XFER_SIZE |
                          CSP_DMA_BIT_CH_CTRL_SI        |
                          CSP_DMA_BIT_CH_CTRL_DI        |
                          CSP_DMA_BIT_CH_CTRL_I);

    DEF_BIT_SET(reg_ctrl, (xfer_size & CSP_DMA_MSK_CH_CTRL_XFER_SIZE));

    if (DEF_BIT_IS_SET(opt, CSP_DMA_OPT_FLAG_XFER_SRC_INC)) {
        DEF_BIT_SET(reg_ctrl, CSP_DMA_BIT_CH_CTRL_SI);
    }
    
    if (DEF_BIT_IS_SET(opt, CSP_DMA_OPT_FLAG_XFER_DEST_INC)) {
        DEF_BIT_SET(reg_ctrl, CSP_DMA_BIT_CH_CTRL_DI);        
    }        
    
    
    CPU_CRITICAL_ENTER();
    p_dma_ch_reg->SrcAddr     = (CPU_INT32U )p_src;
    p_dma_ch_reg->DestAddr    = (CPU_INT32U )p_dest;       
    p_dma_ch_reg->Ctrl        = reg_ctrl;
    DEF_BIT_CLR(p_dma_ch_reg->Cfg, CSP_DMA_BIT_CH_CFG_ITC   | 
                                   CSP_DMA_BIT_CH_CFG_IE);
    DEF_BIT_SET(p_dma_ch_reg->Cfg, CSP_DMA_BIT_CH_CFG_CH_EN); 
    CPU_CRITICAL_EXIT();

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                        CSP_DMA_XferStartExt()
*
* Description : Initialize an asynchronous DMA transfer.
*                   
* Argument(s) : ch_nbr        Channel number.
*  
*               p_dest        Pointer to destination memory buffer.
*
*               p_src         Pointer to source      memory buffer.
*
*               xfer_size     Transfer size.
*
*
*               callback   Pointer to  DMA callback function. (see note #1)
*
*               p_arg      Pointer to DMA callback function argument.
*
*               opt           Transfer pptions flags.
*
*                                 CSP_DMA_OPT_FLAG_XFER_SRC_INC   Source address is incremented.
*                                 CSP_DMA_OPT_FLAG_XFER_DEST_INC  Destination address is incremented.
*
* Return(s)   : DEF_OK,    If the asynchronous DMA transfer was initialized.
*               DEF_FAIL,  Otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) The callback function should be defined as:
*
*               void   MyCallback (CSP_DEV_NBR  ch_nbr, 
*                                  CPU_INT16U   xfer_size_rem,
*                                  void         *p_arg,
*                                  CPU_BOOLEAN   status)
*
*                where,
*
*                     ch_nbr           DMA channel number.
*                     xfer_size_rem    Remianing transfer size.
*                     p_arg            Pointer to the callback argument.
*                     status           DEF_OK    if the DMA stop the trasnfer with not error.
*                                      DEF_FAIL, otherwise.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_DMA_XferAsyncStartExt (CSP_DEV_NBR            ch_nbr, 
                                        void                  *p_dest,                                       
                                        void                  *p_src,
                                        CPU_SIZE_T             xfer_size,
                                        CSP_DMA_CALLBACK_PTR   callback,
                                        void                  *p_arg,
                                        CSP_OPT_FLAGS          opt)
{
    CSP_DMA_REG     *p_dma_reg;
    CSP_DMA_CH_REG  *p_dma_ch_reg; 
    CSP_DMA_CH      *p_dma_ch;
    CPU_INT32U       reg_ctrl;
    CPU_SR_ALLOC();
    
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)             
    if (ch_nbr > CSP_DMA_CH_MAX_NBR - 1u) {                     /* Invalid channel number?                              */
        return (DEF_FAIL);
    }
                                                                /* Channel not available?                               */
    if (CSP_DMA_ChTbl[ch_nbr].State != CSP_DMA_CH_STATE_ALLOC) {
        return (DEF_FAIL);
    }
    
    if (callback == (CSP_DMA_CALLBACK_PTR )0) {
        return (DEF_FAIL);
    }
    
    if ((p_dest  == (void *)0) ||
        (p_src   == (void *)0)) {
        return (DEF_FAIL);
    }
#endif    

    p_dma_reg    = (CSP_DMA_REG *)CSP_ADDR_DMA_REG;                  
    p_dma_ch_reg = &(p_dma_reg->CHx[ch_nbr]);
    p_dma_ch     = &(CSP_DMA_ChTbl[ch_nbr]);    
    reg_ctrl     = p_dma_ch_reg->Ctrl;
    
    DEF_BIT_CLR(reg_ctrl, CSP_DMA_MSK_CH_CTRL_XFER_SIZE |       /* Clears the xfer size mask.                           */
                          CSP_DMA_BIT_CH_CTRL_SI        |       /* Clears the source increment flag.                    */
                          CSP_DMA_BIT_CH_CTRL_DI);              /* Clears the destination increment flag.               */

    DEF_BIT_SET(reg_ctrl, xfer_size & CSP_DMA_MSK_CH_CTRL_XFER_SIZE);
    DEF_BIT_SET(reg_ctrl, CSP_DMA_BIT_CH_CTRL_I);

    if (DEF_BIT_IS_SET(opt, CSP_DMA_OPT_FLAG_XFER_SRC_INC)) {
        DEF_BIT_SET(reg_ctrl, CSP_DMA_BIT_CH_CTRL_SI);
    }
    
    if (DEF_BIT_IS_SET(opt, CSP_DMA_OPT_FLAG_XFER_DEST_INC)) {
        DEF_BIT_SET(reg_ctrl, CSP_DMA_BIT_CH_CTRL_DI);        
    }        
        
    CPU_CRITICAL_ENTER();    
    p_dma_ch_reg->SrcAddr     = (CPU_INT32U )p_src;
    p_dma_ch_reg->DestAddr    = (CPU_INT32U )p_dest;       
    p_dma_ch->CallBackFnctPtr = callback;
    p_dma_ch->CallBackArgPtr  = p_arg;    
    p_dma_ch_reg->Ctrl        = reg_ctrl;

    DEF_BIT_SET(p_dma_ch_reg->Cfg, CSP_DMA_BIT_CH_CFG_ITC   | 
                                   CSP_DMA_BIT_CH_CFG_IE);     
    DEF_BIT_SET(p_dma_ch_reg->Cfg, CSP_DMA_BIT_CH_CFG_CH_EN);
    
    CPU_CRITICAL_EXIT();
        
    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                     CSP_DMA_XferSizeMaxGetExt()
*
* Description : Get maximum DMA transfer size.
*                   
* Argument(s) : ch_nbr        Channel number.
*
* Return(s)   : The maximum DMA transfer size.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  CSP_DMA_XferSizeMaxGetExt (CSP_DEV_NBR  ch_nbr)
{
    (void)ch_nbr;
    
    
    return (DEF_BIT_FIELD(12u, 0u));
}


/*
*********************************************************************************************************
*                                         CSP_DMA_IntHandler()
*
* Description : DMA interrupt global handler.
*              
*                   (1) Check all the channles for pending interrupts
*                   (2) Call call back functions.
*                   
* Argument(s) : p_arg   Interrupt service routine argument.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none
*********************************************************************************************************
*/


static  void  CSP_DMA_IntHandler (void *p_arg)
{    
    CSP_DMA_REG           *p_dma_reg;
    CSP_DMA_CH_REG        *p_dma_ch_reg; 
    CSP_DMA_CH            *p_dma_ch;
    void                  *p_callback_arg;
    CSP_DMA_CALLBACK_PTR   callback_fnct;
    CSP_DEV_NBR            ch_nbr;
    CPU_INT32U             int_tc_stat;
    CPU_INT32U             int_err_stat;
    CPU_BOOLEAN            status;
    CPU_INT16U             xfer_size_rem;

    
    p_dma_reg    = (CSP_DMA_REG *)CSP_ADDR_DMA_REG;    
    int_tc_stat  = p_dma_reg->IntTCStat  & DEF_BIT_FIELD(8u, 0u);
    int_err_stat = p_dma_reg->IntErrStat & DEF_BIT_FIELD(8u, 0u);
    
                                                                /* --------- INTERRUPT TERMINAL COUNT HANDLING -------- */
    while (int_tc_stat != DEF_BIT_NONE) {                       
                                                                /* Get current DMA channel number                       */
        ch_nbr               = 31u - CPU_CntLeadZeros(int_tc_stat);
        p_dma_ch_reg         = &(p_dma_reg->CHx[ch_nbr]);
        p_dma_ch             = &(CSP_DMA_ChTbl[ch_nbr]);

        callback_fnct        = p_dma_ch->CallBackFnctPtr;       /* Get the callback function                            */
        p_callback_arg       = p_dma_ch->CallBackArgPtr;        /* Get the callback argument pointer.                   */        
                                                                /* Get the remaining transfer size.                     */
        xfer_size_rem        = p_dma_ch_reg->Ctrl & DEF_BIT_FIELD(12u, 0u);
        p_dma_reg->IntTCClr  = DEF_BIT(ch_nbr);                 /* Clear the terminal count interrupt.                  */
        
        if (DEF_BIT_IS_SET(int_err_stat, DEF_BIT(ch_nbr))) {    /* If an error occured in the channel...                */
            status                = DEF_FAIL;                   /* ... set the status, and clear the error interrupt.   */
            p_dma_reg->IntErrClr = DEF_BIT(ch_nbr);
        } else {
            status                = DEF_OK;                     
        }
            
        callback_fnct(ch_nbr,                                   /* Call the callback function.                           */
                      xfer_size_rem,
                      p_callback_arg,
                      status);
                                                                /* Read the terminal count and error interrupts status   */
        int_tc_stat  = p_dma_reg->IntTCStat  & DEF_BIT_FIELD(8u, 0u);
        int_err_stat = p_dma_reg->IntErrStat & DEF_BIT_FIELD(8u, 0u);                            
    }
                                                                /* --------------- ERROR INTERRUPT HANDLING ------------ */
    int_err_stat = p_dma_reg->IntErrStat & DEF_BIT_FIELD(8u, 0u);
    
    while (int_err_stat != DEF_BIT_NONE) {
        ch_nbr               = 31u - CPU_CntLeadZeros(int_err_stat);
        p_dma_ch_reg         = &(p_dma_reg->CHx[ch_nbr]);
        p_dma_ch             = &(CSP_DMA_ChTbl[ch_nbr]);
        callback_fnct        = p_dma_ch->CallBackFnctPtr;
        p_callback_arg       = p_dma_ch->CallBackArgPtr;
        p_dma_ch->State      = CSP_DMA_CH_STATE_ALLOC;          /* Set the channel state = 'ALLOC'                      */
        xfer_size_rem        = p_dma_ch_reg->Ctrl & DEF_BIT_FIELD(12u, 0u);        
        status               = DEF_FAIL;
        p_dma_reg->IntErrClr = DEF_BIT(ch_nbr);
            
        callback_fnct(ch_nbr,
                      xfer_size_rem,
                      p_callback_arg,
                      status);
        
        int_err_stat = p_dma_reg->IntErrStat & DEF_BIT_FIELD(8u, 0u);                            
    }
}



