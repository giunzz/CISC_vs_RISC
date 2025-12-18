#include <REGX51.H>
// Led7seg definition
#define SEGMENT_PORT P0   // Connect Led7seg
#define SELECT_PORT  P2   // Choosing Led7seg

// Led7seg(Cathode chung)
unsigned char digit_patterns[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

// 8 values of led7seg
unsigned char led7seg[8] = {0,1,2,3,4,5,6,7};
unsigned char index   = 0;

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 123; j++);  // delay 1ms
}

void ISR_Timer0() interrupt 1
{
		unsigned char display_code;

    TH0 = 0xF8;  
    TL0 = 0x30;
// ------------------- Update LED
//		SELECT_PORT = (index << 2);
//		SEGMENT_PORT = digit_patterns[led7seg[index]];
//		
//		index++;
//		index &= 0x07;
	
// -------------------- Update theo so luong 
//		SEGMENT_PORT = 0x00; 
//    if (index < 3) {
//        SELECT_PORT = (index << 2); 
//        SEGMENT_PORT = digit_patterns[led7seg[index]];
//        index++;
//    } else {
//        index = 0; 
//    }
		
// -------------------- Update co dot	

		SEGMENT_PORT = 0x00; 
    if (index < 3) {
        SELECT_PORT = (index << 2);
        
        display_code = digit_patterns[led7seg[index]];
        
  			if (index == 1) {
            display_code |= 0x80; 
        }
        
        SEGMENT_PORT = display_code;
        index++;
    } else {
        index = 0;
    }
}

void setup_led7seg_timer0_2ms(void)
{
	
	  SEGMENT_PORT = 0x00; 
		index = 0;
		
    TMOD = 0x01; 
    TH0 = 0xF8;   
    TL0 = 0x30;
    IE  = 0x82;   
    TR0 = 1;     
}

unsigned int counter = 0;
void main(void)
{
    setup_led7seg_timer0_2ms();

    while (1)
    {
        led7seg[0] = counter % 10;          
        led7seg[1] = (counter / 10) % 10;   
        led7seg[2] = (counter / 100) % 10;  
        led7seg[3] = 0;
        led7seg[4] = 0;
        led7seg[5] = 0;
        led7seg[6] = 0;
        led7seg[7] = 0;
			
        
        counter++;
        if (counter > 999) counter = 0; 

        delay_ms(300); 
    }
}
