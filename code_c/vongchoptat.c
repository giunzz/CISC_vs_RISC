#include <REGX51.H>

/* ===== LED ===== */
sbit LED1 = P1^0;
sbit LED2 = P1^1;
sbit LED3 = P1^2;

/* ===== CHU KY (ms) ===== */
#define LED1_CYCLE 500
#define LED2_CYCLE 200
#define LED3_CYCLE 300

/* ===== TIMER ===== */
volatile unsigned int ms_tick = 0;

/* ===== FSM ===== */
typedef enum {
    STATE_LED1_ON,
    STATE_LED1_OFF,
    STATE_LED2_ON,
    STATE_LED2_OFF,
    STATE_LED3_ON,
    STATE_LED3_OFF
} led_state_t;

led_state_t state = STATE_LED1_ON;
unsigned int timer = 0;

/* ===== TIMER0 ISR: 1 ms ===== */
void timer0_isr(void) interrupt 1
{
    TH0 = 0xFC;
    TL0 = 0x66;
    ms_tick++;
}

/* ===== TIMER INIT ===== */
void timer0_init(void)
{
    TMOD |= 0x01;
    TH0 = 0xFC;
    TL0 = 0x66;
    ET0 = 1;
    TR0 = 1;
}

/* ===== MAIN ===== */
void main(void)
{
    LED1 = LED2 = LED3 = 1;   // OFF (active low)

    EA = 1;
    timer0_init();

    while(1)
    {
        if(ms_tick >= 1)
        {
            ms_tick = 0;
            timer++;

            switch(state)
            {
                /* ===== LED1 ===== */
                case STATE_LED1_ON:
                    LED1 = 0; LED2 = 1; LED3 = 1;
                    if(timer >= LED1_CYCLE/2)
                        timer = 0, state = STATE_LED1_OFF;
                    break;

                case STATE_LED1_OFF:
                    LED1 = 1;
                    if(timer >= LED1_CYCLE/2)
                        timer = 0, state = STATE_LED2_ON;
                    break;

                /* ===== LED2 ===== */
                case STATE_LED2_ON:
                    LED2 = 0; LED1 = 1; LED3 = 1;
                    if(timer >= LED2_CYCLE/2)
                        timer = 0, state = STATE_LED2_OFF;
                    break;

                case STATE_LED2_OFF:
                    LED2 = 1;
                    if(timer >= LED2_CYCLE/2)
                        timer = 0, state = STATE_LED3_ON;
                    break;

                /* ===== LED3 ===== */
                case STATE_LED3_ON:
                    LED3 = 0; LED1 = 1; LED2 = 1;
                    if(timer >= LED3_CYCLE/2)
                        timer = 0, state = STATE_LED3_OFF;
                    break;

                case STATE_LED3_OFF:
                    LED3 = 1;
                    if(timer >= LED3_CYCLE/2)
                        timer = 0, state = STATE_LED1_ON;
                    break;
            }
        }
    }
}
