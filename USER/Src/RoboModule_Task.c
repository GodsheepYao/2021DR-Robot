#include "RoboModule_Task.h"
#include "FreeRTOS_Task.h"
#include "arm_math.h"

Chassis_resolve_t Chassis_resolve_Exp;
		   
static float Ecos, Esin;
									 
TaskHandle_t RoboModule_Handler;
void RoboModule_Task(void *pvParameters) {
    portTickType xLastWakeTime = xTaskGetTickCount();
    
	Ecos = arm_cos_f32(MOTOR_ANGLE);
	Esin = arm_sin_f32(MOTOR_ANGLE);
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
	
    CAN_RoboModule_DRV_Reset(&hcan1, &Launch_RoboModule4);      
	CAN_RoboModule_DRV_Reset(&hcan1, &Launch_RoboModule5);
	vTaskDelay(500);
		
	CAN_RoboModule_DRV_Mode_Choice(&hcan1, &Launch_RoboModule4, Velocity_Position_Mode); 
	CAN_RoboModule_DRV_Mode_Choice(&hcan1, &Launch_RoboModule5, Velocity_Position_Mode);  
	vTaskDelay(1000);
		
	CAN_RoboModule_DRV_Config(&hcan1,&Launch_RoboModule4,2,0);
	CAN_RoboModule_DRV_Config(&hcan1,&Launch_RoboModule5,2,0);
	
	
    for (;;) {	
		CAN_RoboModule_DRV_Send(&hcan1, &Chassis_RoboModule1);
		vTaskDelay(1);
		CAN_RoboModule_DRV_Send(&hcan1, &Chassis_RoboModule2);
		vTaskDelay(1);
		CAN_RoboModule_DRV_Send(&hcan1, &Chassis_RoboModule3);
		vTaskDelay(1);
			
		CAN_RoboModule_DRV_Send(&hcan1, &Launch_RoboModule4);
		vTaskDelay(1);
		CAN_RoboModule_DRV_Send(&hcan1, &Launch_RoboModule5);
		vTaskDelay(1);
    }
}

void Chassis_MotorData_Process(int16_t Ex, int16_t Ey, int16_t Eangle) {
	Chassis_RoboModule1.ExpVelocity = Ex * Esin - Ey * Ecos -Eangle;
	Chassis_RoboModule2.ExpVelocity = -Ex * Ecos + (-Ex) * Esin - Eangle;
	Chassis_RoboModule3.ExpVelocity = Ex * Esin + Ey * Ecos - Eangle;
}

void setPos(RoboModule_Typedef* m, int32_t speed, int32_t pos) {
    m->ExpVelocity = speed;
    m->ExpPosition = pos;
    while (!CAN_RoboModule_DRV_PositionArrive(m, 40))
            vTaskDelay(2);
}

