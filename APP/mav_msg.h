#ifndef __MAV_MSG_H
#define __MAV_MSG_H

#include "mavlink.h"
 

//typedef enum {
//    PACKET_DATA_ACCEL = 0,
//    PACKET_DATA_GYRO,
//    PACKET_DATA_COMPASS,
//    PACKET_DATA_QUAT,
//    PACKET_DATA_EULER,
//    PACKET_DATA_ROT,
//    PACKET_DATA_HEADING,
//    PACKET_DATA_LINEAR_ACCEL,
//    NUM_DATA_PACKETS
//} eMPL_packet_e;

//#define MAV_MPU9250_COMPONET_ID 	0x77 
//#define MAV_MPU_DATA_EULER_ID				PACKET_DATA_EULER
//#define MAV_MPU_DATA_LINEAR_ACCEL_ID		PACKET_DATA_LINEAR_ACCEL
#define MAVLINK_MSG_ID_SYS_STATUS  0x02
#define MAVLINK_COMP_ID_SYS  0x01


int Mavlink_Msg_Handle(void);
int RF_Msg_Handle(void);
void mav_sendmsg(uint8_t com,uint8_t compid ,uint32_t msgid,uint8_t databuf[],uint8_t len);
int read_messages(uint8_t reccom,uint16_t recsize,uint8_t buf[]);
#endif
