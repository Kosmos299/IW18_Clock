/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRIVER_UART_H_
#define DRIVER_UART_H_

void USART1_Send_Char(char c);
void USART1_Config(void);
void USART1_Send_Byte(uint8_t byte);
void USART1_Send_String(const char* s);

//void USART1_IRQHandler(void);


#endif /* DRIVER_UART_H_ */
