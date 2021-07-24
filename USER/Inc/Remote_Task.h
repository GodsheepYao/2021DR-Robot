#ifndef _USER_REMOTE_H_
#define _USER_REMOTE_H_

#include "RMLibHead.h"
#include "CRC.h"
#include "remote.h"
#include "math.h" 

typedef struct
{
  uint16_t Left_Key_Up : 1;
  uint16_t Left_Key_Down : 1;
  uint16_t Left_Key_Left : 1;
  uint16_t Left_Key_Right : 1;
  uint16_t Left_Rocker : 1;
  uint16_t Left_Encoder : 1;
	uint16_t Left_Switch : 2;
  uint16_t Right_Key_Up : 1;
  uint16_t Right_Key_Down : 1;
  uint16_t Right_Key_Left : 1;
  uint16_t Right_Key_Right : 1;
  uint16_t Right_Rocker : 1;
  uint16_t Right_Encoder : 1;
	uint16_t Right_Switch : 2;
} hw_key_t;

#pragma pack(1)
typedef struct {
	uint8_t head;
	uint16_t rocker[4];
	hw_key_t Key;
	uint32_t Left_Encoder;
	uint32_t Right_Encoder;
  uint16_t crc;
} UART_DataPack;
#pragma pack()

typedef struct {
    int16_t Ex;
    int16_t Ey;
    int16_t Eangle;
    hw_key_t *Key_Control;
} Remote_Handle_t;

extern uint8_t usart4_dma_buff[21];
extern uint8_t usart1_dma_buff[21];
extern SemaphoreHandle_t Remote_Semaphore;
extern TaskHandle_t Remote_Task_Handler;
extern Remote_Handle_t Remote_Control;

void Remote_Recieve(Remote_Handle_t *rc, uint8_t *rx_data);

void Remote_Task(void *pvParameters);

void Remote_DataProcess(Remote_Handle_t *rc);

void Run_Speed(int16_t Ex, int16_t Ey, int16_t Eangle);

#endif

