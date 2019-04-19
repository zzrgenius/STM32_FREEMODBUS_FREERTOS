#include "mav_msg.h"
#include "bsp_rs485.h"
#include "bsp_serial.h"
#include "utilities.h"
#include "checksum.h"
//#include "mavlink_types.h"
#define  MAV_DEBUG       1

#define MAV_SYSTEM_ID  		0x12
#define MAVLINK_MSG_ID_CRC  50
#define MAVLINK_MSG_ID_HEART 0
mavlink_message_t g_mavmsg;
/************

	uint16_t checksum;      ///< sent at end of packet
	uint8_t magic;          ///< protocol magic marker
	uint8_t len;            ///< Length of payload
	uint8_t incompat_flags; ///< flags that must be understood
	uint8_t compat_flags;   ///< flags that can be ignored if not understood
	uint8_t seq;            ///< Sequence of packet
	uint8_t sysid;          ///< ID of message sender system/aircraft
	uint8_t compid;         ///< ID of the message sender component
	uint32_t msgid:24;      ///< ID of message in payload
	uint64_t payload64[(MAVLINK_MAX_PAYLOAD_LEN+MAVLINK_NUM_CHECKSUM_BYTES+7)/8];
	uint8_t ck[2];          ///< incoming checksum bytes
	uint8_t signature[MAVLINK_SIGNATURE_BLOCK_LEN];
	
***///////
void mav_sendmsg(uint8_t com,uint8_t compid ,uint32_t msgid,uint8_t databuf[],uint8_t len)
{
	mavlink_message_t message;
	uint8_t sendbuf[MAVLINK_MAX_PAYLOAD_LEN];
	uint16_t send_len;
	//(mavlink_message_t* msg, uint8_t system_id, uint8_t component_id,mavlink_status_t* status, uint8_t min_length, uint8_t length, uint8_t crc_extra)
	memcpy(_MAV_PAYLOAD_NON_CONST(&message), databuf, len);  
	message.msgid = msgid;
	//mavlink_finalize_message(&message, MAV_SYSTEM_ID, compid, len, len, MAVLINK_MSG_ID_CRC);
	mavlink_finalize_message_chan(&message, MAV_SYSTEM_ID, compid, com,len, len, MAVLINK_MSG_ID_CRC);
	send_len = mavlink_msg_to_send_buffer(sendbuf,&message);
	rs485_senddata(com,send_len,sendbuf); 

} 

int read_messages(uint8_t reccom,uint16_t recsize,uint8_t buf[])
{
 	uint8_t i,temp;
	mavlink_status_t status;
//	static 		mavlink_status_t lastStatus;

 
	// --------------------------------------------------------------------------
	//   READ FROM PORT
	// --------------------------------------------------------------------------

	// this function locks the port during read
	//int result = _read_port(cp);

	// --------------------------------------------------------------------------
	//   PARSE MESSAGE
	// --------------------------------------------------------------------------
	if (recsize > 0)
   	{
		// Something received - print out all bytes and parse packet
		mavlink_message_t msg;
		mavlink_status_t status;
		RS485_printf("Bytes Received: %d\nDatagram: ", (int)recsize);
		for (i = 0; i < recsize; ++i)
		{
			temp = buf[i];
			RS485_printf("%02x ", (unsigned char)temp);
			if (mavlink_parse_char(reccom, buf[i], &msg, &status))
			{
				// Packet received
				RS485_printf("\nReceived packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
			}
		}
		RS485_printf("\n");
	}
	return 0;
}
extern 	SerialDataBuf_TypeDef gSerialBuf;

int Mavlink_Msg_Handle(void)
{
		uint8_t recsize;
	uint8_t i,temp;
	mavlink_status_t status;
	static 		mavlink_status_t lastStatus;
//	bool success;               // receive success flag
//	Time_Stamps this_timestamps;
//	mavlink_message_t current_messages;
	// Blocking wait for new data
 
		// ----------------------------------------------------------------------
		//   READ MESSAGE
		// ----------------------------------------------------------------------
		 
//		success = read_messages(&message);
		if(gSerialBuf.MessageCom != COM1)
			return 0;
		// ----------------------------------------------------------------------
		//   HANDLE MESSAGE
		// ----------------------------------------------------------------------
		if( gSerialBuf.data_len > 0)
		{
			recsize = gSerialBuf.data_len; 
			RS485_printf("Bytes Received: %d\nDatagram: ", (int)recsize);
			for (i = 0; i < recsize; ++i)
			{
				temp = gSerialBuf.data_buf[i];
				RS485_printf("%02x ", (unsigned char)temp);
				if (mavlink_parse_char(gSerialBuf.MessageCom,gSerialBuf.data_buf[i], &g_mavmsg, &status))
				{
					// Packet received
					#if MAV_DEBUG
					RS485_printf("\nReceived packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", g_mavmsg.sysid, g_mavmsg.compid, g_mavmsg.len, g_mavmsg.msgid);
					#endif
				}
			}
			#if MAV_DEBUG
			RS485_printf("\n");
			#endif
			gSerialBuf.data_len = 0;
			// Handle Message ID
			switch (g_mavmsg.msgid) 
			{
				case MAVLINK_MSG_ID_HEARTBEAT:
				{
					//printf("MAVLINK_MSG_ID_HEARTBEAT\n");
					//					mavlink_msg_heartbeat_decode(&message, &(current_messages.heartbeat));
					//					current_messages.time_stamps.heartbeat = get_time_usec();
					//					this_timestamps.heartbeat = current_messages.time_stamps.heartbeat;
					break;
				}
				case MAVLINK_MSG_ID_SYS_STATUS:
				{
					//printf("MAVLINK_MSG_ID_SYS_STATUS\n");
//					mavlink_msg_sys_status_decode(&message, &(current_messages.sys_status));
//					current_messages.time_stamps.sys_status = get_time_usec();
//					this_timestamps.sys_status = current_messages.time_stamps.sys_status;
					break;
				}	 
 
 

				default:
				{
					// printf("Warning, did not handle message id %i\n",message.msgid);
					break;
				}


			} // end: switch msgid

		} // end: if read message 
		return 1;
}
int RF_Msg_Handle(void)
{
	uint8_t recsize;
	uint8_t i,temp;
	uint8_t databuf[USART_BUF_SIZE];
	uint8_t id_hexbuf[5];
	uint32_t mcu_device[3];
	uint16_t crcUID;
//	bool success;               // receive success flag
//	Time_Stamps this_timestamps;
//	mavlink_message_t current_messages;
	// Blocking wait for new data
 
		// ----------------------------------------------------------------------
		//   READ MESSAGE
		// ----------------------------------------------------------------------
		 
//		success = read_messages(&message);
		if(gSerialBuf.MessageCom == COM1)
			return 0;
		// ----------------------------------------------------------------------
		//   HANDLE MESSAGE
		// ----------------------------------------------------------------------
		if( gSerialBuf.data_len == 13)
		{
			recsize = gSerialBuf.data_len; 
			if(gSerialBuf.data_buf[0] == 0x02)
			{				 
				if(gSerialBuf.data_buf[recsize-1] == 0x03)
				{
					ascs2hex(id_hexbuf,(char*)&gSerialBuf.data_buf[1],10);
					temp = id_hexbuf[0];
					for(i=1;i<5;i++)
						temp ^= id_hexbuf[i];
					if(temp ==  gSerialBuf.data_buf[11] )
					{
						//success
						HAL_GetUID(mcu_device);
						crcUID = crc_calculate((const uint8_t*)mcu_device, sizeof(uint32_t)*3);
						mav_sendmsg(COM1,gSerialBuf.MessageCom , crcUID,gSerialBuf.data_buf,gSerialBuf.data_len);
					}
				}  
			}
//			for (i = 0; i < recsize; ++i) 
//			{
//				temp = gSerialBuf.data_buf[i];
//			 //	RS485_printf("%02x ", (unsigned char)temp);
//			 
//			}			 
			gSerialBuf.data_len = 0; 
			return 0;
		}
		return 1;
}
