#ifndef Driver_Measrements_
#define Driver_Measrements_

#include <stdint.h>

#define I2C_PORT GPIOB
#define I2C_SCL 		GPIO_Pin_6
#define I2C_SDA 		GPIO_Pin_7

/* SHT21 register definition */
#define SHT_ADDR 	0x80 //0x40 << 1
#define TRIG_T_HM 	0xE3
#define TRIG_H_HM 	0xE5
#define TRIG_T_NHM 	0xF3
#define TRIG_H_NHM 	0xF5
#define USR_W 		0xE6
#define URS_R 		0xE7
#define SOFT_RST 	0xFE

/* SHT21 User register content
 *
 * +-------+------------------------+----------+
 * | Bits: | Description:           | Default: |
 * +-------+------------------------+----------+
 * | 7,0   | Measurement resolution | 00       |
 * +-------+------------------------+----------+
 * | 6     | Status: End of battery | 0        |
 * +-------+------------------------+----------+
 * | 5,4,3 | Reserved               |          |
 * +-------+------------------------+----------+
 * | 2     | Enable on-chip heater  | 0        |
 * +-------+------------------------+----------+
 * | 1     | Disable OTP Reload     | 1        |
 * +-------+------------------------+----------+
 *
 * +----------+--------+--------+
 * | Setbits  | RH     | T      |
 * +----------+--------+--------+
 * | "00"     | 12bits | 14bits |
 * +----------+--------+--------+
 * | "01"     | 8bits  | 12bits |
 * +----------+--------+--------+
 * | "10"     | 10bits | 13bits |
 * +----------+--------+--------+
 * | "11"     | 11bits | 11bits |
 * +----------+--------+--------+
 *
 */

#define SHT_14b	00000000b
#define SHT_13b	10000000b
#define SHT_12b	00000001b
#define SHT_11b	10000001b

void I2C1_Config(void);

void I2C1_Byte_Write(uint8_t data_written, uint8_t target_address);
void I2C1_Byte_Write_To_Register(uint8_t data_written, uint8_t register_address,  uint8_t target_address);
void I2C1_Two_Byte_Write_To_Register(uint16_t data_written, uint8_t register_address,  uint8_t target_address);

uint8_t I2C1_Byte_Read(uint8_t address_r);
uint16_t I2C1_Two_Byte_Read(uint8_t target_address);
//uint16_t I2C1_Two_Byte_Read_From_Register(uint8_t register_address, uint8_t target_address);
void I2C1_N_Byte_Burst_Read_From_Register(uint8_t target_address, uint8_t register_address, uint8_t* data_received, uint8_t byte_count);

void DBG_Test_SHT(void);

#endif
