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
* @file		STimer.c
* @date		22.10.2019
* @version	0.1
* @brief	Software emulated timer
*
* @details
* Using Software timer 
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


/****************************************************************************************
* Include files
****************************************************************************************/
#include "STimer.h"

/****************************************************************************************
* Global variables
****************************************************************************************/
stimer_ch_t stimer_times[STIMER_NBR];


/**
* @brief     Initializes hardware timer to trigger
*			 software timer handle
*
*/
void stimerInit (void)
{
	GCLK->GENDIV.bit.ID = 2;
	GCLK->GENDIV.bit.DIV = 12;

	GCLK->GENCTRL.bit.ID = 2;
	GCLK->GENCTRL.reg |= GCLK_GENCTRL_SRC_DFLL48M;
	GCLK->GENCTRL.bit.GENEN = 1;
	GCLK->GENCTRL.bit.OE = 1;

	GCLK->CLKCTRL.bit.ID = GCLK_CLKCTRL_ID_TCC2_TC3;
	GCLK->CLKCTRL.bit.GEN = 2;
	GCLK->CLKCTRL.bit.CLKEN  = 1;

	PM->APBCMASK.reg |= PM_APBCMASK_TC3; // enable clock for TC3

	/* Set timer to 8 bit mode */
	TC3->COUNT8.CTRLA.reg |= TC_CTRLA_MODE_COUNT8;
	/* Set prescaler to 16 */
	TC3->COUNT8.CTRLA.bit.PRESCALER = TC_CTRLA_PRESCALER_DIV16_Val;


	/*Enable timer */
	TC3->COUNT8.CTRLA.reg |= TC_CTRLA_ENABLE;
	/*Wait for clock sync */
	while(TC3->COUNT8.STATUS.bit.SYNCBUSY == 1) {}
	/*Set period */
	TC3->COUNT8.PER.reg = 249;
	/*Enable match interrupt */
	TC3->COUNT8.INTENSET.reg |= TC_INTENSET_MC0;
	NVIC_EnableIRQ(TC3_IRQn);
	__enable_irq();
	/*Start timer */
	TC3->COUNT8.CTRLBSET.reg = TC_CTRLBSET_CMD_RETRIGGER;
		 
}


void TC3_Handler(void)
{
	uint32_t n;
	uint32_t ul_dummy;
	TC3->COUNT8.INTFLAG.reg |= TC_INTFLAG_MC0;
	for(n = 0; n < STIMER_NBR; n++)
	{
		if(stimer_times[n].running)
		{
			if(stimer_times[n].stopwatch) // upcounting mode
			{
				stimer_times[n].time++;
			}
			else
			{
				stimer_times[n].time--;
				if(!stimer_times[n].time) // timer has expired
				{
					if(stimer_times[n].function)
					{
						stimer_times[n].function(); // call callback function
					}
					if(stimer_times[n].auto_reload)
					{
						stimer_times[n].time = stimer_times[n].auto_reload;
					}
					else
					{
						stimer_times[n].running = 0;
					}
				}
			}
		}
	}	
}

/**
* @brief     Sets timer on a software timer channel
* @param	 timer, timer channel
* @param     t time in milliseconds
* @param	 autoreload If set to 1 timer will automatically reload to set
*			 value and start another countdown.	
* @return	 0 on error, 1 on success
*/

uint32_t stimerSetTime(uint8_t timer, uint32_t t, uint8_t autoreload)
{
	if(timer > (STIMER_NBR - 1))
	{
		return 0;
	}
	stimer_times[timer].time = t;
	if(autoreload)
	{
		stimer_times[timer].auto_reload = t;
	}
	else
	{
		stimer_times[timer].auto_reload = 0;
	}
	return 1;
}

/**
* @brief     Starts software timer
* @param	 timer, timer channel
* @return	 0 on error, 1 on success
*/
uint32_t stimerStart(uint8_t timer)
{
	if(timer > (STIMER_NBR - 1))
	{
		return 0;
	}
	if(!stimer_times[timer].running) //check that timer is not running yet
	{
		if(!stimer_times[timer].stopwatch) // downcounting mode
		{
			if(!stimer_times[timer].time)
			{
				return 0;
			}
		}
		stimer_times[timer].running = 1;
	}
	return 1;
}

/**
* @brief     Stops software timer
* @param	 timer, timer channel
* @return	 0 on error, 1 on success
*/
uint32_t stimerStop (uint8_t timer)
{
	if(timer > (STIMER_NBR - 1))
	{
		return 0;
	}
	stimer_times[timer].running = 0;
	return 1;
}

/**
* @brief     Resets software timer
* @param	 timer, timer channel
* @return	 0 on error, 1 on success
*/
uint32_t stimerReset (uint8_t timer)
{
	if(timer > (STIMER_NBR - 1))
	{
		return 0;
	}
	stimer_times[timer].time = 0;
	return 1;
	
}

/**
* @brief     Returns time stored in timer 
* @param	 timer, timer channel
* @return	 Time stored in timer in milliseconds
*/
uint32_t stimerGetTime (uint8_t timer)
{
	return stimer_times[timer].time;
}

/**
* @brief     Configures timer as stopwatch.
*			 Timer will count up and never reset.
* @param	 timer, timer channel
* @return	 0 on error, 1 on success
*/
uint32_t stimerSetAsStopwatch (uint8_t timer)
{
	if(timer > (STIMER_NBR - 1))
	{
		return 0;
	}
	stimer_times[timer].stopwatch = 1;
	return 1;
}

/**
* @brief     Configures timer as timer.
*			 Timer will count down.
* @param	 timer, timer channel
* @return	 0 on error, 1 on success
*/
uint32_t stimerSetAsTimer (uint8_t timer)
{
	if(timer > (STIMER_NBR - 1))
	{
		return 0;
	}
	stimer_times[timer].stopwatch = 0;
	return 1;
}

/**
* @brief     Registers callback for timer. When timer counts to 0,
*			 callback function will get called.
* @param	 timer, timer channel
* @return	 0 on error, 1 on success
*/
uint32_t stimerRegisterCallback (uint8_t timer, void(*funct)(void))
{
	if(timer > (STIMER_NBR - 1))
	{
		return 0;
	}
	stimer_times[timer].function = funct;
	return 1;
}

/**
* @brief     Removes callback function from timer
* @param	 timer, timer channel
* @return	 0 on error, 1 on success
*/
uint32_t stimerUnregisterCallback (uint8_t timer)
{
	if(timer > (STIMER_NBR - 1))
	{
		return 0;
	}
	stimer_times[timer].function = 0;
	return 1;
}