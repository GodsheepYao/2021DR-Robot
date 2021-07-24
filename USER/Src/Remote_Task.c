#include "Remote_Task.h"
#include "FreeRTOS_Task.h"
#include "Auto_Task.h"
#include "RoboModule_Task.h"

uint8_t usart4_dma_buff[21];
uint8_t usart1_dma_buff[21];
UART_DataPack RemoteData;  
Remote_Handle_t Remote_Control;
uint8_t Mode_Change = 1;

SemaphoreHandle_t Remote_Semaphore;
TaskHandle_t Remote_Task_Handler;
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
void RemoteControlProcess(Remote *rc) {
    Chassis_RoboModule1.ExpVelocity = ((rc->ch2 - REMOTE_CONTROLLER_STICK_OFFSET) * Sensitivity_Y * sin(MOTOR_ANGLE) 
                                        - (rc->ch3 - REMOTE_CONTROLLER_STICK_OFFSET) * Sensitivity_X * cos(MOTOR_ANGLE) 
                                        + (rc->ch0 - REMOTE_CONTROLLER_STICK_OFFSET));
    Chassis_RoboModule2.ExpVelocity = (-(rc->ch2 - REMOTE_CONTROLLER_STICK_OFFSET) * Sensitivity_Y * sin(MOTOR_ANGLE) 
                                        + (-(rc->ch2 - REMOTE_CONTROLLER_STICK_OFFSET)) * Sensitivity_X * cos(MOTOR_ANGLE) 
                                        + (rc->ch0 - REMOTE_CONTROLLER_STICK_OFFSET));
    Chassis_RoboModule3.ExpVelocity = ((rc->ch2 - REMOTE_CONTROLLER_STICK_OFFSET) * Sensitivity_Y * sin(MOTOR_ANGLE)
                                        + (rc->ch3 - REMOTE_CONTROLLER_STICK_OFFSET) * Sensitivity_X * cos(MOTOR_ANGLE) 
                                        + (rc->ch0 - REMOTE_CONTROLLER_STICK_OFFSET));

}

#else 
/* 遥控器接收 */
void Remote_Recieve(Remote_Handle_t *rc, uint8_t *rx_data) {
    if(rx_data[0] == 0x5A) {
		if (((UART_DataPack *)rx_data)->crc == Verify_CRC16_Check_Sum(rx_data, sizeof(UART_DataPack) - 2)) {
			memcpy(&RemoteData, rx_data, sizeof(UART_DataPack));
			rc->Ex = (RemoteData.rocker[0] - 2047);
			rc->Ey = (RemoteData.rocker[1] - 2047);
			rc->Eangle = -(RemoteData.rocker[2] - 2047);
            rc->Key_Control = &RemoteData.Key;
            Remote_DataProcess(rc);
        }
    }
}

/* 遥控器数据处理 */
void Remote_DataProcess(Remote_Handle_t *rc) {
	if (rc->Key_Control->Left_Switch == 0) {
		Mode_Change = 1;
		Auto_step = 0;

		if(rc->Key_Control->Right_Switch == 0)
            Chassis_resolve_Exp.Angle += rc->Eangle * 0.0007f;
        else if (rc->Key_Control->Right_Switch == 2)
            Chassis_resolve_Exp.Angle = 90;
		else if(rc->Key_Control->Right_Switch == 1) {
			Chassis_resolve_Exp.Vx = rc->Ex * 0.15;
			Chassis_resolve_Exp.Vy = rc->Ey * 0.15;
			Chassis_resolve_Exp.Angle += rc->Eangle * 0.00004f;
		}
        
		if (rc->Key_Control->Right_Key_Up) 
			Manual_Flat.Paw_status = 1;
		else 
			Manual_Flat.Paw_status = 0;
		
		if (rc->Key_Control->Right_Key_Down) 
			Manual_Flat.Arrow4_status = 1;
		else 
			Manual_Flat.Arrow4_status = 0;
		
		if (rc->Key_Control->Right_Key_Left) 
			Manual_Flat.Arrow5_status = 1;
		else 
			Manual_Flat.Arrow5_status = 0;
        
		if (rc->Key_Control->Right_Switch != 1) {
			Chassis_resolve_Exp.Vx = rc->Ex * 0.7;
			Chassis_resolve_Exp.Vy = rc->Ey * 0.7;  
		}
	}
	else if (rc->Key_Control->Left_Switch == 2) {
		xSemaphoreGive(Auto_Semaphore);
		if (Mode_Change == 1) {
			Auto_step = 1;
			Mode_Change = 0;
		}
	}
	else if (rc->Key_Control->Left_Switch == 1) {
		if (rc->Key_Control->Right_Key_Down) 
			Manual_Flat.Arrow4_status = 1;
		else 
			Manual_Flat.Arrow4_status = 0;
		
		if (rc->Key_Control->Right_Key_Left) 
			Manual_Flat.Arrow5_status = 1;
		else 
			Manual_Flat.Arrow5_status = 0;
	}
}

#endif


