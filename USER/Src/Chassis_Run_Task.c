#include "Chassis_Run_Task.h"
#include "FreeRTOS_Task.h"

TaskHandle_t Chassis_Run_Handler;
void Chassis_Run_Task(void *pvParameters) {
    portTickType xLastWakeTime = xTaskGetTickCount();
    
    CAN_RoboModule_DRV_Reset(&hcan1, &Chassis_RoboModule1);      
    CAN_RoboModule_DRV_Reset(&hcan1, &Chassis_RoboModule2); 	 
    CAN_RoboModule_DRV_Reset(&hcan1, &Chassis_RoboModule3);      
    vTaskDelay(1000);
    
    CAN_RoboModule_DRV_Reset(&hcan1, &Chassis_RoboModule1);      
    CAN_RoboModule_DRV_Reset(&hcan1, &Chassis_RoboModule2); 	 
    CAN_RoboModule_DRV_Reset(&hcan1, &Chassis_RoboModule3);      
    vTaskDelay(500);
    
    CAN_RoboModule_DRV_Mode_Choice(&hcan1, &Chassis_RoboModule1, Velocity_Mode); 
    CAN_RoboModule_DRV_Mode_Choice(&hcan1, &Chassis_RoboModule2, Velocity_Mode);   
    CAN_RoboModule_DRV_Mode_Choice(&hcan1, &Chassis_RoboModule3, Velocity_Mode);   
    vTaskDelay(500);
    
    for(;;) {
        CAN_RoboModule_DRV_Velocity_Mode(&hcan1, &Chassis_RoboModule1);
        CAN_RoboModule_DRV_Velocity_Mode(&hcan1, &Chassis_RoboModule2);
        CAN_RoboModule_DRV_Velocity_Mode(&hcan1, &Chassis_RoboModule3);
        vTaskDelayUntil(&xLastWakeTime, 10);
    }
}

