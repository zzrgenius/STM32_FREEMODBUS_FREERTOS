/**
  ******************************************************************************
  * @file    xxx.c
  * @author  LoryTech HARDWARE TEAM
  * @version V1.1.0
  * @date    2018-12-18
  * @brief  xxx
  ******************************************************************************
  * @attention
  * Copyright (c) LoryTech. 
  * All rights reserved.
  *
 
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "bsp_serial.h"
#include "gpio.h"
#include "bsp_rs485.h"
#include "bsp_led.h"
 
#if USE_RTOS
 #include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "semphr.h"
#include "queue.h"
#endif
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define USE_DMA_RS485  1

 
 RS485_DE_TypeDef gRS485DE[6] = { { &huart1,RS485_DE_1_Pin, RS485_DE_1_GPIO_Port } ,
								  { &huart2,RS485_DE_2_Pin, RS485_DE_2_GPIO_Port } ,
								  { &huart3,RS485_DE_3_Pin, RS485_DE_3_GPIO_Port } ,
								  { &huart4,RS485_DE_4_Pin, RS485_DE_4_GPIO_Port } ,
								  { &huart5,RS485_DE_5_Pin, RS485_DE_5_GPIO_Port } ,
								  { &huart6,RS485_DE_6_Pin, RS485_DE_6_GPIO_Port } 
								};
//  UART_HandleTypeDef gHuart[6] = { huart1,huart2, huart3 , huart4,huart5,huart6 };
 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

void rs485_rx_enable(uint8_t com)
{
	
  HAL_GPIO_WritePin( gRS485DE[com].DE_PIN_PORT, gRS485DE[com].DE_PIN, GPIO_PIN_RESET );
}
void rs485_rx_disable(uint8_t com)
{
	HAL_GPIO_WritePin( gRS485DE[com].DE_PIN_PORT, gRS485DE[com].DE_PIN, GPIO_PIN_SET );
} 
void rs485_config(void)
{

		rs485_rx_enable(COM1);
		rs485_rx_enable(COM2);
		rs485_rx_enable(COM3);
		rs485_rx_enable(COM4);
		rs485_rx_enable(COM5);
		rs485_rx_enable(COM6);

	
}

void rs485_senddata(uint8_t com,uint8_t data_len,uint8_t *databuf)
{
	 
	 rs485_rx_disable(com);
	HAL_Delay(3);	
	HAL_UART_Transmit(gRS485DE[com].handle, databuf, data_len, 0xFFFF);
	rs485_rx_enable(com);
}
void rs485_sendstring(uint8_t com,char *strbuf)
{
	 rs485_rx_disable(com); 
	HAL_Delay(3);
	 HAL_UART_Transmit(gRS485DE[com].handle, (uint8_t*)strbuf, strlen(strbuf), 0xFFFF);
	rs485_rx_enable(com);
}

//uint8_t rs485_rx_msg_handle(uint16_t wlen,uint8_t *data_buf)
//{
//	uint8_t *pData = data_buf;
// 
//	uint16_t t_wlength;
//	if(wlen < 6)
//		return 1;
//	t_wlength = wlen;
//	while(t_wlength--)
//	{
////		if(*pData++ == 0xE0)
////		{
////			if(*pData == 0xF0)
////			{
////				p_msg = (message_t *)(pData-3);
////				memcpy((uint8_t*)&g_rx_message,(uint8_t*)p_msg,MESSAGE_HEADER_SIZE);
////				if(message_check_headersum(&g_rx_message) != 0)
////					return 	MESSAGE_HEADER_ERR;
////				if(g_rx_message.msg_header.length  > 2)
////				{
////					memcpy((uint8_t*)&g_rx_message.buf,p_msg->buf,g_rx_message.msg_header.length);
////					g_rx_message.crc16 = *(uint16_t*)&p_msg->buf[g_rx_message.msg_header.length];
////					HandleMSG(&g_rx_message);
////					LED_AllOff();
////					return MESSAGE_OK;
////				}
////			}
////		}
//		
//	}
//	return 0;
//}

//void rs485_rxidle_handle(uint16_t wlen)
//{		 	
////		memcpy(tMsg.data_buf,usart1_rx_buf,wlength);
//		rs485_rx_msg_handle(wlen,rs485_rx_buf);
//		HAL_UART_Receive_DMA(&huart2,rs485_rx_buf,USART_BUF_SIZE);
//// 			
//}
