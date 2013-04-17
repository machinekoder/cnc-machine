#ifndef _LEDDRIVER_H_
#define _LEDDRIVER_H_

#define LEDCOUNT 8
#define LEDOFFSET 3
#define MAXLED LEDCOUNT+LEDOFFSET
#define INIT_OK 0

#define LATCH_EN (1<<2)
#define LED1	 (1<<4)  /* PIO1_4  */
#define LED2	 (1<<5)  /* PIO1_5  */
#define LED3	 (1<<6)  /* PIO1_6  */
#define LED4	 (1<<7)  /* PIO1_7  */
#define LED5	 (1<<8)  /* PIO1_8  */
#define LED6	 (1<<9)  /* PIO1_9  */
#define LED7	 (1<<10) /* PIO1_10 */
#define LED8     (1<<11) /* PIO1_11 */

#define LED_GREEN_ENB   (1<<0)
#define LED_RED_ENB     (1<<2)

#define SET_LED(x)      LPC_GPIO1->DATA &= (~(1<<(x+LEDOFFSET)))
#define CLEAR_LED(x)    LPC_GPIO1->DATA |= (1<<(x+LEDOFFSET))
#define READ_LED(x)     !(LPC_GPIO1->DATA & (1<<(x+LEDOFFSET)))
#define ENABLE_GREEN()  LPC_GPIO2->DATA &= ~(LED_GREEN_ENB)
#define DISABLE_GREEN() LPC_GPIO2->DATA |= LED_GREEN_ENB
#define ENABLE_RED()    LPC_GPIO1->DATA |= LED_RED_ENB
#define DISABLE_RED()   LPC_GPIO1->DATA &= ~(LED_RED_ENB)

#include <LPC13xx.h>
#include <types.h>

uint8 initializeLeds(void);

void setGreenLedsEnabled(bool enabled);
/** Enables or disables the red Leds if possible.
 *  @param enabled
 */
void setRedLedsEnabled(bool enabled);

#endif
