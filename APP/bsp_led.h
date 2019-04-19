#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "main.h"
#include "stm32f4xx_hal.h"


typedef enum 
{
  LED1 = 0,
  LED2 ,
  LED3 ,
  LED4 ,
  LED5,
  LED6
} Led_E; 

typedef struct
{
  uint16_t          LED_PIN;
  GPIO_TypeDef *    LED_PIN_PORT;
} Led_TypeDef;

//#define  LED1_PIN               GPIO_PIN_12
//#define  LED1_PIN_PORT          GPIOE
//#define  LED2_PIN               GPIO_PIN_13
//#define  LED2_PIN_PORT          GPIOE

//#define  LED3_PIN               GPIO_PIN_14
//#define  LED3_PIN_PORT          GPIOE
//#define  LED4_PIN               GPIO_PIN_15
//#define  LED4_PIN_PORT          GPIOE
//void initLED(void);
void LED_On(const Led_E led);
void LED_Off(const Led_E led);
void LED_Toggle(const Led_E led);
void LED_AllOff(void);
#endif
