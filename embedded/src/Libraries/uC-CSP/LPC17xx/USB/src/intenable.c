#include <LPC17xx.h>

void enable_USB_interrupts(void)
{
	NVIC_EnableIRQ(USB_IRQn);
}

void disable_USB_interrupts(void)
{
	NVIC_DisableIRQ(USB_IRQn);
}
