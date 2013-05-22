#include <usbdesc.h>
#include <csp_grp.h>
#include <csp.h>

void deleteBulkOutBuf()
{
	unsigned int count=0;
	while(count<=USB_BUFFER_SIZE-1)
	{
		abBulkOutBuf[count] = 0;
		count++;
	}
	count = 0;
	BulkOutSize = 0;
}

void deleteBulkInBuf()
{
	unsigned int count=0;
	while(count<=USB_BUFFER_SIZE-1)
	{
		abBulkInBuf[count] = 0;
		count++;
	}
	count = 0;
	BulkInSize = 0;
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
		abBulkOutBuf[i] = 0;
	}

	USBHwEPRead(bEP, abBulkOutBuf, sizeof(abBulkOutBuf));

	BulkOutSize = abBulkOutBuf[1]<<8 | abBulkOutBuf[2];
	BulkOutSize = BulkOutSize - 1;
}


/*
	Local function to handle outgoing bulk data
	@param [in] bEP
	@param [in] bEPStatus
*/
void BulkIn(uint8_t bEP, uint8_t bEPStatus)
{
	if(BulkInSize > 0)
	{
		USBHwEPWrite(bEP, abBulkInBuf, BulkInSize);
		deleteBulkInBuf();
	}
}
