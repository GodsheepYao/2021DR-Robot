#include "Reset_Task.h"
#include "Remote_Task.h"
#include "FreeRTOS_Task.h"
#include "RoboModule_Task.h"
#include "Manual_Task.h"


int16_t start_position1,start_position2;
								 
TaskHandle_t Reset_Task_Handler;
void Reset_Task(void *pvParameters)
{
	portTickType xLastWakeTime = xTaskGetTickCount();
	
	uint8_t position_buff[8] = { 0x55 };
	setPos(&Launch_RoboModule5, 300, 300);
	setPos(&Launch_RoboModule4, 300, 400);
	vTaskDelay(100);
	
	while(HAL_GPIO_ReadPin(SW3_GPIO_Port,SW3_Pin) == GPIO_PIN_SET || HAL_GPIO_ReadPin(SW4_GPIO_Port,SW4_Pin) == GPIO_PIN_SET) {
		if(HAL_GPIO_ReadPin(SW3_GPIO_Port,SW3_Pin) == GPIO_PIN_SET)
			Launch_RoboModule4.ExpPosition -= 1;
		if(HAL_GPIO_ReadPin(SW4_GPIO_Port,SW4_Pin) == GPIO_PIN_SET)
			Launch_RoboModule5.ExpPosition -= 1;
		vTaskDelay(1);
	}
	
	for(;;)
	{
		if((HAL_GPIO_ReadPin(SW3_GPIO_Port,SW3_Pin) == GPIO_PIN_RESET) && (HAL_GPIO_ReadPin(SW4_GPIO_Port,SW4_Pin) == GPIO_PIN_RESET)) {
			vTaskDelay(10);
			if((HAL_GPIO_ReadPin(SW3_GPIO_Port,SW3_Pin) == GPIO_PIN_RESET) && (HAL_GPIO_ReadPin(SW4_GPIO_Port,SW4_Pin) == GPIO_PIN_RESET)) {
				start_position1 = Launch_RoboModule4.Position;
				start_position2 = Launch_RoboModule5.Position;
				setPos(&Launch_RoboModule4, 500, start_position1 + 2900);
				setPos(&Launch_RoboModule5, 500, start_position2 + 3200);
				
				/* 遥控器任务 */
				xTaskCreate(Remote_Task, "Remote_task", 128, NULL, 2, &Remote_Task_Handler);
				
                /* 半自动任务 */
				xTaskCreate(Manual_task, "Manual_task", 128, NULL, 2, &ManualTask_Handler);
				
				CAN_Send_StdDataFrame(&hcan2, 0x710, position_buff);
                
				vTaskDelete(NULL);
			}
		}
		vTaskDelayUntil(&xLastWakeTime, 2);
	}
}
