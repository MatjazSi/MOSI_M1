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
* @file		I2C_Int.c
* @brief	This is driver for on boaard I2C bus, connecting Thermometer,
*			Accelerometer and OLED Display. This is only basic driver, supporting
			only non interrupt or DMA operations.
* @date		04.10.2019
* @version	1.0
*/

/****************************************************************************************
* Include files
****************************************************************************************/
#include "I2C_Int.h"


/****************************************************************************************
* Global variables
****************************************************************************************/
struct i2c_master_module i2cMasterModule;
struct i2c_master_packet i2cData;


/**
* @param     clk I2C clock frequency in kHz
* @return    None.
*
*/
void i2cIntInit(uint32_t clk)
{ 
	struct i2c_master_config i2c;
	
	/* Validate clk and set it to default if it is not ok */
	if(clk > I2C_INT_CLK_MAX)
	{
		clk = I2C_INT_CLK_DEFAULT;
	}
	else if (clk < I2C_INT_CLK_MIN)
	{
		clk = I2C_INT_CLK_DEFAULT;
	}
	
	/*Set Pin MUx for I2C Module */
	PORT->Group[0].PMUX[4].bit.PMUXE = 3;
	PORT->Group[0].PMUX[4].bit.PMUXO = 3;
	PORT->Group[0].PINCFG[8].bit.PMUXEN |= (1 << 0);
	PORT->Group[0].PINCFG[9].bit.PMUXEN |= (1 << 0);
		
	i2c_master_get_config_defaults(&i2c);
	i2c.baud_rate = clk;
	i2c.buffer_timeout = 100;
	//i2cMasterModule.buffer_timeout = 10;
	//i2cMasterModule.unknown_bus_state_timeout = 1;
	i2c_master_init(&i2cMasterModule, SERCOM2, &i2c);
	i2c_master_enable(&i2cMasterModule);
		
}


/**
* @brief     Transmits data on on board I2C
* @param     packet Pointer to a structure holding data and settings for transmission
* @return    I2C_INT_OK on success
*			 I2C_INT_ERR on error 
*
*/
i2cIntRet_t i2cIntTx (i2cIntPacket_t *packet)
{
	uint8_t regAddr[2];
	uint16_t n;
	
	i2cIntRet_t retVal = I2C_INT_ERR;
	if(packet->txLen)
	{
		/* Transmit device address and register address */
		regAddr[0] = (uint8_t)(packet->regAddress >> 8) & 0x00FF;
		regAddr[1] = (uint8_t)packet->regAddress & 0x00FF;
		i2cData.address = packet->deviceAddress;
		i2cData.data_length = packet->regAddrLen;
		i2cData.data = packet->regAddrLen > 1 ? &regAddr[0] : &regAddr[1];
		i2c_master_write_packet_wait_no_stop(&i2cMasterModule, &i2cData);
		
		/* Now write data */
		for(n = 0; n < packet->txLen; n++)
		{
			i2c_master_write_byte(&i2cMasterModule, *((packet->txBuff) + n));
		}
		i2c_master_send_stop(&i2cMasterModule);
		retVal = I2C_INT_OK;
		
	}
	return retVal;
}


/**
* @brief     Receives data on on board I2C
* @param     packet Pointer to a structure holding data and settings for reception
* @return    I2C_INT_OK on success
*			  I2C_INT_ERR on error
*
*/
i2cIntRet_t i2cIntRx (i2cIntPacket_t *packet)
{
	uint8_t regAddr[2];

	
	i2cIntRet_t retVal = I2C_INT_ERR;
	if(packet->rxLen)
	{
		/* Transmit device address and register address */
		regAddr[0] = (uint8_t)(packet->regAddress >> 8) & 0x00FF;
		regAddr[1] = (uint8_t)packet->regAddress & 0x00FF;
		i2cData.address = packet->deviceAddress;
		i2cData.data_length = packet->regAddrLen;
		i2cData.data = packet->regAddrLen > 1 ? &regAddr[0] : &regAddr[1];
		i2c_master_write_packet_wait_no_stop(&i2cMasterModule, &i2cData);
		
		/* Now receive data */
		i2cData.data = packet->rxBuff;
		i2cData.data_length = packet->rxLen;
		i2c_master_read_packet_wait(&i2cMasterModule, &i2cData);
		
	}
}
