#include <REGX51.H>

sbit LED = P2^5;

void delay_10ms(void)
{
    TMOD = 0x01;
    TH0  = 0xDB;
    TL0  = 0xE0;

    TR0 = 1;
    while (TF0 == 0);

    TR0 = 0;
    TF0 = 0;
}

void delay_1s(void)
{
    unsigned char i;
    for (i = 0; i < 100; i++)
    {
        delay_10ms();
    }
}

void main(void)
{
    LED = 1;
    while (1)
    {
        LED = ~LED;
        delay_1s();
    }
}
