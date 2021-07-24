#include "Auto_Task.h"
#include "FreeRTOS_Task.h"
#include "Remote_Task.h"
#include "RoboModule_Task.h"
#include "Ground_to_walk.h"

int aaa = 2800;//2820  2800
int xxx = 7710;
float hhh = 45.2;

uint8_t Auto_step = 0;
SemaphoreHandle_t Auto_Semaphore; 
TaskHandle_t Auto_Task_Handler;
void Auto_Task(void *pvParameters)
{
	portTickType xLastWakeTime = xTaskGetTickCount();
	
	Straight_Init Straight_line;
    Piecewise_Init Piecewise_line;
	uint8_t position_buff[8] = { 0x55 };
    Segment_Init Segment_line[3] = {{.point = {.X = -750, .Y = 1812}, .V_max = 5000, .S_a = 700, .error = 200},
                                    {.point = {.X = -3504, .Y = 4882}, .V_max = 5000, .S_a = 1500, .error = 1000},
                                    {.point = {.X = -4800, .Y = 4550}, .V_max = 5000, .S_a = 900, .error = 20}};  //4860 blue 
    int16_t Tpoint_flag = 0;
	BaseType_t err = pdFALSE;
    for( ;; ) {
        err = xSemaphoreTake(Auto_Semaphore, portMAX_DELAY);

        if (err == pdTRUE) {
			switch (Auto_step) {
				case 0:
					break;
				case 1:
					setPos(&Launch_RoboModule5, 2685, start_position2 + 8400);//发射2  3345 7700  //3100  7600  //3100  7930  短杆 25.4V
					setPos(&Launch_RoboModule5, 1500, start_position2);	
					vTaskDelay(800);
					setPos(&Launch_RoboModule4, 2815, start_position1 + 8400);//发射1  //25.1~25.5 2800   7710
					setPos(&Launch_RoboModule4, 1500, start_position1);  	
					CAN_Send_StdDataFrame(&hcan2, 0x710, position_buff);
					if(Auto_step == 0)break;
					Auto_step = 2;
					break;
				case 2:
					piecewise_Init(&Piecewise_line, Segment_line, 3);
                    while (Tpoint_flag != 0xff) {
                        Tpoint_flag = piecewise_line(&Piecewise_line);
                        if(Tpoint_flag == 2) Chassis_resolve_Exp.Angle = 45;
                        if(Tpoint_flag == 3) Chassis_resolve_Exp.Angle = 0;
                        if(Auto_step == 0)break;
                        vTaskDelay(5);
                    }
					Auto_step = 3;
					break;
				case 3:
					straight_Init(&Straight_line, 2000, 7000, -4750, 3420, 20); // 4780 blue
					while(!straight_line(&Straight_line)) vTaskDelay(2);
				    CAN_Send_StdDataFrame(&hcan2, 0x710, position_buff);
					Chassis_resolve_Exp.Angle = 0;
					if(Auto_step == 0)break;
					Auto_step = 0xff;
					break;
				default:
					Chassis_resolve_Exp.Vx = 0;
					Chassis_resolve_Exp.Vy = 0;
					break;
			}
			
        }
        else if (err == pdFALSE){
            vTaskDelay(10);
        }
    }
}

Point_Typedef GetLocation() {
    Point_Typedef p = {0, 0};
	p.X = Position.X;
	p.Y = Position.Y;
    return p;
}

void SetSpeed(float Vx, float Vy) {
    Chassis_resolve_Exp.Vx = Vx;
	Chassis_resolve_Exp.Vy = Vy;
}
