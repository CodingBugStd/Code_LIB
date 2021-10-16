#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_

#include "stm32f10x.h"
#include "self_type.h"

/*************************************************
 * 基于stm32f103c8t6的SPI支持包
 * 使用了官方固件库
 * 硬件SPI
 * 
 * Create by: 庞碧璋
 * Github: https://github.com/CodingBugStd
 * csdn:   https://blog.csdn.net/RampagePBZ
 * Encoding: utf-8
 * create date: 2021/10/12
 * last date:   2021/10/12
*************************************************/

#define SPI_Pin_Count    5
#define SPI_Pin_CLK_Init()  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); \
                            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE)
#define SPI_CLK_Init()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE); \
                            RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE)

#define SPI_Rx_SbufferSize  64

static SPI_TypeDef* Target_SPI[2] = {SPI1,SPI2};
static uint8_t SPI_Rx_Sbuffer[SPI_Rx_SbufferSize] = {0};

static const Pin SPI_Pin[SPI_Pin_Count] = {
    {GPIO_Pin_5,GPIOA},{GPIO_Pin_6,GPIOA},{GPIO_Pin_7,GPIOA},
    {GPIO_Pin_13,GPIOB},{GPIO_Pin_15,GPIOB}};

void BSP_SPI_Init(void);
void SPI_Config(void);
void SPI_NVIC_Config(void);
void SPI_GPIO_Config(void);

//发送和接收
uint8_t SPI_Replace_Byte(uint8_t SPIx,uint8_t TxData);                  //直接使用SPI交换一个字节
uint8_t SPI_Send_Byte(uint8_t SPIx,uint8_t TxData);                     //直接使用SPI发送一个字节
uint8_t*SPI_Repalce_NumByte(uint8_t SPIx,uint8_t*dat,uint8_t len);      //使用SPI交换len个字节,接收的字节存入sbuffer,并且返回首地址
uint8_t SPI_Send_NumByte(uint8_t SPIx,uint8_t*dat,uint8_t len);         //使用SPI发送len个字节,只保留最后交换的字节
uint8_t*SPI_Recieve_NumByte(uint8_t SPIx,uint8_t dat,uint8_t len);      //使用SPI接收len个字节,使用dat做为交换字节
uint8_t SPI_Send_Byte_2(uint8_t SPIx,uint8_t TxData);                   //直接使用SPI发送一个字节,只有在发送全部完成后才返回

#endif
