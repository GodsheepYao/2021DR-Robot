#include "FreeRTOS_Task.h"
#include "Remote_Task.h"
#include "RoboModule_Task.h"
#include "Auto_Task.h"
#include "Reset_Task.h"
#include "PID_task.h"

RoboModule_Typedef Chassis_RoboModule1 = {.ID = 0x10, .ExpPWM = 5000, .mode = Velocity_Mode},
                   Chassis_RoboModule2 = {.ID = 0x20, .ExpPWM = 5000, .mode = Velocity_Mode}, 
                   Chassis_RoboModule3 = {.ID = 0x30, .ExpPWM = 5000, .mode = Velocity_Mode};

RoboModule_Typedef Launch_RoboModule4 = {.ID = 0x110, 
                                         .ExpPWM = 5000,
                                         .mode = Velocity_Position_Mode,
                                         .ExpPosition = 300,
                                         .ExpVelocity = 300},
                   Launch_RoboModule5 = {.ID = 0x120, 
                                         .ExpPWM = 5000,
                                         .mode = Velocity_Position_Mode,
                                         .ExpPosition = 300,
                                         .ExpVelocity = 300};                   

CAN_Pack_Typedef Locate_data;
Position_Typedef Position;
Position_Typedef Pot_Position;
Manual_Flat_t Manual_Flat;										 								
uint8_t All_Ready = 0;
										 
void StartTask(void) {
    Remote_Semaphore = xSemaphoreCreateBinary();
    Auto_Semaphore = xSemaphoreCreateBinary();
	
    CanFilter_Init(&hcan1);
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

	CanFilter_Init(&hcan2);
	HAL_CAN_Start(&hcan2);
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING); 
    
#if RM_REMOTE
    HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart1, usart1_dma_buff, sizeof(usart1_dma_buff));
#else    
    HAL_NVIC_DisableIRQ(DMA1_Stream2_IRQn);
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart4, usart4_dma_buff, sizeof(usart4_dma_buff));
#endif
    
    /* 蓝砖任务 */
    xTaskCreate(RoboModule_Task, "RoboModule_task", 128, NULL, 3, &RoboModule_Handler);
						
	/* 复位任务 */
	xTaskCreate(Reset_Task, "Reset_task", 128, NULL, 1, &Reset_Task_Handler);	

	/* 自动任务 */
	xTaskCreate(Auto_Task, "Auto_Task", 256, NULL, 2, &Auto_Task_Handler);
	
	/* PID任务 */
	xTaskCreate(PID_task, "PID_task", 128, NULL, 2, &PIDTask_Handler);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
     if (hcan->Instance == CAN1) {
        uint32_t can_id = CAN_Receive_DataFrame(&hcan1, CAN1_buff);
        switch (can_id) {
			case 0x11B:
				CAN_RoboModule_DRV_Feedback(&Launch_RoboModule4,CAN1_buff);
				break;
			case 0x12B:
				CAN_RoboModule_DRV_Feedback(&Launch_RoboModule5,CAN1_buff);
				break;
		}   					
    }
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if (hcan->Instance == CAN2) {
		uint32_t can_id = CAN_Receive_DataFrame(&hcan2, CAN2_buff);
		switch (can_id) {
			case 0x710:
			memcpy(&Locate_data, CAN2_buff, sizeof(CAN_Pack_Typedef));
			Position.X = Locate_data.X;
			Position.Y = Locate_data.Y;
			Position.Angle = Locate_data.Angle / 100.0f + 360.0f * Locate_data.rand;
			break;
		}
	}
}

uint32_t Get_TimerTick() {
	return TIM2->CNT;
}

