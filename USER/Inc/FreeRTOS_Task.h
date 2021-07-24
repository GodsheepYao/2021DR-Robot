#ifndef _FREERTOS_TASK_H_
#define _FREERTOS_TASK_H_
#include "RMLibHead.h"

#include "usart.h"
#include "tim.h"

#include "ramp.h"
#include "PID.h"
#include "CANDrive.h"
#include "RoboModule_DRV.h"

#define PI	            3.14159265358979f
#define MOTOR_ANGLE     34.75f / 180.0f * PI
#define RM_REMOTE       0
#define Sensitivity_X   5.0f
#define Sensitivity_Y   5.0f

#pragma pack(1)
typedef struct {
    int16_t X;
    int16_t Y;
    int16_t Angle;
	int16_t rand;
} CAN_Pack_Typedef;
#pragma pack()

typedef struct {
    int16_t X;
    int16_t Y;
    float Angle;
} Position_Typedef;

typedef struct {
    uint8_t Pot_status    : 1;
	uint8_t Paw_status 	  : 1;
	uint8_t Arrow4_status : 1;
	uint8_t Arrow5_status : 1;
	uint8_t Run_Self_Left : 1;
	uint8_t Run_Self_Right: 1;
} Manual_Flat_t;


extern RoboModule_Typedef Chassis_RoboModule1, Chassis_RoboModule2,     //!<@brief 底盘蓝砖结构体
                          Chassis_RoboModule3;
extern RoboModule_Typedef Launch_RoboModule4, Launch_RoboModule5;		//!<@brief 上部机构蓝砖结构体

extern uint8_t All_Ready;

extern int16_t start_position1,start_position2;                         

extern Position_Typedef Position;                                       //!<@brief 当前位置

extern Position_Typedef Pot_Position;                                   //!<@brief 壶位置

extern Manual_Flat_t Manual_Flat;                                       //!<@brief 半自动模式动作

void StartTask(void);
void Launch_Send_Back_Data(uint8_t rx_data[]);
#endif
