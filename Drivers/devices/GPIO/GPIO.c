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
* @file		GPIO.c
* @brief	Driver for GPIO pins
* @date		22.10.2019
* @version	0.1
*/

/****************************************************************************************
* Include files
****************************************************************************************/

#include "GPIO.h"


/****************************************************************************************
* Global variables
****************************************************************************************/

/** @brief Constant variable that maps logic pin number to phizical GPIO pin */
const uint8_t GPIOMap [GPIO_NBR] ={
	PIN_PA28, PIN_PA27, PIN_PB23, PIN_PA20,
	PIN_PA00, PIN_PA01, PIN_PA07, PIN_PA10,
	PIN_PA11, PIN_PA13, PIN_PA02, PIN_PA03,
	PIN_PB08, PIN_PB09, PIN_PB02, PIN_PA04,
	PIN_PA05, PIN_PA06, PIN_PA21, PIN_PA22,
	PIN_PA23								
};

/**
* @brief    Sets selected pin as output or input
* @param	pin Pin we want to set 
* @param    dir Direction of pin
* @param	pullup If true pullup will be enabled if 
*			pin is set as input. Otherwise it will be ignored
*/
void GPIOSetDirection (GPIOPin_t pin, GPIOPinDir_t dir, bool pullup)
{
	struct port_config conf;
	port_get_config_defaults(&conf);
	/*set structure to input or output */
	dir == GPIOOut ? (conf.direction = PORT_PIN_DIR_OUTPUT) : (conf.direction = PORT_PIN_DIR_INPUT);
	if(dir == GPIOIn)
	{
		pullup ? (conf.input_pull = PORT_PIN_PULL_UP) : (conf.input_pull = PORT_PIN_PULL_NONE);
	}
	port_pin_set_config(GPIOMap[pin], &conf);
}

/**
* @brief     Sets output state of pin
* @param	 pin Pin we want to set
* @param     level Logic level of pin (1 or 0)
*
*/
void GPIOSetLevel (GPIOPin_t pin, uint8_t level)
{
	level ? port_pin_set_output_level(GPIOMap[pin], true) : port_pin_set_output_level(GPIOMap[pin], false);
}

/**
* @brief     Reads state of the pin
* @param	 pin Pin we want to read
* @return    level Logic level of pin (true -> hign, false -> low)
*
*/
bool GPIOGetLevel (GPIOPin_t pin)
{
	bool ret;
	port_pin_get_input_level(GPIOMap[pin]) ? (ret = true) : (ret = false);
	return ret;
}

