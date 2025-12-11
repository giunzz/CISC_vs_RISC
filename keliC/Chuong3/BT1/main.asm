NAME    MAIN
ORG     0000H


DELAY200MS:
    MOV     R2, #200
D2_LOOP:
    MOV     R1, #200
D1_LOOP:
    DJNZ    R1, D1_LOOP
    DJNZ    R2, D2_LOOP
    RET


MAIN:
    MOV     A, #0FFH     ; base = 0xFF

MAIN_LOOP:
    MOV     R6, #00H     ; i = 0

LOOP_I:
    CJNE    R6, #08, OK_I
    SJMP    MAIN_LOOP    

OK_I:
    ; mask = base >> i
    MOV     A, #0FFH     ; A = base
    MOV     B, R6        ; B = i

SHIFT_RIGHT_I:
    CJNE    B, #00H, DO_SHIFT
    SJMP    SHIFT_DONE
DO_SHIFT:
    RR      A            ; A = A >> 1
    DJNZ    B, DO_SHIFT
SHIFT_DONE:
    MOV     R5, A        

    MOV     R7, #00H     ; j = 0
    MOV     R4, #08H
    CLR     C
    SUBB    R4, R6       


LOOP_J:
    CJNE    R7, R4, OK_J
    SJMP    NEXT_I       

OK_J:
    ; Tính (1 << j)
    MOV     A, #01H
    MOV     B, R7

SHIFT_LEFT_J:
    CJNE    B, #00H, DO_LSHIFT
    SJMP    LSHIFT_DONE
DO_LSHIFT:
    RL      A
    DJNZ    B, DO_LSHIFT
LSHIFT_DONE:
    MOV     R3, A        ; R3 = (1 << j)

    ; ~(1 << j)
    CPL     R3

    ; P2 = mask & ~(1 << j)
    MOV     A, R5
    ANL     A, R3
    MOV     P2, A

    ; delay200ms()
    LCALL   DELAY200MS

    INC     R7
    SJMP    LOOP_J


NEXT_I:
    INC     R6
    SJMP    LOOP_I


END
