/*******************************************************************************************************
 * 1.如果要使用库函数,则需要将SDK中的 source 和 source/thired_party/CMSIS/Include 加入
 * 	keil的工作路径中,包含头文件时使用尖括号<>来包含driverlib.h,而且是相对工作路径的路径
 * 	<ti/devices/msp432p4xx/driverlib/driverlib.h>	(注意:源文件没有设置成只读,千万别乱改)
 * 2.尽量将msp432p4xx_driverlin.lib包含进工程,路径ti\source\ti\devices\msp432p4xx\driverlib\keil\msp432p4xx_driverlib.lib
 * 	虽然不加入也编译没有报错,但是样板工程中包含了!
 * 3.MSP432的driverlib.h文件相当于STM32F的stm32f10x_config.h文件,这些头文件中包含了各个
 * 	外设的库函数头文件,在STM32F中通常只包含stm32f10x.h,其实stm32f10x.h中包含了stm32f10x_config.h,
 * 	stm32f10x.h最下面有USE_STDPERIPH_DRIVER的宏定义判断!
 * 4.如果工程使用了RTE加入CMSIS和STARTCODE 文件,则需要在keil C/C++ 中设置好文件包含路径
 * 	才能使VSCode打开不报错(不影响编译,只是用VSCode看着碍眼,具体包含路径跟着报错走)
 * 5.msp.h头文件里头根据全局宏定义定义了对应芯片的寄存器地址和中断号
 * 6.msp432的中断是通过库函数和自定义的库函数关联的,STM32则直接在启动代码中指定了中断函数
 * 				2021/8/26	PBZ
******************************************************************************************************/
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdbool.h>
#include "pid.h"
#include "isr.h"	//自定义的中断服务函数

int main(void)
{
	MAP_WDT_A_holdTimer();	//关闭看门狗
	//设置TIMER32_0
	Timer32_initModule(TIMER32_0_BASE,TIMER32_PRESCALER_16,TIMER32_16BIT,TIMER32_PERIODIC_MODE);
	//开启总中断
	if(Interrupt_enableMaster() ==  true)
	GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
	return 0;
}
