#include <REGX51.H>

#define SEGMENT_PORT P0  
#define SELECT_PORT  P2  
sbit LED1 = P1^0;

volatile unsigned int period1 = 100;   
volatile unsigned int period2 = 1000;   
unsigned int c1 = 0;
unsigned int val;

void small_delay()
{
    unsigned int i;
    for(i = 0; i < 200; i++);
}
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 123; j++);
}
void timer0_isr(void) interrupt 1
{
    TH0 = 0xFC;
    TL0 = 0x66;   // 1ms tick
    c1++;
}

void timer0_init()
{
    TMOD |= 0x01;  // Mode 1
    TH0 = 0xFC; TL0 = 0x66;
    ET0 = 1;
    TR0 = 1;
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
void display_digit(unsigned char index, unsigned char value, unsigned char dot)
{
		if (dot == 0){
			SELECT_PORT = (index << 2);
			SEGMENT_PORT = digit_patterns[value];
			delay_ms(1);
		}
		else 
		{
			SELECT_PORT = (index << 2);
			SEGMENT_PORT = digit_patterns[value] | 0x80;
			delay_ms(1);
		}
			
}

void display4_digit(unsigned char val)
{
		val = period2;
    SELECT_PORT = (0 << 2);
    SEGMENT_PORT = digit_patterns[val%10];
    delay_ms(2);
		val = val/10;
    SELECT_PORT = (1 << 2);
    SEGMENT_PORT = digit_patterns[val%10];
    delay_ms(2);
		val = val/10;
    SELECT_PORT = (2 << 2);
    SEGMENT_PORT = digit_patterns[val%10];
    delay_ms(2);
		val = val/10;
    SELECT_PORT = (1 << 2);
    SEGMENT_PORT = digit_patterns[val];
    delay_ms(2);
}
void external0_isr(void) interrupt 0
{
		val = period2;
		display4_digit(val);
		LED1 = 0;
		delay_ms(period2);
}

void external1_isr(void) interrupt 2
{
    period2 += 500;
    if(period2 > 2000) period2 = 500;
}

void main()
{
    LED1 = 1;
    IT0 = 1;  
    IT1 = 1;   
    EX0 = 1;   
    EX1 = 1;   
    EA  = 1;   
		timer0_init(); 
    while(1)
    {
				small_delay();
				c1++;
        // LED1
        if(c1 >= period1)
        {
            LED1 = !LED1;
            c1 = 0;
        }
        val = period2;
				display_digit(0,val%10,0);
				val = val/10;
				display_digit(1,val%10,0);
				val = val / 10;
				display_digit(2,val %10,0);
				val = val / 10;
				display_digit(3,val,0);
    }
}
