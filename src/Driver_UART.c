#include <BSP.h>
#include <Driver_UART.h>
#include <stm32f10x.h>

#include <string.h>

int __io_putchar(int c)
{
	USART1_Send_Char(c);
    return c;
}

void USART1_Config()
{
	/* Bit configuration structure for GPIOA PIN9 and PIN10 */
	GPIO_InitTypeDef gpioa_init_struct;

    /* GPIOA PIN9 alternative function Tx */
    gpioa_init_struct.GPIO_Pin = EXT_USART_TX;
    gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	gpioa_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(EXT_USART_PORT, &gpioa_init_struct);


    /* GPIOA PIN10 alternative function Rx */
 	gpioa_init_struct.GPIO_Pin = EXT_USART_RX;
	gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	gpioa_init_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(EXT_USART_PORT, &gpioa_init_struct);

    /* USART configuration structure for USART1 */
    USART_InitTypeDef usart1_init_struct;

    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);
    /* Baud rate 9600, 8-bit data, One stop bit
     * No parity, Do both Rx and Tx, No HW flow control
     */

    usart1_init_struct.USART_BaudRate = 56000;
    usart1_init_struct.USART_WordLength = USART_WordLength_8b;
    usart1_init_struct.USART_StopBits = USART_StopBits_1;
    usart1_init_struct.USART_Parity = USART_Parity_No;
    usart1_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart1_init_struct.USART_Mode = /*USART_Mode_Rx | */USART_Mode_Tx;

    /* Configure USART1 */
    USART_Init(USART1, &usart1_init_struct);

}

void USART1_Send_Char(char c)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, c);
}

void USART1_Send_Byte(uint8_t data_written)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // Wait for Empty
	USART_SendData(USART1, data_written); // Send byte
}

void USART1_Send_String(const char* s)
{
    while (*s)
    	USART1_Send_Char(*s++);
}
