/*
 * zentoolworksDriver.h
 *
 *  Created on: 04.05.2013
 *      Author: Koorosh
 */

#ifndef ZENTOOLWORKSDRIVER_H_
#define ZENTOOLWORKSDRIVER_H_

#define MOTOR_XYZ_ENABLE_PORT   0
#define MOTOR_XYZ_ENABLE_PIN    6

#define MOTOR_X_DIR_PORT        0
#define MOTOR_X_DIR_PIN         10
#define MOTOR_X_CLK_PORT        0
#define MOTOR_X_CLK_PIN         11
#define MOTOR_X_POWER_PORT      1
#define MOTOR_X_POWER_PIN       24

#define MOTOR_Y_DIR_PORT        1
#define MOTOR_Y_DIR_PIN         20
#define MOTOR_Y_CLK_PORT        1
#define MOTOR_Y_CLK_PIN         23
#define MOTOR_Y_POWER_PORT      1
#define MOTOR_Y_POWER_PIN       25

#define MOTOR_Z_DIR_PORT        0
#define MOTOR_Z_DIR_PIN         0
#define MOTOR_Z_CLK_PORT        0
#define MOTOR_Z_CLK_PIN         1
#define MOTOR_Z_POWER_PORT      1
#define MOTOR_Z_POWER_PIN       26

#include "gpio.h"
#include "pincon.h"

void cnc_initialize ();

#endif /* ZENTOOLWORKSDRIVER_H_ */
