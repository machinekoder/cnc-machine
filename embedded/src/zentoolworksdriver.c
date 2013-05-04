/*
 * zentoolworksdriver.c
 *
 *  Created on: 04.05.2013
 *      Author: Koorosh
 */
#include "gpio.h"
#include "pincon.h"

void init()
{

Gpio_setDirection(2,1,Gpio_Direction_Output);
Pin_setMode(2,1,Pin_Mode_PullUp);
}
