#include <REGX51.H>
#include <stdio.h>

volatile bit uart_flag = 0;      
volatile unsigned char recvData; 
unsigned char current_mode = 0;  

unsigned int timer_count = 0;    
unsigned char step = 0;          

// --- Khai bao bien cho hieu ung Sang Don ---
unsigned char sd_mask = 0xFF; 
signed char sd_run = 0;       // Dung signed char de so sanh < 0 tot hon
signed char sd_limit = 7;            

void UART_Init() {
    SCON = 0x50;    
    TMOD &= 0x0F;    
    TMOD |= 0x20;    
    TH1 = 0xFD;      
    TR1 = 1;      
    TI = 1;             
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

void UART_EnableInterrupt(void) {
    ES = 1;  
    EA = 1;  
}

void UART_ISR(void) interrupt 4 {
    if (RI) {
        recvData = SBUF; 
        RI = 0;           
        uart_flag = 1;    
    }
}

void Display_Menu(void) {
    UART_SendString("\r\n=========== MENU HIEU UNG LED ===========\r\n");
    UART_SendString("1. Sang dan (Left -> Right)\r\n");
    UART_SendString("2. Tat dan  (Right -> Left)\r\n");
    UART_SendString("3. Dich Bit (Running LED)\r\n");
    UART_SendString("4. Chop Tat (Blinking)\r\n");
    UART_SendString("5. Sang Don (Stacking)\r\n"); // Them muc 5
    UART_SendString("0. Tat het (STOP)\r\n");
    UART_SendString("=========================================\r\n");
    UART_SendString("Moi chon che do: ");
}

void Effect_SangDan() {
    unsigned char code pattern[] = {0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00};    
    P2 = pattern[step]; 
    step++;
    if (step > 8) step = 0; 
}

void Effect_TatDan() {
    unsigned char code pattern[] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF};
    P2 = pattern[step];
    step++;
    if (step > 8) step = 0;
}

void Effect_DichBit() {
    if (step == 0) P2 = 0xFE; 
    else {
        P2 = (P2 << 1) | 0x01; 
    }
    
    step++;
    if (step > 7) step = 0; 
}

void Effect_ChopTat() {
    if (step % 2 == 0) 
        P2 = 0x00; 
    else 
        P2 = 0xFF; 
        
    step++;
    if (step > 10) step = 0;
}

void Effect_SangDon() {
    if (step == 0) {
        sd_mask = 0xFF; 
        sd_run = 0;
        sd_limit = 7;
        step = 1;       
    }
    P2 = sd_mask & ~(1 << sd_run);

    sd_run++;

    if (sd_run > sd_limit) {
        sd_mask &= ~(1 << sd_limit);
        
        sd_limit--; 
        
        sd_run = 0; 

        if (sd_limit < 0) {
            sd_limit = 7;
            sd_mask = 0xFF;
        }
    }
}

void main(void) {
    UART_Init();
    UART_EnableInterrupt();
    
    P2 = 0xFF; 
    Display_Menu();
    
    while(1) {
        // --- XU LY LENH UART ---
        if (uart_flag == 1) {
            uart_flag = 0; 
            
            switch (recvData) {
                case '1': 
                    current_mode = 1; step = 0; 
                    UART_SendString("\r\n-> Che do: Sang Dan\r\n"); 
                    break;
                case '2': 
                    current_mode = 2; step = 0; 
                    UART_SendString("\r\n-> Che do: Tat Dan\r\n"); 
                    break;
                case '3': 
                    current_mode = 3; step = 0; 
                    UART_SendString("\r\n-> Che do: Dich Bit\r\n"); 
                    break;
                case '4': 
                    current_mode = 4; step = 0; 
                    UART_SendString("\r\n-> Che do: Chop Tat\r\n"); 
                    break;
                case '5': // Them case 5
                    current_mode = 5; step = 0; 
                    UART_SendString("\r\n-> Che do: Sang Don\r\n"); 
                    break;
                case '0': 
                    current_mode = 0; 
                    P2 = 0xFF; 
                    UART_SendString("\r\n-> DUNG HIEU UNG.\r\n"); 
                    Display_Menu(); 
                    break;
                default:
                    UART_SendString("\r\nLenh khong hop le!\r\n");
                    break;
            }
        }
        
        // --- XU LY HIEU UNG ---
        if (current_mode != 0) {
            timer_count++; 
            
            if (timer_count >= 3000) { 
                timer_count = 0; 
                
                switch (current_mode) {
                    case 1: Effect_SangDan(); break; // Da sua dung ten ham
                    case 2: Effect_TatDan(); break;
                    case 3: Effect_DichBit(); break;
                    case 4: Effect_ChopTat(); break;
                    case 5: Effect_SangDon(); break; // Them goi ham Sang Don
                }
            }
        }
    }   
}