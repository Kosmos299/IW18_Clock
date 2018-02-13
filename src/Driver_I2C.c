#include <Driver_I2C.h>

#include <BSP.h>
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

#include "stm32f10x.h"

/**
  * @brief  I2C1 Config
  * Configures I2C1 peripheral and pins required.
  * @param  None
  * @retval None
  */
void I2C1_Config(void){

	GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin = I2C_SCL|I2C_SDA;
    gpio.GPIO_Mode = GPIO_Mode_AF_OD;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_PORT, &gpio);

    I2C_InitTypeDef I2C_InitStruct;
    I2C_StructInit(&I2C_InitStruct);
    /* Configure I2C1 */
    I2C_DeInit(I2C1);

    /* Set the I2C structure parameters */
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0xEE;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStruct.I2C_ClockSpeed = 100000;
    /* Initialize the I2C peripheral w/ selected parameters */
    I2C_Init(I2C1,&I2C_InitStruct);
    /* Enable the I2C peripheral */
    I2C_Cmd(I2C1, ENABLE);

}

/**
  * @brief  Send single byte.
  * Sends single byte of data to address provided,
  * when selecting target register is not required/impossible.
  * Address should be shifted bitwise left once
  * @param uint8_t data_written - payload byte
  * @param uint8_t target_address - address byte
  * @retval None
  */
void I2C1_Byte_Write(uint8_t data_written, uint8_t target_address)
{

	/* Wait until the line is free */
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));								//Check if interface busy
	I2C_GenerateSTART(I2C1, ENABLE);											//Generate start sequence 						- S
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));					//check if transmitter set properly

	I2C_Send7bitAddress(I2C1, target_address, I2C_Direction_Transmitter);   	//Send 7-bit target device address + Write bit 	- AD+W
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); 	//Wait for target acknowledge 					- ACK

	I2C_SendData(I2C1, data_written);								     		//Send 8-bit data - first half					- DATA
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));			//Wait for target acknowledge 					- ACK

	I2C_GenerateSTOP(I2C1, ENABLE); 								    		//Generate stop sequence 						- P
}

/**
  * @brief  Send single byte.
  * Sends single byte of data to address provided,
  * to selected target register.
  * Address should be shifted bitwise left once
  * @param uint8_t data_written - payload byte
  * @param uint8_t target_address - address byte
  * @param uint8_t register_address - target register
  * @retval None
  */
void I2C1_Byte_Write_To_Register(uint8_t data_written, uint8_t register_address,  uint8_t target_address)
{


	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));								//Check if interface busy
	I2C_GenerateSTART(I2C1, ENABLE);											//Generate start sequence 						- S
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));					//check if transmitter set properly

	I2C_Send7bitAddress(I2C1, target_address, I2C_Direction_Transmitter);       //Send 7-bit target device address + Write bit 	- ADR+W
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));	//Wait for target acknowledge 					- ACK

	I2C_SendData(I2C1, register_address);								     	//Send 8-bit target register address 			- RA
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));			//Wait for target acknowledge 					- ACK

	I2C_SendData(I2C1, data_written);								     		//Send 8-bit data 								- DATA
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));			//Wait for target acknowledge 					- ACK

	I2C_GenerateSTOP(I2C1, ENABLE); 								    		//Generate stop sequence 						- P

}

/**
  * @brief  Send two bytes.
  * Sends two bytes of data to address provided,
  * to selected target register.
  * Address should be shifted bitwise left once
  * @param uint16_t data_written - payload
  * @param uint8_t target_address - address byte
  * @param uint8_t register_address - target register
  * @retval None
  */
void I2C1_Two_Byte_Write_To_Register(uint16_t data_written, uint8_t register_address,  uint8_t target_address)
{

	uint8_t data_l, data_h;
	data_l = (0x00FF & data_written);
	data_h = (data_written >> 8);

	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));								//Check if interface busy
	I2C_GenerateSTART(I2C1, ENABLE);											//Generate start sequence 						- S
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));					//check if transmitter set properly

	I2C_Send7bitAddress(I2C1, target_address, I2C_Direction_Transmitter);       //Send 7-bit target device address + Write bit 	- AD+W
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));	//Wait for target acknowledge 					- ACK

	I2C_SendData(I2C1, register_address);								     	//Send 8-bit target register address 			- RA
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));			//Wait for target acknowledge 					- ACK

	I2C_SendData(I2C1, data_h);								     				//Send 8-bit data - first half					- DATA
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));			//Wait for target acknowledge 					- ACK

	I2C_SendData(I2C1, data_l);								     				//Send 8-bit data - second half					- DATA
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));			//Wait for target acknowledge 					- ACK

	I2C_GenerateSTOP(I2C1, ENABLE); 								    		//Generate stop sequence 						- P

}

/////////////////////////////////////

/**
  * @brief  Read single byte.
  * Reads single byte of data from address provided,
  * when selecting target register is not required/impossible.
  * Address should be shifted bitwise left once
  * @param uint8_t target_address - address byte
  * @retval uint8_t - received payload
  */
uint8_t I2C1_Byte_Read(uint8_t target_address)
{
	uint8_t received_data=0;

	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));								//Check if interface busy
	I2C_GenerateSTART(I2C1, ENABLE);											//Generate start sequence 						- S
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));					//check if transmitter set properly

	I2C_Send7bitAddress(I2C1, target_address, I2C_Direction_Receiver);       	//Send 7-bit target device address + Read bit 	- AD+R
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));		//Wait for target acknowledge 					- ACK

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));				//Wait for receive signaling
	received_data = I2C_ReceiveData(I2C1);										//Receive data									- DATA

	I2C_GenerateSTOP(I2C1, ENABLE); 									 		//Generate stop sequence 						- P

	return received_data;
}

/**
  * @brief  Read two bytes.
  * Reads two bytes of data from address provided,
  * when selecting target register is not required/impossible.
  * Address should be shifted bitwise left once
  * @param uint8_t target_address - address byte
  * @retval uint16_t - received payload
  */

/*
uint16_t I2C1_Two_Byte_Read(uint8_t target_address)
{
	uint16_t received_data=0;

	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));								//Check if interface busy
	I2C_GenerateSTART(I2C1, ENABLE);											//Generate start sequence 						- S
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));					//check if transmitter set properly

	I2C_Send7bitAddress(I2C1, target_address, I2C_Direction_Receiver);       	//Send 7-bit target device address + Read bit 	- AD+R
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));		//Wait for target acknowledge 					- ACK

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));				//Wait for receive signaling
	received_data |= (I2C_ReceiveData(I2C1) << 8);								//Receive data									- DATA
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));				//Wait for receive signaling
	received_data |= I2C_ReceiveData(I2C1);										//Receive data									- DATA

	I2C_GenerateSTOP(I2C1, ENABLE); 									 		//Generate stop sequence 						- P

	return received_data;
}
*/

/**
  * @brief  Read two bytes from register.
  * Reads two bytes of data from address provided,
  * from selected target register and next sequential.
  * Address should be shifted bitwise left once
  * @param uint8_t target_address - address byte
  * @param uint8_t register_address - target register
  * @retval uint16_t - received payload
  */
uint16_t I2C1_Two_Byte_Read_From_Register(uint8_t register_address,  uint8_t target_address)
{
	uint16_t received_data = 0;

	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));								//Check if interface busy
	I2C_GenerateSTART(I2C1, ENABLE);											//Generate start sequence 						- S
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));					//check if transmitter set properly

	I2C_Send7bitAddress(I2C1, target_address, I2C_Direction_Transmitter);       //Send 7-bit target device address + Write bit 	- AD+W
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));	//Wait for target acknowledge 					- ACK

	I2C_SendData(I2C1, register_address);								     	//Send 8-bit target register address 			- RA
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));			//Wait for target acknowledge 					- ACK

	I2C_GenerateSTART(I2C1, ENABLE);											//Generate start sequence 						- S !
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));					//check if transmitter set properly

	I2C_Send7bitAddress(I2C1, target_address, I2C_Direction_Receiver);       	//Send 7-bit target device address + Read bit 	- AD+R
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));		//Wait for target acknowledge 					- ACK

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));				//Wait for receive signaling
	received_data |= (I2C_ReceiveData(I2C1) << 8);								//Receive data									- DATA

	I2C_AcknowledgeConfig(I2C1, DISABLE);										//Disable acknowledge check

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));				//Wait for receive signaling
	received_data |= I2C_ReceiveData(I2C1);										//Receive data									- DATA
	I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);						//Do not acknowledge received data 				- NACK

	I2C_GenerateSTOP(I2C1, ENABLE); 								    		//Generate stop sequence 						- P
	I2C_AcknowledgeConfig(I2C1, ENABLE);										//Re-enable acknowledge check

	return received_data;
}

/**
  * @brief  sequential read.
  * Reads specified number bytes of data from address provided,
  * starting at selected target register and incrementing sequentially.
  * Address should be shifted bitwise left once. Data is stored in array under pionter provided
  * @param uint8_t target_address - address byte
  * @param uint8_t register_address - starting register
  * @param uint8_t* data_received - pointer for storing received data
  * @param uint8_t byte_count - number of bytes to read
  * @retval none
  */
void I2C1_N_Byte_Burst_Read_From_Register(uint8_t target_address, uint8_t register_address, uint8_t* data_received, uint8_t byte_count)
{
	int i;

	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));								//Check if interface busy
	I2C_GenerateSTART(I2C1, ENABLE);											//Generate start sequence 						- S
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));					//check if transmitter set properly

	I2C_Send7bitAddress(I2C1, target_address, I2C_Direction_Transmitter);       //Send 7-bit target device address + Write bit 	- AD+W
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));	//Wait for target acknowledge 					- ACK

	I2C_SendData(I2C1, register_address);								     	//Send 8-bit target register address 			- RA
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));			//Wait for target acknowledge 					- ACK

	I2C_GenerateSTART(I2C1, ENABLE);											//Generate start sequence 						- S !
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));					//check if transmitter set properly

	I2C_Send7bitAddress(I2C1, target_address, I2C_Direction_Receiver);       	//Send 7-bit target device address + Read bit 	- AD+R
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));		//Wait for target acknowledge 					- ACK

	for (i=0 ; i < byte_count-1 ; i++)
	{
	// (N-1) consecutive single byte reads, each acknowledged
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));				//Receive data									- DATA
	data_received[i] = I2C_ReceiveData(I2C1);									//Wait for target acknowledge 					- ACK
	}

	I2C_AcknowledgeConfig(I2C1, DISABLE);										//Disable acknowledge check

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));				//Wait for receive signaling
	data_received[byte_count] = I2C_ReceiveData(I2C1);							//Receive data									- DATA
	I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);						//Do not acknowledge received data 				- NACK

	I2C_GenerateSTOP(I2C1, ENABLE); 								    		//Generate stop sequence 						- P
	I2C_AcknowledgeConfig(I2C1, ENABLE);										//Re-enable acknowledge check

}
