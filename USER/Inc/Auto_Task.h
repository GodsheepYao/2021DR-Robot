#ifndef _Auto_Task_H_
#define _Auto_Task_H_
#include "RMLibHead.h"

#include "PID.h"

extern TaskHandle_t Auto_Task_Handler;
extern SemaphoreHandle_t Auto_Semaphore;  
extern uint8_t Auto_step;
void Auto_Task(void *pvParameters);

#endif
