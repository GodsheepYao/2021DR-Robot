#ifndef _FREERTOS_TASK_H_
#define _FREERTOS_TASK_H_
#include "RMLibHead.h"

#include "usart.h"

#include "PID.h"
#include "CANDrive.h"
#include "RoboModule_DRV.h"

#define PI	            3.14159265358979f
#define MOTOR_ANGLE     25.0f / 180.0f * PI
#define VY_CONSTANT     0.42262f
#define VX_CONSTANT     0.90631f
#define RM_REMOTE       0
#define Sensitivity_X   5.0f
#define Sensitivity_Y   5.0f
extern RoboModule_Typedef Chassis_RoboModule1, Chassis_RoboModule2, Chassis_RoboModule3;

void StartTask(void);

#endif
