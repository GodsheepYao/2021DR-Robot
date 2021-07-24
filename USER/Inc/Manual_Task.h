#ifndef _MANUAL_TASK_H_
#define _MANUAL_TASK_H_

#include "RMLibHead.h"
                        
extern TaskHandle_t ManualTask_Handler;
void Manual_task(void *pvParameters);
void ManualTask_Init(void);

#endif
