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
*                                           IMAGES TABLE
*
* Filename      :img_keil_logo.c
* Version       :V1.00
* Programmer(s) :FT
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

/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

extern  const  BSP_LCD_IMG  ImgMicriumLogo;
extern  const  BSP_LCD_IMG  ImgKeilLogo;
extern  const  BSP_LCD_IMG  ImgNXP_Logo;
extern  const  BSP_LCD_IMG  ImgOS_Logo;

const CPU_ADDR BSP_LCD_ImgTbl[] = {
     (CPU_ADDR)&ImgMicriumLogo,
     (CPU_ADDR)&ImgKeilLogo,
     (CPU_ADDR)&ImgNXP_Logo,
     (CPU_ADDR)&ImgOS_Logo,
     (CPU_ADDR)0,
};
