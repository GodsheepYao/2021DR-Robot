#include "Remote_Task.h"
#include "FreeRTOS_Task.h"

uint8_t usart4_dma_buff[30];
uint8_t usart1_dma_buff[30];
UART_DataPack dataPack;
uint8_t Mode_Change_flag = 0;

Remote_Handle_t Remote_Control;
SemaphoreHandle_t Remote_Semaphore;
TaskHandle_t Remote_Task_Handler;
void Remote_Recieve(Remote_Handle_t *rc, uint8_t *rx_data);

void Remote_Task(void *pvParameters) {
    BaseType_t err = pdFALSE;
    for( ;; ) {
        err = xSemaphoreTake(Remote_Semaphore, portMAX_DELAY);
        if (err == pdTRUE) {
#if RM_REMOTE
            Remote_Rx(usart1_dma_buff);
#else
            Remote_Recieve(&Remote_Control, usart4_dma_buff);           
#endif     
        }
        else if (err == pdFALSE){
            vTaskDelay(10);
        }
    }
}

#if RM_REMOTE
/* RMÒ£¿ØÆ÷ */
//void RemoteControlProcess(Remote *rc) {
//    Chassis_RoboModule3.ExpVelocity = (int16_t)((rc->ch2 - REMOTE_CONTROLLER_STICK_OFFSET) * Sensitivity_Y * VY_CONSTANT 
//                                        + (rc->ch3 - REMOTE_CONTROLLER_STICK_OFFSET) * Sensitivity_X * VX_CONSTANT 
//                                        + (rc->ch0 - REMOTE_CONTROLLER_STICK_OFFSET));
//    Chassis_RoboModule2.ExpVelocity = (int16_t)(-(rc->ch2 - REMOTE_CONTROLLER_STICK_OFFSET) * Sensitivity_Y * VY_CONSTANT 
//                                        + (-(rc->ch2 - REMOTE_CONTROLLER_STICK_OFFSET)) * Sensitivity_X * VX_CONSTANT 
//                                        + (rc->ch0 - REMOTE_CONTROLLER_STICK_OFFSET));
//    Chassis_RoboModule1.ExpVelocity = (int16_t)((rc->ch2 - REMOTE_CONTROLLER_STICK_OFFSET) * Sensitivity_Y * VY_CONSTANT 
//                                        - (rc->ch3 - REMOTE_CONTROLLER_STICK_OFFSET) * Sensitivity_X * VX_CONSTANT 
//                                        + (rc->ch0 - REMOTE_CONTROLLER_STICK_OFFSET));
//}

void RemoteControlProcess(Remote *rc) {
    Chassis_RoboModule1.ExpVelocity = (int16_t)((rc->ch2 - REMOTE_CONTROLLER_STICK_OFFSET) * Sensitivity_Y * sin(MOTOR_ANGLE) 
                                        - (rc->ch3 - REMOTE_CONTROLLER_STICK_OFFSET) * Sensitivity_X * cos(MOTOR_ANGLE) 
                                        + (rc->ch0 - REMOTE_CONTROLLER_STICK_OFFSET));
    Chassis_RoboModule2.ExpVelocity = (int16_t)(-(rc->ch2 - REMOTE_CONTROLLER_STICK_OFFSET) * Sensitivity_Y * sin(MOTOR_ANGLE) 
                                        + (-(rc->ch2 - REMOTE_CONTROLLER_STICK_OFFSET)) * Sensitivity_X * cos(MOTOR_ANGLE) 
                                        + (rc->ch0 - REMOTE_CONTROLLER_STICK_OFFSET));
    Chassis_RoboModule3.ExpVelocity = (int16_t)((rc->ch2 - REMOTE_CONTROLLER_STICK_OFFSET) * Sensitivity_Y * sin(MOTOR_ANGLE)
                                        + (rc->ch3 - REMOTE_CONTROLLER_STICK_OFFSET) * Sensitivity_X * cos(MOTOR_ANGLE) 
                                        + (rc->ch0 - REMOTE_CONTROLLER_STICK_OFFSET));

}

#else 
/* Ò£¿ØÆ÷½ÓÊÕ */
void Remote_Recieve(Remote_Handle_t *rc, uint8_t *rx_data) {
    if(rx_data[0] == 0x5A) {
        UART_DataPack* pack = (UART_DataPack *)(rx_data);          
        if(pack->crc == Verify_CRC16_Check_Sum(rx_data, 19)) {
            dataPack = *pack;  
            
            rc->Ex = (dataPack.rocker[0] - 2047);
            rc->Ey = (dataPack.rocker[1] - 2047);
            rc->Eangle = (dataPack.rocker[2] - 2047);
            rc->Key_Control = &dataPack.Key;
            
            Remote_DataProcess(rc);
        }
    }
}

/* Ò£¿ØÆ÷¹¦ÄÜ */
void Remote_DataProcess(Remote_Handle_t *rc) {
    if (rc->Key_Control->Left_Rocker) {
        Chassis_RoboModule1.ExpVelocity = (int16_t) rc->Ey * 0.5;
        Chassis_RoboModule2.ExpVelocity = (int16_t) rc->Ey * 0.5;
        Chassis_RoboModule3.ExpVelocity = (int16_t) rc->Ey * 0.5;
    } else {
        Chassis_RoboModule1.ExpVelocity = (int16_t)(rc->Ex * cos(MOTOR_ANGLE) - rc->Ey * sin(MOTOR_ANGLE));
        Chassis_RoboModule2.ExpVelocity = (int16_t)(-rc->Ex * cos(MOTOR_ANGLE) + (-rc->Ex) * sin(MOTOR_ANGLE));
        Chassis_RoboModule3.ExpVelocity = (int16_t)(rc->Ex * cos(MOTOR_ANGLE) + rc->Ey * sin(MOTOR_ANGLE));
    }
}

#endif


