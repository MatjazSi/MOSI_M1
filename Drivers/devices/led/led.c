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
* @file		led.c
* @brief	This module implements driver for on board LED.
* @date		04.10.2019
* @version	0.1
*/


/****************************************************************************************
* Include files
****************************************************************************************/
#include "led.h"

/**
* @brief     Initializes a on board LED.
* @param     None.
* @return    None.
*/
void ledInit (void)
{
	struct port_config ledPort;
	port_get_config_defaults(&ledPort);
	ledPort.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(LED_R_PIN, &ledPort);
	port_pin_set_config(LED_G_PIN, &ledPort);
	port_pin_set_config(LED_B_PIN, &ledPort);
	
	port_pin_set_output_level(LED_R_PIN, true);
	port_pin_set_output_level(LED_G_PIN, true);
	port_pin_set_output_level(LED_B_PIN, true);
}

/**
* @brief     Turns on Red LED.
* @param     None.
* @return    None.
*
*/
void ledSetR (void)
{
	port_pin_set_output_level(LED_R_PIN, false);
}


/**
* @brief     Turns off Red LED.
* @param     None.
* @return    None.
*
*/
void ledClearR (void)
{
	port_pin_set_output_level(LED_R_PIN, true);
}


/**
* @brief     Turns on Green LED.
* @param     None.
* @return    None.
*
*/
void ledSetG (void)
{
	port_pin_set_output_level(LED_G_PIN, false);
}


/**
* @brief     Turns off Green LED.
* @param     None.
* @return    None.
*
*/
void ledClearG (void)
{
	port_pin_set_output_level(LED_G_PIN, true);
}


/**
* @brief     Turns on Blue LED.
* @param     None.
* @return    None.
*
*/
void ledSetB (void)
{
	port_pin_set_output_level(LED_B_PIN, false);
}


/**
* @brief     Turns off Blue LED.
* @param     None.
* @return    None.
*
*/
void ledClearB (void)
{
	port_pin_set_output_level(LED_B_PIN, true);
}



/**
* @brief     Turns on LED according to given bit mask.
*			  bit0 -> R, bit1 -> G, bit2-> B.
* @param     rgb LED bit mask.
* @return    0 on fail, 1 on success
*
*/
uint8_t ledSetRGB (uint8_t rgb)
{
	uint8_t retVal = 1;
	if(rgb > 7)
	{
		/* error */
		retVal = 0;
	}
	else
	{
		/* turn off all LEDs*/
		port_pin_set_output_level(LED_R_PIN, true);
		port_pin_set_output_level(LED_G_PIN, true);
		port_pin_set_output_level(LED_B_PIN, true);
	
		if(rgb & (1 << 0))
		{
			port_pin_set_output_level(LED_R_PIN, false);
		}
	
		if(rgb & (1 << 1))
		{
			port_pin_set_output_level(LED_G_PIN, false);
		}
	
		if(rgb & (1 << 2))
		{
			port_pin_set_output_level(LED_B_PIN, false);
		}
	}
	return retVal;
	
}


