/**
  ******************************************************************************
  * @file    bsp_serial.c
  * @author  LoryTech HARDWARE TEAM
  * @version V1.1.0
  * @date    2018-10-08
  * @brief   基于hal库的 串口空闲中断接收
  ******************************************************************************
  * @attention
  * Copyright (c) LoryTech. 
  * All rights reserved.
  *
 
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>
#include "bsp_serial.h"
#include "bsp_rs485.h"

#if USE_RTOS
 #include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "semphr.h"
#include "queue.h"
#endif
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#ifndef	DEBUG
	#define DEBUG 	1
#endif
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if USE_RTOS

QueueHandle_t xMSG_RX_Queue;
SemaphoreHandle_t xPrintfSemaphore = NULL;
#endif

//MSG_TypeDef Msg_nb;
//MSG_TypeDef Msg_debug;
 uint8_t usart1_rx_buf[USART_BUF_SIZE];
 uint8_t usart2_rx_buf[USART_BUF_SIZE];
 uint8_t usart3_rx_buf[USART_BUF_SIZE];
 uint8_t usart4_rx_buf[USART_BUF_SIZE];
 uint8_t usart5_rx_buf[USART_BUF_SIZE];
 uint8_t usart6_rx_buf[USART_BUF_SIZE];

 extern UART_HandleTypeDef huart1;
  extern UART_HandleTypeDef huart2;

 	SerialDataBuf_TypeDef gSerialBuf;
/* Private function prototypes -----------------------------------------------*/
 


/**
  * @brief  DeInitializes usart buf . 
  * @param  
  * @retval none/status
  */
void bsp_serial_config(void)
{
	#if USE_RTOS

	xMSG_RX_Queue	= xQueueCreate(4,sizeof(MSG_TypeDef));
	xPrintfSemaphore = xSemaphoreCreateBinary();
	if( xPrintfSemaphore == NULL )
   {
       /* 因堆栈不足，信号量创建失败，这里进行失败处理*/
	   printf("xPrintfSemaphore creat error\r\n");
   }
   else
   {
 	   xSemaphoreGive(xPrintfSemaphore);
   }
       
	if( xMSG_RX_Queue == NULL )
    {
        /* Failed to create the queue. */
		#if DEBUG
		printf("Faild to crate msg rx queue! \r\n");
		#endif		
    }
		#endif
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart1,usart1_rx_buf,USART_BUF_SIZE);
	
//	__HAL_UART_CLEAR_IDLEFLAG(&huart2);
//	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
//	HAL_UART_Receive_DMA(&huart2,usart2_rx_buf,USART_BUF_SIZE);

	__HAL_UART_CLEAR_IDLEFLAG(&huart3);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart3,usart3_rx_buf,USART_BUF_SIZE);

	__HAL_UART_CLEAR_IDLEFLAG(&huart4);
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart4,usart4_rx_buf,USART_BUF_SIZE);

	__HAL_UART_CLEAR_IDLEFLAG(&huart5);
	__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart5,usart5_rx_buf,USART_BUF_SIZE);

	__HAL_UART_CLEAR_IDLEFLAG(&huart6);
	__HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart6,usart6_rx_buf,USART_BUF_SIZE);
	

}
#if 0
void HAL_LPUSART_RXT_IDLE_Handle(UART_HandleTypeDef *huart)  
{
	static	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
   	uint16_t wlength ;
	uint32_t temp; 	
	T_SIM_RECMSG tSimMsg = {0};
	if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
	{   
		__HAL_UART_FLUSH_DRREGISTER(huart);
			__HAL_UART_CLEAR_IT(huart,UART_CLEAR_IDLEF);

 		__HAL_UART_CLEAR_IDLEFLAG(huart);
		huart->Instance->ICR |= UART_CLEAR_IDLEF;
		temp = huart->Instance->ISR; //USART2->SR;
		temp = huart->Instance->RDR;  //USART2->DR;	
		HAL_UART_DMAStop(huart); 
		//temp = huart->hdmarx->Instance->CNDTR;
		wlength = (uint16_t) (USART_BUF_SIZE - __HAL_DMA_GET_COUNTER(huart->hdmarx));	

		tSimMsg.wLen = wlength;
		memcpy(tSimMsg.Data ,lpuart_rx_buf,wlength);
		HAL_UART_Receive_DMA(&hlpuart1,lpuart_rx_buf,USART_BUF_SIZE);
		
		#if USE_OS
 		xQueueSendFromISR( g_SIM_RecDataProcQueue, &tSimMsg, &xHigherPriorityTaskWoken );
		#endif	
	}   
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );

}
#endif
 /**
   * @brief  uart idle irq handle . 
   * @param  huart  
   * @retval none
   */
void HAL_USART_RXT_IDLE_Handle(UART_HandleTypeDef *huart)  
{ 
	uint16_t wlength = 0;
	uint32_t temp; 

	#if USE_RTOS

	static portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	#endif
	
	__HAL_UART_FLUSH_DRREGISTER(huart);
//	__HAL_UART_CLEAR_IT(huart,UART_CLEAR_IDLEF);
	__HAL_UART_CLEAR_IDLEFLAG(huart);
	// huart->Instance->RQR |= 
 	
	HAL_UART_DMAStop(huart); 
	temp = huart->hdmarx->Instance->NDTR;  
	wlength = (uint16_t) (USART_BUF_SIZE - temp);	
	if(huart->Instance == USART1)
	{
		//HAL_UART_DMAPause(huart);
		gSerialBuf.MessageCom = COM1;
		gSerialBuf.data_len = wlength;
		memcpy(gSerialBuf.data_buf,usart1_rx_buf,wlength);
		HAL_UART_Receive_DMA(&huart1,usart1_rx_buf,USART_BUF_SIZE);
		#if USE_RTOS
		xQueueSendFromISR( xMSG_RX_Queue,( void * ) &tMsg, &xHigherPriorityTaskWoken );
		#endif				

	}
	if(huart->Instance == USART2)
	{
		//HAL_UART_DMAPause(huart);
//		gSerialBuf.MessageCom = COM2;
//		gSerialBuf.data_len = wlength;
//		memcpy(gSerialBuf.data_buf,usart2_rx_buf,wlength);
//		HAL_UART_Receive_DMA(&huart2,usart2_rx_buf,USART_BUF_SIZE);
//		#if USE_RTOS
//		xQueueSendFromISR( xMSG_RX_Queue,( void * ) &tMsg, &xHigherPriorityTaskWoken );
//		#endif				

	}
	if(huart->Instance == USART3)
	{
		//HAL_UART_DMAPause(huart);
		gSerialBuf.MessageCom = COM3;
		gSerialBuf.data_len = wlength;
		memcpy(gSerialBuf.data_buf,usart3_rx_buf,wlength);
		HAL_UART_Receive_DMA(&huart3,usart3_rx_buf,USART_BUF_SIZE);
		#if USE_RTOS
		xQueueSendFromISR( xMSG_RX_Queue,( void * ) &tMsg, &xHigherPriorityTaskWoken );
		#endif				

	}
	if(huart->Instance == UART4)
	{
		//HAL_UART_DMAPause(huart);
		gSerialBuf.MessageCom = COM4;
		gSerialBuf.data_len = wlength;
		memcpy(gSerialBuf.data_buf,usart4_rx_buf,wlength);
		HAL_UART_Receive_DMA(&huart4,usart4_rx_buf,USART_BUF_SIZE);
		#if USE_RTOS
		xQueueSendFromISR( xMSG_RX_Queue,( void * ) &tMsg, &xHigherPriorityTaskWoken );
		#endif				

	}
	if(huart->Instance == UART5)
	{
		//HAL_UART_DMAPause(huart);
		gSerialBuf.MessageCom = COM5;
		gSerialBuf.data_len = wlength;
		memcpy(gSerialBuf.data_buf,usart5_rx_buf,wlength);
		HAL_UART_Receive_DMA(&huart5,usart5_rx_buf,USART_BUF_SIZE);
		#if USE_RTOS
		xQueueSendFromISR( xMSG_RX_Queue,( void * ) &tMsg, &xHigherPriorityTaskWoken );
		#endif				

	}
	if(huart->Instance == USART6)
	{
		//HAL_UART_DMAPause(huart);
		gSerialBuf.MessageCom = COM6;
		gSerialBuf.data_len = wlength;
		memcpy(gSerialBuf.data_buf,usart6_rx_buf,wlength);
		HAL_UART_Receive_DMA(&huart6,usart6_rx_buf,USART_BUF_SIZE);
		#if USE_RTOS
		xQueueSendFromISR( xMSG_RX_Queue,( void * ) &tMsg, &xHigherPriorityTaskWoken );
		#endif				

	}
	
	
//	if(huart->Instance == USART2)
//	{
////				tMsg.MessageCom = COM2;
//		rs485_rxidle_handle(wlength);
////		tMsg.data_len = wlength;
////		memcpy(tMsg.data_buf,usart1_rx_buf,wlength);
////		HAL_UART_Receive_DMA(&huart2,usart1_rx_buf,USART_BUF_SIZE);
//// 			

//	}
	 
	 

#if USE_RTOS
		portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
		#endif
	}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	#if USE_OS
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	#endif
	 

//	if(huart->hdmarx == &hdma_usart3_rx)
//	{
//	//	__HAL_DMA_CLEAR_FLAG(huart->hdmarx,DMA_FLAG_TCIF3);  

//		HAL_UART_DMAStop(huart); 
//		//xQueueSendFromISR(g_GPSDataProcQueue,( void * )usart3_rx_buf , &xHigherPriorityTaskWoken );
//		//__HAL_DMA_DISABLE_IT(huart3.hdmarx, (DMA_IT_TC | DMA_IT_TE));		
//  	}
#if USE_RTOS
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	#endif
		
		 
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	    uint8_t i = 0;
	
	if((__HAL_UART_GET_FLAG(huart,UART_FLAG_ORE) != RESET))  
	{
//		__HAL_UART_CLEAR_IT(huart,UART_CLEAR_OREF);
		HAL_UART_Receive(huart,(uint8_t *)&i,1,0xff);
		
	}
	
}
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
 // HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
	rs485_rx_disable(COM1);
  HAL_UART_Transmit(&huart1,(uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}
//			if(wlength > 6 )
//			{
//					HAL_UART_DMAStop(huart); 
//					temp = huart->Instance->ISR; //USART2->SR;
//					temp = huart->Instance->RDR;  //USART2->DR;
//					temp = huart->hdmarx->Instance->CNDTR;  			 
//					wlength = (uint16_t) (USART_BUF_SIZE - temp);
//					Msg_debug.data_len = wlength;
//					memcpy(Msg_debug.data_buf,usart1_rx_buf,wlength);
//				#if USE_OS
//					xQueueSendFromISR( xMSG_RX_Queue, &Msg_debug, &xHigherPriorityTaskWoken );
//				#endif
//					//PN532_RXCounter = wlength;
//					//			flag_rev_finish = 1;
//					//			memcpy(PN532_RxBuffer,uart2_recdata,wlength);
//					//			memset(uart2_recdata,0,wlength);
//					//			wlength = 0;
//					//			HAL_UART_Receive_DMA(&huart2,uart2_recdata,512);
//			}
//			else
//			{ 
//				
//				
//				HAL_UART_DMAResume(huart); 
//			}
