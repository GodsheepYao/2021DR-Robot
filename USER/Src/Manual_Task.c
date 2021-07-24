#include "FreeRTOS_Task.h"
#include "Remote_Task.h"
#include "RoboModule_Task.h"
#include "Manual_Task.h"
#include "Ground_to_walk.h"

TaskHandle_t ManualTask_Handler;
void Manual_task(void *pvParameters) {
	portTickType xLastWakeTime = xTaskGetTickCount(); 
	Straight_Init Pot_Line;
	for(;;) {	
        /* 正壶 */
		if (Manual_Flat.Paw_status) {
			HAL_GPIO_TogglePin(IN1_GPIO_Port, IN1_Pin);
			while(Manual_Flat.Paw_status) vTaskDelay(4);
		}
		
        /* 投箭测试1 */
		if (Manual_Flat.Arrow4_status) {
			setPos(&Launch_RoboModule4, 2812, start_position1 + 8400);//2811  8400  2820 blue
			vTaskDelay(100);
			setPos(&Launch_RoboModule4, 1500, start_position1 + 2900);
			while(Manual_Flat.Arrow4_status) vTaskDelay(2);
		}
		
        /* 投箭测试2 */
		if(Manual_Flat.Arrow5_status) {
			setPos(&Launch_RoboModule5, 2681, start_position2 + 8400);//8310  2651 2724 2650 2694 1点34 blue////
			vTaskDelay(160);
			setPos(&Launch_RoboModule5, 1500, start_position2 + 3200);
			while(Manual_Flat.Arrow4_status) vTaskDelay(2);
		}
		
        /* 跑壶 */
        if (Manual_Flat.Pot_status) {
            straight_Init(&Pot_Line, 3000, 5000, Pot_Position.X, Pot_Position.Y, 100);
            while(!straight_line(&Pot_Line)) vTaskDelay(5);
            Manual_Flat.Pot_status = 0;
        }
		
		vTaskDelayUntil(&xLastWakeTime, 5);
	}
}
