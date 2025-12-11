#include <REGX51.H>

void delay200ms() {
    unsigned char R2, R1;
    for(R2 = 200; R2 > 0; R2--) {
        for(R1 = 200; R1 > 0; R1--) {
        }
    }
}

void delay1s() {
    unsigned char R3;
    for(R3 = 5; R3 > 0; R3--) {
        delay200ms();
    }
}

void main()
{
    unsigned char base = 0xFF;   
    unsigned char mask;
    unsigned char i, j;

    while(1)
    {
       
        for(i = 0; i < 8; i++)
        {
            mask = base >> i;   
            for(j = 0; j < 8-i; j++)
            {
                P2 = mask & ~(1 << j);   
                delay200ms();
            }
        }
    }
}
