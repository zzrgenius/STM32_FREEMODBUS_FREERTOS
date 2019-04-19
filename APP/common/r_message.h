#ifndef __MESSAGE_H
#define __MESSAGE_H

#include "main.h"
#define  	SERIAL_PROTO_PACKET_ACK_MASK 			0x80
#define  	SERIAL_PROTO_PACKET_NOACK_MASK 		0x00
#define 	MESSAGE_CONST_HEADER  							4
#define 	MESSAGE_HEADER_SIZE    (MESSAGE_CONST_HEADER+8)
#define 	MESSAGE_BUF_LOAD  		 (0x80+2)
#define   MESSAGE_ANTI_HEADER     	0x55AA
#define   MESSAGE_SYNC_HEADER				0xF0E0


enum MESSAGE_ERR{
	MESSAGE_OK,      
	MESSAGE_HEADER_ERR,
	MESSAGE_LENGTH_ERR,             
  MESSAGE_CRC_ERROR           
};
struct message_const_head {
	uint16_t anti_head;
	uint16_t sync_head;
};

typedef  struct message_header {
	uint16_t serial_number;
  uint16_t device_id;	
	
  uint8_t type;
  uint8_t cmd;
  
	uint8_t length;
	uint8_t checksum;
} message_header_t;


typedef struct MESSAGE_T
{ 
	struct message_const_head const_header;
	message_header_t msg_header;
	uint8_t buf[MESSAGE_BUF_LOAD];
	uint16_t crc16;//
}message_t; 
uint8_t message_check_headersum(message_t * msg); 
int8_t message_pack(message_t * msg,uint8_t cmd,uint8_t *data,uint8_t length);
int HandleMSG(message_t *msg);

#endif 
