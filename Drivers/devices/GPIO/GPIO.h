/****************************************************************************************
*  _____ _                    _        													*
* / ____(_)                  | |														*
*| (___  ___      _____  _ __| | _____													*
* \___ \| \ \ /\ / / _ \| '__| |/ / __|													*
* ____) | |\ V  V / (_) | |  |   <\__ \													*
*|_____/|_| \_/\_/ \___/|_|  |_|\_\___/													*
*																						*
*	ProjectName Firmware																*
*	Copyright (c) 2019, Siworks, All rights reserved.									*
*																						*
****************************************************************************************/

/**
* @file		GPIO.h
* @brief	Driver for GPIO pins
* @date		22.10.2019
* @version	0.1
*/
#ifndef GPIO_H_
#define GPIO_H_

/****************************************************************************************
* Include files
****************************************************************************************/
#include <stdbool.h>

#include "port.h"


/****************************************************************************************
* Macro definitions
****************************************************************************************/
/** @brief number of all GPIO pins */
#define GPIO_NBR 21


/****************************************************************************************
* Type definitions
****************************************************************************************/
/** @brief Pin enumerators */
typedef enum
{
	IO0,
	IO1,
	IO2,
	IO3,
	IO4,
	IO5,
	IO6,
	IO7,
	IO8,
	IO9,
	IO10,
	IO11,
	IO12,
	IO13,
	IO14,
	IO15,
	IO16,
	IO17,
	IO18,
	IO19,
	IO20
}GPIOPin_t;

/** @brief Pin direction enumerators */
typedef enum
{
	GPIOOut,
	GPIOIn	
}GPIOPinDir_t;


/****************************************************************************************
* Function prototypes
****************************************************************************************/
void GPIOSetDirection (GPIOPin_t pin, GPIOPinDir_t dir, bool pullup);
void GPIOSetLevel (GPIOPin_t pin, uint8_t level);
bool GPIOGetLevel (GPIOPin_t pin);

#endif /* GPIO_H_ */