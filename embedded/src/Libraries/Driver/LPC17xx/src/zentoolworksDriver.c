/*
 * zentoolworksdriver.c
 *
 *  Created on: 04.05.2013
 *      Author: Koorosh
 */

#include "gpio.h"
#include "pincon.h"

void cncInit ()
{
	//+++++++++++++++++++++++++++++++++++++++++MOTOR++++++++++++++++++++++++++++++++++++++++++++++++++
	//Motor x Dir
    Gpio_setDirection(0,10,Gpio_Direction_Output);
    Pin_setMode(0,10,Pin_Mode_PullUp); //high --> turn right
    Gpio_set(0,10);

    //Motor x Clk
    Gpio_setDirection(0,11,Gpio_Direction_Output);
    Pin_setMode(0,11,Pin_Mode_PullUp); //high --> clk is disabled
    Gpio_set(0,11);

    //Motor x Enable
    Gpio_setDirection(0,6,Gpio_Direction_Output);
    Pin_setMode(0,6,Pin_Mode_PullDown); //low --> Enable is active
    Gpio_set(0,6);

    //Motor x Power(stromabsenkung)
    Gpio_setDirection(1,24,Gpio_Direction_Output);
    Pin_setMode(1,24,Pin_Mode_PullDown);
    Gpio_set(1,24);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Motor y Dir
    Gpio_setDirection(1,20,Gpio_Direction_Output);
    Pin_setMode(1,20,Pin_Mode_PullUp);
    Gpio_set(1,20);

    //Motor y Clk
    Gpio_setDirection(1,23,Gpio_Direction_Output);
    Pin_setMode(1,23,Pin_Mode_PullUp);
    Gpio_set(1,23);

    //Motor y Enable
    Gpio_setDirection(0,6,Gpio_Direction_Output);
    Pin_setMode(0,6,Pin_Mode_PullUp);
    Gpio_set(0,6);

    //Motor y Power(stromabsenkung)
    Gpio_setDirection(1,25,Gpio_Direction_Output);
    Pin_setMode(1,25,Pin_Mode_PullDown);
    Gpio_set(1,25);

   	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Motor z Dir
    Gpio_setDirection(0,0,Gpio_Direction_Output);
    Pin_setMode(0,0,Pin_Mode_PullUp); //high --> turn right
    Gpio_set(0,0);

    //Motor z Clk
    Gpio_setDirection(0,1,Gpio_Direction_Output);
    Pin_setMode(0,1,Pin_Mode_PullUp); //high --> clk is disabled
    Gpio_set(0,1);

    //Motor z Enable
    Gpio_setDirection(0,6,Gpio_Direction_Output);
    Pin_setMode(0,6,Pin_Mode_PullDown); //low --> Enable is active
    Gpio_set(0,6);

    //Motor z Power(stromabsenkung)
    Gpio_setDirection(1,26,Gpio_Direction_Output);
    Pin_setMode(1,26,Pin_Mode_PullDown);//low --> Stromabsenkung is active
    Gpio_set(1,26);

}
