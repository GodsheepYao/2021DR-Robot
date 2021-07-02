#include "FreeRTOS_Task.h"
#include "Remote_Task.h"
#include "Chassis_Run_Task.h"

RoboModule_Typedef Chassis_RoboModule1 = {.ID = 0x10, .ExpPWM = 5000},
                   Chassis_RoboModule2 = {.ID = 0x20, .ExpPWM = 5000}, 
                   Chassis_RoboModule3 = {.ID = 0x30, .ExpPWM = 5000};

void StartTask(void) {
    Remote_Semaphore = xSemaphoreCreateBinary();
    
    CanFilter_Init(&hcan1);
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
    
#if RM_REMOTE
    HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart1, usart1_dma_buff, sizeof(usart1_dma_buff));
#else    
    HAL_NVIC_DisableIRQ(DMA1_Stream2_IRQn);
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart4, usart4_dma_buff, sizeof(usart4_dma_buff));
#endif
    
    /* 底盘任务 */
    xTaskCreate((TaskFunction_t)Chassis_Run_Task,
            (const char *)"Chassis_Run_task",
            (uint16_t)256,
            (void *)NULL,
            (UBaseType_t)2,
            (TaskHandle_t *)&Chassis_Run_Handler);
      
    /* 遥控器接收任务 */
    xTaskCreate((TaskFunction_t)Remote_Task,
            (const char *)"Remote_task",
            (uint16_t)256,
            (void *)NULL,
            (UBaseType_t)2,
            (TaskHandle_t *)&Remote_Task_Handler);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
     if (hcan->Instance == CAN1) {
        uint32_t can_id = CAN_Receive_DataFrame(&hcan1, CAN1_buff);  
    }
}


