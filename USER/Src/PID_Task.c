#include "PID_task.h"
#include "Auto_Task.h"
#include "FreeRTOS_Task.h"
#include "Remote_task.h"
#include "RoboModule_Task.h"
#include "arm_math.h"

extern int PID_Flag;
PID Z_PID = {.Kp = 35, .Ki = 0.005, .Kd = 20, .limit = 150};
Pow_Msg_t Power_Control = {80, 100};
TaskHandle_t PIDTask_Handler;
void PID_task(void *pvParameters) {
    portTickType xLastWakeTime = xTaskGetTickCount();  
    float _x = 0;
    float _y = 0;
    float _sin = 0;
    float _cos = 0;
	uint8_t position_buff[8] = { 0x55 };	
    for (;;) {	
		PID_Control(Position.Angle, Chassis_resolve_Exp.Angle, &Z_PID);
		
		limit(Z_PID.pid_out, 1000, -1000);
		arm_sin_cos_f32(-fmodf(Position.Angle, 360), &_sin, &_cos);
		_x = Chassis_resolve_Exp.Vx * _cos + Chassis_resolve_Exp.Vy * -_sin;
		_y = Chassis_resolve_Exp.Vx * _sin + Chassis_resolve_Exp.Vy * _cos;
		
		if (Remote_Control.Key_Control->Left_Switch == 1) { 
			Chassis_MotorData_Process(0, 0, 0);
			CAN_Send_StdDataFrame(&hcan2, 0x710, position_buff);
			Chassis_resolve_Exp.Angle = 0;
		}
		else {
			Chassis_MotorData_Process(_x, _y, Z_PID.pid_out);
		}
		CAN_Send_StdDataFrame(&hcan2, 0x210, (uint8_t *)&Power_Control);
        vTaskDelayUntil(&xLastWakeTime, 5);
    }
}

