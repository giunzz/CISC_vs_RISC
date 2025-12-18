#include <REGX51.H>
#include <stdio.h>
#define SEGMENT_PORT P0  
#define SELECT_PORT  P2   

sbit LED1 = P1^0;
volatile unsigned int period1 = 1000;  
volatile unsigned int ms_tick = 0;
#define CMD_MAX_LEN 6

/* Button polling */
sbit BTN1 = P3^4;
/* Button interrupt */
sbit BTN2 = P3^2;   
sbit BTN3 = P3^3;   

unsigned int den = 0,quat = 0, maylanh = 0;
unsigned char cnt = 0;
char cmd[CMD_MAX_LEN];
unsigned char uart_index = 0;
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

void Display_Menu(void) {
    UART_SendString("\r\n===== TRANG THAI =====\r\n");
		if (den == 0) UART_SendString("- Trang thai den OFF \r\n");
		else UART_SendString("- Trang thai den ON \r\n");
		if (quat == 0) UART_SendString("- Trang thai quat OFF \r\n");
		else UART_SendString("- Trang thai quat ON \r\n");
		if(maylanh == 0) UART_SendString("- Trang thai maylanh OFF \r\n");
    else UART_SendString("- Trang thai maylanh ON \r\n");
    UART_SendString("================================\r\n");
		UART_SendString("\r\n=====DIEU KHIEN CHU KY LED=====\r\n");
    UART_SendString("Nhap chu ky (VD: 2000, 310, 50) + nhan enter ms \r\n");
}
void Process_Command(void)
{
    Display_Menu();
		UART_SendString("\r\nSet OK\r\n");
}

void int0_isr(void) interrupt 0
{
    den = ~den; 
		Process_Command();
}
void int1_isr(void) interrupt 2
{
    quat = ~quat; 
		Process_Command();
}
void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 123; j++);
}

void scan_buttons(void)
{
    if (BTN1 == 0) { 
			delay_ms(20); 
			if (BTN1 == 0) 
				{ 
				maylanh = ~maylanh; 
				Process_Command();
				while (BTN1 == 0); 
				} 
		}
			
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
unsigned int state_timer = 0;
unsigned int remain_s;
void small_delay()
{
    unsigned int i;
    for(i = 0; i < 200; i++);
}
unsigned int parse_uint(char *s)
{
    unsigned int val = 0;
    while(*s)
    {
        if(*s < '0' || *s > '9') break;
        val = val * 10 + (*s - '0');
        s++;
    }
    return val;
}

void Process_CK(void)
{
    unsigned int val = parse_uint(cmd);
		period1 = val / 2;

		UART_SendString("\r\nSet chu ky = ");
    sprintf(cmd, "%u", val);
    UART_SendString(cmd);
    UART_SendString(" ms\r\n");
}
void UART_Receive_Handler(void)
{
    if(RI)
    {
        char c;
        RI = 0;
        c = SBUF;

        if(c == '\r' || c == '\n')
        {
            cmd[uart_index] = '\0';   
            Process_CK();
            uart_index = 0;
        }
        else
        {
            if(uart_index < CMD_MAX_LEN - 1)
            {
                cmd[uart_index++] = c;
            }
        }
    }
}


void main(void)
{
    
		UART_Init();
		Display_Menu();
		den = quat = maylanh = 0;
		LED1 = 0;
    IT0 = 1;   
    IT1 = 1;  
    EX0 = 1;   
    EX1 = 1;   
    EA  = 1;  
		timer0_init();
    while (1)
    {
				scan_buttons(); 
				small_delay();
				state_timer++;
				UART_Receive_Handler();
				if(state_timer >= period1/10)
				{
						 LED1 = ~LED1;
						 state_timer = 0;
				}
						remain_s = period1;
						display_digit(0,remain_s%10,0);
						remain_s = remain_s/10;
						display_digit(1,remain_s%10,0);
						remain_s = remain_s / 10;
						display_digit(2,remain_s %10,0);
						remain_s = remain_s / 10;
						display_digit(3,remain_s,0);
    }
}
