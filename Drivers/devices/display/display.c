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
* @file		display.c
* @brief	This is driver for on board I2C OLED display with SSD1306 controller.
*			It uses on board I2C for communication.
*			For high level control it uses uGUi library by Achim Dobler, For user mnual
*			please see https://embeddedlightning.com/ugui/
* @date		21.10.2019
* @version	0.1
*/


/****************************************************************************************
* Include files
****************************************************************************************/
#include "display.h"
#include "font.h"
#include "ugui/ugui.h"

#include "I2C_Int.h"
#include <stdint.h>

/****************************************************************************************
* Global variables
****************************************************************************************/
/** @brief uGui display object */
UG_GUI Gui;
/** @brief Display buffer */
uint8_t displayBuffer[128 * 64 / 8]; // 128 * 8 display resolution 
/** @brief I2C Display packet object */
i2cIntPacket_t displayI2CPacket;



/**
 * @brief Writes data do dispaly registers
 *
 * @param reg_addr Register addres
 * @param value Value to write to register
 */
static void display_write_reg (uint8_t reg_addr, uint8_t value)
{
	
	displayI2CPacket.regAddress = reg_addr;
	displayI2CPacket.regAddrLen = 1;
	displayI2CPacket.txBuff = &value;	
	displayI2CPacket.txLen = 1;
	
	i2cIntTx(&displayI2CPacket);
}

/**
 * @brief Writes comand to display
 *
 * @param com Comand number
 */
static void display_comand (uint8_t com)
{
	display_write_reg(0, com);
}

/**
 * @brief Writes data to display
 *
 * @param data Data to be written
 */
static void display_data (uint8_t data)
{
	display_write_reg(64, data);
}

/**
 * @brief Initiates I2C interface in microcontroller
 *
 */
static void twi_init (void)
{
	displayI2CPacket.deviceAddress = DISPLAY_ADR;	
	i2cIntInit(400);
}
/**
 * @brief Selects a column to write to
 *
 * @param col Selected column
 */
static void display_select_column (uint8_t col)
{
	if(col > 128)
	{
		col = 128;
	}
	display_comand(0x0F & (uint8_t)col);
	display_comand(0x10 | (col >> 4));
}

/**
 * @brief Selects a row to write to
 *
 * @param row Selected row
 */
static void display_select_row (uint8_t row)
{
	if(row > 8)
	{
		row = 8;
	}
	display_comand(0xB0 | row);
}


/**
 * @brief Writes an single pixel in display buffer
 *
 * @param x X coordinate
 * @param y Y coordinate
 * @param c C color
 */
void pset(UG_U16 x, UG_U16 y, UG_COLOR c)
{
	unsigned int i,p;

	if ( x > 127 ) return;
	p = y>>3; // :8
	p = p<<7; // *128
	p +=x;

	if( c )
	{
		displayBuffer[p] |= 1 << (y % 8);
	}
	else
	{
		displayBuffer[p] &= ~(1 << (y % 8));
	}

}

/**
 * @brief Clear the dispaly
 *
 */
void displayClear (void)
{
	uint8_t page, col;
	for(page = 0; page < 8; page++)
	{
		display_select_row(page);
		display_select_column(0);
		for(col = 0; col < 128; col++)
		{
			display_data(0x00);
		}
		
	}
}

/**
 * @brief Initializes the display
 *
 * This function should be called before using display
 */
void displayInit (void)
{
	
	twi_init();
	display_comand(SSD1306_DISPLAYOFF);

	display_comand(SSD1306_SETDISPLAYCLOCKDIV);
	display_comand(0x80);

	display_comand(SSD1306_SETMULTIPLEX);
	display_comand(0x3F);
	
	display_comand(SSD1306_SETDISPLAYOFFSET);
	display_comand(0x00);
	
	display_comand(SSD1306_SETSTARTLINE | 0x00);
	
	// We use internal charge pump
	display_comand(SSD1306_CHARGEPUMP);
	display_comand(0x14);
	
	// Horizontal memory mode
	display_comand(SSD1306_MEMORYMODE);
	display_comand(0x00);
	
	display_comand(SSD1306_SEGREMAP | 0x1);

	display_comand(SSD1306_COMSCANDEC);

	display_comand(SSD1306_SETCOMPINS);
	display_comand(0x12);

	// Max contrast
	display_comand(SSD1306_SETCONTRAST);
	display_comand(0xCF);

	display_comand(SSD1306_SETPRECHARGE);
	display_comand(0xF1);

	display_comand(SSD1306_SETVCOMDETECT);
	display_comand(0x40);

	display_comand(SSD1306_DISPLAYALLON_RESUME);

	// Non-inverted display
	display_comand(SSD1306_NORMALDISPLAY);
	
	// Turn display back on
	display_comand(SSD1306_DISPLAYON);
		
	displayClear();
	UG_Init(&Gui, pset, 128, 64);
	

}

 
/**
 * @brief Draws data from buffer to display
 *
 */
void displayUpdate (void)
{
	uint32_t n;
	display_select_column(0);
	display_select_row(0);
	for(n = 0; n < (128 * 64 / 8); n++)
	{
		display_data(displayBuffer[n]);
	}
}


/**
 * @brief Draws a picture stored in RAM to display
 *
 * @param x X size of image
 * @param y Y size of image
 * @param img Pointer to the image
 */
void displayDrawImage (uint32_t x_size, uint32_t y_size, uint8_t *img)
{
	uint32_t x, y, n, byte;
	for(y = 0; y < y_size; y++)
	{
		for(x = 0; x < x_size; x++)
		{
			byte = ((y * (x_size )) + x) / 8;
			n = x % 8;
			if((*(img + byte)) & (1 <<  7 - n))
			{
				pset(x, y, 1);
			}
			else
			{
				pset(x, y, 0);
			}
		}
	}
	
}




