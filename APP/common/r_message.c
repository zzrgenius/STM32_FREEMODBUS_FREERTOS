/**
  ******************************************************************************
  * @file    xxx.c
  * @author  LoryTech HARDWARE TEAM
  * @version V1.1.0
  * @date    2018-10-08
  * @brief  xxx
  ******************************************************************************
  * @attention
  * Copyright (c) LoryTech. 
  * All rights reserved.
  *
 
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <string.h>
#include "r_message.h"
#include "checksum.h"
#include "utilities.h"
#include "bsp_serial.h"
#include "bsp_rs485.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


 
//message_t g_tx_messgage,g_rx_message;
uint8_t  message_headersum(message_t *msg)
{
	int i=0;
	uint8_t *p_data = (uint8_t*)&(msg->msg_header);
	uint8_t sum = 0;
	for(i=0;i<sizeof(message_header_t)-1;i++)
		sum += *p_data++;
//	msg->msg_header.checksum = sum;
	return sum;
		
}
void message_set_header(message_t * msg,uint8_t cmd,uint8_t length)
{
	static uint16_t ser_ticks = 0;
	msg->const_header.anti_head = MESSAGE_ANTI_HEADER;
	msg->const_header.sync_head = MESSAGE_SYNC_HEADER;
	msg->msg_header.serial_number = ser_ticks++;
	msg->msg_header.device_id = crc_16((uint8_t*)HAL_GetDEVID(),sizeof(uint32_t));	
	msg->msg_header.cmd = cmd;
	msg->msg_header.length = length;  
	msg->msg_header.checksum = message_headersum(msg);  
}
uint8_t message_check_headersum(message_t * msg)
{
	if(msg->msg_header.checksum ==message_headersum(msg))
		return 0;
	else
		return 1;
}
void send_message_frame(message_t * msg)
{
	uint8_t i =0;
	uint8_t *pData = (uint8_t*)msg;
	SerialDataBuf_TypeDef sendBuf;
	sendBuf.MessageCom = COM2;
	sendBuf.data_len = 0;
	for (i = 0; i <(msg->msg_header.length + MESSAGE_HEADER_SIZE);i++)
	{
		sendBuf.data_buf[i] = *pData++;
		sendBuf.data_len++;
	}
	sendBuf.data_buf[i] = (uint8_t)(msg->crc16);
	sendBuf.data_len++;
	sendBuf.data_buf[i+1] = (uint8_t)(msg->crc16 >> 8);
	sendBuf.data_len++;
	
	
	rs485_senddata(sendBuf.data_len,sendBuf.data_buf);
	
}

int8_t message_pack(message_t * msg,uint8_t cmd,uint8_t *data,uint8_t length)
{
 //	uint16_t crc16;
	
	if(msg == NULL)
		return -1;
	if(length > 1)
		message_set_header(msg,cmd,length);
 else 
	 return 1;
		memcpy(msg->buf,(uint8_t*)data,length); 
	 
		msg->crc16 = crc_16((uint8_t*)data,length);
 
	send_message_frame(msg);
		return 0;
}


void msg_response_create(int msg_id,const char *data, int length)
{
	
}
int HandleMSG(message_t *msg)
{
	if(msg->crc16 != crc_16(msg->buf,msg->msg_header.length))
		return 		MESSAGE_CRC_ERROR ;
	switch(msg->msg_header.cmd)
	{
		case 0:
			
			break;
		default:
			break;
		
	}
	return 0;	
}
///**
// * @brief  Compute and add checksum
// * @param  Msg pointer to the message
// * @retval None
// */
//void ComputeAndAdd(TMsg *Msg)
//{
//  uint8_t CHK = 0;
//  int i;

//  for(i = 0; i < Msg->Len; i++)
//  {
//    CHK -= Msg->Data[i];
//  }
//  Msg->Data[i] = CHK;
//  Msg->Len++;
//}
