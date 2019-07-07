/**
 * main.c
 */

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "MCAL/DIO/DIO.h"
#include <MCAL/Can/Can_Driver.h>
#include "RTOS/FreeRTOS.h"
#include "RTOS/task.h"

#define ID 3U
#define MASK 0xFFFFFFFFU
#define MAILBOX 1U
#define DATA 3U


static TaskHandle_t Start_up_handeler = NULL, Normal_Handeler = NULL,
        Led_Handeler = NULL;

void vLed_task(void);
void vInitialization_Task(void);
void vStartup_state(void);
void vNormal_state(void);

int main(void)
{

    SysCtlClockSet(
    SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    Can_Initalization();
    Led_Initalization();

    xTaskCreate((TaskFunction_t) vInitialization_Task, NULL, 350, NULL, 5, NULL);
    xTaskCreate((TaskFunction_t) vStartup_state, NULL, 350, NULL, 4,
                &Start_up_handeler);
    xTaskCreate((TaskFunction_t) vLed_task, NULL, 350, NULL, 3, &Led_Handeler);
    xTaskCreate((TaskFunction_t) vNormal_state, NULL, 350, NULL, 2,
                &Normal_Handeler);
    vTaskStartScheduler();

    while (1)
    {

    }
}

void vInitialization_Task(void)
{
    SysCtlClockSet(
    SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    Can_Initalization();
    Led_Initalization();
    vTaskSuspend(NULL);
}

void vStartup_state(void)
{

    while(1)
    {
        unsigned char recieved_data=0;
        recieved_data=Can_Recieve(2U,MASK,MAILBOX);
        if(recieved_data == 2U)
        {

            xTaskNotifyGive(Led_Handeler);
            ulTaskNotifyTake( pdTRUE, portMAX_DELAY);
            xTaskNotifyGive(Normal_Handeler);
            vTaskSuspend(NULL);
        }
    }


}

void vNormal_state(void)
{
    unsigned char recieved_data = 0;
    ulTaskNotifyTake( pdTRUE, portMAX_DELAY);
    while (1)
    {

        VidMCAN_Transmit( DATA,ID,MASK,MAILBOX);

        recieved_data = Can_Recieve(2U, MASK, MAILBOX);

        while (recieved_data != 2U)
        {
            recieved_data = Can_Recieve(2U, MASK, MAILBOX);

        }
        xTaskNotifyGive(Led_Handeler);
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY);

    }
}

void vLed_task(void)
{
    static char flag = 0;
    while (1)
    {
        ulTaskNotifyTake( pdTRUE, portMAX_DELAY);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
        vTaskDelay(1000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);

        if (flag == 0)
        {
            flag = 1;
            xTaskNotifyGive(Start_up_handeler);
        }
        else
        {
            xTaskNotifyGive(Normal_Handeler);
        }

    }
}



