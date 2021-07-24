#ifndef _PID_TASK_H_
#define _PID_TASK_H_

#include "RMLibHead.h"
#include "PID.h"

typedef struct {
	float power_limit;              //!<@brief 当前裁判系统功率限制
	uint16_t chassis_power_buffer;  //!<@brief 当前底盘缓冲能量
}Pow_Msg_t;

extern TaskHandle_t PIDTask_Handler;
void PID_task(void *pvParameters);
void PIDTask_Init(void);

#endif
