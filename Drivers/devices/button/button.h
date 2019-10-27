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
* @file		button.h
* @brief	This module implements driver for on board button.
* @date		05.10.2019
* @version	0.1
*/

#ifndef BUTTON_H_
#define BUTTON_H_

/****************************************************************************************
* Include files
****************************************************************************************/
#include "port.h"
#include "interrupt.h"
#include "extint.h"
#include "system_interrupt.h"
#include <stdbool.h>


/****************************************************************************************
* Macro definitions
****************************************************************************************/
#define BTN_PIN	PIN_PA28
#define BTN_EXTINT	MUX_PA28A_EIC_EXTINT8


/****************************************************************************************
* Type definitions
****************************************************************************************/
typedef enum
{
	BTN_POLLING,
	BTN_CALLBACK	
}btn_drv_type_t;


/****************************************************************************************
* Function prototypes
****************************************************************************************/
void buttonInit (btn_drv_type_t driver);
bool buttonGet (void);
void buttonAttachCallback (void(*btnCallback)(void));
void buttonCallbackEnable (void);
void buttonCallbackDisable (void);



#endif /* BUTTON_H_ */