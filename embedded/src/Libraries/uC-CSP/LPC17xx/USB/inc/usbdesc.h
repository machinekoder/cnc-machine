#ifndef _usbdesc_h_
#define _usbdesc_h_

#include <usbstruct.h>
#include <usbapi.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define ACK 1
#define USB_BUFFER_SIZE 360
#define MAX_PACKET_SIZE USB_BUFFER_SIZE
#define INT_IN_EP		0x81
#define BULK_OUT_EP		0x05
#define BULK_IN_EP		0x82
#define LE_WORD(x)		((x)&0xFF),((x)>>8)

uint16_t usbSendBufferSize;
uint16_t usbReceiveBufferSize;
uint8_t usbReceiveBuffer[USB_BUFFER_SIZE];
uint8_t usbSendBuffer[USB_BUFFER_SIZE];

static const uint8_t abDescriptors[] = {

	/* device descriptor */
	0x12,
	DESC_DEVICE,
	LE_WORD(0x0200),			/* bcdUSB */
	0xFF,						/* bDeviceClass */
	0x00,						/* bDeviceSubClass */
	0x00,						/* bDeviceProtocol */
	MAX_PACKET_SIZE0,			/* bMaxPacketSize */
	LE_WORD(0xfefe),			/* idVendor */
	LE_WORD(0x0001),			/* idProduct */
	LE_WORD(0x0100),			/* bcdDevice */
	0x01,						/* iManufacturer */
	0x02,						/* iProduct */
	0x03,						/* iSerialNumber */
	0x01,						/* bNumConfigurations */

	/* configuration descriptor */
	0x09,
	DESC_CONFIGURATION,
	LE_WORD(                    /* wTotalLength */
	    1*0x9 +					/* ConfigDesc */
	    1*0x9 +  				/* InterfaceDesc */
	    2*0x7    	            /* bulk endpoints */
	),
	0x01,						/* bNumInterfaces */
	0x01,						/* bConfigurationValue */
	0x00,						/* iConfiguration */
	0x80,						/* bmAttributes */
	0x32,						/* bMaxPower */

	/* interface descriptor */
	0x09,
	DESC_INTERFACE,
	0x00,						/* bInterfaceNumber */
	0x00,						/* bAlternateSetting */
	0x02,						/* bNumEndPoints */
	0xFF,						/* bInterfaceClass */
	0x00,						/* bInterfaceSubClass */
	0x00,						/* bInterfaceProtocol */
	0x00,						/* iInterface */

	/* control class interface
	0x09,
	DESC_INTERFACE,
	0x00,						// bInterfaceNumber
	0x00,						// bAlternateSetting
	0x02,						// bNumEndPoints
	0xFF,						// bInterfaceClass
	0x00,						// bInterfaceSubClass
	0x00,						// bInterfaceProtocol, linux requires value of 1 for the cdc_acm module
	0x00,						// iInterface

	// header functional descriptor
	0x05,
	CS_INTERFACE,
	0x00,
	LE_WORD(0x0110),

    // call management functional descriptor
	0x05,
	CS_INTERFACE,
	0x01,
	0x01,						// bmCapabilities = device handles call management
	0x01,						// bDataInterface

	// ACM functional descriptor
	0x04,
	CS_INTERFACE,
	0x02,
	0x02,						// bmCapabilities

	// union functional descriptor
	0x05,
	CS_INTERFACE,
	0x06,
	0x00,						// bMasterInterface
	0x01,						// bSlaveInterface0

	// notification EP
	0x07,
	DESC_ENDPOINT,
	INT_IN_EP,					// bEndpointAddress
	0x03,						// bmAttributes = intr
	LE_WORD(8),					// wMaxPacketSize
	0x0A,						// bInterval
*/

	/* data EP OUT */
	0x07,
	DESC_ENDPOINT,
	BULK_OUT_EP,				/* bEndpointAddress */
	0x02,						/* bmAttributes = bulk */
	LE_WORD(MAX_PACKET_SIZE),	/* wMaxPacketSize */
	0x00,						/* bInterval */

	/* data EP in */
	0x07,
	DESC_ENDPOINT,
	BULK_IN_EP,					/* bEndpointAddress */
	0x02,						/* bmAttributes = bulk */
	LE_WORD(MAX_PACKET_SIZE),	/* wMaxPacketSize */
	0x00,						/* bInterval */

	/* string descriptors */
	/* Index 0 */
	0x04,
	DESC_STRING,
	LE_WORD(0x0409),

	/* Index 1 */
	0x0A,
	DESC_STRING,
	'F', 0, 'H', 0, 'T', 0, 'W', 0,

	/* Index 2 */
    0x14,
	DESC_STRING,
	'C', 0, 'N', 0, 'C', 0, ' ', 0, 'u', 0, 'C', 0, 'O', 0, 'S', 0, '3', 0,

	/* Index 3 */
	0x14,
	DESC_STRING,
	'0', 0, '0', 0, '0', 0, '0', 0, ':', 0, '0', 0, '0', 0, ':', 0, '1', 0,

	/* terminating zero */
	0
};

void USBIntHandler(void);
void deleteBulkOutBuf();
void deleteBulkInBuf();
void BulkOut(uint8_t bEP, uint8_t bEPStatus);
void BulkIn(uint8_t bEP, uint8_t bEPStatus);
void deleteBulkOutBuf();
void deleteBulkInBuf();
void enable_USB_interrupts(void);
void disable_USB_interrupts(void);
void USB_IRQHandler(void);

int8_t USB_printf(char *format, ...);

#endif
