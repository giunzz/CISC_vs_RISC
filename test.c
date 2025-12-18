#include <REGX51.H>   
#define SEGMENT_PORT P0  
#define SELECT_PORT  P2   
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
void main(void)
{
    unsigned char led_index = 8;    
    SEGMENT_PORT = 0x00;             
        //SELECT_PORT = (led_index) << 2; 
    SELECT_PORT = (led_index & 0x07) << 2; 
    SEGMENT_PORT = digit_patterns[5];  
    while (1)
    {
    }
}

