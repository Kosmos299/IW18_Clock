/*** Include *****************************************************************/
#include <Driver_Terminal.h>

#include <stm32f10x.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_conf.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <Utilities.h>

/*** Preprocessor definitions ************************************************/
/* NO DEFINITIONS */

/*** Definition of variables *************************************************/
bool fbDebugEnabled = TRUE;

/// Definition of cyclic terminal buffer.
struct
{
  slong slPosIn, slPosOut;	//int
  slong islCounter;	//int
  schar aslTable[TERMINAL_BUFFER_SIZE];	//char
} TerminalBuffer;

/// Buffer used to commons routines in background.
/// Usually called by stdio library functions.
static char ascCommonBuffer[COMMON_BUFFER_SIZE];

/*** Prototypes of functions *************************************************/
/* NO DEFINITIONS */
/*** Definitions of functions *************************************************/
void TERMINAL_Init()
{
	/* Bit configuration structure for GPIOA PIN9 and PIN10 */
	GPIO_InitTypeDef gpioa_init_struct;

    /* GPIOA PIN9 alternative function Tx */
    gpioa_init_struct.GPIO_Pin = EXT_USART1_TX;
    gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	gpioa_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(EXT_USART1_PORT, &gpioa_init_struct);


    /* GPIOA PIN10 alternative function Rx */
 	gpioa_init_struct.GPIO_Pin = EXT_USART1_RX;
	gpioa_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	gpioa_init_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(EXT_USART1_PORT, &gpioa_init_struct);

    /* USART configuration structure for USART1 */
    USART_InitTypeDef usart1_init_struct;

    usart1_init_struct.USART_BaudRate = TERMINAL_PORT_BAUDRATE;
    usart1_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart1_init_struct.USART_Mode = /*USART_Mode_Rx | */USART_Mode_Tx;
    usart1_init_struct.USART_Parity = USART_Parity_No;
    usart1_init_struct.USART_StopBits = USART_StopBits_1;
    usart1_init_struct.USART_WordLength = USART_WordLength_8b;

    /* Configure USART1 */
    USART_Init(USART1, &usart1_init_struct);

    NVIC_InitTypeDef stNVIC_Init;

    /* NVIC configuration */
    stNVIC_Init.NVIC_IRQChannel = USART1_IRQn;
    stNVIC_Init.NVIC_IRQChannelPreemptionPriority = 15;
    stNVIC_Init.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&stNVIC_Init);

    /* Enable the USART Transmit interrupt */
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

    /* Enable the USART Receive interrupt */
    //  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);

}
/////////////////////////////////////////////////////////////////////////



/******************************************************
 ** Name            : TerminalBufferPut
 **
 ** Created from/on : RM / 16.11.2015
 **
 ** Description     : Puts scharacter to the cyclic
 **                   terminal buffer
 **
 ** Calling         : Not important
 **
 ** InputValues     : variable - byte which should be added to buffer
 ** OutputValues    : TRUE - if data added successfully
 **                   FALSE - if buffer is already full
 ******************************************************/
static bool TerminalBufferPut(schar scVariable)
{
  if (TerminalBuffer.islCounter == TERMINAL_BUFFER_SIZE)
  {
    return FALSE;
  }
  TerminalBuffer.aslTable[TerminalBuffer.slPosIn] = scVariable;
  TerminalBuffer.islCounter++;
  TerminalBuffer.slPosIn++;
  if (TerminalBuffer.slPosIn == TERMINAL_BUFFER_SIZE)
  {
    TerminalBuffer.slPosIn = 0;
  }
  return TRUE;
}

/******************************************************
 ** Name            : TerminalBufferGet
 **
 ** Created from/on : RM / 16.11.2015
 **
 ** Description     : Gets scharacter from the cyclic
 **                   terminal buffer
 **
 ** Calling         : Not important
 **
 ** InputValues     : variable - byte which should be added to buffer
 ** OutputValues    : TRUE - if data added successfully
 **                   FALSE - if buffer is already full
 ******************************************************/
static bool TerminalBufferGet(schar* pscVariable)
{
  if (TerminalBuffer.islCounter == 0)
  {
    return FALSE;
  }
  *pscVariable = TerminalBuffer.aslTable[TerminalBuffer.slPosOut];
  TerminalBuffer.islCounter--;
  TerminalBuffer.slPosOut++;
  if (TerminalBuffer.slPosOut == TERMINAL_BUFFER_SIZE)
  {
    TerminalBuffer.slPosOut = 0;
  }
  return TRUE;
}

/******************************************************
 ** Name            : TERMINAL_iTransmitData
 **
 ** Created from/on : RM / 16.11.2015
 **
 ** Description     : Function called from UART1 transmit
 **                   interrupt handler.
 **                   One scharacter from terminal cyclic
 **                   buffer is sent in the routine.
 **
 ** Calling         : From interrupt.
 **
 ** InputValues     : none
 ** OutputValues    : none
 ******************************************************/
void TERMINAL_iTransmitData(void)
{
  schar scVariable;

  if (TerminalBufferGet(&scVariable) != FALSE)     // there is something to send
  {
    USART_SendData(USART1, scVariable);               // flag is cleared here
  }
  else
  {
    /* Disable the USART1 Transmit interrupt */
    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
  }
}

/******************************************************
 ** Name            : SendToTerminal
 **
 ** Created from/on : RM / 16.11.2015
 **
 ** Description     : Send string finished by NULL into TX buffer.
 **
 ** Calling         : Not important
 **
 ** InputValues     : *info - string buffer finished by NULL
 ** OutputValues    : none
 ******************************************************/
static void SendToTerminal(char* pscInfo)
{
  // Disables interrupt when writing to the buffer.
  // ----------------------------------------------
  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);

  // Sends data until NULL scharacter occures.
  // ----------------------------------------
  while (*pscInfo)
  {
    TerminalBufferPut(*pscInfo++);
  }

  // Enable transmit buffer empty interrupt
  // to send buffer data.
  // --------------------------------------
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

/******************************************************
 ** Name            : TERMINAL_IsEnabled
 **
 ** Created from/on : RM / 16.11.2015
 **
 ** Description     : Check if terminale is enabled
 **
 ** Calling         : Not important
 **
 ** InputValues     : none
 ** OutputValues    : TRUE - if usart terminal enabled
 **                   FALSE - if usart terminal disabled
 ******************************************************/
bool TERMINAL_IsEnabled(void)
{
  return fbDebugEnabled;
}

/******************************************************
 ** Name            : TERMINAL_Enable
 **
 ** Created from/on : RM / 16.11.2015
 **
 ** Description     : Enable or disable usart debug terminal
 **
 ** Calling         : Not important
 **
 ** InputValues     : TRUE - if usart terminal enabled
 **                   FALSE - if usart terminal disabled
 ** OutputValues    : none
 ******************************************************/
void TERMINAL_Enable(bool enable)
{
  fbDebugEnabled = enable;
}

/******************************************************
 ** Name            : TERMINAL
 **
 ** Created from/on : RM / 16.11.2015
 **
 ** Description     : Print debugs with multiple params
 **
 ** Calling         : Not important
 **
 ** InputValues     : *text - format text
 **                   ... - multiple params for format text
 ** OutputValues    : none
 ******************************************************/
void TERMINAL(const char* pscTEXT, ...)
{
  uint16_t i = 0;
  char scString[50] = { 0 };
  va_list myList;
  va_start(myList, pscTEXT);

  if (!TERMINAL_IsEnabled())
    return;

  while (*pscTEXT && (*pscTEXT != '%') && (i < ARRAY_SIZE(scString) - 1))
    scString[i++] = *pscTEXT++;

  SendToTerminal(scString);

  while (*pscTEXT)
  {
    i = 0;
    memset(scString, 0, ARRAY_SIZE(scString));

    do
    {
      scString[i++] = *pscTEXT++;
    } while (*pscTEXT && (*pscTEXT != '%') && (i < ARRAY_SIZE(scString) - 1));

    //CommonBuffer have to got size at least 'scString' + 1
    snprintf(ascCommonBuffer, ARRAY_SIZE(ascCommonBuffer), scString, (scString[0] == '%') ? va_arg(myList, void*) : NULL );

    SendToTerminal(ascCommonBuffer);
  }

  va_end(myList);
}

/******************************************************
 ** Name            : USART1_IRQHandler
 **
 ** Created from/on : RM / 16.11.2015
 **
 ** Description     : USART interrupt callback. Sends data
 **                   to TX if something is in USART tx queue
 **
 ** Calling         : Interrupt routine
 **
 ** InputValues     : none
 ** OutputValues    : none
 ******************************************************/
void USART1_IRQHandler(void)
{
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    USART_ReceiveData(USART1 );
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }

  if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {
    TERMINAL_iTransmitData();
    USART_ClearITPendingBit(USART1, USART_IT_TXE);
  }
}

/////////////////////////////////////////////////////////////////////////
/* legacy code */
/////////////////////////////////////////////////////////////////////////
void USART1_Send_Char(char c)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, c);
}

int __io_putchar(int c)
{
	USART1_Send_Char(c);
    return c;
}
