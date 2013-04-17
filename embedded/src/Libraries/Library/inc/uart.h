/**
 * @file uart.h
 * @author Alexander Rössler
 * @brief UART Library
 * @date 06-11-2012
 */
/** @defgroup UART UART (Universal Asynchronous Receiver and Transmitter)
 * @ingroup Peripheral
 * @{
 */
#pragma once
#include <stdlib.h>

/** Size of UARTs circular buffer for reading */
#ifndef UART_READ_BUFFER_SIZE
#define UART_READ_BUFFER_SIZE 420
#endif
/** Size of UARTs circular buffer for writing */
#ifndef UART_WRITE_BUFFER_SIZE
#define UART_WRITE_BUFFER_SIZE 420
#endif
/** Defines wheter the UART buffer should be used or not */
#ifndef USE_UART_BUFFER
#define USE_UART_BUFFER 1
#endif

/** Defines wheter a UART task to process incoming commands should be used or not. */
#ifndef USE_UART_TASK
#define USE_UART_TASK 1
#endif

#ifndef USE_UART0
#define USE_UART0 1
#endif
#ifndef USE_UART1
#define USE_UART1 0
#endif
#ifndef USE_UART2
#define USE_UART2 0
#endif
#ifndef USE_UART3
#define USE_UART3 0
#endif

#ifndef UART_PRINTF_BUFFER_SIZE
#define UART_PRINTF_BUFFER_SIZE 420
#endif
#ifndef UART_TASK_BUFFER_SIZE
#define UART_TASK_BUFFER_SIZE   450
#endif

#define UART0_COMMAND_CHAR  13  // Carriage Return (CR)
#define UART1_COMMAND_CHAR  13
#define UART2_COMMAND_CHAR  13
#define UART3_COMMAND_CHAR  13

#define UART0_SECONDARY_COMMAND_CHAR    '\a'
#define UART1_SECONDARY_COMMAND_CHAR    '\a'
#define UART2_SECONDARY_COMMAND_CHAR    '\a'
#define UART3_SECONDARY_COMMAND_CHAR    '\a'

#include <uartDriver.h>
#include <circularbuffer.h>
#include <types.h>
#include <stdarg.h>
#include <stdio.h>

typedef enum {
    Uart0 = 0,
    Uart1 = 1,
    Uart2 = 2,
    Uart3 = 3
} Uart;

/** Initializes a UART
 *  The UART is configured with following configuration: 8 bits, no Parity, 1 Stop bit
 *  @param uart     The UART to initialize.
 *  @param baudrate The baudrate the UART should run with
 */
int8 Uart_initialize(Uart uart, uint32 baudrate);

/** Function to send character over UART.
 *  @param uart The UART to output data.
 *  @param c The character to send.
 *  @return 0 if successful -1 if nothing to write
 */
int8 Uart_putchar(Uart uart, char c);

/** Function to write a defined a number of bytes from memory to UART3.
 * @param uart The UART to output data.
 * @param data Pointer to the data.
 * @param length Number of bytes to write.
 * @return 0 if successful -1 if nothing to write
 */
int8 Uart_writeData(Uart uart, void *data, uint32 length);

/** Function to get a character from UART.
  *  @param uart The UART to read data from.
  *  @param c Pointer to the received character.
  *  @return 0 if successful -1 if nothing to reading
 */
int8 Uart_getchar(Uart uart, char* c);

/** Custom printf function for UART.
 *  @param uart The UART to output data to.
 *  @param format Formated string.
 *  @param ... Formatting parameters.
 *  @return 0 if successful -1 if nothing to write
 */
int8 Uart_printf(Uart uart, char* format, ...);

/** Cleans in and output buffers a UART.
 *  @param uart The UART to output data to. 
 */
void Uart_flush(Uart uart);

/** Changes the baudrate of a UART
 *  @param uart The UART to change the baudrate of.
 *  @param baudrate The target baudrate.
 */
void Uart_setBaudrate(Uart uart, uint32 baudrate);

#if (USE_UART_TASK == 1)
/** Sets the command processing function for UART3, 
 *  only needed if process tasks are used.
 *  @param uart The UART to set the function to.
 *  @param func Pointer to the function that should be called when a command arives
 */ 
void Uart_setProcessFunction(Uart uart, void (* func)(char *));

/** Sets the command error function for a UART.
 *  only needed if process tasks are used.
 *  @param uart The UART to set the function to.
 *  @param func Pointer to the function that should be called when a error occures
 */ 
void Uart_setErrorFunction(Uart uart, void (* func)());

/** Task function for UART0. This function processes incoming characters
 *  to find commands that end with a command char (see defines).
 */
void Uart_processTask0();
/** Task function for UART1. This function processes incoming characters
 *  to find commands that end with a command char (see defines).
 */
void Uart_processTask1();
/** Task function for UART2. This function processes incoming characters
 *  to find commands that end with a command char (see defines).
 */
void Uart_processTask2();
/** Task function for UART3. This function processes incoming characters
 *  to find commands that end with a command char (see defines).
 */
void Uart_processTask3();

void Uart_processTask(Uart uart);
#endif

/**
 * @}
 */
