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

	usbReceiveBufferSize = usbReceiveBuffer[0]<<8 | usbReceiveBuffer[1];
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

int8_t USB_printf(char *format, ...)
{
    va_list arg_ptr;
    
    va_start(arg_ptr,format);
    vsnprintf((char *)(&usbSendBuffer[2]), USB_BUFFER_SIZE-2, format, arg_ptr);
    va_end(arg_ptr);
    
    usbSendBufferSize = strlen((char *)(&usbSendBuffer[2])) + 3;                  // calculate string length of outgoing data 
       
    usbSendBuffer[0]=(uint8_t)(0x00ff & ((usbSendBufferSize-2)>>8));                 // Highbyte
    usbSendBuffer[1]=(uint8_t)(0x00ff & (usbSendBufferSize-2));                      // Lowbyte
    
    /* static char buffer[USB_BUFFER_SIZE-2];
    va_list arg_ptr;
    
    va_start(arg_ptr,format);
    vsnprintf(buffer, USB_BUFFER_SIZE-2, format, arg_ptr);
    va_end(arg_ptr);
    
    usbSendBufferSize = strlen(buffer);//strlen((char *)(&usbSendBuffer[2])) + 3;                  // calculate string length of outgoing data 
    usbSendBuffer[0]=0x00ff&((usbSendBufferSize+1)>>8);                 // Highbyte
    usbSendBuffer[1]=0x00ff&(usbSendBufferSize+1);                      // Lowbyte
    snprintf((char*)&usbSendBuffer[2], USB_BUFFER_SIZE-2, "%s", buffer);
    usbSendBufferSize += 3;
    */
    
    return 0;
}
