/*
 * Can_Driver.c
 *
 *  Created on: Jul 3, 2019
 *      Author: AVE-LAP-60
 */

#include "Can_Driver.h"

#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "inc/hw_can.h"
#include "inc/hw_memmap.h"

void Can_Initalization(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));

    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);

    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_CAN0));

    CANInit(CAN0_BASE);

    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);

    CANEnable(CAN0_BASE);



}


unsigned char Can_Recieve(uint32_t ID,uint32_t mask ,uint32_t mail_box)
{
    tCANMsgObject sMsgObjectRx;
    uint8_t Recieved_data=0;
    sMsgObjectRx.ui32MsgID = ID;
    sMsgObjectRx.ui32MsgIDMask = mask;
    sMsgObjectRx.ui32MsgLen=1;
    sMsgObjectRx.ui32Flags = MSG_OBJ_USE_ID_FILTER ;
    sMsgObjectRx.pui8MsgData = &Recieved_data;

    CANMessageSet(CAN0_BASE, 1, &sMsgObjectRx, MSG_OBJ_TYPE_RX);
    unsigned long status;
    status=CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
    while(!(status&CAN_STATUS_RXOK))
    status=CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
    CANMessageGet(CAN0_BASE, mail_box, &sMsgObjectRx, 0);
    return Recieved_data;

}



void VidMCAN_Transmit(uint8_t  MsgData, uint32_t MsgID, uint32_t MsgIDMask, uint32_t MailBox)
{
   tCANMsgObject sCANMessage;
   uint8_t ui8MsgData =MsgData;
   uint8_t *pui8MsgData=&ui8MsgData;

   //pui8MsgData = MsgData;
   sCANMessage.ui32MsgID = MsgID;
   //sCANMessage.ui32MsgIDMask = MsgIDMask;
   sCANMessage.ui32Flags = 0;
   sCANMessage.ui32MsgLen = 0X01;
   sCANMessage.pui8MsgData = pui8MsgData;

   CANMessageSet(CAN0_BASE, MailBox, &sCANMessage, MSG_OBJ_TYPE_TX);
}


