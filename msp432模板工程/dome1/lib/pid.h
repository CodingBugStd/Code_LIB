#ifndef _PID_H_
#define _PID_H_

/*************************************************
 * 增量式PID
 * Creat by: 庞碧璋
 * Github: https://github.com/CodingBugStd
 * csdn:   https://blog.csdn.net/RampagePBZ
 * Encoding: utf-8
 * date:    2021/8/27
*************************************************/

#ifndef USE_STDLIB
    #define USE_STDLIB  0   //不使用动态内存分配
#endif  //USE_STDLIB
#ifndef USE_DOUBLE
    #define USE_DOUBLE  1   //使用双精度浮点型
#endif  //USE_DOUBLE

#if USE_DOUBLE == 1
    typedef double float_p;
#else
    typedef float  float_p;
#endif

typedef struct
{
    float_p P;
    float_p I;
    float_p D;
    float_p Output;  //输出
    float_p Target;  //目标值
    float_p Actual;  //实际值
    float_p Err[3];  //三次误差
}PID_Handle;

float_p PID_IncOperation(PID_Handle*handle,float_p Actual);

#if USE_STDLIB == 1
PID_Handle* New_PID_Handle(double P,double I,double D);
void Delete_PID_Handle(PID_Handle*handle);
#endif

#endif
