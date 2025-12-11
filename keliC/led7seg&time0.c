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
    // Auto clear Timer0 flag (TF0)
    TH0 = 0xF8;   // Reload for 2ms overflow
    TL0 = 0x30;
    // Update 1 LED
		SELECT_PORT = (index << 2);
		SEGMENT_PORT = digit_patterns[led7seg[index]];
		
		index++;
		index &= 0x07;
}

void setup_led7seg_timer0_2ms(void)
{
	
	  SEGMENT_PORT = 0x00; 
		index = 0;
		
	
    TMOD = 0x01;  // Mode 1: 16-bit timer
    TH0 = 0xF8;   // Initial load
    TL0 = 0x30;
    IE  = 0x82;   // Enable global + Timer0 interrupt
    TR0 = 1;      // Start Timer0
}


void main(void)
{
	setup_led7seg_timer0_2ms();

	while (1)
	{
		led7seg[0]++;
		led7seg[0] = (led7seg[0]<10)?led7seg[0]:0;
		delay_ms(300);
		
	}
}
