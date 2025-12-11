#include <REGX51.H>

#define SEGMENT_PORT P0  
#define SELECT_PORT  P2  
sbit LED1 = P1^0;
sbit LED2 = P1^7;

volatile unsigned int period1 = 1000;   
volatile unsigned int period2 = 500;   
unsigned int c1 = 0, c2 = 0;
char cmd[3];
unsigned char uart_index = 0;

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
    c2++;
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
void external0_isr(void) interrupt 0
{
		
    period1 += 250;
    period2 += 250;

    if(period1 > 2500) period1 = 2500;
    if(period2 > 2500) period2 = 2500;
}

void external1_isr(void) interrupt 2
{
    period1 -= 250;
    period2 -= 250;

    if(period1 < 500) period1 = 500;
    if(period2 < 500) period2 = 500;
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
    unsigned char led = cmd[0] - '0';
    unsigned int val  = (cmd[1]-'0') * 10 + (cmd[2]-'0');
    unsigned int new_period = val * 50;

    if(new_period < 500) new_period = 500;
    if(new_period > 2500) new_period = 2500;

    if(led == 1) period1 = new_period;
    else if(led == 2) period2 = new_period;

    UART_SendString("\r\nSet OK\r\n");
}

void UART_Receive_Handler(void)
{
    if(RI)
    {
        RI = 0;
        cmd[uart_index] = SBUF;   
        uart_index++;
        if(uart_index >= 3)
        {
            Process_Command();     
            uart_index = 0;        
        }
    }
}

void main()
{
		
    LED1 = 1;
    LED2 = 1;

    IT0 = 1;  
    IT1 = 1;   
    EX0 = 1;   
    EX1 = 1;   
    EA  = 1;   
		timer0_init(); 
		UART_Init();
		UART_SendString("\r\n=== DIEU KHIEN LED QUA UART ===\r\n");
    UART_SendString("Nhap lenh (VD: 105, 210)\r\n");

    while(1)
    {
				small_delay();
				c1++;
				c2++;
				UART_Receive_Handler();
        // LED1
        if(c1 >= period1)
        {
            LED1 = !LED1;
            c1 = 0;
        }
        // LED2
        if(c2 >= period2)
        {
            LED2 = !LED2;
            c2 = 0;
        }
				display_digit(1,period1/500,1);
				display_digit(0,period1%500/50,0);
				display_digit(7,period2/500,1);
				display_digit(6,period2%500/50,0);
    }
}
