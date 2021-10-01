#ifndef _SELF_TYPE_H_
#define _SELF_TYPE_H_

#include "stm32f10x.h"

//用户指令
typedef struct
{
    uint8_t Source;     //指令来源
    uint8_t Cmd_Type;   //指令类型
    uint8_t dat_len;    //指令长度
    uint8_t*dat;        //指令
}User_Cmd;

//MCU引脚
typedef struct
{
    uint16_t Pin;
    GPIO_TypeDef*GPIO;
}Pin;

typedef struct
{
    uint8_t num;
    uint16_t on_time;
    uint16_t cycle;
}LED_Shine_Data;

typedef struct 
{
	uint8_t TaskPriority;
	uint16_t TaskDelayTime;
}Usart_Send_TaskDat;

#endif
