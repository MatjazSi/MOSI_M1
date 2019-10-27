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
* @file		main.c
* @date		25.10.2019
* @version	0.1
* @brief	Demo program for MOSI M1 module
*
* @details
* This is demo firmware for MOSI M1 Lite module. It demonstrates use of LED, button, 
* display and software timer. Device constantly cycles through all LED colors. On button
* press toggling alternates between faster or slower. Each button press is also counted
* and displayed on OLED display.
*
* Led cycling and button reading is done using software timer. At the beginning of code
* LED timer is configured to change LED color every 600 ms. Button timer is configure to
* read button every 10 ms. This way all pulses generated when button changes state are 
* ignored.
*
* After power up, OLED shows logo for a short time, then it displays the number of
* button presses. Because updating OLED display over I2C is a slow task, it is preformed
* in main loop and not in the software timer callback function. That way interrupts from
* other sources can be executed while OLED is being updated. Display is only updated, if
* new value of button preses is different form the old one. 
*/



#include <asf.h>
#include "led.h"
#include "button.h"
#include "I2C_Int.h"
#include "display.h"
//#include "GPIO.h"
#include "STimer.h"
#include "logo.h"

volatile bool displayNewData = false;
volatile uint16_t ledUpdateInterval = 600;
volatile uint32_t btnPressCnt = 0;

#define LED_TMR	0
#define BTN_TMR	1
#define CLK_TMR	2

/* button reading task */
void buttonTask (void)
{
	static bool pressed = false;
	/* By using pressed variable, only changes in button state can be detected */
	if(pressed == false)
	{
		if(buttonGet())
		{
			pressed = true;
			btnPressCnt++;
			if(ledUpdateInterval > 500)
			{
				ledUpdateInterval = 60;
			}	
			else
			{
				ledUpdateInterval = 600;
			}
		}
	}
	else
	{
		if(!buttonGet())
		{
			pressed = false;
		}
	}

}

/* led switching task */
void ledTask (void)
{
	static uint16_t currentUpdateIterval = 1000;
	static uint8_t ledState = 0;
	switch (ledState)
	{
		case 0:
			ledClearR();
			ledSetG();
			ledState = 1;
			break;
		case 1:
			ledClearG();
			ledSetB();
			ledState = 2;
			break;
		case 2:
			ledClearB();
			ledSetR();
			ledState = 0;
			break;
	}
	if(ledUpdateInterval != currentUpdateIterval)
	{
		currentUpdateIterval = ledUpdateInterval;
		stimerSetTime(LED_TMR, currentUpdateIterval, 1);
	}
}

/* Update display with new button preses ncount */
void pressCntUpdate (void)
{
	char str[25];
	/* Delete old string */
	UG_FillFrame(5, 50, 120, 60, 0);
	sprintf(str, "Button pressed: %3u", btnPressCnt);
	UG_FontSelect(&FONT_5X12);
	/*Write new string */
	UG_PutString(5, 50, str);
}



/* Main loop */
int main (void)
{
	
	static uint16_t curretnBtnPressCnt = 0;
	
	/* Initialize the system */
	system_init();
	ledInit();
	displayInit();
	buttonInit(BTN_POLLING);
	stimerInit();
	
	/*Logo display */
	ledSetR();
	UG_FontSelect(&FONT_5X12);
	displayDrawImage(128, 64, sw_logo);
	displayUpdate();
	
	/* CLK_TMR is used to stop program execution for 2 s */
	stimerSetTime(CLK_TMR, 2000, 0);
	stimerStart(CLK_TMR);
	while(stimerGetTime(CLK_TMR))
	{
		
	}
	
	displayClear();
	
	/* Setup all other software timers */
	stimerSetTime(LED_TMR, ledUpdateInterval, 1);
	stimerRegisterCallback(LED_TMR, ledTask);
	stimerStart(LED_TMR);
	
	stimerSetTime(BTN_TMR, 10, 1);
	stimerRegisterCallback(BTN_TMR, buttonTask);
	stimerStart(BTN_TMR);
	
	/* Write stuff on OLED */
	UG_FontSelect(&FONT_10X16);
	UG_FillScreen(0);
	UG_PutString(5, 10, "MOSI M1");
	UG_PutString(65, 28, "DEMO");
	pressCntUpdate();
	displayUpdate();
	UG_FontSelect(&FONT_5X12);
	
	while(1)
	{
		/* If button has been pressed since last loop execution
		   update display with new count */
		if(curretnBtnPressCnt != btnPressCnt)
		{
			curretnBtnPressCnt = btnPressCnt;
			pressCntUpdate();
			displayUpdate();
		}
	}
	
	
}


