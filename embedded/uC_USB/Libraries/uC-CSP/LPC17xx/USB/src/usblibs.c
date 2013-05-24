#include <usbdesc.h>
#include <csp_grp.h>
#include <csp.h>

void deleteBulkOutBuf()
{
	unsigned int count=0;
	while(count<=USB_BUFFER_SIZE-1)
	{
		usbReceiveBuffer[count] = 0;
		count++;
	}
	count = 0;
	usbReceiveBufferSize = 0;
}

void deleteBulkInBuf()
{
	unsigned int count=0;
	while(count<=USB_BUFFER_SIZE-1)
	{
		usbSendBuffer[count] = 0;
		count++;
	}
	count = 0;
	usbSendBufferSize = 0;
}

/*
	Interrupt handler
	Simply calls the USB ISR
*/
void USB_IRQHandler(void)
{
	USBHwISR();
	CSP_IntClr(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_USB_00);
	CPU_IntSrcEn(CPU_INT_SYSTICK);                              /* Enable the SYSTICK interrupt. Resume OS operation.   */
}

/*
	Local function to handle incoming bulk data
	@param [in] bEP
	@param [in] bEPStatus
*/
void BulkOut(uint8_t bEP, uint8_t bEPStatus)
{
	int i;
	/* disable_USB_interrupts(); */
	for(i=0;i<=USB_BUFFER_SIZE-1;i++)
	{
		usbReceiveBuffer[i] = 0;
	}

	USBHwEPRead(bEP, usbReceiveBuffer, sizeof(usbReceiveBuffer));

	usbReceiveBufferSize = usbReceiveBuffer[1]<<8 | usbReceiveBuffer[2];
	usbReceiveBufferSize = usbReceiveBufferSize - 1;
}


/*
	Local function to handle outgoing bulk data
	@param [in] bEP
	@param [in] bEPStatus
*/
void BulkIn(uint8_t bEP, uint8_t bEPStatus)
{
	if(usbSendBufferSize > 0)
	{
		USBHwEPWrite(bEP, usbSendBuffer, usbSendBufferSize);
		deleteBulkInBuf();
	}
}
