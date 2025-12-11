ORG 0000H

MAIN:
    CLR P1.0           ; LED OFF ban d?u

LOOP:
    CPL P1.0           ; dao
    LCALL DELAY500MS   
    SJMP LOOP

DELAY500MS:
    MOV R7,#10         ; 10 × 50ms = 500ms

DELAY_LOOP:
    MOV TMOD,#10H      ; Timer1 Mode1 (16-bit)
    MOV TH1,#03CH      
    MOV TL1,#0B0H

    SETB TR1           ; bat Timer1

WAIT_T1:
    JNB TF1,WAIT_T1    
    CLR TR1            ; tat Timer1
    CLR TF1            ; xóa co tràn

    DJNZ R7,DELAY_LOOP 

    RET

END
