#ifndef  __BSP_RS485_H__
#define  __BSP_RS485_H__

#include "main.h"
#include "stm32f4xx_hal.h"


extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
#define COM1_UART_HANDLE huart1
#define COM2_UART_HANDLE huart2
#define COM3_UART_HANDLE huart3
#define COM4_UART_HANDLE huart4
#define COM5_UART_HANDLE huart5
#define COM6_UART_HANDLE huart6
  
typedef struct
{
  UART_HandleTypeDef *handle;
  uint16_t          DE_PIN;
  GPIO_TypeDef *    DE_PIN_PORT;
} RS485_DE_TypeDef;
#define RS485_printf(format, ...)      {rs485_rx_disable(COM1);printf (format, ##__VA_ARGS__);rs485_rx_enable(COM1);}

void rs485_config(void);
void rs485_rx_enable(uint8_t com);
void rs485_rx_disable(uint8_t com); 
void rs485_senddata(uint8_t com,uint8_t data_len,uint8_t *databuf);
void rs485_sendstring(uint8_t com,char *strbuf);
// void rs485_rxidle_handle(uint16_t wlen);
#endif
