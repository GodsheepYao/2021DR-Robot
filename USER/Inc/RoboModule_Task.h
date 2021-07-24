#ifndef __CHASSIS_RUN_TASK_H
#define __CHASSIS_RUN_TASK_H

#include "RMLibHead.h"
#include "RoboModule_DRV.h"

typedef struct {
	int16_t Vx;
	int16_t Vy;
	float Angle;
}Chassis_resolve_t;

extern Chassis_resolve_t Chassis_resolve_Exp;

extern TaskHandle_t RoboModule_Handler;
void RoboModule_Task(void *pvParameters);

void Chassis_MotorData_Process(int16_t Ex, int16_t Ey, int16_t Eangle);
void setPos(RoboModule_Typedef* m, int32_t speed, int32_t pos);

#endif

