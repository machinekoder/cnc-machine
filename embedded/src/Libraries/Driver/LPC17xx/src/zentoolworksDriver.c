/*
 * zentoolworksdriver.c
 *
 *  Created on: 04.05.2013
 *      Author: Koorosh
 */

#include "zentoolworksDriver.h"



void cnc_initialize ()
{
        //+++++++++++++++++++++++++++++++++++++++++MOTOR++++++++++++++++++++++++++++++++++++++++++++++++++
        //Motor x Dir
    Gpio_setDirection(MOTOR_X_DIR_PORT,MOTOR_X_DIR_PIN,Gpio_Direction_Output);
    Pin_setMode(MOTOR_X_CLK_PORT,MOTOR_X_CLK_PIN,Pin_Mode_PullUp); //high --> turn right
    Gpio_set(MOTOR_X_CLK_PORT,MOTOR_X_CLK_PIN);

    //Motor x Clk
    Gpio_setDirection(MOTOR_X_CLK_PORT,MOTOR_X_CLK_PIN,Gpio_Direction_Output);
    Pin_setMode(MOTOR_X_CLK_PORT,MOTOR_X_CLK_PIN,Pin_Mode_PullUp); //high --> clk is disabled
    Gpio_set(MOTOR_X_CLK_PORT,MOTOR_X_CLK_PIN);

    //Motor xyz Enable
    Gpio_setDirection(MOTOR_XYZ_ENABLE_PORT,MOTOR_XYZ_ENABLE_PIN,Gpio_Direction_Output);
    Pin_setMode(MOTOR_XYZ_ENABLE_PORT,MOTOR_XYZ_ENABLE_PIN,Pin_Mode_PullDown); //low --> Enable is active
    Gpio_clear(MOTOR_XYZ_ENABLE_PORT,MOTOR_XYZ_ENABLE_PIN);

    //Motor x Power(stromabsenkung)
    Gpio_setDirection(MOTOR_X_POWER_PORT,MOTOR_X_POWER_PIN,Gpio_Direction_Output);
    Pin_setMode(MOTOR_X_POWER_PORT,MOTOR_X_POWER_PIN,Pin_Mode_PullDown);
    Gpio_clear(MOTOR_X_POWER_PORT,MOTOR_X_POWER_PIN);

        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Motor y Dir
    Gpio_setDirection(MOTOR_Y_DIR_PORT,MOTOR_Y_DIR_PIN,Gpio_Direction_Output);
    Pin_setMode(MOTOR_Y_DIR_PORT,MOTOR_Y_DIR_PIN,Pin_Mode_PullUp);
    Gpio_set(MOTOR_Y_DIR_PORT,MOTOR_Y_DIR_PIN);

    //Motor y Clk
    Gpio_setDirection(MOTOR_Y_CLK_PORT,MOTOR_Y_CLK_PIN,Gpio_Direction_Output);
    Pin_setMode(MOTOR_Y_CLK_PORT,MOTOR_Y_CLK_PIN,Pin_Mode_PullUp);
    Gpio_set(MOTOR_Y_CLK_PORT,MOTOR_Y_CLK_PIN);

    //Motor y Power(stromabsenkung)
    Gpio_setDirection(MOTOR_Y_POWER_PORT,MOTOR_Y_POWER_PIN,Gpio_Direction_Output);
    Pin_setMode(MOTOR_Y_POWER_PORT,MOTOR_Y_POWER_PIN,Pin_Mode_PullDown);
    Gpio_clear(MOTOR_Y_POWER_PORT,MOTOR_Y_POWER_PIN);

        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Motor z Dir
    Gpio_setDirection(MOTOR_Z_DIR_PORT,MOTOR_Z_DIR_PIN,Gpio_Direction_Output);
    Pin_setMode(MOTOR_Z_DIR_PORT,MOTOR_Z_DIR_PIN,Pin_Mode_PullUp); //high --> turn right
    Gpio_set(MOTOR_Z_DIR_PORT,MOTOR_Z_DIR_PIN);

    //Motor z Clk
    Gpio_setDirection(MOTOR_Z_CLK_PORT,MOTOR_Z_CLK_PIN,Gpio_Direction_Output);
    Pin_setMode(MOTOR_Z_CLK_PORT,MOTOR_Z_CLK_PIN,Pin_Mode_PullUp); //high --> clk is disabled
    Gpio_set(MOTOR_Z_CLK_PORT,MOTOR_Z_CLK_PIN);

    //Motor z Power(stromabsenkung)
    Gpio_setDirection(MOTOR_Z_POWER_PORT,MOTOR_Z_POWER_PIN,Gpio_Direction_Output);
    Pin_setMode(MOTOR_Z_POWER_PORT,MOTOR_Z_POWER_PIN,Pin_Mode_PullDown);//low --> Stromabsenkung is active
    Gpio_clear(MOTOR_Z_POWER_PORT,MOTOR_Z_POWER_PIN);

}
