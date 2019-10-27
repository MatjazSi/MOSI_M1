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
* @file		STimer.h
* @date		22.10.2019
* @version	0.1
* @brief	Software emulated timer
*
* @details
* Using Software timer: 
* This code emulates hardware timer. With it you can create many different timers
* with 1ms resolution, by only occupying one hardware timer (TC3).
* 
* Each timer can be configured ac timer (count down) or stopwatch (upcount).
* When in count down mode, timer will count from set valoe to zero and then trigger 
* attached callback function (if function has been attached prior to starting a timer).
* If auto reload flag is set to 1, timer will automatically start another cycle, otherwise
* it will remain stopped.
* 
* In stopwatch mode, timer will count from set value upwards until it reaches its maximum
* value (it is a 32 bit timer). In this mode timer is intended to be read by using 
* stimergetTime function.
* 
* To use this code one must:
* 1. Define maximum number of software timers (channels) by setting value of 
*    <b>STIMER_NBR</b>  macro in stimer.h
* 2. Set the timer mode
* 3. If mode is set to timer, set expiry time and auto reload
* 4. If you want, set callback function
* 5. Start the timer
*/


#ifndef STIMER_H_
#define STIMER_H_

/****************************************************************************************
* Include files
****************************************************************************************/
#include "samd21.h"

/****************************************************************************************
* Macro definitions
****************************************************************************************/
/** @brief Maximum number of channels */
#define STIMER_NBR	5

/****************************************************************************************
* Type definitions
****************************************************************************************/
typedef struct
{
	uint32_t time;
	uint8_t running;
	uint8_t stopwatch; // if true timer is upcounting and callback function is disabled
	uint32_t auto_reload;
	volatile void (*function) (void);
}stimer_ch_t;

/****************************************************************************************
* Function prototypes
****************************************************************************************/
void stimerInit (void);
uint32_t stimerSetTime(uint8_t timer, uint32_t t, uint8_t autoreload);
uint32_t stimerStart(uint8_t timer);
uint32_t stimerStop (uint8_t timer);
uint32_t stimerReset (uint8_t timer);
uint32_t stimerGetTime (uint8_t timer);
uint32_t stimerSetAsStopwatch (uint8_t timer);
uint32_t stimerSetAsTimer (uint8_t timer);
uint32_t stimerRegisterCallback (uint8_t timer, void(*funct)(void));
uint32_t stimerUnregisterCallback (uint8_t timer);



#endif /* STIMER_H_ */