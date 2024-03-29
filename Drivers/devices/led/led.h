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
* @file		led.h
* @brief	This module implements driver for on board LED.
* @date		04.10.2019
* @version	0.1
*
*/

#ifndef LED_H_
#define LED_H_

/****************************************************************************************
* Include files
****************************************************************************************/
#include "port.h"


/****************************************************************************************
* Macro definitions
****************************************************************************************/
#define LED_R_PIN PIN_PA12
#define LED_G_PIN PIN_PB10
#define LED_B_PIN PIN_PB11



/****************************************************************************************
* Function prototypes
****************************************************************************************/
void ledInit (void);
void ledSetR (void);
void ledClearR (void);
void ledSetG (void);
void ledClearG (void);
void ledSetB (void);
void ledClearB (void);
uint8_t ledSetRGB (uint8_t rgb);




#endif /* LED_H_ */