#include "pid.h"

float_p PID_IncOperation(PID_Handle*handle,float_p Actual)
{
    float_p Inc;
    handle->Err[2] = handle->Err[1];
    handle->Err[1] = handle->Err[0];
    handle->Err[0] = handle->Target - Actual;
    Inc = handle->P * (handle->Err[0] - handle->Err[1])
         + handle->I * (handle->Err[0])
         + handle->D * (handle->Err[0] - handle->Err[1]*2 + handle->Err[2]) ;
    handle->Output += Inc;
    return handle->Output;
}

#if USE_STDLIB == 1
#include <stdlib.h>
PID_Handle* New_PID_Handle(double P,double I,double D)
{
    PID_Handle* handle;
    handle = (PID_Handle*)malloc(sizeof(PID_Handle));
    if(handle == NULL)
        return handle;
    handle->P = P;
    handle->I = I;
    handle->D = D;
    handle->Actual = 0;
    handle->Output = 0;
    handle->Target = 0;
    for(unsigned char temp=0;temp<3;temp++)
        handle->Err[temp] = 0;
    return handle;
}

void Delete_PID_Handle(PID_Handle*handle)
{
    if(handle != NULL)
        free(handle);
}
#endif
