#include "soft_delay.h"

void soft_delay_ms(unsigned int ms)
{
    while(ms!=0)
    {
        soft_delay_us(1000);
        ms--;
    }
}

void soft_delay_us(unsigned int us)
{
    unsigned short int temp;
    while(us!=0)
    {
        for(temp=72;temp!=0;temp--);
        us--;
    }
}
