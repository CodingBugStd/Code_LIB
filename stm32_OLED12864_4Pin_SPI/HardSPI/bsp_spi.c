#include "bsp_spi.h"

void BSP_SPI_Init(void)
{
    SPI_GPIO_Config();
    SPI_Config();
}   

void SPI_GPIO_Config(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;

    SPI_Pin_CLK_Init();

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    for(uint8_t temp = 0;temp<SPI_Pin_Count;temp++)
    {
        GPIO_InitStruct.GPIO_Pin = SPI_Pin[temp].Pin;
        GPIO_Init(SPI_Pin[temp].GPIO,&GPIO_InitStruct);
        GPIO_SetBits(SPI_Pin[temp].GPIO,SPI_Pin[temp].Pin);
    }
}

void SPI_NVIC_Config(void)
{
    NVIC_InitTypeDef    NVIC_InitStruct;

    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;

    NVIC_InitStruct.NVIC_IRQChannel = SPI1_IRQn;
    NVIC_Init(&NVIC_InitStruct);
    NVIC_InitStruct.NVIC_IRQChannel = SPI2_IRQn;
    NVIC_Init(&NVIC_InitStruct);

    SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_TXE,ENABLE);
    SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_TXE,ENABLE);
}

void SPI_Config(void)
{
    SPI_InitTypeDef SPI_InitStruct;

    SPI_CLK_Init();

    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_CRCPolynomial = 7;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

    SPI_Init(SPI1,&SPI_InitStruct);
    SPI_CalculateCRC(SPI1,DISABLE);     //关闭CRC校验

    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStruct.SPI_CRCPolynomial = 7;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

    SPI_Init(SPI2,&SPI_InitStruct);
    SPI_CalculateCRC(SPI2,DISABLE);     //关闭CRC校验

    SPI_Cmd(SPI1,ENABLE);
    SPI_Cmd(SPI2,ENABLE);
    SPI_Replace_Byte(1,0xff);
    SPI_Replace_Byte(2,0xff);
}

uint8_t SPI_Replace_Byte(uint8_t SPIx,uint8_t TxData)
{
    uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(Target_SPI[SPIx - 1], SPI_I2S_FLAG_TXE) == RESET)
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(Target_SPI[SPIx - 1], TxData);
	retry=0;
	while (SPI_I2S_GetFlagStatus(Target_SPI[SPIx - 1], SPI_I2S_FLAG_RXNE) == RESET)
		{
		retry++;
		if(retry>200)return 0;
		}
	return SPI_I2S_ReceiveData(Target_SPI[SPIx - 1]);
}

uint8_t*SPI_Repalce_NumByte(uint8_t SPIx,uint8_t*dat,uint8_t len)
{
    for(uint8_t temp=0;temp<len;temp++)
        SPI_Rx_Sbuffer[temp] = SPI_Replace_Byte(SPIx,*(dat+temp));
    return SPI_Rx_Sbuffer;
}

uint8_t SPI_Send_Byte(uint8_t SPIx,uint8_t TxData)
{
    uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(Target_SPI[SPIx - 1], SPI_I2S_FLAG_TXE) == RESET)
		{
		retry++;
		if(retry>200)return 1;
		}			  
	SPI_I2S_SendData(Target_SPI[SPIx - 1], TxData);
    return 0;
}

uint8_t SPI_Send_Byte_2(uint8_t SPIx,uint8_t TxData)
{
    uint8_t retry=0;				 	
	while (SPI_I2S_GetFlagStatus(Target_SPI[SPIx - 1], SPI_I2S_FLAG_TXE) == RESET)
		{
		retry++;
		if(retry>200)return 1;
		}			  
	SPI_I2S_SendData(Target_SPI[SPIx - 1], TxData);
    return 0;
}

uint8_t SPI_Send_NumByte(uint8_t SPIx,uint8_t*dat,uint8_t len)
{
    for(uint8_t temp=0;temp < len-1;temp++)
        SPI_Send_Byte(SPIx,*(dat+temp));
    return SPI_Replace_Byte(SPIx,*(dat+len-1));
}

uint8_t*SPI_Recieve_NumByte(uint8_t SPIx,uint8_t dat,uint8_t len)
{
    for(uint8_t temp=0;temp<len;temp++)
        SPI_Rx_Sbuffer[temp] = SPI_Replace_Byte(SPIx,dat);
    return SPI_Rx_Sbuffer;
}
