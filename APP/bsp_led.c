
/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  LoryTech HARDWARE TEAM
  * @version V1.1.0
  * @date    2018-10-18
  * @brief   This file provides all the RCC firmware functions.
  ******************************************************************************
  * @attention
  * Copyright (c) LoryTech. 
  * All rights reserved.
  *
 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "bsp_led.h" 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

 Led_TypeDef gLed[6] = { { LED1_Pin, LED1_GPIO_Port } , 
                         { LED2_Pin, LED2_GPIO_Port } ,
						 { LED3_Pin, LED3_GPIO_Port } ,
						 { LED4_Pin, LED4_GPIO_Port } ,					
						 { LED5_Pin, LED5_GPIO_Port } , 						
						 { LED6_Pin, LED6_GPIO_Port } 						
                       };
/**
 * @}
 */
                                       
/* Public Functions ----------------------------------------------------------*/
/** @defgroup X_NUCLEO_NFC03A1_Led_Public_Functions
 * @{
 */

/**
  * @brief  This function initialize the GPIO to manage the Leds through GPIO
  * @param  None
  * @retval None
  */
void initLED(void)
{
	
	// initialized in gpio.c
//  GPIO_InitTypeDef GPIO_InitStruct;
//  
//  LED_GPIO_CLK_ENABLE();
//  
//  /* Configure GPIO for LEDs (PA8,PB10,PB4,PB5)*/
//  GPIO_InitStruct.Pin 	= NFC03A1_LED2_PIN  ;
//  GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;	
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;//GPIO_SPEED_LOW;
//  GPIO_InitStruct.Pull 	= GPIO_NOPULL;
//  HAL_GPIO_Init(NFC03A1_LED2_PIN_PORT, &GPIO_InitStruct);
//  
// 
//  
//  GPIO_InitStruct.Pin = NFC03A1_LED1_PIN;
//  HAL_GPIO_Init(NFC03A1_LED1_PIN_PORT, &GPIO_InitStruct);
//  
//  LED_Off(BLUE_LED2);
//  LED_Off(GREEN_LED1);

}

/**
  * @brief  This function light on selected Led
  * @param  led : Led to be lit on
  * @retval None
  */
void LED_On(const Led_E led)
{
  HAL_GPIO_WritePin( gLed[led].LED_PIN_PORT, gLed[led].LED_PIN, GPIO_PIN_RESET  );
}

/**
  * @brief  This function light off selected Led
  * @param  led : Led to be lit off
  * @retval None
  */
void LED_Off(const Led_E led)
{
  HAL_GPIO_WritePin( gLed[led].LED_PIN_PORT, gLed[led].LED_PIN, GPIO_PIN_SET );
}

/**
  * @brief  Toggles the selected LED
  * @param  led : Specifies the Led to be toggled
  * @retval None
  */
void LED_Toggle(const Led_E led)
{
  HAL_GPIO_TogglePin( gLed[led].LED_PIN_PORT, gLed[led].LED_PIN );
}

/**
  * @brief  this function turns all LEDs Off.
  */
void LED_AllOff(void)
{
  LED_Off(LED1);
  LED_Off(LED2);
	LED_Off(LED3);
  LED_Off(LED4);

}
