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
* @file		display.h
* @brief	This is driver for on board I2C OLED display with SSD1306 controller.
*			It uses on board I2C for communication.
*			For high level control it uses uGUi library by Achim Dobler, For user mnual
*			please see https://embeddedlightning.com/ugui/
* @date		21.10.2019
* @version	0.1
*/

#ifndef DISPLAY_H_
#define DISPLAY_H_


/****************************************************************************************
* Include files
****************************************************************************************/
#include <stdint.h>
#include "ugui/ugui.h"


/****************************************************************************************
* Macro definitions
****************************************************************************************/

/** @brief Display I2C address */
#define DISPLAY_ADR		0x3C

/* Various display commands. Please see datasheet of SSD1306 */
#define SSD1306_DEFAULT_ADDRESS 0x78
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_SWITCHCAPVCC 0x2
#define SSD1306_NOP 0xE3

/** @brief Display width in pixels */
#define SSD1306_WIDTH 128
/** @brief Display height in pixels */
#define SSD1306_HEIGHT 64
/** @brief SIze of display buffer in bytes */
#define SSD1306_BUFFERSIZE (SSD1306_WIDTH*SSD1306_HEIGHT)/8

/****************************************************************************************
* Function prototypes
****************************************************************************************/
void displayInit (void); /* Initializes I2c and display */
void displayClear (void); /*Clears display */
void pset(UG_U16 x, UG_U16 y, UG_COLOR c);
void displayUpdate (void);
void displayDrawImage (uint32_t x_size, uint32_t y_size, uint8_t *img);



#endif /* DISPLAY_H_ */
