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
* @file		button.c
* @brief	This module implements driver for on board button.
* @date		05.10.2019
* @version	0.1
*/


/****************************************************************************************
* Include files
****************************************************************************************/
#include "button.h"


/**
* @brief     Initializes a on board button.
* @param     driver If driver is set to BTN_CALLBACK, driver is initialized with
*			  callback, othewrise with pooling
* @return    None.
*/
void buttonInit (btn_drv_type_t driver)
{
	struct port_config btnPort;
	struct extint_chan_conf extinChan;
	
	port_get_config_defaults(&btnPort);
	btnPort.direction = PORT_PIN_DIR_INPUT;
	btnPort.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(BTN_PIN, &btnPort);
	if(driver == BTN_CALLBACK)
	{
		extinChan.gpio_pin = BTN_PIN;
		extinChan.gpio_pin_mux = BTN_EXTINT;
		extinChan.gpio_pin_pull = EXTINT_PULL_UP;
		extinChan.detection_criteria = EXTINT_DETECT_FALLING;
		
		extint_chan_set_config(8, &extinChan);
		extint_chan_enable_callback(8, EXTINT_CALLBACK_TYPE_DETECT);
		system_interrupt_enable_global();
		/* TODO: add callback initialization */
	}
}


/**
* @brief     Reads the state of on board button.
* @param     None.
* @return    True if button is pressed otherwise false.
*/
bool buttonGet (void)
{
	if(port_pin_get_input_level(BTN_PIN))
	{
		return false;
	}
	else
	{
		return true;
	}
}


/**
* @brief     Attaches callback function to button press.
* @param     Pointer to callback function.
* @return    None.
*/

void buttonAttachCallback (void(*btnCallback)(void))
{
	extint_register_callback(btnCallback, 8, EXTINT_CALLBACK_TYPE_DETECT);
}


/**
* @brief     Enables button callback.
* @param     None.
* @return    None.
*/
void buttonCallbackEnable (void)
{
	extint_chan_enable_callback(8, EXTINT_CALLBACK_TYPE_DETECT);
}


/**
* @brief     Disables button callback.
* @param     None.
* @return    None.
*/
void buttonCallbackDisable(void)
{
	extint_chan_disable_callback(8, EXTINT_CALLBACK_TYPE_DETECT);
}

