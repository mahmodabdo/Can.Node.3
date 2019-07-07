/*
 * Can_Driver.h
 *
 *  Created on: Jul 3, 2019
 *      Author: AVE-LAP-60
 */

#ifndef MCAL_CAN_CAN_DRIVER_H_
#define MCAL_CAN_CAN_DRIVER_H_

#include <stdbool.h>
#include <stdint.h>





void Can_Initalization(void);
unsigned char Can_Recieve(uint32_t ID,uint32_t mask ,uint32_t mail_box);
void VidMCAN_Transmit(uint8_t MsgData, uint32_t MsgID, uint32_t MsgIDMask, uint32_t MailBox);





#endif /* MCAL_CAN_CAN_DRIVER_H_ */
