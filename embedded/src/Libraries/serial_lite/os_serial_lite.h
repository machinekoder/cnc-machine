/*
 * os_serial_lite.h
 *
 *  Created on: 27 Nov 2012
 *      Author: es
 */

#ifndef OS_SERIAL_LITE_H_
#define OS_SERIAL_LITE_H_

#include <lpc17xx_pinsel.h>
#include <lpc17xx_clkpwr.h>
#include <lpc17xx_uart.h>
#include <stdint.h>
#include <string.h>
#include <os.h>
#include <csp.h>

#ifdef _SERIAL_LITE

#undef _INTERRUPT_DRIVEN

#define LPC1343_BUFMAX 30

typedef struct {
/*transmitter*/
	char *tx_ptr;
	uint16_t tx_len;
	uint16_t tx_i;
/*receiver*/
	char *rx_ptr;
	uint16_t rx_len;
	uint16_t rx_i;
} uart_context_t;

uart_context_t com_context;


int os_serial_pincfg(uint8_t dev);

/*use uart1*/
int os_serial_sync_send(uint8_t dev, char *buf, uint16_t len);

int os_serial_async_send(uint8_t dev, char *buf, uint16_t len);

int os_serial_lite_tx_enable(uint8_t dev);

int os_serial_lite_init(uint8_t dev, UART_CFG_Type *cfg);


#endif /*_SERIAL_LITE*/

#endif /* OS_SERIAL_LITE_H_ */
