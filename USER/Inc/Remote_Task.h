#ifndef _USER_REMOTE_H_
#define _USER_REMOTE_H_

#include "RMLibHead.h"
#include "CRC.h"
#include "remote.h"
#include "math.h" 

typedef enum {
    Ru_11_KEY	= 0x11,
    Rr_12_KEY	= 0x12,
    Rd_13_KEY	= 0x13,
    Rl_14_KEY	= 0x14,
    Lu_15_KEY	= 0x15,
    Lr_16_KEY	= 0x16,
    Ld_17_KEY	= 0x17,
    Ll_18_KEY	= 0x18,
    L_ROCKET_SW = 0x23,
    R_ROCKET_SW	= 0x24,
    MANUAL_RUN	= 0xdd
}Remote_Status;

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

typedef struct 
{
	uint8_t Pack_H;	//Head
	uint8_t Pack_L;	//Tail
	uint8_t Pack_Length;
	uint8_t Recvdat;
	uint8_t RecvFlag;
	uint8_t Pack[50];
	uint8_t RecvSum;
	
	volatile uint8_t RecvPoint;
}Recv_Typedef;

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

extern uint8_t usart4_dma_buff[30];
extern uint8_t usart1_dma_buff[30];
extern SemaphoreHandle_t Remote_Semaphore;
extern TaskHandle_t Remote_Task_Handler;

void Remote_Task(void *pvParameters);

void Remote_DataProcess(Remote_Handle_t *rc);

#endif

