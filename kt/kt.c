#include <REGX51.H>


#define SEGMENT_PORT P0  
#define SELECT_PORT  P2 

sbit LED1 = P1^0;
sbit LED2 = P1^1;
sbit LED3 = P1^2;


#define T1 500
#define T2 200
#define T3 300
volatile unsigned int period = 100;   
volatile unsigned int ms_tick = 0;
char cmd[3];
unsigned char uart_index = 0;

typedef enum {
    STATE_LED1,
    STATE_LED2,
    STATE_LED3
} led_state_t;

typedef enum {
    MODE_NORMAL = 0,   
    MODE_SANGDAN = 1,  
    MODE_BLINK = 2     
} system_mode_t;

volatile system_mode_t mode = MODE_NORMAL;


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
    period += 100;
    if(period > 900) period = 100;
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
unsigned char pattern_led[] = {
    0xFF,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80, 0x00
};
void effect_sangdan()
{
    unsigned char i;
    for(i=0;i<9;i++){
        P1 = pattern_led[i];
				display_digit(5,period/1000,0);
        delay_ms(period);
    }
}
void effect_choptat()
{
    unsigned char i;
    for(i=0;i<8;i++){
        P1 = 0x00;
        display_digit(5,period/1000,0);
        delay_ms(period);

        P1 = 0xFF;
        delay_ms(period);
    }
}


void UART_Init(void) {
    SCON = 0x50;        // UART mode 1, enable receiver
    TMOD &= 0x0F;       // Clear Timer1 config
    TMOD |= 0x20;       // Timer1 mode 2
    TH1 = 0xFD;         // Baud 9600 (11.0592 MHz)
    TR1 = 1;            // Start Timer1
    TI = 1;             // Allow first transmit
}

void UART_SendChar(char c) {
    while (!TI);   
    TI = 0;        
    SBUF = c;      
}

void UART_SendString(char *str) {
    while (*str) {
        UART_SendChar(*str);  
        str++;                
    }
}
void Process_Command(void)
{
    if(cmd[0] == '0' && cmd[1] == '0')
    {
        mode = MODE_NORMAL;
        state = STATE_LED1;
        state_timer = 0;
        UART_SendString("\r\nMODE 00: NORMAL 3 LED\r\n");
    }
    else if(cmd[0] == '0' && cmd[1] == '1')
    {
        mode = MODE_SANGDAN;
        UART_SendString("\r\nMODE 01: SANG DAN\r\n");
    }
    else if(cmd[0] == '0' && cmd[1] == '2')
    {
        mode = MODE_BLINK;
        UART_SendString("\r\nMODE 02: BLINK 8 LAN\r\n");
    }
    else
    {
        UART_SendString("\r\nLENH SAI! DUNG: 00 / 01 / 02\r\n");
    }
}


void UART_Receive_Handler(void)
{
    if(RI)
    {
        RI = 0;
        cmd[uart_index] = SBUF;   
        uart_index++;
        if(uart_index >= 2)
        {
            Process_Command();     
            uart_index = 0;        
        }
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
		UART_Init();
		UART_SendString("\r\n=== DIEU KHIEN LED QUA UART ===\r\n");
    UART_SendString("Nhap lenh (VD: 00, 01,02)\r\n");

    while(1)
    {
		UART_Receive_Handler();
		switch(mode)
        {
            case MODE_NORMAL:
            
                if(ms_tick >= 1)     
                {
                    ms_tick = 0;
                    state_timer++;
                                    ms_tick++;
                                
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
                                                remain_s = (T1 - state_timer) / 100;
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
                                                remain_s = (T2 - state_timer) / 100;
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
                                                remain_s = (T3 - state_timer) / 100;
                                                display_digit(7,remain_s,0);
                            
                            break;
                    }
                                display_digit(5,period/100,0);
                                
                }
                break;
            case MODE_SANGDAN:
                effect_sangdan();
                break;
            case MODE_BLINK:
                effect_choptat();
                break;
        }
    }
}
