/**
 * @file circularbuffer.h
 * @author Alexander Rössler
 * @brief Circular Buffer Library
 * @date 07-11-2012
 */
/** @defgroup CircularBuffer CircularBuffer
 * @ingroup Functional
 * @{
 */
#pragma once

#include <types.h>
#include <stdlib.h>
#include <string.h>

/** Initialize this structure with the initializeCb function! */
typedef struct
{
    void *inPointer;        /** Pointer holding the write position */
    void *outPointer;       /** Pointer holding the read position */
    void *startPointer;     /** Pointer to the beginning of the data */
    uint16     dataSize;    /** Size of one element */
    uint16     bufferSize;  /** Size of the buffer in bytes */
    
} CircularBuffer;

/** Initializes a circular buffer.
 *  @param buffer The buffer to initialize,
 *  @param bufferSize The number of elements the buffer should store (the usable elements will be bufferSize-1)
 *  @param dataSize The size of one element in bytes.
 *  @param data A pointer to the freed space in memory where the buffer should be place, if NULL memory will be allocated.
 *  @return Returns -1 if not enough memory is available and 0 if successful.
 */
int8 Cb_initialize(CircularBuffer* buffer, uint16 bufferSize, uint16 dataSize, void* data);
/** Puts one element to the circular buffer
 *  @param buffer Target buffer.
 *  @param item   Pointer to the element to add, only the data of the element will be used.
 *  @return Returns -1 if the buffer is full and 0 if successful.
 */
int8 Cb_put(CircularBuffer *buffer, void *item);
/** Get one element from the circular buffer.
 *  @param buffer Target buffer.
 *  @param item Pointer to the element where the data should be stored.
 *  @return Returns -1 if the buffer is empty and 0 if successful.
 */
int8 Cb_get(CircularBuffer *buffer, void *item);

/**
 * @}
 */
