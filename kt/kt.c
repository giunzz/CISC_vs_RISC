#include <REGX51.H>   

#define SEGMENT_PORT P0  
#define SELECT_PORT  P2   

unsigned char cnt = 0;
char cmd[3];
unsigned char uart_index = 0;

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

// 8 values of led7seg
unsigned char led7seg[8] = {0,1,2,3,4,5,6,7};
unsigned char index   = 0;

unsigned char pattern_led[] = {
    0xFF,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80, 0x00
};

void ISR_Timer0() interrupt 1
{
		unsigned char display_code;
    TH0 = 0xF8;
    TL0 = 0x30;

    SEGMENT_PORT = 0x00; 
    if (index < 2) {
        SELECT_PORT = (index << 2);
        display_code = digit_patterns[led7seg[index]];
        SEGMENT_PORT = display_code;
        index++;
    } else {
        index = 0;
    }
}
void update_display(unsigned char led_value, unsigned char cnt)
{
    led7seg[0] = led_value; 
    led7seg[1] = cnt;       
}

void setup_led7seg_timer0_2ms(void)
{
	
	  SEGMENT_PORT = 0x00; 
		index = 0;
		TMOD &= 0xF0; 
    TMOD |= 0x01;
    TH0 = 0xF8;   
    TL0 = 0x30;
    IE  = 0x82;   
    TR0 = 1;     
}

void effect_sangdan()
{
    unsigned char i;
		unsigned char old = cnt;
    for(i=0;i<9;i++){
				if(cnt != old) break; 
        P1 = pattern_led[i];
				update_display(i,cnt);
        delay_ms(200);
    }
}

void effect_tatdan()
{
    char i;
		unsigned char old = cnt;
    for(i=8; i >= 0; i--){
				if(cnt != old) break; 
        P1 = pattern_led[i];
        update_display(i,cnt);
        delay_ms(200);
    }
}

void effect_choptat()
{
    unsigned char i;
		unsigned char old = cnt;
    for(i=0;i<8;i++){
				if(cnt != old) break; 
        P1 = 0x00;
        update_display(0,cnt);
        delay_ms(200);
				if(cnt != old) break; 
        P1 = 0xFF;
        update_display(8,cnt);
        delay_ms(200);
    }
}

// P3^2
void external0_isr(void) interrupt 0
{
    cnt++;
    if(cnt > 3) cnt = 0;

}

void external1_isr(void) interrupt 2
{
    cnt = (cnt + 3) % 4;
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
    cnt =  cmd[0] - '0';
		UART_SendString("\r\nSet OK\r\n");
}

void UART_Receive_Handler(void)
{
    if(RI)
    {
        RI = 0;
        cmd[uart_index] = SBUF;   
        uart_index++;
        if(uart_index >= 1)
        {
            Process_Command();     
            uart_index = 0;        
        }
    }
}
void scan_led()
{
	effect_sangdan();
	effect_tatdan();
	effect_choptat();
}

void main()
{
    	
		 
		UART_Init();
    UART_SendString("\r\n=== DIEU KHIEN LED QUA UART ===\r\n");
    UART_SendString("Nhap lenh (VD: 0,1,2,3)\r\n");
		setup_led7seg_timer0_2ms();
		IT0 = 1;  
    IT1 = 1;   
    EX0 = 1;   
    EX1 = 1;   
    EA  = 1;    
		cnt = 0;
    while (1)
    {
			UART_Receive_Handler();
			if(cnt == 0) scan_led();
			if(cnt == 1) effect_sangdan();
			if(cnt == 2) effect_tatdan();
			if(cnt == 3) effect_choptat();
    }
}
