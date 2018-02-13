/*
 * SysClk.c
 *
 *  Created on: 29.10.2017
 *      Author: Dawid Adamik
 */

#include <Driver_Core.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_rcc.h>

/**
  * @brief  Configures clock path and source for System domain. Enables clock for peripherals.
  * @param  None
  * @retval None
  */
void RCC_Config(void)
//System Clock configuration, External quartz, PLL, etc.
{
	/* HSE initialization */
    ErrorStatus HSEStartUpStatus;

    RCC_DeInit();									//De-initialize previous settings

    /* HSE is quartz, f = 8.0MHz */
    RCC_HSEConfig(RCC_HSE_ON);						//HSE switch on
    HSEStartUpStatus = RCC_WaitForHSEStartUp(); 	//wait for HSE

    if(HSEStartUpStatus == SUCCESS)
    {

        RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_4); 	//HSE feed to PLL, no pre-division, 4x multiplier

    	/* SysClk = 32.0 MHz from PLL */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);        	//SYSCLK feeds to HCLK, no change
        RCC_PCLK2Config(RCC_HCLK_Div1);         	//HCLK feeds to PCLK1, no change
        RCC_PCLK1Config(RCC_HCLK_Div1);				//HCLK feeds to PCLK2, no change

        /* HCLK = PCLK1 = PCLK2 = 32.0 MHz */
        RCC_PLLCmd(ENABLE); 						//PLL switch on
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); //Wait for  PLL

        /* Core Clock = 32.0 MHz */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	//HCLK feeds to Core Clock, no change
        while(RCC_GetSYSCLKSource() != 0x08);		//Wait for Core Clock

        /* peripheral configuration */
        SysTick_Config(SystemCoreClock/1000);
        RCC_ADCCLKConfig(RCC_PCLK2_Div4);			// ADC1 Clock = 8Mhz

        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	//Flash memory delay
        FLASH_SetLatency(FLASH_Latency_1);						//depending of core clock
        														//0:  f<24MHZ; 1: f=24 to 48 Mhz; 2: f>48Mhz
    }
    else
    {
    	/* If HSE fails to start-up, the application will have wrong clock configuration. */
    	    while (1)
    	    { 	/* Go to infinite loop */
    	    }
    }

    /* Peripheral clocks switch on */

	RCC_APB1PeriphClockCmd(    	// APB1 domain peripherals
		RCC_APB1Periph_TIM2|
		RCC_APB1Periph_TIM4|
		RCC_APB1Periph_I2C1
		, ENABLE);

	RCC_APB2PeriphClockCmd( 	// APB2 domain peripherals
		RCC_APB2Periph_GPIOA |
		RCC_APB2Periph_GPIOB |
		RCC_APB2Periph_AFIO	 |
		RCC_APB2Periph_ADC1  |
		RCC_APB2Periph_SPI1  |
		RCC_APB2Periph_USART1
		, ENABLE);
}


/**
  * @brief  Configures clock path and source for Backup power domain.
  * @param  None
  * @retval None
  */
void RTC_Config (void)
{
	RCC_APB1PeriphClockCmd(
		RCC_APB1Periph_BKP |  //Backup power domain
		RCC_APB1Periph_PWR    //Power functions
		, ENABLE);

	/* LSE initialization */
	PWR_BackupAccessCmd(ENABLE); 	//power-up backup domain
	BKP_DeInit();					//De-initialize previous settings

	/* begin RTC domain configuration */
	RTC_EnterConfigMode();

	RCC_LSEConfig(RCC_LSE_ON);								//LSE switch on
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){}; 	//wait for LSE

	/* RTCClk = 32768 Hz from LSE */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	/* Enable the RTC */
	RCC_RTCCLKCmd(ENABLE);

	/* Synchronize internal registers */
	RTC_WaitForSynchro();
	RTC_WaitForLastTask(); 	//wait for synchronization

	/* set single count to 1s */
	RTC_SetPrescaler(32767);
	RTC_WaitForLastTask(); //wait for prescaler

	/* finish RTC domain configuration */
	RTC_ExitConfigMode();
}

/**
  * @brief  Enables functionalities after powerdown.
  * @param  None
  * @retval None
  */
void RTC_Resume(void)
{
	RCC_APB1PeriphClockCmd(
		RCC_APB1Periph_BKP |  //Backup power domain
		RCC_APB1Periph_PWR    //Power functions
		, ENABLE);
}
