/**
 * @file debug.h
 * @author Alexander Rössler
 * @brief Debug Library
 * @date 16-05-2013
 */
/** @defgroup Debug A library which uses the shared memory debug features of the iSystem debuggers
 * @ingroup Functional
 * @{
 */
#pragma once

#ifndef USE_DEBUG
#define USE_DEBUG 1
#endif

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 5
#endif

/* buffers must be 2^n in size ( max 0x100) */
#define TWBUFF_SIZE 0x100 
#define TRBUFF_SIZE 0x100

/* target and client pointers at the end of the buffers */
#define TWBUFF_TPTR (g_TWBuffer[TWBUFF_SIZE+0])
#define TWBUFF_CPTR (g_TWBuffer[TWBUFF_SIZE+1])
#define TRBUFF_TPTR (g_TRBuffer[TRBUFF_SIZE+0])
#define TRBUFF_CPTR (g_TRBuffer[TRBUFF_SIZE+1])
/* number of pointers */
#define TBUFF_PTR 2
/* buffer lenghth */
#define TWBUFF_LEN (TWBUFF_SIZE+TBUFF_PTR)
#define TRBUFF_LEN (TRBUFF_SIZE+TBUFF_PTR)
/* max buffer size */
#define TWBUFF_MAX (TWBUFF_SIZE-1)
#define TRBUFF_MAX (TRBUFF_SIZE-1)

/* buffer manipulation helpers */
#define TWBUFF_EMPTY() (TWBUFF_TPTR==TWBUFF_CPTR)
#define TWBUFF_FULL() (TWBUFF_TPTR==((TWBUFF_CPTR-1)&(TWBUFF_SIZE-1)))
#define TWBUFF_INC(n) (++n&(TWBUFF_SIZE-1))
#define TRBUFF_EMPTY() (TRBUFF_TPTR==TRBUFF_CPTR)
#define TRBUFF_FULL() (TRBUFF_TPTR==((TRBUFF_CPTR-1)&(TRBUFF_SIZE-1)))
#define TRBUFF_INC(n) (++n&(TRBUFF_SIZE-1))

#include <stdio.h>
#include <stdarg.h>
#include <types.h>

typedef enum {
    Debug_Level_0 = 0u,
    Debug_Level_1 = 1u,
    Debug_Level_2 = 2u,
    Debug_Level_3 = 3u,
    Debug_Level_4 = 4u,
    Debug_Level_5 = 5u
} Debug_Level;

/** Prints a string to the debug output.
 *  @param pFormat Formated string.
 *  @param ... Formatting parameters.
 *  @return 0 if successful -1 if nothing to write
 */
int8 Debug_printf(Debug_Level debugLevel, const char *pFormat,...);

/** Checks if data is available.
 *  @return TRUE if data is available FALS if not.
 */
bool Debug_dataAvailable(void);

/** Returns data.
 *  @return One character from the debug buffer.
 */
char Debug_getChar(void);

/**
 * @}
 */
