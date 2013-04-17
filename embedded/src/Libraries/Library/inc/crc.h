/**********************************************************************
 *
 * Filename:    crc.h
 * 
 * Description: A header file describing the various CRC standards.
 *
 * Notes:       
 *
 * 
 * Copyright (c) 2000 by Michael Barr.  This software is placed into
 * the public domain and may be used for any purpose.  However, this
 * notice must not be changed or removed and no warranty is either
 * expressed or implied by its publication or distribution.
 **********************************************************************/
/**
 * @file crc.h
 * @author Michael Barr & Alexander Rössler
 * @brief Library for controlling the IAP functions
 * @date 2000, Modified 2013
 */
/** @defgroup CRC CRC (Circular Redundancy Check)
 * @ingroup Functional
 * @{
 */
#pragma once

/*
 * Select the CRC standard from the list that follows.
 */
#define CRC_CCITT

#include <types.h>


#if defined(CRC_CCITT)

typedef uint16  crc;

#define CRC_NAME			"CRC-CCITT"
#define POLYNOMIAL			0x1021
#define INITIAL_REMAINDER	0xFFFF
#define FINAL_XOR_VALUE		0x0000
#define REFLECT_DATA		FALSE
#define REFLECT_REMAINDER	FALSE
#define CHECK_VALUE			0x29B1

#elif defined(CRC16)

typedef uint16  crc;

#define CRC_NAME			"CRC-16"
#define POLYNOMIAL			0x8005
#define INITIAL_REMAINDER	0x0000
#define FINAL_XOR_VALUE		0x0000
#define REFLECT_DATA		TRUE
#define REFLECT_REMAINDER	TRUE
#define CHECK_VALUE			0xBB3D

#elif defined(CRC32)

typedef uint16  crc;

#define CRC_NAME			"CRC-32"
#define POLYNOMIAL			0x04C11DB7
#define INITIAL_REMAINDER	0xFFFFFFFF
#define FINAL_XOR_VALUE		0xFFFFFFFF
#define REFLECT_DATA		TRUE
#define REFLECT_REMAINDER	TRUE
#define CHECK_VALUE			0xCBF43926

#else

#error "One of CRC_CCITT, CRC16, or CRC32 must be #define'd."

#endif

/** Initializes the fast CRC function.*/
void  Crc_initialize(void);

/** Creates a CRC checksum.
 *  @param message A pointer to the data to verify.
 *  @param nBytes  The number of bytes to verify.
 */
crc   Crc_slow(char* message, uint32 nBytes);

/** Creates a CRC checksum, uses a table which needs to be initialized.
 *  @param message A pointer to the data to verify.
 *  @param nBytes  The number of bytes to verify.
 */
crc   Crc_fast(char* message, uint32 nBytes);

/**
 * @}
 */
