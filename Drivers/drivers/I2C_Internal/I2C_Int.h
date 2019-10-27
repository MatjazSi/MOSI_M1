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
* @file		I2C_Int.h
* @brief	This is driver for on boaard I2C bus, connecting Thermometer,
*			Accelerometer and OLED Display. This is only basic driver, supporting
*			only non interrupt or DMA operations.
* @date		04.10.2019
* @version	0.1
*/

#ifndef _I2C_INIT_H_
#define _I2C_INIT_H_

/****************************************************************************************
* Include files
****************************************************************************************/
#include <stdint.h>
#include "samd21g18a.h"
#include "sercom.h"
#include "i2c_master.h"
#include "ioport.h"


/****************************************************************************************
* Macro definitions
****************************************************************************************/
/** @brief On board I2C minimum clock frequency in  kHz */
#define I2C_INT_CLK_MIN		50
/** @brief On board I2C maximum clock frequency in  kHz */
#define I2C_INT_CLK_MAX		400
/** @brief On board I2C default clock frequency in  kHz */
#define I2C_INT_CLK_DEFAULT	100


/****************************************************************************************
* Type definitions
****************************************************************************************/

/** @brief On board I2C slave data structure */ 
typedef struct			 
{
	uint8_t *txBuff;			/**< Pointer to transmitt buffer */		
	uint8_t *rxBuff;			/**< Pointer to receive buffer */
	uint16_t txLen;				/**< Lenght of transmitt buffer */
	uint16_t rxLen;				/**< Lenght of receive bufferr */
	uint8_t deviceAddress;		/**< Address of slave */
	uint16_t regAddress;		/**< Register address */
	uint8_t regAddrLen;			/**< Lenght of register address in bytes */
}i2cIntPacket_t;

/** @brief On board I2C return type */
typedef enum
{
	I2C_INT_ERR,		/**< I2C error */
	I2C_INT_BUSY,		/**< I2C bussy */
	I2C_INT_OK			/**< I2C ok */
}i2cIntRet_t;


/****************************************************************************************
* Function prototypes
****************************************************************************************/
void i2cIntInit(uint32_t clk);
i2cIntRet_t i2cIntTx ( i2cIntPacket_t *packet);
i2cIntRet_t i2cIntRx (i2cIntPacket_t *packet);

#endif /*_I2C_INIT_H_ */