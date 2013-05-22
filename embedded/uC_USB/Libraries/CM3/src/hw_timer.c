#include "hw_timer.h"
#include "LPC17xx.h"

char TimerInit(char timer)
{
	//LPC_SC->PCONP |= (1<<1);
	switch(timer){
		case 0: LPC_TIM0->TCR = 0x02;	/* stop and reset timer */
				LPC_TIM0->PR  = 100000;	/* set PRE to 1ms */
				LPC_TIM0->PC  = 0x00;
				LPC_TIM0->TC  = 0x00;
				LPC_TIM0->MR0 = 25;	/* set MR0 to 0.025s */
				LPC_TIM0->IR  = 0x01;	/* reset Timer0 IR flag */
				LPC_TIM0->MCR = 0x03;	/* Interrupt on MR0, Reset TC on MR0 */
				break;
		case 1: /* not yet implemented */
				break;
		case 2: /* not yet implemented */
				break;
		case 3: /* not yet implemented */
				break;
		default: break;
	}

	return 0;
}

char TimerStart()
{
	LPC_TIM0->TCR = 0x01;	/* start timer */
	return 0;
}

char TimerResetStop()
{
	LPC_TIM0->TCR = 0x02;	/* stop and reset timer */
	return 0;
}

char TimerClrIRFlag()
{
	LPC_TIM0->IR = 0x01;
	return 0;
}
