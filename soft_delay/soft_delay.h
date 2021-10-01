#ifndef _SOFT_DELAY_H_
#define _SOFT_DELAY_H_

/********************************
 * 仅适用于系统时钟72MHz stm32f10x
 * 误差5%
 * 2021/6/29 庞碧璋
********************************/

#include <stdio.h>

void soft_delay_ms(unsigned int ms);
void soft_delay_us(unsigned int us);
void swap_byte(unsigned char *a,unsigned char *b);

#endif
