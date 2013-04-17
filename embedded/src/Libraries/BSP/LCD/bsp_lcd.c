/*
*********************************************************************************************************
*                                     MICRIUM BOARD SUPPORT SUPPORT
*
*                          (c) Copyright 2003-2010; Micrium, Inc.; Weston, FL
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
*                                          LCD DISPLAY DRIVER
*                                     KEIL MCB1700 DEVELOPMENT KIT
*
* Filename      : bsp_lcd.c
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#ifndef   BSP_LCD_MODULE
#define   BSP_LCD_MODULE
#endif

#include  <bsp_lcd.h>
#include  <os.h>
#include  <csp.h>


/*
*********************************************************************************************************
*                                                DEFINES
*
* Note(s) (1): LCD orientation and GRAM configuration:
*
*                             | ------ 240 ------ |
*                             +===================+  ---
*                             |                   |   |
*                GRAM         | ----------------> |   |
*             ADDRES UPDATE   | ----------------> |   |
*                             |                   |   |
*                             |                   |   |
*                             |                   |  320
*                             |                   |   |
*                             |                   |   |
*                             |                   |   |
*                             |                   |   |
*                             |                   |   |
*                             +===================+  ---
*
*********************************************************************************************************
*/

#define  BSP_LCD_WIDTH                                  240u    /* Screen Width (in pixels)                             */
#define  BSP_LCD_HEIGHT                                 320u    /* Screen Hight (in pixels)                             */

#define  BSP_LCD_SIZE                           (BSP_LCD_WIDTH *  \
                                                 BSP_LCD_HEIGHT)

#define  BSP_LCD_MAX_NBR_IMG                            256u
#define  BSP_LCD_MAX_NBR_FONT                           128u

#define  BSP_LCD_ILI9320_SPI_FREQ                   25000000
#define  BSP_LCD_ILI9320_DEV_CODE                     0xC990    /* LCD controller device code.                          */
#define  BSP_LCD_SPFD5408_DEV_CODE                    0x2A04    /* LCD Controller device code. (SPFD5408)               */

/*
*********************************************************************************************************
*                                                 SSP REGISTER
*********************************************************************************************************
*/


#define  BSP_LCD_SSP1_ADDR_BASE                   (CPU_ADDR   )(0x40030000)
#define  BSP_LCD_SSP1_REG_CR0                   (*(CPU_REG32 *)(BSP_LCD_SSP1_ADDR_BASE + 0x00))
#define  BSP_LCD_SSP1_REG_CR1                   (*(CPU_REG32 *)(BSP_LCD_SSP1_ADDR_BASE + 0x04))
#define  BSP_LCD_SSP1_REG_DR                    (*(CPU_REG32 *)(BSP_LCD_SSP1_ADDR_BASE + 0x08))
#define  BSP_LCD_SSP1_REG_SR                    (*(CPU_REG32 *)(BSP_LCD_SSP1_ADDR_BASE + 0x0C))
#define  BSP_LCD_SSP1_REG_CPSR                  (*(CPU_REG32 *)(BSP_LCD_SSP1_ADDR_BASE + 0x10))
#define  BSP_LCD_SSP1_REG_IMSC                  (*(CPU_REG32 *)(BSP_LCD_SSP1_ADDR_BASE + 0x14))
#define  BSP_LCD_SSP1_REG_RIS                   (*(CPU_REG32 *)(BSP_LCD_SSP1_ADDR_BASE + 0x18))
#define  BSP_LCD_SSP1_REG_MIS                   (*(CPU_REG32 *)(BSP_LCD_SSP1_ADDR_BASE + 0x1C))
#define  BSP_LCD_SSP1_REG_ICR                   (*(CPU_REG32 *)(BSP_LCD_SSP1_ADDR_BASE + 0x20))

#define  BSP_LCD_SSP1_BIT_CR0_CPOL              DEF_BIT_06
#define  BSP_LCD_SSP1_BIT_CR0_CPHA              DEF_BIT_07
#define  BSP_LCD_SSP1_BIT_CR0_FRF_TI            DEF_BIT_MASK(1u, 4u)
#define  BSP_LCD_SSP1_BIT_CR0_FRF_MICRO         DEF_BIT_MASK(2u, 4u)

#define  BSP_LCD_SSP1_BIT_SR_TFE                DEF_BIT_00
#define  BSP_LCD_SSP1_BIT_SR_TNF                DEF_BIT_01
#define  BSP_LCD_SSP1_BIT_SR_RNE                DEF_BIT_02
#define  BSP_LCD_SSP1_BIT_SR_RFF                DEF_BIT_03
#define  BSP_LCD_SSP1_BIT_SR_BSY                DEF_BIT_04

#define  CSP_SSP_BIT_CR1_SSE                    DEF_BIT_01


/*
*********************************************************************************************************
*                                             ILI9320 REGISTER DEFINES
*********************************************************************************************************
*/

#define  ILI9320_REG_DVR_CODE_RD                0x00
#define  ILI9320_REG_START_OSC                  0x00
#define  ILI9320_REG_DRV_OUT_CTRL1              0x01
#define  ILI9320_REG_LCD_DRV_CTRL1              0x02
#define  ILI9320_REG_ENTRY_MODE                 0x03
#define  ILI9320_REG_RESIZE_CTRL                0x04
#define  ILI9320_REG_DISP_CTRL1                 0x07
#define  ILI9320_REG_DISP_CTRL2                 0x08
#define  ILI9320_REG_DISP_CTRL3                 0x09
#define  ILI9320_REG_DISP_CTRL4                 0x0A
#define  ILI9320_REG_RGB_DISP_IF_CTRL1          0x0C
#define  ILI9320_REG_FRAME_MAKER_POS            0x0D
#define  ILI9320_REG_RGB_DISP_IF_CTRL2          0x0F

#define  ILI9320_REG_PWR_CTRL1                  0x10
#define  ILI9320_REG_PWR_CTRL2                  0x11
#define  ILI9320_REG_PWR_CTRL3                  0x12
#define  ILI9320_REG_PWR_CTRL4                  0x13


#define  ILI9320_REG_HOR_GRAM_ADDR_SET          0x20
#define  ILI9320_REG_VER_GRAM_ADDR_SET          0x21
#define  ILI9320_REG_GRAM_DATA_WR               0x22
#define  ILI9320_REG_PWR_CTRL7                  0x29
#define  ILI9320_REG_FRAME_RATE_CTRL            0x2B


#define  ILI9320_REG_GAMA_CTRL1                 0x30
#define  ILI9320_REG_GAMA_CTRL2                 0x31
#define  ILI9320_REG_GAMA_CTRL3                 0x32
#define  ILI9320_REG_GAMA_CTRL4                 0x35
#define  ILI9320_REG_GAMA_CTRL5                 0x36
#define  ILI9320_REG_GAMA_CTRL6                 0x37
#define  ILI9320_REG_GAMA_CTRL7                 0x38
#define  ILI9320_REG_GAMA_CTRL8                 0x39
#define  ILI9320_REG_GAMA_CTRL9                 0x3C
#define  ILI9320_REG_GAMA_CTRL10                0x3D


#define  ILI9320_REG_HOR_ADDR_START             0x50
#define  ILI9320_REG_HOR_ADDR_END               0x51
#define  ILI9320_REG_VER_ADDR_START             0x52
#define  ILI9320_REG_VER_ADDR_END               0x53

#define  ILI9320_REG_DRV_OUT_CTRL2              0x60
#define  ILI9320_REG_BASE_IMG_DISP_CTRL         0x61
#define  ILI9320_REG_VER_SCROLL_CTRL            0x6A

#define  ILI9320_REG_PAR_IMG_DISP_POS1          0x80
#define  ILI9320_REG_PAR_IMG_DISP_AREA_START1   0x80
#define  ILI9320_REG_PAR_IMG_DISP_AREA_END1     0x82
#define  ILI9320_REG_PAR_IMG_DISP_POS2          0x83
#define  ILI9320_REG_PAR_IMG_DISP_AREA_START2   0x84
#define  ILI9320_REG_PAR_IMG_DISP_AREA_END2     0x85

#define  ILI9320_REG_PANEL_IF_CTRL1             0x90
#define  ILI9320_REG_PANEL_IF_CTRL2             0x92
#define  ILI9320_REG_PANEL_IF_CTRL3             0x93
#define  ILI9320_REG_PANEL_IF_CTRL4             0x95
#define  ILI9320_REG_PANEL_IF_CTRL5             0x97
#define  ILI9320_REG_PANEL_IF_CTRL6             0x98


#define  ILI9320_BIT_DRV_OUT_CTRL1_SS           DEF_BIT_08
#define  ILI9320_BIT_DRV_OUT_CTRL1_SM           DEF_BIT_10

#define  ILI9320_BIT_LCD_DRV_CTRL1_EOR          DEF_BIT_08
#define  ILI9320_BIT_LCD_DRV_CTRL1_BC           DEF_BIT_09
#define  ILI9320_BIT_LCD_DRV_CTRL1_SET_LINE    (DEF_BIT_10 | DEF_BIT_09 | DEF_BIT_08)


#define  ILI9320_BIT_PWR_CTRL1_AP_HALT          DEF_BIT_00
#define  ILI9320_BIT_PWR_CTRL1_AP_0_50          DEF_BIT_MASK(1u, 4u)
#define  ILI9320_BIT_PWR_CTRL1_AP_0_75          DEF_BIT_MASK(12, 4u)
#define  ILI9320_BIT_PWR_CTRL1_AP_1             DEF_BIT_MASK(13, 4u)
#define  ILI9320_BIT_PWR_CTRL1_APE              DEF_BIT_07
#define  ILI9320_BIT_PWR_CTRL1_SAP              DEF_BIT_12


#define  ILI9320_BIT_PWR_CTRL3_PON              DEF_BIT_04
#define  ILI9320_BIT_PWR_CTRL3_VCMR             DEF_BIT_08


#define  BSP_LCD_SPI_ILI9320_START_RW           DEF_BIT_00
#define  BSP_LCD_SPI_ILI9320_START_RS           DEF_BIT_01
#define  BSP_LCD_SPI_ILI9320_START_ID_CODE     (DEF_BIT_04 | DEF_BIT_05 | DEF_BIT_06)

#define  BSP_LCD_SPI_ILI9320_START_SET_IX      (BSP_LCD_SPI_ILI9320_START_ID_CODE)

#define  BSP_LCD_SPI_ILI9320_START_RD_STAT     (BSP_LCD_SPI_ILI9320_START_ID_CODE | \
                                                BSP_LCD_SPI_ILI9320_START_RW)
#define  BSP_LCD_SPI_ILI9320_START_WR_REG      (BSP_LCD_SPI_ILI9320_START_ID_CODE | \
                                                BSP_LCD_SPI_ILI9320_START_RS)
#define  BSP_LCD_SPI_ILI9320_START_RD_REG      (BSP_LCD_SPI_ILI9320_START_ID_CODE | \
                                                BSP_LCD_SPI_ILI9320_START_RW      | \
                                                BSP_LCD_SPI_ILI9320_START_RS)


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*
*********************************************************************************************************
*/


typedef  struct  bsp_lcd_dev {
    BSP_LCD_COLOR       BgColor;                                /* Background color.                                    */
    BSP_LCD_COLOR       TextColor;                              /* Text color.                                          */
    BSP_LCD_TEXT_POS    TextLineMax;                            /* Text maximum number of lines.                        */
    BSP_LCD_TEXT_POS    TextLineCur;                            /* Text current line number.                            */
    BSP_LCD_TEXT_POS    TextColMax;                             /* Text maximum number of columns.                      */
    BSP_LCD_TEXT_POS    TextColCur;                             /* Text current column number.                          */
    BSP_LCD_PIXEL       TextPixelCur;                           /* Text current horizontal pixel.                       */
    BSP_LCD_FONT       *TextFontPtr;                            /* Text font pointer.                                   */
    OS_MUTEX            Lock;                                   /* Display lock.                                        */
    CPU_TS32            MaxTimeOut;                             /* Max timeout for SPI transfers.                       */
    BSP_LCD_IMG       **ImgTblPtr;                              /* Image table pointer.                                 */
    BSP_LCD_FONT      **FontTblPtr;                             /* Image table pointer.                                 */
} BSP_LCD_DEV;


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


static  BSP_LCD_DEV   BSP_LCD_Ctxt;


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/

#define  BSP_LCD_CTRL_CS_LOW()        {    CSP_GPIO_BitClr(CSP_GPIO_PORT_NBR_00, DEF_BIT_06);    }
#define  BSP_LCD_CTRL_CS_HIGH()       {    CSP_GPIO_BitSet(CSP_GPIO_PORT_NBR_00, DEF_BIT_06);    }


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void         BSP_LCD_Lock             (void);
static  void         BSP_LCD_Unlock           (void);

static  CPU_INT16U   BSP_LCD_CtrlRegRd        (CPU_INT08U      reg_off);
static  void         BSP_LCD_CtrlRegWr        (CPU_INT08U      reg_off,
                                               CPU_INT16U      reg_val);

static  CPU_INT08U   BSP_LCD_SPI_Xfer         (CPU_INT08U      datum);

static  void         BSP_LCD_BitmapDrawUnlock (BSP_LCD_PIXEL   x,
                                               BSP_LCD_PIXEL   y,
                                               BSP_LCD_PIXEL   width,
                                               BSP_LCD_PIXEL   height,
                                               BSP_LCD_COLOR  *p_bitmap);

static   void        BSP_LCD_RectDrawUnlock   (BSP_LCD_PIXEL   x,
                                               BSP_LCD_PIXEL   y,
                                               BSP_LCD_PIXEL   width,
                                               BSP_LCD_PIXEL   height,
                                               CPU_BOOLEAN     fill,
                                               BSP_LCD_COLOR   fill_color,
                                               BSP_LCD_PIXEL   line_weight,
                                               BSP_LCD_COLOR   line_color);

static  void         BSP_LCD_TextCharUnlock   (CPU_CHAR        c);


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
*                                             BSP_LCD_Init()
*
* Description : Initialize the LCD Display.
*
* Argument(s) : none.
*
* Return(s)   : DEF_OK,  If the LCD driver was initialized.
*               DEF_FAIL, Otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN   BSP_LCD_Init (void)
{
    CPU_INT16U       clk_div;
    CPU_INT32U       pclk_freq;
    CPU_TS_TMR_FREQ  tmr_freq;
    CPU_INT16U       dev_id;
    OS_ERR           err;
    CPU_ERR          cpu_err;


    CSP_GPIO_Cfg((CSP_DEV_NBR    )CSP_GPIO_PORT_NBR_00,         /* ... Port0.                                           */
                 (CSP_GPIO_MSK   )DEF_BIT_06,                   /* ... Pin 0.06 GPIO.                                   */
                 (CSP_OPT        )CSP_GPIO_DIR_OUT,             /* ... Pin configured as output.                        */
                 (CSP_OPT_FLAGS  )CSP_GPIO_FLAG_MODE_NONE,      /* ... Neither  pull-up nor pull-down resistor.         */
                 (CPU_BOOLEAN    )DEF_NO,                       /* ... Interrupt disabled.                              */
                 (CSP_OPT        )0,
                 (CSP_OPT        )CSP_GPIO_FNCT_00);


    CSP_GPIO_Cfg((CSP_DEV_NBR    )CSP_GPIO_PORT_NBR_00,         /* ... Port0.                                           */
                 (CSP_GPIO_MSK   )(DEF_BIT_07 |                 /* ... Pin 0.07 SCK1.                                   */
                                   DEF_BIT_08 |                 /* ... Pin 0.08 MISO1.                                  */
                                   DEF_BIT_09),                 /* ... Pin 0.09 MOSI1.                                  */
                 (CSP_OPT        )CSP_GPIO_DIR_IN,              /* ... Pin configured as input.                         */
                 (CSP_OPT_FLAGS  )CSP_GPIO_FLAG_MODE_NONE,      /* ... Neither  pull-up nor pull-down resistor.         */
                 (CPU_BOOLEAN    )DEF_NO,                       /* ... Interrupt disabled.                              */
                 (CSP_OPT        )0,
                 (CSP_OPT        )CSP_GPIO_FNCT_02);

    CSP_GPIO_Cfg((CSP_DEV_NBR    )CSP_GPIO_PORT_NBR_04,         /* ... Port0.                                           */
                 (CSP_GPIO_MSK   )DEF_BIT_28 | DEF_BIT_29,      /* ... Pin 4.28 GPIO.                                   */
                 (CSP_OPT        )CSP_GPIO_DIR_OUT,             /* ... Pin configured as output.                        */
                 (CSP_OPT_FLAGS  )CSP_GPIO_FLAG_MODE_NONE,      /* ... Neither  pull-up nor pull-down resistor.         */
                 (CPU_BOOLEAN    )DEF_NO,                       /* ... Interrupt disabled.                              */
                 (CSP_OPT        )0,
                 (CSP_OPT        )CSP_GPIO_FNCT_00);

    CSP_PM_PerClkDivCfg(CSP_PM_PER_CLK_NBR_SSP_01,              /* Set the SSP frequency to CLK /2                      */
                     2u);

    pclk_freq = CSP_PM_PerClkFreqGet(CSP_PM_PER_CLK_NBR_SSP_01);

    if (pclk_freq < BSP_LCD_ILI9320_SPI_FREQ) {
        return (DEF_FAIL);
    }

                                                                /* Calculate the divider. Preescaler = 2u.              */
    clk_div  = ((pclk_freq / BSP_LCD_ILI9320_SPI_FREQ) - 1u) / 2u;

    if (clk_div > DEF_INT_08U_MAX_VAL) {
        return (DEF_FAIL);
    }

    OSMutexCreate((OS_MUTEX *)&BSP_LCD_Ctxt.Lock,               /* Create mutex to lock the LCD.                        */
                  (CPU_CHAR *)"LCD lock",
                  (OS_ERR   *)&err);

    if (err != OS_ERR_NONE) {
        return (DEF_FAIL);
    }

    BSP_LCD_Lock();

    CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_SSP_01);                 /* Enable SSP1 peripheral clock.                        */

    BSP_LCD_SSP1_REG_CR0       = (8u - 1u)                      /* ... 8 bits data size.                                */
                               | BSP_LCD_SSP1_BIT_CR0_CPOL      /* ... High polarity.                                   */
                               | BSP_LCD_SSP1_BIT_CR0_CPHA;     /* ... Capture serial data on the second clock.         */

    BSP_LCD_SSP1_REG_CR1       = CSP_SSP_BIT_CR1_SSE;           /* Enable the SSP.                                      */
    BSP_LCD_SSP1_REG_MIS       = DEF_BIT_NONE;                  /* Disable all interrupts.                              */
    BSP_LCD_SSP1_REG_CPSR      = 2u;                            /* Preescaler = 2.                                      */

    BSP_LCD_Ctxt.BgColor       = BSP_LCD_COLOR_BLACK;
    BSP_LCD_Ctxt.TextFontPtr   = (BSP_LCD_FONT    *)0;
    BSP_LCD_Ctxt.TextColor     = (BSP_LCD_COLOR    )BSP_LCD_COLOR_WHITE;
    BSP_LCD_Ctxt.TextLineMax   = (BSP_LCD_TEXT_POS )0u;
    BSP_LCD_Ctxt.TextLineCur   = (BSP_LCD_TEXT_POS )0u;
    BSP_LCD_Ctxt.TextColMax    = (BSP_LCD_TEXT_POS )0u;
    BSP_LCD_Ctxt.TextColCur    = (BSP_LCD_TEXT_POS )0u;
    BSP_LCD_Ctxt.TextPixelCur  = (BSP_LCD_PIXEL    )0u;
    BSP_LCD_Ctxt.FontTblPtr    = (BSP_LCD_FONT   **)0;
    BSP_LCD_Ctxt.ImgTblPtr     = (BSP_LCD_IMG    **)0;


    CSP_GPIO_BitSet(CSP_GPIO_PORT_NBR_04,
                    DEF_BIT_29);

                                                                /* Calculate the max SPI timeout in TS value.           */
    tmr_freq                 = CPU_TS_TmrFreqGet(&cpu_err);
    tmr_freq                /= DEF_TIME_NBR_mS_PER_SEC;
    pclk_freq               /= DEF_TIME_NBR_mS_PER_SEC;
    BSP_LCD_Ctxt.MaxTimeOut = (tmr_freq  * (32u))
                            / (pclk_freq / (2u * (clk_div + 1u)));

    OSTimeDlyHMSM(0u, 0u, 0u, 200u,                             /* Discharge cap power voltage (200ms).                 */
                  OS_OPT_TIME_HMSM_STRICT,
                  &err);

    BSP_LCD_BackLightOff();

    dev_id = BSP_LCD_CtrlRegRd(ILI9320_REG_DVR_CODE_RD);       /* Read the Device code.                                */
    dev_id = BSP_LCD_CtrlRegRd(ILI9320_REG_DVR_CODE_RD);       /* Read the Device code.                                */

    if ((dev_id != BSP_LCD_ILI9320_DEV_CODE ) &&
        (dev_id != BSP_LCD_SPFD5408_DEV_CODE)) {
        BSP_LCD_Unlock();
        return (DEF_FAIL);
    }

                                                                /* -------------- ILI9320 INITIAL SEQUENCE ------------ */
    BSP_LCD_CtrlRegWr(ILI9320_REG_DRV_OUT_CTRL1,                /* Set SS bit.                                          */
                      ILI9320_BIT_DRV_OUT_CTRL1_SS);

    BSP_LCD_CtrlRegWr(ILI9320_REG_LCD_DRV_CTRL1,                /* Set 1 line inversion.                                */
                      ILI9320_BIT_LCD_DRV_CTRL1_SET_LINE);

    BSP_LCD_CtrlRegWr(ILI9320_REG_RESIZE_CTRL,                  /* Resize register.                                     */
                      DEF_BIT_NONE);                            /* .... No resizing, not remaining pixels.              */

    BSP_LCD_CtrlRegWr(ILI9320_REG_DISP_CTRL2,                   /* 2 lines front, 7 back porch.                         */
                      (2 << 8u) | (7u << 0u));

    BSP_LCD_CtrlRegWr(ILI9320_REG_DISP_CTRL3,                   /* Set non-disp area refresh cyc ISC.                   */
                      DEF_BIT_NONE);

    BSP_LCD_CtrlRegWr(ILI9320_REG_DISP_CTRL4,                   /* FMARK function                                       */
                      DEF_BIT_NONE);

    BSP_LCD_CtrlRegWr(ILI9320_REG_RGB_DISP_IF_CTRL1,            /* RGB interface setting.                               */
                      DEF_BIT_NONE);

    BSP_LCD_CtrlRegWr(ILI9320_REG_FRAME_MAKER_POS,              /* Frame marker Position.                               */
                      DEF_BIT_NONE);

    BSP_LCD_CtrlRegWr(ILI9320_REG_RGB_DISP_IF_CTRL2,            /* RGB interface polarity.                              */
                      DEF_BIT_NONE);
                                                                /* ------------------- POWER ON SEQUENCE -------------- */
    BSP_LCD_CtrlRegWr(ILI9320_REG_PWR_CTRL1, DEF_BIT_NONE);     /* Reset Power Control 1.                               */
    BSP_LCD_CtrlRegWr(ILI9320_REG_PWR_CTRL2, DEF_BIT_NONE);     /* Reset Power Control 2.                               */
    BSP_LCD_CtrlRegWr(ILI9320_REG_PWR_CTRL3, DEF_BIT_NONE);     /* Reset Power Control 3.                               */
    BSP_LCD_CtrlRegWr(ILI9320_REG_PWR_CTRL4, DEF_BIT_NONE);     /* Reset Power Control 4.                               */

    OSTimeDlyHMSM(0u, 0u, 0u, 200u,                             /* Discharge cap power voltage (200ms).                 */
                  OS_OPT_TIME_HMSM_STRICT,
                  &err);

    BSP_LCD_CtrlRegWr(ILI9320_REG_PWR_CTRL1,
                      (ILI9320_BIT_PWR_CTRL1_AP_0_50 |          /* ... 0.5 Current.                                     */
                       ILI9320_BIT_PWR_CTRL1_APE     |          /* ... Power supply enable.                             */
                       (2u << 8u)                    |          /* ... Vci 1 x 2.                                       */
                       ILI9320_BIT_PWR_CTRL1_SAP));             /* ... Source driver output control enabled.            */

    BSP_LCD_CtrlRegWr(ILI9320_REG_PWR_CTRL2,
                      0x0007);

    BSP_LCD_CtrlRegWr(ILI9320_REG_PWR_CTRL3,
                      ((0xD << 0u)              |               /* ... VciLVL x 1.90.                                   */
                      ILI9320_BIT_PWR_CTRL3_PON |               /* ... VGL Output enable.                               */
                      ILI9320_BIT_PWR_CTRL3_VCMR));             /* ... Internal electric volume.                        */

    OSTimeDlyHMSM(0u, 0u, 0u, 50u,                              /* Delay for 50ms                                       */
                  OS_OPT_TIME_HMSM_STRICT,
                  &err);

    BSP_LCD_CtrlRegWr(ILI9320_REG_PWR_CTRL4,
                     (0x14 << 8u));                             /* ... VREG1OUT x 0.96.                                 */

    BSP_LCD_CtrlRegWr(ILI9320_REG_PWR_CTRL7,
                     (0x0E << 0u));                             /* ... VEREGOUT x 0.83                                  */

    OSTimeDlyHMSM(0u, 0u, 0u, 50u,                              /* Delay for 50ms                                       */
                  OS_OPT_TIME_HMSM_STRICT,
                  &err);

    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_GRAM_ADDR_SET, 0x0000);   /* GRAM horizontal Address.                             */
    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_GRAM_ADDR_SET, 0x0000);   /* GRAM vertical   Address.                             */

                                                                /* ------------ ADJUST THE GAMMA CURVE ---------------  */
    if (dev_id == BSP_LCD_ILI9320_DEV_CODE) {                   /* ILI9320 Gamma initialization                         */

        BSP_LCD_CtrlRegWr(ILI9320_REG_GAMA_CTRL1,  0x0006);
        BSP_LCD_CtrlRegWr(ILI9320_REG_GAMA_CTRL2,  0x0101);
        BSP_LCD_CtrlRegWr(ILI9320_REG_GAMA_CTRL3,  0x0003);
        BSP_LCD_CtrlRegWr(ILI9320_REG_GAMA_CTRL4,  0x0106);
        BSP_LCD_CtrlRegWr(ILI9320_REG_GAMA_CTRL5,  0x0B02);
        BSP_LCD_CtrlRegWr(ILI9320_REG_GAMA_CTRL6,  0x0302);
        BSP_LCD_CtrlRegWr(ILI9320_REG_GAMA_CTRL7,  0x0707);
        BSP_LCD_CtrlRegWr(ILI9320_REG_GAMA_CTRL8,  0x0007);
        BSP_LCD_CtrlRegWr(ILI9320_REG_GAMA_CTRL9,  0x0600);
        BSP_LCD_CtrlRegWr(ILI9320_REG_GAMA_CTRL10, 0x020B);
     } else {
                                                                 /* SPFD5408 Gamma initalization                        */
    	BSP_LCD_CtrlRegWr(0x30, 0x0B0D);
    	BSP_LCD_CtrlRegWr(0x31, 0x1923);
    	BSP_LCD_CtrlRegWr(0x32, 0x1C26);
    	BSP_LCD_CtrlRegWr(0x33, 0x261C);
    	BSP_LCD_CtrlRegWr(0x34, 0x2419);
    	BSP_LCD_CtrlRegWr(0x35, 0x0D0B);
    	BSP_LCD_CtrlRegWr(0x36, 0x1006);
    	BSP_LCD_CtrlRegWr(0x37, 0x0610);
    	BSP_LCD_CtrlRegWr(0x38, 0x0706);
    	BSP_LCD_CtrlRegWr(0x39, 0x0304);
    	BSP_LCD_CtrlRegWr(0x3A, 0x0E05);
    	BSP_LCD_CtrlRegWr(0x3B, 0x0E01);
    	BSP_LCD_CtrlRegWr(0x3C, 0x010E);
    	BSP_LCD_CtrlRegWr(0x3D, 0x050E);
    	BSP_LCD_CtrlRegWr(0x3E, 0x0403);
    	BSP_LCD_CtrlRegWr(0x3F, 0x0607);
     }
                                                                /* ------------------ SET GRAM AREA ------------------- */
    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_ADDR_START, 0x0000);      /* Horizontal GRAM Start Address.                       */
    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_ADDR_END,
                      (BSP_LCD_WIDTH - 1u));                    /* Horizontal GRAM End   Address.                       */

    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_ADDR_START, 0x0000);      /* Vertical   GRAM Start Address.                       */
    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_ADDR_END,
                     (BSP_LCD_HEIGHT - 1u));                    /* Vertical   GRAM End   Address.                       */

    if (dev_id == BSP_LCD_ILI9320_DEV_CODE) {
        BSP_LCD_CtrlRegWr(ILI9320_REG_DRV_OUT_CTRL2, 0x2700);   /* Gate Scan Line.                                      */
    } else {
        BSP_LCD_CtrlRegWr(ILI9320_REG_DRV_OUT_CTRL2, 0xA700);
    }

    BSP_LCD_CtrlRegWr(ILI9320_REG_BASE_IMG_DISP_CTRL, 0x0001);  /* NDL,VLE, REV.                                        */
    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_SCROLL_CTRL, 0x0000);     /* Set scrolling line.                                  */

                                                                /* ------------ PANEL CONTROL CONFIGURATION ----------- */
    BSP_LCD_CtrlRegWr(ILI9320_REG_PANEL_IF_CTRL1, 0x0010);
    BSP_LCD_CtrlRegWr(ILI9320_REG_PANEL_IF_CTRL2, 0x0000);
    BSP_LCD_CtrlRegWr(ILI9320_REG_PANEL_IF_CTRL3, 0x0003);
    BSP_LCD_CtrlRegWr(ILI9320_REG_PANEL_IF_CTRL4, 0x0110);
    BSP_LCD_CtrlRegWr(ILI9320_REG_PANEL_IF_CTRL5, 0x0000);
    BSP_LCD_CtrlRegWr(ILI9320_REG_PANEL_IF_CTRL6, 0x0000);

                                                                /* Set GRAM write direction                             */
                                                                /* ... I/D = '11'(Hor : inc, Ver : inc)                 */
                                                                /* ... AM  = '1' (address is updated in ver. direction  */
    BSP_LCD_CtrlRegWr(ILI9320_REG_ENTRY_MODE, 0x1030);
    BSP_LCD_CtrlRegWr(ILI9320_REG_DISP_CTRL1, 0x0137);          /* 262K color and display ON                            */

    BSP_LCD_Unlock();

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                           BSP_LCD_Clr()
*
* Description : Clear the LCD..
*
* Argument(s) : none.
*
* Return(s)   : none.
*               DEF_FAIL, Otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LCD_Clr (void )
{
    CPU_INT32U     pixel;
    BSP_LCD_COLOR  color;


    BSP_LCD_Lock();

    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_ADDR_START, 0x0000);      /* Horizontal GRAM Start Address.                       */
    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_ADDR_END,
                      (BSP_LCD_WIDTH - 1u));                    /* Horizontal GRAM End   Address.                       */

    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_ADDR_START, 0x0000);      /* Vertical   GRAM Start Address.                       */
    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_ADDR_END,
                      (BSP_LCD_HEIGHT - 1u));                   /* Vertical   GRAM End   Address.                       */

    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_GRAM_ADDR_SET, 0);
    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_GRAM_ADDR_SET, 0);

    BSP_LCD_CTRL_CS_LOW();

    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_SET_IX);
    BSP_LCD_SPI_Xfer(0u);
    BSP_LCD_SPI_Xfer(ILI9320_REG_GRAM_DATA_WR);

    BSP_LCD_CTRL_CS_HIGH();

    BSP_LCD_CTRL_CS_LOW();
    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_WR_REG);

    color   = BSP_LCD_Ctxt.BgColor;

    for (pixel = 0u; pixel < BSP_LCD_SIZE; pixel++) {
        BSP_LCD_SPI_Xfer(color >> 8u);
        BSP_LCD_SPI_Xfer(color & DEF_INT_08_MASK);
    }

    BSP_LCD_CTRL_CS_HIGH();

    BSP_LCD_Unlock();
}

/*
*********************************************************************************************************
*                                            BSP_LCD_BG_ColorSet()
*
* Description : Set the background color.
*
* Argument(s) : color  Color to be set.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void   BSP_LCD_BG_ColorSet (BSP_LCD_COLOR  color)
{
    BSP_LCD_Lock();

    BSP_LCD_Ctxt.BgColor = color;

    BSP_LCD_Unlock();
}


/*
*********************************************************************************************************
*                                              IMAGE FUNCTIONS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            BSP_LCD_ImgDraw()
*
* Description : Draw a image.
*
* Argument(s) : x      X axis position in pixels
*
*               y      Y axis Position
*
*               p_img  Pointer to image.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/


void  BSP_LCD_ImgDraw (BSP_LCD_PIXEL   x,
                       BSP_LCD_PIXEL   y,
                       BSP_LCD_IMG    *p_img)
{

    if (p_img == (BSP_LCD_IMG *)0) {
        return;
    }

    BSP_LCD_Lock();                                             /* Acquire LCD lock.                                    */

    BSP_LCD_BitmapDrawUnlock(x,                                 /* Display the image.                                   */
                             y,
                             p_img->Width,
                             p_img->Height,
                             p_img->BitmapPtr);

    BSP_LCD_Unlock();                                           /* Release LCD lock.                                    */
}


/*
*********************************************************************************************************
*                                            BSP_LCD_ImgTblSet()
*
* Description : Set the image table.
*
* Argument(s) : p_tbl   Pointer to the image table.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LCD_ImgTblSet (BSP_LCD_IMG  **p_tbl)
{
    if (p_tbl == (BSP_LCD_IMG **) 0) {
        return;
    }

    BSP_LCD_Lock();

    BSP_LCD_Ctxt.ImgTblPtr = p_tbl;

    BSP_LCD_Unlock();
}


/*
*********************************************************************************************************
*                                       BSP_LCD_ImgGetByName()
*
* Description : Look for an image from the image table using the image name.
*
* Argument(s) : p_name   Pointer to the image name.
*
* Return(s)   : Pointer to the required image. If the image cannot be found in the image table a 'NULL'
*               pointer is returned.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

BSP_LCD_IMG  *BSP_LCD_ImgGetByName  (CPU_CHAR  *p_name)
{
    BSP_LCD_IMG     **p_img_tbl;
    BSP_LCD_IMG      *p_img;
    CPU_INT16S        cmp;
    CPU_SIZE_T        img_ix;
    CPU_BOOLEAN       found;

                                                                /* ------------------- ARGUMENTS CHECKING ------------- */
    if (p_name == (CPU_CHAR *)0) {                              /* NULL pointer ?                                       */
        return ((BSP_LCD_IMG *)0);
    }

    p_img_tbl = BSP_LCD_Ctxt.ImgTblPtr;

    if (p_img_tbl == (BSP_LCD_IMG **)0) {                       /* If a image table has not been set.                    */
        return ((BSP_LCD_IMG *)0);
    }

    found  = DEF_NO;
    img_ix = 0u;
    p_img  = (BSP_LCD_IMG *)0;
                                                               /* Search the image in the font table by name.          */
    while ((p_img_tbl[img_ix] !=  (BSP_LCD_IMG     *)0) &&
           (found             ==  DEF_NO              ) &&
           (img_ix             <  BSP_LCD_MAX_NBR_IMG)) {

        p_img = p_img_tbl[img_ix];
        cmp   = Str_Cmp(p_name,                                 /* Compare the image name.                              */
                        p_img->NamePtr);

        if (cmp == (CPU_INT16S)0) {                             /* If the image is found ...                            */
            found = DEF_YES;                                    /* ... exit the search routine.                         */
        } else {
            img_ix++;                                           /* ... move to the next entry, otherwise.               */
        }
    }

    if (found == DEF_NO) {                                      /* If an image could not been found in the image table  */
        return ((BSP_LCD_IMG *)0);                              /* ... return a 'NULL' pointer.                         */
    }

    return ((p_img));
}


/*
*********************************************************************************************************
*                                              TEXT FUNCTIONS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            BSP_LCD_ColorSet()
*
* Description : Set the text color.
*
* Argument(s) : color    Color to be set.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void   BSP_LCD_TextColorSet  (BSP_LCD_COLOR  color)
{
    BSP_LCD_Lock();

    BSP_LCD_Ctxt.TextColor = color;

    BSP_LCD_Unlock();
}


/*
*********************************************************************************************************
*                                            BSP_LCD_ColorSet()
*
* Description : Clear a line.
*
* Argument(s) : line :   Line to clear.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LCD_TextLineClr (BSP_LCD_TEXT_POS line)
{
    BSP_LCD_DEV      *p_dev;
    BSP_LCD_FONT     *p_font;
    BSP_LCD_PIXEL     pixel_x;
    BSP_LCD_PIXEL     pixel_y;
    BSP_LCD_PIXEL     pixel_line;
    BSP_LCD_COLOR     color;



    p_dev = &BSP_LCD_Ctxt;                                      /* Get a reference to LCD device.                       */

    if (line > p_dev->TextLineMax) {                            /* Check the text coordinates boundaries.               */
        return;
    }

    p_font = (BSP_LCD_FONT *)p_dev->TextFontPtr;                /* Get a reference to the current font.                 */

    if (p_font == (BSP_LCD_FONT *)0) {
        return;
    }

    BSP_LCD_Lock();                                             /* Acquire LCD lock.                                    */

    color       = p_dev->BgColor;
    pixel_line  = (BSP_LCD_PIXEL)((BSP_LCD_HEIGHT * line)
                / p_dev->TextLineMax);

    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_ADDR_START, 0);
    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_ADDR_END,   BSP_LCD_WIDTH - 1u);

    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_ADDR_START, pixel_line);
    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_ADDR_END,   pixel_line + p_font->Height - 1u);

    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_GRAM_ADDR_SET, 0);
    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_GRAM_ADDR_SET, pixel_line);

    BSP_LCD_CTRL_CS_LOW();

    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_SET_IX);
    BSP_LCD_SPI_Xfer(0u);
    BSP_LCD_SPI_Xfer(ILI9320_REG_GRAM_DATA_WR);

    BSP_LCD_CTRL_CS_HIGH();

    BSP_LCD_CTRL_CS_LOW();
    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_WR_REG);


    for (pixel_y = 0; pixel_y < p_font->Height; pixel_y++) {
        for (pixel_x = 0; pixel_x < BSP_LCD_WIDTH - 1u; pixel_x++) {
            BSP_LCD_SPI_Xfer(color >> 8u);
            BSP_LCD_SPI_Xfer(color & DEF_INT_08_MASK);
        }
    }

    BSP_LCD_CTRL_CS_HIGH();

    BSP_LCD_Unlock();                                           /* Release LCD lock.                                    */

}


/*
*********************************************************************************************************
*                                          BSP_LCD_TextDispStr()
*
* Description : Display a string.
*
* Argument(s) : col    Column where the text will be displayed.
*
*               line   Line   where the text will be displayed.
*
*               p_str  Pointer to the string.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LCD_TextDispStr  (BSP_LCD_TEXT_POS   col,
                            BSP_LCD_TEXT_POS   line,
                            CPU_CHAR          *p_str)
{


    BSP_LCD_TextDispStr_N((BSP_LCD_TEXT_POS   )col,
                          (BSP_LCD_TEXT_POS   )line,
                          (CPU_CHAR          *)p_str,
                          (CPU_SIZE_T         )DEF_INT_CPU_U_MAX_VAL);
}


/*
*********************************************************************************************************
*                                          BSP_LCD_TextDispStr_N()
*
* Description : Display a string up to a maximum number of characters.
*
* Argument(s) : col       Column where the text will be displayed.
*
*               line      Line   where the text will be displayed.
*
*               p_str     Pointer to the string.
*
*               len_max   Maximum number of characters to display.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LCD_TextDispStr_N (BSP_LCD_TEXT_POS   col,
                             BSP_LCD_TEXT_POS   line,
                             CPU_CHAR          *p_str,
                             CPU_SIZE_T         len_max)
{
    BSP_LCD_DEV   *p_dev;
    BSP_LCD_FONT  *p_font;
    BSP_LCD_PIXEL  pixel_cur;
    CPU_SIZE_T     str_len;


    p_dev = &BSP_LCD_Ctxt;                                      /* Get a reference to LCD device.                       */

    if ((line > p_dev->TextLineMax) ||                          /* Check the text coordinates boundaries.               */
        (col  > p_dev->TextColMax)) {
        return;
    }

    p_font = (BSP_LCD_FONT *)p_dev->TextFontPtr;                /* Get a reference to the current font.                 */

    if (p_font == (BSP_LCD_FONT *)0) {
        return;
    }

    str_len = 0u;
    BSP_LCD_Lock();                                             /* Acquire LCD lock.                                    */

                                                                /* Update the text context...                           */
    p_dev->TextColCur   = col;                                  /* ... update current column.                           */
    p_dev->TextLineCur  = line;                                 /* ... update current line.                             */
                                                                /* ... update current pixel.                            */
    pixel_cur           = (BSP_LCD_PIXEL)((BSP_LCD_WIDTH * col)
                        / p_dev->TextColMax);
    p_dev->TextPixelCur = pixel_cur;

    while ((*p_str != (CPU_CHAR)0) &&
           (str_len < len_max    )) {

        switch (*p_str) {
            case ASCII_CHAR_CARRIAGE_RETURN:
                 p_dev->TextColCur   = 0u;
                 p_dev->TextPixelCur = pixel_cur;
                 break;

            case ASCII_CHAR_LINE_FEED:
                 if (p_dev->TextLineCur + 1u > p_dev->TextLineMax) {
                     p_dev->TextLineCur = 0u;
                 } else {
                     p_dev->TextLineCur++;
                 }
                 break;

            default:
                 BSP_LCD_TextCharUnlock(*p_str);
                 if ((p_dev->TextPixelCur + p_font->Width) > BSP_LCD_WIDTH) {
                     p_dev->TextColCur   = 0u;
                     p_dev->TextPixelCur = pixel_cur;
                     if (p_dev->TextLineCur + 1u > p_dev->TextLineMax) {
                         p_dev->TextLineCur = 0u;
                     } else {
                         p_dev->TextLineCur++;
                     }
                 } else {
                     p_dev->TextColCur++;
                 }
                 break;
        }
        p_str++;
        str_len++;
    }

    BSP_LCD_Unlock();                                           /* Release LCD lock.                                    */
}

/*
*********************************************************************************************************
*                                          BSP_LCD_TextDispStr()
*
* Description : Display a character.
*
* Argument(s) : col    Column where the text will be displayed.
*
*               line   Line   where the text will be displayed.
*
*               c      Character to display.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LCD_TextDispChar (BSP_LCD_TEXT_POS   col,
                            BSP_LCD_TEXT_POS   line,
                            CPU_CHAR           c)
{
    BSP_LCD_DEV   *p_dev;
    BSP_LCD_FONT  *p_font;


    p_dev = &BSP_LCD_Ctxt;                                      /* Get a reference to LCD device.                       */

    if ((line > p_dev->TextLineMax) ||                          /* Check the text coordinates boundaries.               */
        (col  > p_dev->TextColMax)) {
        return;
    }

    p_font = (BSP_LCD_FONT *)p_dev->TextFontPtr;                /* Get a reference to the current font.                 */

    if (p_font == (BSP_LCD_FONT *)0) {
        return;
    }

    BSP_LCD_Lock();                                             /* Acquire LCD lock.                                    */

                                                                /* Update the text context...                           */
    p_dev->TextColCur   = col;                                  /* ... update current column.                           */
    p_dev->TextLineCur  = line;                                 /* ... update current line.                             */
                                                                /* ... update current pixel.                            */
    switch (c) {
        case ASCII_CHAR_CARRIAGE_RETURN:
             p_dev->TextColCur = 0u;
             break;

        case ASCII_CHAR_LINE_FEED:
             if (p_dev->TextLineCur + 1u > p_dev->TextLineMax) {
                 p_dev->TextLineCur = 0u;
             } else {
                 p_dev->TextLineCur++;
             }
             break;

        default:
             BSP_LCD_TextCharUnlock(c);
             if ((p_dev->TextPixelCur + p_font->Width) > BSP_LCD_WIDTH) {
                 p_dev->TextColCur = 0u;
                 if (p_dev->TextLineCur + 1u > p_dev->TextLineMax) {
                     p_dev->TextLineCur = 0u;
                 } else {
                     p_dev->TextLineCur++;
                 }
             } else {
                 p_dev->TextColCur++;
             }
             break;
    }

    BSP_LCD_Unlock();                                           /* Release LCD lock.                                    */
}


/*
*********************************************************************************************************
*                                         BSP_LCD_TextFontTblSet()
*
* Description : Set the font table address.
*
* Argument(s) : p_tbl   Pointer to an array of 'BSP_LCD_FONT' pointers.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LCD_TextFontTblSet (BSP_LCD_FONT  **p_font_tbl)
{

    if (p_font_tbl == (BSP_LCD_FONT **)0) {
        return;
    }

    BSP_LCD_Lock();                                             /* Acquire LCD lock.                                    */

    BSP_LCD_Ctxt.FontTblPtr = p_font_tbl;

    BSP_LCD_Unlock();                                           /* Release LCD lock.                                    */
}


/*
*********************************************************************************************************
*                                       BSP_LCD_TextFontGetByName()
*
* Description : Return a font from the font table using the font name.
*
* Argument(s) : p_name   Pointer to the font name.
*
* Return(s)   : Pointer to the required font. If the font cannot be found in the font table a 'NULL'
*               pointer is returned.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

BSP_LCD_FONT  *BSP_LCD_TextFontGetByName (CPU_CHAR  *p_name)
{
    BSP_LCD_FONT  **p_font_tbl;
    BSP_LCD_FONT   *p_font;
    CPU_INT16S      cmp;
    CPU_SIZE_T      font_ix;
    CPU_BOOLEAN     found;

                                                                /* ------------------- ARGUMENTS CHECKING ------------- */
    if (p_name == (CPU_CHAR *)0) {                              /* NULL pointer ?                                       */
        return ((BSP_LCD_FONT *)0);
    }

    p_font_tbl = BSP_LCD_Ctxt.FontTblPtr;

    if (p_font_tbl == (BSP_LCD_FONT **)0) {                     /* If a font table has not been set.                    */
        return ((BSP_LCD_FONT *)0);
    }

    found   = DEF_NO;
    font_ix = 0u;
    p_font  = (BSP_LCD_FONT *)0;
                                                                /* Search the font in the font table by name.           */
    while ((p_font_tbl[font_ix] !=  (BSP_LCD_FONT       *)0    ) &&
           (found               ==  DEF_NO               ) &&
           (font_ix              <  BSP_LCD_MAX_NBR_FONT )) {

        p_font = p_font_tbl[font_ix];
        cmp    = Str_Cmp(p_name,                                /* Compare the font name.                               */
                         p_font->NamePtr);

        if (cmp == (CPU_INT16S)0) {                             /* If the font is found ...                             */
            found = DEF_YES;                                    /* ... exit the search routine.                         */
        } else {
            font_ix++;                                          /* ... move to the next entry, otherwise.               */
        }
    }

    if (found == DEF_NO) {                                      /* If a fount could not been found in the font table    */
        return ((BSP_LCD_FONT *)0);                             /* ... return a 'NULL' pointer.                         */
    }

    return ((p_font));
}


/*
*********************************************************************************************************
*                                            BSP_LCD_TextFontSet()
*
* Description : Set a font.
*
* Argument(s) : p_font   Pointer to the font structure.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/


void   BSP_LCD_TextFontSet (BSP_LCD_FONT  *p_font)
{
    BSP_LCD_DEV  *p_dev;
    CPU_INT08U    width;
    CPU_INT08U    height;
    CPU_INT08U    line_max;
    CPU_INT08U    col_max;
    CPU_INT08U    pos_cur;


    if (p_font == (void *)0) {
        return;
    }

    width     =  p_font->Width;
    height    =  p_font->Height;
    line_max  = (BSP_LCD_HEIGHT / height);
    col_max   = (BSP_LCD_WIDTH  / width);
    p_dev     = &BSP_LCD_Ctxt;

    BSP_LCD_Lock();                                             /* Acquire LCD lock.                                    */

    p_dev->TextFontPtr = p_font;
    pos_cur            = p_dev->TextLineCur;
    p_dev->TextLineCur = (pos_cur * p_dev->TextLineMax) / (line_max);
    pos_cur            = p_dev->TextColCur;
    p_dev->TextColCur  = (pos_cur * p_dev->TextColMax ) / (col_max);
    p_dev->TextLineMax = line_max;
    p_dev->TextColMax  = col_max;

    BSP_LCD_Unlock();                                           /* Release LCD lock.                                    */
}




/*
*********************************************************************************************************
*                                            BSP_LCD_BarGraph()
*
* Description : Display a bargraph.
*
* Argument(s) : x            X axis position in pixels.
*
*               y            Y axis position in pixels.
*
*               width        Bargraph width.
*
*               height       Bargraph height.
*
*               bg_color     Background color.
*
*               level_color  Level color.
*
*               level_pct    Level porcentage value (0 - 100%)
*
*               p_str        Pointer to the string array.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) The following diagram explain the relationship the arguments and the bargraph.
*
*                   |------- level % -------- |
*                   | --------------- width --------------- |
*                   +=======================================+    ---
*                   |/////////////////////////\\\\\\\\\\\\\\|     |
*                   |/////////////////////////\\\\\\\\\\\\\\|     |
*                   |/////////////////////////\\\\\\\\\\\\\\|   height
*                   |/////////////////////////\\\\\\\\\\\\\\|     |
*                   |/////////////////////////\\\\\\\\\\\\\\|     |
*                   |///////|/////////////////\\\\\\\\|\\\\\|     |
*                   +=======|=================|=======|=====+    ---
*                           |                         |
*                           |                         +----------->  Background color
*                           +-------> Level color
*********************************************************************************************************
*/


void  BSP_LCD_BarGraphDraw  (BSP_LCD_PIXEL  x,
                             BSP_LCD_PIXEL  y,
                             BSP_LCD_PIXEL  width,
                             BSP_LCD_PIXEL  height,
                             BSP_LCD_COLOR  bg_color,
                             BSP_LCD_COLOR  level_color,
                             CPU_INT08U     level_pct)
{
    BSP_LCD_COLOR  color;
    BSP_LCD_PIXEL  pixel_x;
    BSP_LCD_PIXEL  pixel_y;
    BSP_LCD_PIXEL  pixel_pct;

    if (x + width  >  BSP_LCD_WIDTH) {
        return;
    }

    if (y + height >  BSP_LCD_HEIGHT) {
        return;
    }

    pixel_pct = (width * level_pct / 100u);

    BSP_LCD_Lock();                                             /* Acquire LCD lock.                                    */


    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_ADDR_START, x);
    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_ADDR_END,   x + width - 1u);

    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_ADDR_START, y);
    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_ADDR_END,   y + height - 1u);

    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_GRAM_ADDR_SET, x);
    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_GRAM_ADDR_SET, y);

    BSP_LCD_CTRL_CS_LOW();

    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_SET_IX);
    BSP_LCD_SPI_Xfer(0u);
    BSP_LCD_SPI_Xfer(ILI9320_REG_GRAM_DATA_WR);

    BSP_LCD_CTRL_CS_HIGH();

    BSP_LCD_CTRL_CS_LOW();
    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_WR_REG);


    for (pixel_y = 0; pixel_y <  height; pixel_y++) {
        for (pixel_x = 0; pixel_x < width; pixel_x++) {
            if (pixel_x < pixel_pct) {
                color = bg_color;
            } else {
                color = level_color;
            }

            BSP_LCD_SPI_Xfer(color >> 8u);
            BSP_LCD_SPI_Xfer(color & DEF_INT_08_MASK);
        }
    }
    BSP_LCD_CTRL_CS_HIGH();

    BSP_LCD_Unlock();                                           /* Release LCD lock.                                    */
}


/*
*********************************************************************************************************
*                                            BSP_LCD_BitmapDraw()
*
* Description : Display a bargraph.
*
* Argument(s) : x         X axis position in pixels.
*
*               y         Y axis position in pixels.
*
*               width     Bitmap width.
*
*               heigh     Bitmap heigh.
*
*               p_bitmap  Pointer to the color bitmap.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/


void  BSP_LCD_BitmapDraw (CPU_INT16U   x,
                          CPU_INT16U   y,
                          CPU_INT16U   width,
                          CPU_INT16U   height,
                          CPU_INT16U  *p_bitmap)
{
                                                                /* Check the bitmap boundaries                          */

    BSP_LCD_Lock();                                             /* Acquire LCD lock.                                    */

    BSP_LCD_BitmapDrawUnlock(x,
                             y,
                             width,
                             height,
                             p_bitmap);
    BSP_LCD_Unlock();                                           /* Release LCD lock.                                    */

}

/*
*********************************************************************************************************
*                                            BSP_LCD_RectDraw()
*
* Description : Draw an unfilled rectangule.
*
* Argument(s) : x            X axis position in pixels.
*
*               y            Y axis position in pixels.
*
*               width        Rectangle width in pixels.
*
*               heigh        Rectangle width in pixels.
*
*               line_weight  Line weight.
*
*               line_color   Line color.
*
*               p_bitmap     Pointer to the color bitmap.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LCD_RectDraw  (BSP_LCD_PIXEL  x,
                         BSP_LCD_PIXEL  y,
                         BSP_LCD_PIXEL  width,
                         BSP_LCD_PIXEL  height,
                         BSP_LCD_PIXEL  line_weight,
                         BSP_LCD_COLOR  line_color)
{
    BSP_LCD_Lock();                                             /* Acquire LCD lock.                                    */

    BSP_LCD_RectDrawUnlock(x,
                           y,
                           width,
                           height,
                           DEF_NO,
                           0u,
                           line_weight,
                           line_color);

    BSP_LCD_Unlock();                                           /* Release LCD lock.                                    */
}


/*
*********************************************************************************************************
*                                         BSP_LCD_BitmapDrawUnlock()
*
* Description : Draw a filled rectangle.
*
* Argument(s) : x             X axis position in pixels.
*
*               y             Y axis position in pixels.
*
*               width         Rectangle width in pixels.
*
*               heigh         Rectangle width in pixels height.
*
*               fill_color    Rectangle fill color.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LCD_RectDrawFill (BSP_LCD_PIXEL  x,
                            BSP_LCD_PIXEL  y,
                            BSP_LCD_PIXEL  width,
                            BSP_LCD_PIXEL  height,
                            BSP_LCD_PIXEL  color)
{
    BSP_LCD_Lock();                                             /* Acquire LCD lock.                                    */

    BSP_LCD_RectDrawUnlock(x,
                           y,
                           width,
                           height,
                           DEF_YES,
                           color,
                           0u,
                           0u);

    BSP_LCD_Unlock();                                           /* Release LCD lock.                                    */

}


/*
*********************************************************************************************************
*                                   BSP_LCD_RectDrawFillLine()
*
* Description : Draw a filled rectangle with a border.
*
* Argument(s) : x             X axis position in pixels.
*
*               y             Y axis position in pixels.
*
*               width         Rectangle width in pixels.
*
*               heigh         Rectangle width in pixels height.
*
*               fill_color    Rectangle fill color.
*
*               line_weight   Line weight.
*
*               line_color    Line color.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/


void  BSP_LCD_RectDrawFillLine (BSP_LCD_PIXEL  x,
                                BSP_LCD_PIXEL  y,
                                BSP_LCD_PIXEL  width,
                                BSP_LCD_PIXEL  height,
                                BSP_LCD_PIXEL  fill_color,
                                BSP_LCD_PIXEL  line_weight,
                                BSP_LCD_COLOR  line_color)
{
    BSP_LCD_Lock();                                             /* Acquire LCD lock.                                    */

    BSP_LCD_RectDrawUnlock(x,
                           y,
                           width,
                           height,
                           DEF_YES,
                           fill_color,
                           line_weight,
                           line_color);

    BSP_LCD_Unlock();                                           /* Release LCD lock.                                    */
}



/*
*********************************************************************************************************
*                                   BSP_LCD_Pixel()
*
* Description : Draw a pixel.
*
* Argument(s) : x             X axis position in pixels.
*
*               y             Y axis position in pixels.
*
*               color         Pixel color.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/


void  BSP_LCD_Pixel (BSP_LCD_PIXEL  x,
                     BSP_LCD_PIXEL  y,
                     BSP_LCD_COLOR  color)
{
    BSP_LCD_Lock();                                             /* Acquire LCD lock.                                    */

    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_ADDR_START, x);
    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_ADDR_END,   x + 1u);

    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_ADDR_START, y);
    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_ADDR_END,   y + 1u);

    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_GRAM_ADDR_SET, x);
    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_GRAM_ADDR_SET, y);

    BSP_LCD_CTRL_CS_LOW();

    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_SET_IX);
    BSP_LCD_SPI_Xfer(0u);
    BSP_LCD_SPI_Xfer(ILI9320_REG_GRAM_DATA_WR);

    BSP_LCD_CTRL_CS_HIGH();

    BSP_LCD_CTRL_CS_LOW();
    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_WR_REG);

    BSP_LCD_SPI_Xfer(color >> 8u);
    BSP_LCD_SPI_Xfer(color & DEF_INT_08_MASK);

    BSP_LCD_CTRL_CS_HIGH();

    BSP_LCD_Unlock();                                           /* Release LCD lock.                                    */
}


/*
*********************************************************************************************************
*                                               BACKLIGHT FUNCTIONS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             BSP_LCD_BackLightOff()
*
* Description : Turn the LCD backlight Off.
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

void  BSP_LCD_BackLightOff (void)
{
    CSP_GPIO_BitClr(CSP_GPIO_PORT_NBR_04,
                    DEF_BIT_28);
}


/*
*********************************************************************************************************
*                                             BSP_LCD_BackLightOn()
*
* Description : Turn the LCD backlight On.
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

void  BSP_LCD_BackLightOn  (void)
{
    CSP_GPIO_BitSet(CSP_GPIO_PORT_NBR_04,
                    DEF_BIT_28);
}


/*
*********************************************************************************************************
*                                             BSP_LCD_BackLightToggle()
*
* Description : Toggle the LCD backlight..
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

void  BSP_LCD_BackLightToggle (void)
{
    CSP_GPIO_BitToggle(CSP_GPIO_PORT_NBR_04,
                       DEF_BIT_28);

}


/*
*********************************************************************************************************
*                                          LCD DRIVER INTERNAL FUNCTIONS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       BSP_LCD_TextCharUnlock()
*
* Description : Disaply a character without acquiring the LCD lock.
*
* Argument(s) : c   Character to display.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_LCD_TextDispChar()
*               BSP_LCD_TextDispStr()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_LCD_TextCharUnlock (CPU_CHAR  c)

{
    BSP_LCD_DEV        *p_dev;
    BSP_LCD_FONT       *p_font;
    BSP_LCD_FONT_CHAR  *p_font_char;
    CPU_INT08U         *p_font_char_bitmap;
    BSP_LCD_TEXT_POS    pos;
    BSP_LCD_PIXEL       char_width;
    BSP_LCD_PIXEL       char_height;
    CPU_INT08U          byte_ix;
    CPU_INT08U          bit_ix;
    CPU_INT08U          bit_map;
    BSP_LCD_PIXEL       pixel_x;
    BSP_LCD_PIXEL       pixel_y;
    BSP_LCD_COLOR       color;


    p_dev  = &BSP_LCD_Ctxt;                                     /* Obtain a reference to the LCD text context.          */
    p_font = p_dev->TextFontPtr;                                /* Obtain a reference to the LCD text font.             */

    if (p_font == (BSP_LCD_FONT *)0) {
        return;
    }

    if ((c < p_font->CharStart) ||                              /* Check if the Font contains the character.            */
        (c > p_font->CharEnd  )) {
        return;
    }

    c                 -= p_font->CharStart;                     /* Get the character index.                             */
                                                                /* Get the character info.                              */
    p_font_char        = (BSP_LCD_FONT_CHAR *)&p_font->CharInfoTbl[(int)c];
    p_font_char_bitmap = (CPU_INT08U        *)p_font_char->BitMapPtr;

    char_width  = p_font_char->Width;
    char_height = p_font->Height;
    pixel_x     = p_dev->TextPixelCur;
    pos         = p_dev->TextLineCur;
    pixel_y     = (BSP_LCD_PIXEL)((BSP_LCD_HEIGHT  * pos)
                / p_dev->TextLineMax);

                                                                /* Set the GRAM window                                  */
    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_ADDR_START, pixel_x);
    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_ADDR_END,   pixel_x + char_width - 1u);

    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_ADDR_START, pixel_y);
    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_ADDR_END,   pixel_y + char_height - 1u);

    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_GRAM_ADDR_SET, pixel_x);
    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_GRAM_ADDR_SET, pixel_y);


    BSP_LCD_CTRL_CS_LOW();                                      /* Start writing data to the GRAM memory                */

    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_SET_IX);
    BSP_LCD_SPI_Xfer(0u);
    BSP_LCD_SPI_Xfer(ILI9320_REG_GRAM_DATA_WR);

    BSP_LCD_CTRL_CS_HIGH();

    BSP_LCD_CTRL_CS_LOW();
    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_WR_REG);

    for (pixel_y = 0u; pixel_y < char_height; pixel_y++) {
       for (byte_ix = 0u ; byte_ix < p_font_char->NbrBytes; byte_ix++) {
            bit_ix   = 0u;
                                                                /* Get the character bitmap.                            */
            bit_map  = p_font_char_bitmap[pixel_y * p_font_char->NbrBytes + byte_ix];

            while ((bit_ix                   < 8u) &&
                   (byte_ix * 8u ) + bit_ix  < char_width) {
                if (DEF_BIT_IS_CLR(bit_map, DEF_BIT(7u - bit_ix))) {
                    color = p_dev->BgColor;
                } else {
                    color = p_dev->TextColor;
                }
                BSP_LCD_SPI_Xfer(color >> 8u);
                BSP_LCD_SPI_Xfer(color & DEF_INT_08_MASK);
                bit_ix++;
            }
        }
    }
    BSP_LCD_CTRL_CS_HIGH();
    p_dev->TextPixelCur += p_font_char->Dist;
}

/*
*********************************************************************************************************
*                                         BSP_LCD_BitmapDrawUnlock()
*
* Description : Draw a bitmap without acquiring the lock.
*
* Argument(s) : x         X axis position in pixels.
*
*               y         Y axis position in pixels.
*
*               width     Bitmap width.
*
*               heigh     Bitmap heigh.
*
*               p_bitmap  Pointer to the color bitmap.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_LCD_BitmapDrawUnlock (BSP_LCD_PIXEL  x,
                                        BSP_LCD_PIXEL  y,
                                        BSP_LCD_PIXEL  width,
                                        BSP_LCD_PIXEL  height,
                                        CPU_INT16U    *p_bitmap)
{
    BSP_LCD_COLOR  color;
    BSP_LCD_PIXEL  pixel;


    if (x + width  >  BSP_LCD_WIDTH) {
        return;
    }

    if (y + height >  BSP_LCD_HEIGHT) {
        return;
    }

    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_ADDR_START, x);
    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_ADDR_END,   x + width - 1u);

    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_ADDR_START, y);
    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_ADDR_END,   y + height - 1u);

    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_GRAM_ADDR_SET, x);
    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_GRAM_ADDR_SET, y);

    BSP_LCD_CTRL_CS_LOW();

    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_SET_IX);
    BSP_LCD_SPI_Xfer(0u);
    BSP_LCD_SPI_Xfer(ILI9320_REG_GRAM_DATA_WR);

    BSP_LCD_CTRL_CS_HIGH();

    BSP_LCD_CTRL_CS_LOW();
    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_WR_REG);

    for (pixel = 0; pixel < width * height; pixel++) {
        color = p_bitmap[pixel];
        BSP_LCD_SPI_Xfer(color >> 8u);
        BSP_LCD_SPI_Xfer(color & DEF_INT_08_MASK);
    }

    BSP_LCD_CTRL_CS_HIGH();

}

/*
*********************************************************************************************************
*                                         BSP_LCD_BitmapDrawUnlock()
*
* Description : Draw a rectangle without acquiring the lock.
*
* Argument(s) : x             X axis position in pixels.
*
*               y             Y axis position in pixels.
*
*               width         Rectangle width in pixels.
*
*               heigh         Rectangle width in pixels height.
*
*               fill_color    Rectangle fill color.
*
*               line_weight   Rectangle line weight in pixels.
*
*               line_color    Rectangle line color.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static   void  BSP_LCD_RectDrawUnlock  (BSP_LCD_PIXEL  x,
                                        BSP_LCD_PIXEL  y,
                                        BSP_LCD_PIXEL  width,
                                        BSP_LCD_PIXEL  height,
                                        CPU_BOOLEAN    fill,
                                        BSP_LCD_COLOR  fill_color,
                                        BSP_LCD_PIXEL  line_weight,
                                        BSP_LCD_COLOR  line_color)
{
    BSP_LCD_PIXEL  pixel_x;
    BSP_LCD_PIXEL  pixel_y;
    BSP_LCD_COLOR  color;

    if (x + width  > BSP_LCD_WIDTH) {
        return;
    }

    if (y + height > BSP_LCD_HEIGHT) {
        return;
    }

    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_ADDR_START, x);
    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_ADDR_END,   x + width - 1u);

    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_ADDR_START, y);
    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_ADDR_END,   y + height - 1u);

    BSP_LCD_CtrlRegWr(ILI9320_REG_HOR_GRAM_ADDR_SET, x);
    BSP_LCD_CtrlRegWr(ILI9320_REG_VER_GRAM_ADDR_SET, y);

    BSP_LCD_CTRL_CS_LOW();

    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_SET_IX);
    BSP_LCD_SPI_Xfer(0u);
    BSP_LCD_SPI_Xfer(ILI9320_REG_GRAM_DATA_WR);

    BSP_LCD_CTRL_CS_HIGH();

    BSP_LCD_CTRL_CS_LOW();
    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_WR_REG);


    for (pixel_y = 0; pixel_y < height; pixel_y++) {
        for (pixel_x = 0; pixel_x < width; pixel_x++) {
            if ((pixel_x  < line_weight         ) ||
                (pixel_x >= width  - line_weight) ||
                (pixel_y  < line_weight         ) ||
                (pixel_y >= height - line_weight)) {
                color = line_color;
            } else if (fill == DEF_YES) {
                color = fill_color;
            } else {
                color = BSP_LCD_Ctxt.BgColor;
            }
            BSP_LCD_SPI_Xfer(color >> 8u);
            BSP_LCD_SPI_Xfer(color & DEF_INT_08_MASK);
        }
    }

    BSP_LCD_CTRL_CS_HIGH();
}



/*
*********************************************************************************************************
*                                             BSP_LCD_CtrlRegRd()
*
* Description : Read a register form the LCD controller.
*
* Argument(s) : reg_off     Register offset.
*
* Return(s)   : Register value (16-bits).
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/


static  CPU_INT16U   BSP_LCD_CtrlRegRd (CPU_INT08U  reg_off)
{
    CPU_INT16U  reg_val;


    BSP_LCD_CTRL_CS_LOW();

    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_SET_IX);
    BSP_LCD_SPI_Xfer(0u);
    BSP_LCD_SPI_Xfer(reg_off);

    BSP_LCD_CTRL_CS_HIGH();

    BSP_LCD_CTRL_CS_LOW();

    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_RD_REG);
    BSP_LCD_SPI_Xfer(0u);
    reg_val  = BSP_LCD_SPI_Xfer(0u);
    reg_val  = (reg_val << 8u);
    reg_val |= BSP_LCD_SPI_Xfer(0u);

    BSP_LCD_CTRL_CS_HIGH();

    return (reg_val);
}

/*
*********************************************************************************************************
*                                             BSP_LCD_CtrlRegRd()
*
* Description : Write a value to one the register of the  LCD controller.
*
* Argument(s) : reg_off     Register offset.
*
*               reg_val     Register value.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_LCD_CtrlRegWr (CPU_INT08U  reg_off,
                                 CPU_INT16U  reg_val)
{
    BSP_LCD_CTRL_CS_LOW();

    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_SET_IX);
    BSP_LCD_SPI_Xfer(0u);
    BSP_LCD_SPI_Xfer(reg_off);

    BSP_LCD_CTRL_CS_HIGH();

    BSP_LCD_CTRL_CS_LOW();

    BSP_LCD_SPI_Xfer(BSP_LCD_SPI_ILI9320_START_WR_REG);
    BSP_LCD_SPI_Xfer(reg_val >> 8u);
    BSP_LCD_SPI_Xfer(reg_val & DEF_INT_08_MASK);

    BSP_LCD_CTRL_CS_HIGH();
}


/*
*********************************************************************************************************
*                                     BSP_LCD_SPI_Xfer()
*
* Description : Perform a SPI transfer.
*
* Argument(s) : datum:   Byte to be sent.
*
* Return(s)   : Byte received.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_INT08U  BSP_LCD_SPI_Xfer (CPU_INT08U  datum)
{
    CPU_TS32    ts_start;
    CPU_TS32    ts_elapsed;
    CPU_TS32    ts_elapsed_max;
    CPU_INT32U  reg_val;


    BSP_LCD_SSP1_REG_DR = (CPU_INT32U)datum;                    /* Write data into the data register.                   */
    ts_elapsed_max      = BSP_LCD_Ctxt.MaxTimeOut;
    ts_start            = CPU_TS_Get32();
    ts_elapsed          = CPU_TS_Get32() - ts_start;            /* Calculated the elapsed time.                         */

                                                                /* While the receive register is not empty or a ....    */
                                                                /* ... timeout has not occurred?                        */
    while ((DEF_BIT_IS_CLR(BSP_LCD_SSP1_REG_SR, BSP_LCD_SSP1_BIT_SR_RNE)) &&
           (ts_elapsed < ts_elapsed_max)) {
        ts_elapsed = CPU_TS_Get32() - ts_start;                 /* Calculate the elapsed time.                          */
    }

    if (DEF_BIT_IS_CLR(BSP_LCD_SSP1_REG_SR, BSP_LCD_SSP1_BIT_SR_RNE)) {
        return ((CPU_INT08U)0);
    }

    reg_val = (BSP_LCD_SSP1_REG_DR & DEF_INT_08_MASK);

    return ((CPU_INT08U)reg_val);
}


/*
*********************************************************************************************************
*                                        BSP_LCD_Lock()
*
* Description : Acquire the LCD Lock.
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

static  void  BSP_LCD_Lock (void)
{
    OS_ERR  err;


    OSMutexPend((OS_MUTEX  *)&BSP_LCD_Ctxt.Lock,
                (OS_TICK    )0,
                (OS_OPT     )OS_OPT_PEND_BLOCKING,
                (CPU_TS    *)0,
                (OS_ERR    *)&err);

    (void)&err;
}

/*
*********************************************************************************************************
*                                       BSP_LCD_Unlock()
*
* Description : Release the LCD Lock.
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

static  void  BSP_LCD_Unlock()
{
    OS_ERR  err;


    OSMutexPost((OS_MUTEX  *)&BSP_LCD_Ctxt.Lock,
                (OS_OPT     )OS_OPT_POST_NONE,
                (OS_ERR    *)&err);
}
