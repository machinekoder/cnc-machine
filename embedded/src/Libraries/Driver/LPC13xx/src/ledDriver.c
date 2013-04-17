#include "../inc/ledDriver.h"

uint8 initializeLeds(void)
{
	LPC_IOCON->JTAG_nTRST_PIO1_2 |= (1<<0);     /* configure LED_RED_EN as IO pin */

	/* configure the direction of several IO pins */
	LPC_GPIO3->DIR |= LATCH_EN;
	LPC_GPIO1->DIR |= (LED_RED_ENB | LED1 | LED2 | LED3 | LED4 | LED5 | LED6 | LED7 | LED8);
	LPC_GPIO2->DIR |= (LED_GREEN_ENB);

	LPC_GPIO3->DATA = LATCH_EN;					/* enable the Latch                   */
	ENABLE_GREEN();		        /* enable the Green LEDs - low active */
	ENABLE_RED();			/* enable the Red LEDs - high active  */

	return INIT_OK;
}

void setGreenLedsEnabled(bool enabled)
{
    if (enabled)
        ENABLE_GREEN();
    else
        DISABLE_GREEN();
}

void setRedLedsEnabled(bool enabled)
{
    if (enabled)
        ENABLE_RED();
    else
        DISABLE_RED();
}
