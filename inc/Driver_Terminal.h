/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRIVER_TERMINAL_H_
#define DRIVER_TERMINAL_H_

/*** Include *****************************************************************/
#include <stm32f10x.h>
#include <Utilities.h>

/*** Preprocessor definitions ************************************************/
#define EXT_USART1_PORT 	GPIOA
#define EXT_USART1_TX 	GPIO_Pin_9
#define EXT_USART1_RX 	GPIO_Pin_10

/// Terminal port speed.
#define TERMINAL_PORT_BAUDRATE  56000
/// Size of TerminalBuffer.
#define TERMINAL_BUFFER_SIZE    1024
/// Size of CommonBuffer.
#define COMMON_BUFFER_SIZE  200

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

/*** Definition of variables *************************************************/
/* NO DEFINITIONS */

/*** Prototypes of functions *************************************************/
void TERMINAL_Init(void);
void TERMINAL_iTransmitData(void);
bool TERMINAL_IsEnabled(void);
void TERMINAL_Enable(bool enable);
void TERMINAL(const char* text, ...);

void USART1_Send_Char(char c);

#endif /* DRIVER_TERMINAL_H_ */
