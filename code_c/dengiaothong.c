// thiết kế mạch đèn giao thông ngõ tư 
// - led 7 đoạn đơn
// - led đơn (đỏ, vàng, xanh)
// - Đèn đỏ 5s, vàng 2s, xanh 3s

#include <REGX51.H>


#define SEGMENT_PORT P0  
#define SELECT_PORT  P2 

sbit LED1 = P1^0;
sbit LED2 = P1^1;
sbit LED3 = P1^2;


#define T1 5000
#define T2 2000
#define T3 3000
volatile unsigned int period = 1000;   
volatile unsigned int ms_tick = 0;

typedef enum {
    STATE_LED1,
    STATE_LED2,
    STATE_LED3
} led_state_t;

led_state_t state = STATE_LED1;
unsigned int state_timer = 0;

void timer0_isr(void) interrupt 1
{
    TH0 = 0xFC;
    TL0 = 0x66;   
    ms_tick++;
}


void timer0_init(void)
{
    TMOD |= 0x01;  
    TH0 = 0xFC;
    TL0 = 0x66;
    ET0 = 1;
    TR0 = 1;
}

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 123; j++);
}
void external0_isr(void) interrupt 0
{
	LED1 = LED2 = LED3 = 0;
	delay_ms(period);
}

void external1_isr(void) interrupt 2
{
    period += 1000;
    if(period > 5000) period = 1000;
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
unsigned int remain_s;

void main(void)
{
    LED1 = LED2 = LED3 = 1;   
		IT0 = 1;  
    IT1 = 1;   
    EX0 = 1;   
    EX1 = 1;   
    EA  = 1;  
    EA = 1;
    timer0_init();

    while(1)
    {
        if(ms_tick >= 1)     
        {
            ms_tick = 0;
            state_timer++;
						
            switch(state)
            {
                case STATE_LED1:
                    LED1 = 0;   
                    LED2 = 1;
                    LED3 = 1;

                    if(state_timer >= T1)
                    {
                        state_timer = 0;
                        state = STATE_LED2;
                    }
										remain_s = (T1 - state_timer) / 1000;
										display_digit(7,remain_s,0);
                    break;

                case STATE_LED2:
                    LED2 = 0;
                    LED1 = 1;
                    LED3 = 1;

                    if(state_timer >= T2)
                    {
                        state_timer = 0;
                        state = STATE_LED3;
                    }
										remain_s = (T2 - state_timer) / 1000;
										display_digit(7,remain_s,0);
                    
                    break;

                case STATE_LED3:
                    LED3 = 0;
                    LED1 = 1;
                    LED2 = 1;

                    if(state_timer >= T3)
                    {
                        state_timer = 0;
                        state = STATE_LED1;
                    }
										remain_s = (T3 - state_timer) / 1000;
										display_digit(7,remain_s,0);
                    
                    break;
            }
						display_digit(5,period/1000,0);
						
        }
    }
}
