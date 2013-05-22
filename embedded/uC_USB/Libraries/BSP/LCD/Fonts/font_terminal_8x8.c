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
*                                             LCD FONT
*                                           TERMINAL 8x8
*
* Filename      :font_terminal_8x8.c
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
*                                                DEFINES
*********************************************************************************************************
*/




/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

const  CPU_INT08U  FontTerminal8x8_0020[8] = { /* code 0020 */
   ________,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________};  /* char ' '  */

const  CPU_INT08U  FontTerminal8x8_0021[8] = { /* code 0021 */
   ___XX___,
   __XXXX__,
   __XXXX__,
   ___XX___,
   ___XX___,
   ________,
   ___XX___,
   ________};  /* char '!'  */

const  CPU_INT08U  FontTerminal8x8_0022[8] = { /* code 0022 */
   _XX__XX_,
   _XX__XX_,
   __X__X__,
   ________,
   ________,
   ________,
   ________,
   ________};  /* char '"'  */

const  CPU_INT08U  FontTerminal8x8_0023[8] = { /* code 0023 */
   _XX_XX__,
   _XX_XX__,
   XXXXXXX_,
   _XX_XX__,
   XXXXXXX_,
   _XX_XX__,
   _XX_XX__,
   ________};  /* char '#'  */

const  CPU_INT08U  FontTerminal8x8_0024[8] = { /* code 0024 */
   ___XX___,
   __XXXXX_,
   _XX_____,
   __XXXX__,
   _____XX_,
   _XXXXX__,
   ___XX___,
   ________};  /* char '$'  */

const  CPU_INT08U  FontTerminal8x8_0025[8] = { /* code 0025 */
   ________,
   XX___XX_,
   XX__XX__,
   ___XX___,
   __XX____,
   _XX__XX_,
   XX___XX_,
   ________};  /* char '%'  */

const  CPU_INT08U  FontTerminal8x8_0026[8] = { /* code 0026 */
   __XXX___,
   _XX_XX__,
   __XXX___,
   _XXX_XX_,
   XX_XXX__,
   XX__XX__,
   _XXX_XX_,
   ________};  /* char '&'  */

const  CPU_INT08U  FontTerminal8x8_0027[8] = { /* code 0027 */
   ___XX___,
   ___XX___,
   __XX____,
   ________,
   ________,
   ________,
   ________,
   ________};  /* char '''  */

const  CPU_INT08U  FontTerminal8x8_0028[8] = { /* code 0028 */
   ____XX__,
   ___XX___,
   __XX____,
   __XX____,
   __XX____,
   ___XX___,
   ____XX__,
   ________};  /* char '('  */

const  CPU_INT08U  FontTerminal8x8_0029[8] = { /* code 0029 */
   __XX____,
   ___XX___,
   ____XX__,
   ____XX__,
   ____XX__,
   ___XX___,
   __XX____,
   ________};  /* char ')'  */

const  CPU_INT08U  FontTerminal8x8_002A[8] = { /* code 002A */
   ________,
   _XX__XX_,
   __XXXX__,
   XXXXXXXX,
   __XXXX__,
   _XX__XX_,
   ________,
   ________};  /* char '*'  */

const  CPU_INT08U  FontTerminal8x8_002B[8] = { /* code 002B */
   ________,
   ___XX___,
   ___XX___,
   _XXXXXX_,
   ___XX___,
   ___XX___,
   ________,
   ________};  /* char '+'  */

const  CPU_INT08U  FontTerminal8x8_002C[8] = { /* code 002C */
   ________,
   ________,
   ________,
   ________,
   ________,
   ___XX___,
   ___XX___,
   __XX____};  /* char ','  */

const  CPU_INT08U  FontTerminal8x8_002D[8] = { /* code 002D */
   ________,
   ________,
   ________,
   _XXXXXX_,
   ________,
   ________,
   ________,
   ________};  /* char '-'  */

const  CPU_INT08U  FontTerminal8x8_002E[8] = { /* code 002E */
   ________,
   ________,
   ________,
   ________,
   ________,
   ___XX___,
   ___XX___,
   ________};  /* char '.'  */

const  CPU_INT08U  FontTerminal8x8_002F[8] = { /* code 002F */
   _____XX_,
   ____XX__,
   ___XX___,
   __XX____,
   _XX_____,
   XX______,
   X_______,
   ________};  /* char '/'  */

const  CPU_INT08U  FontTerminal8x8_0030[8] = { /* code 0030 */
   __XXX___,
   _XX_XX__,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   _XX_XX__,
   __XXX___,
   ________};  /* char '0'  */

const  CPU_INT08U  FontTerminal8x8_0031[8] = { /* code 0031 */
   ___XX___,
   __XXX___,
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   _XXXXXX_,
   ________};  /* char '1'  */

const  CPU_INT08U  FontTerminal8x8_0032[8] = { /* code 0032 */
   _XXXXX__,
   XX___XX_,
   _____XX_,
   ___XXX__,
   __XX____,
   _XX__XX_,
   XXXXXXX_,
   ________};  /* char '2'  */

const  CPU_INT08U  FontTerminal8x8_0033[8] = { /* code 0033 */
   _XXXXX__,
   XX___XX_,
   _____XX_,
   __XXXX__,
   _____XX_,
   XX___XX_,
   _XXXXX__,
   ________};  /* char '3'  */

const  CPU_INT08U  FontTerminal8x8_0034[8] = { /* code 0034 */
   ___XXX__,
   __XXXX__,
   _XX_XX__,
   XX__XX__,
   XXXXXXX_,
   ____XX__,
   ___XXXX_,
   ________};  /* char '4'  */

const  CPU_INT08U  FontTerminal8x8_0035[8] = { /* code 0035 */
   XXXXXXX_,
   XX______,
   XX______,
   XXXXXX__,
   _____XX_,
   XX___XX_,
   _XXXXX__,
   ________};  /* char '5'  */

const  CPU_INT08U  FontTerminal8x8_0036[8] = { /* code 0036 */
   __XXX___,
   _XX_____,
   XX______,
   XXXXXX__,
   XX___XX_,
   XX___XX_,
   _XXXXX__,
   ________};  /* char '6'  */

const  CPU_INT08U  FontTerminal8x8_0037[8] = { /* code 0037 */
   XXXXXXX_,
   XX___XX_,
   ____XX__,
   ___XX___,
   __XX____,
   __XX____,
   __XX____,
   ________};  /* char '7'  */

const  CPU_INT08U  FontTerminal8x8_0038[8] = { /* code 0038 */
   _XXXXX__,
   XX___XX_,
   XX___XX_,
   _XXXXX__,
   XX___XX_,
   XX___XX_,
   _XXXXX__,
   ________};  /* char '8'  */

const  CPU_INT08U  FontTerminal8x8_0039[8] = { /* code 0039 */
   _XXXXX__,
   XX___XX_,
   XX___XX_,
   _XXXXXX_,
   _____XX_,
   ____XX__,
   _XXXX___,
   ________};  /* char '9'  */

const  CPU_INT08U  FontTerminal8x8_003A[8] = { /* code 003A */
   ________,
   ___XX___,
   ___XX___,
   ________,
   ________,
   ___XX___,
   ___XX___,
   ________};  /* char ':'  */

const  CPU_INT08U  FontTerminal8x8_003B[8] = { /* code 003B */
   ________,
   ___XX___,
   ___XX___,
   ________,
   ________,
   ___XX___,
   ___XX___,
   __XX____};  /* char ';'  */

const  CPU_INT08U  FontTerminal8x8_003C[8] = { /* code 003C */
   _____XX_,
   ____XX__,
   ___XX___,
   __XX____,
   ___XX___,
   ____XX__,
   _____XX_,
   ________};  /* char '<'  */

const  CPU_INT08U  FontTerminal8x8_003D[8] = { /* code 003D */
   ________,
   ________,
   _XXXXXX_,
   ________,
   ________,
   _XXXXXX_,
   ________,
   ________};  /* char '='  */

const  CPU_INT08U  FontTerminal8x8_003E[8] = { /* code 003E */
   _XX_____,
   __XX____,
   ___XX___,
   ____XX__,
   ___XX___,
   __XX____,
   _XX_____,
   ________};  /* char '>'  */

const  CPU_INT08U  FontTerminal8x8_003F[8] = { /* code 003F */
   _XXXXX__,
   XX___XX_,
   ____XX__,
   ___XX___,
   ___XX___,
   ________,
   ___XX___,
   ________};  /* char '?'  */

const  CPU_INT08U  FontTerminal8x8_0040[8] = { /* code 0040 */
   _XXXXX__,
   XX___XX_,
   XX_XXXX_,
   XX_XXXX_,
   XX_XXXX_,
   XX______,
   _XXXX___,
   ________};  /* char '@'  */

const  CPU_INT08U  FontTerminal8x8_0041[8] = { /* code 0041 */
   __XXX___,
   _XX_XX__,
   XX___XX_,
   XXXXXXX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   ________};  /* char 'A'  */

const  CPU_INT08U  FontTerminal8x8_0042[8] = { /* code 0042 */
   XXXXXX__,
   _XX__XX_,
   _XX__XX_,
   _XXXXX__,
   _XX__XX_,
   _XX__XX_,
   XXXXXX__,
   ________};  /* char 'B'  */

const  CPU_INT08U  FontTerminal8x8_0043[8] = { /* code 0043 */
   __XXXX__,
   _XX__XX_,
   XX______,
   XX______,
   XX______,
   _XX__XX_,
   __XXXX__,
   ________};  /* char 'C'  */

const  CPU_INT08U  FontTerminal8x8_0044[8] = { /* code 0044 */
   XXXXX___,
   _XX_XX__,
   _XX__XX_,
   _XX__XX_,
   _XX__XX_,
   _XX_XX__,
   XXXXX___,
   ________};  /* char 'D'  */

const  CPU_INT08U  FontTerminal8x8_0045[8] = { /* code 0045 */
   XXXXXXX_,
   _XX___X_,
   _XX_X___,
   _XXXX___,
   _XX_X___,
   _XX___X_,
   XXXXXXX_,
   ________};  /* char 'E'  */

const  CPU_INT08U  FontTerminal8x8_0046[8] = { /* code 0046 */
   XXXXXXX_,
   _XX___X_,
   _XX_X___,
   _XXXX___,
   _XX_X___,
   _XX_____,
   XXXX____,
   ________};  /* char 'F'  */

const  CPU_INT08U  FontTerminal8x8_0047[8] = { /* code 0047 */
   __XXXX__,
   _XX__XX_,
   XX______,
   XX______,
   XX__XXX_,
   _XX__XX_,
   __XXX_X_,
   ________};  /* char 'G'  */

const  CPU_INT08U  FontTerminal8x8_0048[8] = { /* code 0048 */
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XXXXXXX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   ________};  /* char 'H'  */

const  CPU_INT08U  FontTerminal8x8_0049[8] = { /* code 0049 */
   __XXXX__,
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   __XXXX__,
   ________};  /* char 'I'  */

const  CPU_INT08U  FontTerminal8x8_004A[8] = { /* code 004A */
   ___XXXX_,
   ____XX__,
   ____XX__,
   ____XX__,
   XX__XX__,
   XX__XX__,
   _XXXX___,
   ________};  /* char 'J'  */

const  CPU_INT08U  FontTerminal8x8_004B[8] = { /* code 004B */
   XXX__XX_,
   _XX__XX_,
   _XX_XX__,
   _XXXX___,
   _XX_XX__,
   _XX__XX_,
   XXX__XX_,
   ________};  /* char 'K'  */

const  CPU_INT08U  FontTerminal8x8_004C[8] = { /* code 004C */
   XXXX____,
   _XX_____,
   _XX_____,
   _XX_____,
   _XX___X_,
   _XX__XX_,
   XXXXXXX_,
   ________};  /* char 'L'  */

const  CPU_INT08U  FontTerminal8x8_004D[8] = { /* code 004D */
   XX___XX_,
   XXX_XXX_,
   XXXXXXX_,
   XXXXXXX_,
   XX_X_XX_,
   XX___XX_,
   XX___XX_,
   ________};  /* char 'M'  */

const  CPU_INT08U  FontTerminal8x8_004E[8] = { /* code 004E */
   XX___XX_,
   XXX__XX_,
   XXXX_XX_,
   XX_XXXX_,
   XX__XXX_,
   XX___XX_,
   XX___XX_,
   ________};  /* char 'N'  */

const  CPU_INT08U  FontTerminal8x8_004F[8] = { /* code 004F */
   _XXXXX__,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   _XXXXX__,
   ________};  /* char 'O'  */

const  CPU_INT08U  FontTerminal8x8_0050[8] = { /* code 0050 */
   XXXXXX__,
   _XX__XX_,
   _XX__XX_,
   _XXXXX__,
   _XX_____,
   _XX_____,
   XXXX____,
   ________};  /* char 'P'  */

const  CPU_INT08U  FontTerminal8x8_0051[8] = { /* code 0051 */
   _XXXXX__,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX__XXX_,
   _XXXXX__,
   ____XXX_};  /* char 'Q'  */

const  CPU_INT08U  FontTerminal8x8_0052[8] = { /* code 0052 */
   XXXXXX__,
   _XX__XX_,
   _XX__XX_,
   _XXXXX__,
   _XX_XX__,
   _XX__XX_,
   XXX__XX_,
   ________};  /* char 'R'  */

const  CPU_INT08U  FontTerminal8x8_0053[8] = { /* code 0053 */
   __XXXX__,
   _XX__XX_,
   __XX____,
   ___XX___,
   ____XX__,
   _XX__XX_,
   __XXXX__,
   ________};  /* char 'S'  */

const  CPU_INT08U  FontTerminal8x8_0054[8] = { /* code 0054 */
   _XXXXXX_,
   _XXXXXX_,
   _X_XX_X_,
   ___XX___,
   ___XX___,
   ___XX___,
   __XXXX__,
   ________};  /* char 'T'  */

const  CPU_INT08U  FontTerminal8x8_0055[8] = { /* code 0055 */
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   _XXXXX__,
   ________};  /* char 'U'  */

const  CPU_INT08U  FontTerminal8x8_0056[8] = { /* code 0056 */
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   _XX_XX__,
   __XXX___,
   ________};  /* char 'V'  */

const  CPU_INT08U  FontTerminal8x8_0057[8] = { /* code 0057 */
   XX___XX_,
   XX___XX_,
   XX___XX_,
   XX_X_XX_,
   XX_X_XX_,
   XXXXXXX_,
   _XX_XX__,
   ________};  /* char 'W'  */

const  CPU_INT08U  FontTerminal8x8_0058[8] = { /* code 0058 */
   XX___XX_,
   XX___XX_,
   _XX_XX__,
   __XXX___,
   _XX_XX__,
   XX___XX_,
   XX___XX_,
   ________};  /* char 'X'  */

const  CPU_INT08U  FontTerminal8x8_0059[8] = { /* code 0059 */
   _XX__XX_,
   _XX__XX_,
   _XX__XX_,
   __XXXX__,
   ___XX___,
   ___XX___,
   __XXXX__,
   ________};  /* char 'Y'  */

const  CPU_INT08U  FontTerminal8x8_005A[8] = { /* code 005A */
   XXXXXXX_,
   XX___XX_,
   X___XX__,
   ___XX___,
   __XX__X_,
   _XX__XX_,
   XXXXXXX_,
   ________};  /* char 'Z'  */

const  CPU_INT08U  FontTerminal8x8_005B[8] = { /* code 005B */
   __XXXX__,
   __XX____,
   __XX____,
   __XX____,
   __XX____,
   __XX____,
   __XXXX__,
   ________};  /* char '['  */

const  CPU_INT08U  FontTerminal8x8_005C[8] = { /* code 005C */
   XX______,
   _XX_____,
   __XX____,
   ___XX___,
   ____XX__,
   _____XX_,
   ______X_,
   ________};  /* char '\'  */

const  CPU_INT08U  FontTerminal8x8_005D[8] = { /* code 005D */
   __XXXX__,
   ____XX__,
   ____XX__,
   ____XX__,
   ____XX__,
   ____XX__,
   __XXXX__,
   ________};  /* char ']'  */

const  CPU_INT08U  FontTerminal8x8_005E[8] = { /* code 005E */
   ___X____,
   __XXX___,
   _XX_XX__,
   XX___XX_,
   ________,
   ________,
   ________,
   ________};  /* char '^'  */

const  CPU_INT08U  FontTerminal8x8_005F[8] = { /* code 005F */
   ________,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________,
   XXXXXXXX};  /* char '_'  */

const  CPU_INT08U  FontTerminal8x8_0060[8] = { /* code 0060 */
   __XX____,
   ___XX___,
   ____XX__,
   ________,
   ________,
   ________,
   ________,
   ________};  /* char '`'  */

const  CPU_INT08U  FontTerminal8x8_0061[8] = { /* code 0061 */
   ________,
   ________,
   _XXXX___,
   ____XX__,
   _XXXXX__,
   XX__XX__,
   _XXX_XX_,
   ________};  /* char 'a'  */

const  CPU_INT08U  FontTerminal8x8_0062[8] = { /* code 0062 */
   XXX_____,
   _XX_____,
   _XXXXX__,
   _XX__XX_,
   _XX__XX_,
   _XX__XX_,
   XX_XXX__,
   ________};  /* char 'b'  */

const  CPU_INT08U  FontTerminal8x8_0063[8] = { /* code 0063 */
   ________,
   ________,
   _XXXXX__,
   XX___XX_,
   XX______,
   XX___XX_,
   _XXXXX__,
   ________};  /* char 'c'  */

const  CPU_INT08U  FontTerminal8x8_0064[8] = { /* code 0064 */
   ___XXX__,
   ____XX__,
   _XXXXX__,
   XX__XX__,
   XX__XX__,
   XX__XX__,
   _XXX_XX_,
   ________};  /* char 'd'  */

const  CPU_INT08U  FontTerminal8x8_0065[8] = { /* code 0065 */
   ________,
   ________,
   _XXXXX__,
   XX___XX_,
   XXXXXXX_,
   XX______,
   _XXXXX__,
   ________};  /* char 'e'  */

const  CPU_INT08U  FontTerminal8x8_0066[8] = { /* code 0066 */
   __XXXX__,
   _XX__XX_,
   _XX_____,
   XXXXX___,
   _XX_____,
   _XX_____,
   XXXX____,
   ________};  /* char 'f'  */

const  CPU_INT08U  FontTerminal8x8_0067[8] = { /* code 0067 */
   ________,
   ________,
   _XXX_XX_,
   XX__XX__,
   XX__XX__,
   _XXXXX__,
   ____XX__,
   XXXXX___};  /* char 'g'  */

const  CPU_INT08U  FontTerminal8x8_0068[8] = { /* code 0068 */
   XXX_____,
   _XX_____,
   _XX_XX__,
   _XXX_XX_,
   _XX__XX_,
   _XX__XX_,
   XXX__XX_,
   ________};  /* char 'h'  */

const  CPU_INT08U  FontTerminal8x8_0069[8] = { /* code 0069 */
   ___XX___,
   ________,
   __XXX___,
   ___XX___,
   ___XX___,
   ___XX___,
   __XXXX__,
   ________};  /* char 'i'  */

const  CPU_INT08U  FontTerminal8x8_006A[8] = { /* code 006A */
   _____XX_,
   ________,
   _____XX_,
   _____XX_,
   _____XX_,
   _XX__XX_,
   _XX__XX_,
   __XXXX__};  /* char 'j'  */

const  CPU_INT08U  FontTerminal8x8_006B[8] = { /* code 006B */
   XXX_____,
   _XX_____,
   _XX__XX_,
   _XX_XX__,
   _XXXX___,
   _XX_XX__,
   XXX__XX_,
   ________};  /* char 'k'  */

const  CPU_INT08U  FontTerminal8x8_006C[8] = { /* code 006C */
   __XXX___,
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   __XXXX__,
   ________};  /* char 'l'  */

const  CPU_INT08U  FontTerminal8x8_006D[8] = { /* code 006D */
   ________,
   ________,
   XXX_XX__,
   XXXXXXX_,
   XX_X_XX_,
   XX_X_XX_,
   XX_X_XX_,
   ________};  /* char 'm'  */

const  CPU_INT08U  FontTerminal8x8_006E[8] = { /* code 006E */
   ________,
   ________,
   XX_XXX__,
   _XX__XX_,
   _XX__XX_,
   _XX__XX_,
   _XX__XX_,
   ________};  /* char 'n'  */

const  CPU_INT08U  FontTerminal8x8_006F[8] = { /* code 006F */
   ________,
   ________,
   _XXXXX__,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   _XXXXX__,
   ________};  /* char 'o'  */

const  CPU_INT08U  FontTerminal8x8_0070[8] = { /* code 0070 */
   ________,
   ________,
   XX_XXX__,
   _XX__XX_,
   _XX__XX_,
   _XXXXX__,
   _XX_____,
   XXXX____};  /* char 'p'  */

const  CPU_INT08U  FontTerminal8x8_0071[8] = { /* code 0071 */
   ________,
   ________,
   _XXX_XX_,
   XX__XX__,
   XX__XX__,
   _XXXXX__,
   ____XX__,
   ___XXXX_};  /* char 'q'  */

const  CPU_INT08U  FontTerminal8x8_0072[8] = { /* code 0072 */
   ________,
   ________,
   XX_XXX__,
   _XXX_XX_,
   _XX_____,
   _XX_____,
   XXXX____,
   ________};  /* char 'r'  */

const  CPU_INT08U  FontTerminal8x8_0073[8] = { /* code 0073 */
   ________,
   ________,
   _XXXXXX_,
   XX______,
   _XXXXX__,
   _____XX_,
   XXXXXX__,
   ________};  /* char 's'  */

const  CPU_INT08U  FontTerminal8x8_0074[8] = { /* code 0074 */
   __XX____,
   __XX____,
   XXXXXX__,
   __XX____,
   __XX____,
   __XX_XX_,
   ___XXX__,
   ________};  /* char 't'  */

const  CPU_INT08U  FontTerminal8x8_0075[8] = { /* code 0075 */
   ________,
   ________,
   XX__XX__,
   XX__XX__,
   XX__XX__,
   XX__XX__,
   _XXX_XX_,
   ________};  /* char 'u'  */

const  CPU_INT08U  FontTerminal8x8_0076[8] = { /* code 0076 */
   ________,
   ________,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   _XX_XX__,
   __XXX___,
   ________};  /* char 'v'  */

const  CPU_INT08U  FontTerminal8x8_0077[8] = { /* code 0077 */
   ________,
   ________,
   XX___XX_,
   XX_X_XX_,
   XX_X_XX_,
   XXXXXXX_,
   _XX_XX__,
   ________};  /* char 'w'  */

const  CPU_INT08U  FontTerminal8x8_0078[8] = { /* code 0078 */
   ________,
   ________,
   XX___XX_,
   _XX_XX__,
   __XXX___,
   _XX_XX__,
   XX___XX_,
   ________};  /* char 'x'  */

const  CPU_INT08U  FontTerminal8x8_0079[8] = { /* code 0079 */
   ________,
   ________,
   XX___XX_,
   XX___XX_,
   XX___XX_,
   _XXXXXX_,
   _____XX_,
   XXXXXX__};  /* char 'y'  */

const  CPU_INT08U  FontTerminal8x8_007A[8] = { /* code 007A */
   ________,
   ________,
   _XXXXXX_,
   _X__XX__,
   ___XX___,
   __XX__X_,
   _XXXXXX_,
   ________};  /* char 'z'  */

const  CPU_INT08U  FontTerminal8x8_007B[8] = { /* code 007B */
   ____XXX_,
   ___XX___,
   ___XX___,
   _XXX____,
   ___XX___,
   ___XX___,
   ____XXX_,
   ________};  /* char '{'  */

const  CPU_INT08U  FontTerminal8x8_007C[8] = { /* code 007C */
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   ___XX___,
   ________};  /* char '|'  */

const  CPU_INT08U  FontTerminal8x8_007D[8] = { /* code 007D */
   _XXX____,
   ___XX___,
   ___XX___,
   ____XXX_,
   ___XX___,
   ___XX___,
   _XXX____,
   ________};  /* char '}'  */

const  CPU_INT08U  FontTerminal8x8_007E[8] = { /* code 007E */
   _XXX_XX_,
   XX_XXX__,
   ________,
   ________,
   ________,
   ________,
   ________,
   ________};   /* char '~'  */

const  BSP_LCD_FONT_CHAR  FontTerminal8x8_CharTbl[95] = {
   {   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0020 } /* code 0020 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0021 } /* code 0021 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0022 } /* code 0022 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0023 } /* code 0023 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0024 } /* code 0024 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0025 } /* code 0025 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0026 } /* code 0026 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0027 } /* code 0027 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0028 } /* code 0028 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0029 } /* code 0029 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_002A } /* code 002A */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_002B } /* code 002B */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_002C } /* code 002C */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_002D } /* code 002D */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_002E } /* code 002E */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_002F } /* code 002F */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0030 } /* code 0030 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0031 } /* code 0031 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0032 } /* code 0032 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0033 } /* code 0033 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0034 } /* code 0034 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0035 } /* code 0035 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0036 } /* code 0036 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0037 } /* code 0037 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0038 } /* code 0038 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0039 } /* code 0039 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_003A } /* code 003A */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_003B } /* code 003B */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_003C } /* code 003C */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_003D } /* code 003D */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_003E } /* code 003E */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_003F } /* code 003F */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0040 } /* code 0040 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0041 } /* code 0041 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0042 } /* code 0042 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0043 } /* code 0043 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0044 } /* code 0044 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0045 } /* code 0045 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0046 } /* code 0046 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0047 } /* code 0047 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0048 } /* code 0048 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0049 } /* code 0049 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_004A } /* code 004A */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_004B } /* code 004B */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_004C } /* code 004C */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_004D } /* code 004D */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_004E } /* code 004E */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_004F } /* code 004F */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0050 } /* code 0050 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0051 } /* code 0051 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0052 } /* code 0052 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0053 } /* code 0053 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0054 } /* code 0054 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0055 } /* code 0055 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0056 } /* code 0056 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0057 } /* code 0057 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0058 } /* code 0058 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0059 } /* code 0059 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_005A } /* code 005A */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_005B } /* code 005B */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_005C } /* code 005C */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_005D } /* code 005D */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_005E } /* code 005E */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_005F } /* code 005F */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0060 } /* code 0060 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0061 } /* code 0061 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0062 } /* code 0062 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0063 } /* code 0063 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0064 } /* code 0064 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0065 } /* code 0065 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0066 } /* code 0066 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0067 } /* code 0067 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0068 } /* code 0068 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0069 } /* code 0069 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_006A } /* code 006A */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_006B } /* code 006B */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_006C } /* code 006C */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_006D } /* code 006D */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_006E } /* code 006E */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_006F } /* code 006F */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0070 } /* code 0070 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0071 } /* code 0071 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0072 } /* code 0072 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0073 } /* code 0073 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0074 } /* code 0074 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0075 } /* code 0075 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0076 } /* code 0076 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0077 } /* code 0077 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0078 } /* code 0078 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_0079 } /* code 0079 */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_007A } /* code 007A */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_007B } /* code 007B */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_007C } /* code 007C */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_007D } /* code 007D */
  ,{   8,   8,  1, (CPU_INT08U *)FontTerminal8x8_007E } /* code 007E */
};

const  BSP_LCD_FONT  FontTerminal8x8 = {
    (CPU_CHAR           *)"Terminal8x8",                        /* Font name.                                           */
    (CPU_CHAR            )32u,                                  /* First character.                                     */
    (CPU_CHAR            )126u,                                 /* Last character.                                      */
    (BSP_LCD_PIXEL       )8u,                                   /* Font width.                                          */
    (BSP_LCD_PIXEL       )8u,                                   /* Font height.                                         */
    (BSP_LCD_FONT_CHAR  *)&FontTerminal8x8_CharTbl[0]           /* Address of first character.                          */
};
