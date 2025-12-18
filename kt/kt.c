#include <REGX51.H>   
#define SEGMENT_PORT P0  
#define SELECT_PORT  P2   

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 123; j++);
}

unsigned char digit_patterns[] = {
    0x3F,   // 0
    0x06,   // 1
    0x5B,   // 2
    0x4F,   // 3
    0x66,   // 4
    0x6D,   // 5
    0x7D,   // 6
    0x07,   // 7
    0x7F,   // 8
    0x6F    // 9
};
unsigned char value1 =  0x00;
unsigned char value2 = 0x00;  
unsigned char value3 = 0x00;  

void main(void)
{   
    SEGMENT_PORT = 0x00;                
    while (1)
    {
                SELECT_PORT = (0 << 2); 
                SEGMENT_PORT = digit_patterns[value1];
                delay_ms(2);
                SELECT_PORT = (1 << 2); 
                SEGMENT_PORT = digit_patterns[value2];
                delay_ms(2);
                SELECT_PORT = (2 << 2); 
                SEGMENT_PORT = digit_patterns[value3];
                delay_ms(2);
                
                value1++;
        if(value1 == 0x09)   
        {
            value1 = 0x00;
            value2++;      
            if(value2 == 0x09)
                        {
                            value2 = 0x00;
                            value3++;
                            if(value3 == 0x00)
                                value3 = 0x09;
                            }
        }
        delay_ms(20);  
            
                
        
    }
}
