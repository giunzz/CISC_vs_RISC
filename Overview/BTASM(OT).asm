;Đề bài: Hệ thống gồm 2 led đơn
;- Bật/tắt bằng 4 nút nhấn
;- Led sáng 3s rồi tự tắt (Led7seg hiện số 3 rồi đếm lùi về 0 thì led đơn tắt)
;- Led7seg hiện 2 số (mỗi số là thời gian sáng của led đơn tương ứng) (2 Led đơn có thể sáng cùng lúc.)
SEGMENT_PORT EQU P0     
VALUE1    DATA 30H 
VALUE3    DATA 31H 
COUNT_END EQU 7

RUN_FLAG     EQU 31H   
    
LED1     BIT P2.0 
LED2     BIT P2.1 
LED3    BIT P2.2
    
BTN1_ON     BIT P3.0        
BTN1_OFF    BIT P3.1 
BTN2_ON    BIT P3.3
BTN2_OFF     BIT P3.4
BTN3_ON    BIT P3.5
BTN3_OFF     BIT P3.6

ORG 0000H
    SJMP MAIN

MAIN:
    SETB LED2
    MOV VALUE1, #COUNT_END       ; start
    MOV VALUE3, #COUNT_END
    MOV RUN_FLAG, #00H

MAIN_LOOP:
    JNB BTN1_ON, TURN_ON 
    JNB BTN1_OFF, TURN_OFF 
    
    JNB BTN2_ON, START_COUNTDOWN_2
    JNB BTN2_OFF, STOP_COUNTDOWN_2
    JNB BTN3_ON, START_COUNTDOWN_3
    JNB BTN3_OFF, STOP_COUNTDOWN_3
    
    MOV A, RUN_FLAG
    JZ MAIN_LOOP
    SJMP DO_COUNT
    
START_COUNTDOWN:
    CLR LED1        
    MOV VALUE1, #COUNT_END
    MOV RUN_FLAG, #01H
    LCALL DEBOUNCE
    SJMP MAIN_LOOP

STOP_COUNTDOWN:
    SETB LED1       
    MOV RUN_FLAG, #00H
    LCALL DEBOUNCE
    SJMP MAIN_LOOP
    
TURN_ON:
    CLR LED2          
    SJMP MAIN_LOOP

TURN_OFF:
    SETB LED2         
    SJMP MAIN_LOOP
     
DEBOUNCE:              ; 20ms debounce
    MOV R2, #40
DB1:
    MOV R1, #250
DB2:
    DJNZ R1, DB2
    DJNZ R2, DB1
    RET

DO_COUNT:
    JNB BTN2_OFF, STOP_COUNTDOWN

    MOV DPTR, #TABLE
    MOV A, VALUE1
    MOVC A, @A+DPTR      
    MOV SEGMENT_PORT, A  
    
    JNB BTN1_ON, TURN_ON
    JNB BTN1_OFF, TURN_OFF
     
     MOV R4, #COUNT_END
WAIT_LOOP:
        LCALL DELAY_200MS
        JNB BTN2_OFF, STOP_COUNTDOWN
        JNB BTN2_ON, START_COUNTDOWN
        DJNZ R4, WAIT_LOOP
    
    DJNZ VALUE1, MAIN_LOOP
    MOV SEGMENT_PORT, #3Fh ; end = 0
    SETB LED1
    LCALL DELAY1S
    
    MOV VALUE1, #COUNT_END
    CLR LED1 
    SJMP MAIN_LOOP
    
;-------------------------------delay
DELAY_200MS:
    MOV R2, #200
D1:
    MOV R1, #200
D2:
    DJNZ R1, D2
    DJNZ R2, D1
    RET

DELAY1S:
    MOV R3, #5
L1:
    LCALL DELAY_200MS
    DJNZ R3, L1
    RET
;-------------------------------------------------------- delay timer
DELAY_10MS:
    MOV TMOD, #01H     ; Timer0 Mode1 (16-bit)
    MOV TH0, #0D8H    
    MOV TL0, #0F0H

    SETB TR0           +

WAIT_T0:
    JNB TF0, WAIT_T0   
    CLR TR0            
    CLR TF0            
    RET
    
DELAY_1S:
    MOV R7, #100
DELAY_LOOP: 
    LCALL DELAY_10MS
    DJNZ R7, DELAY_LOOP
    RET

TABLE:
    DB 03FH
    DB 006H
    DB 05BH
    DB 04FH
    DB 066H
    DB 06DH
    DB 07DH
    DB 007H
    DB 07FH
    DB 06FH

END

