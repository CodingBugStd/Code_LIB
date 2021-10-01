#ifndef _BSP_OLED12864_H_
#define _BSP_OLED12864_H_

#include "stm32f10x.h"
#include "self_type.h"

/*************************************************
 * 基于stm32f103zet6的OLED12864 4Pin_SPI
 * OLED12864驱动芯片:SSD1306
 * Creat by: 庞碧璋
 * Github: https://github.com/CodingBugStd
 * csdn:   https://blog.csdn.net/RampagePBZ
 * Encoding: utf-8
 * date:    2021/6/22
*************************************************/

//外部延时函数
#include "soft_delay.h"
#define OLED12864_delay_ms(ms)  soft_delay_ms(ms)
extern void OLED12864_delay_ms(unsigned int ms);

//硬件相关宏定义,定义物理引脚名字
#define OLED_D0_Pin     GPIO_Pin_9
#define OLED_D0_GPIO    GPIOD
#define OLED_D1_Pin     GPIO_Pin_11
#define OLED_D1_GPIO    GPIOD
#define OLED_RES_Pin    GPIO_Pin_13
#define OLED_RES_GPIO   GPIOD
#define OLED_DC_Pin     GPIO_Pin_15
#define OLED_DC_GPIO    GPIOD
#define OLED_CS_Pin     GPIO_Pin_3
#define OLED_CS_GPIO    GPIOG
#define x_MAX           128
#define y_MAX           64
#define page_MAX        8

//OLED模式
#define USE_POINT_CRT       1   //使用像素点级别操作
//第二层宏定义,定义引脚功能
#define OLED_SCLK_Pin   OLED_D0_Pin
#define OLED_SCLK_GPIO  OLED_D0_GPIO
#define OLED_SDIN_Pin   OLED_D1_Pin
#define OLED_SCIN_GPIO  OLED_D1_GPIO


//引脚编号
#define OLED_SCLK   0
#define OLED_SDIN   1
#define OLED_RES    2
#define OLED_DC     3
#define OLED_CS     4
static const Pin OLED_Pin[5] = {
    {OLED_SCLK_Pin,OLED_SCLK_GPIO},
    {OLED_SDIN_Pin,OLED_SCIN_GPIO},
    {OLED_RES_Pin,OLED_RES_GPIO},
    {OLED_DC_Pin,OLED_DC_GPIO},
    {OLED_CS_Pin,OLED_CS_GPIO}
};

/****************************
 * 在其它地方定义
 typedef struct
{
    uint16_t Pin;
    GPIO_TypeDef*GPIO;
}Pin;
******************************/


#define OLED_CMD    0
#define OLED_DATA   1

/************************************************
 * OLED12864缓存
 * [paeg][x]
 * 一个元素保函8像素点信息
 * 在屏幕上,Bit0~Bit7自上向下排列 高位在下
************************************************/
static uint8_t OLED12864_Sbuffer[8][128];
//OLED_初始化指令
static const uint8_t OLED12864_InitCmd[28] = {
    0xae,0x00,0x10,0x40,0x81,0xcf,
    0xa1,0xc8,0xa6,0xa8,0x3f,0xd3,
    0x00,0xd5,0x80,0xd9,0xf1,0xda,
    0x12,0xdb,0x40,0x20,0x02,0x8d,
    0x14,0xa4,0xa6,0xaf
};

//初始化
void BSP_OLED12864_Init(void);
void OLED12864_GPIO_Init(void);
void OLED12864_Hard_Reset(void);

void OLED12864_Set_Bit(uint8_t bit);                //底层函数
void OLED12864_Reset_Bit(uint8_t bit);              //底层函数
void OLED12864_Send_Byte(uint8_t dat,uint8_t cmd);
void OLED12864_Send_NumByte(const uint8_t*dat,uint8_t len,uint8_t cmd);
void OLED12864_Refresh(void);
void OLED12864_Set_Position(uint8_t page,uint8_t x);
void OLED12864_Clear_Sbuffer(void);   
void OLED12864_Clear(void);

//y坐标位置和高度都以page为单位的图形操作
void OLED12864_Clear_PageBlock(uint8_t page,uint8_t x,uint8_t len);
void OLED12864_Clear_Page(uint8_t page);
void OLED12864_Show_Char(uint8_t page,uint8_t x,uint8_t chr,uint8_t size);
void OLED12864_Show_String(uint8_t page,uint8_t x,uint8_t*str,uint8_t size);
uint8_t OLED12864_Show_Num(uint8_t page,uint8_t x,int num,uint8_t size);    //返回num的长度
uint8_t OLED12864_Show_fNum(uint8_t page,uint8_t x,double num,uint8_t size,uint8_t d_len);

    //以单个像素点为单位的图形操作
    #if USE_POINT_CRT == 1
    void OLED12864_Draw_Point(uint8_t x,uint8_t y,uint8_t bit);             //<--像素点操作底层函数
    void OLED12864_Draw_Line(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
    void OLED12864_Draw_Rect(uint8_t x,uint8_t y,uint8_t len,uint8_t hight);
    //*img 以字节竖直,高位在上,数据水平
    void OLED12864_Draw_Img(uint8_t x,uint8_t y,uint8_t len,uint8_t hight,uint8_t*img);
    void OLED12864_Draw_aImg(uint8_t x,uint8_t y,uint8_t*img);  //自带图形大小识别
    #endif  //USE_POINT_CRT

#endif

