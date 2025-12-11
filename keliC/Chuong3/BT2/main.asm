BASE    DATA 30H
MASK    DATA 31H

ORG 0000H

START:
    MOV BASE, #0FFH
    MOV MASK, #0FFH

MAIN_LOOP:

    MOV R6, #0                 

FOR_I:
    CJNE R6,#8, OK_I
    MOV A, MASK
    MOV P2, A
    LCALL DELAY200MS
    SJMP START
OK_I:
    
	; base >>= 1
    MOV A, BASE
    RR A
    MOV BASE, A

    ; j = 0
    MOV R7, #0

FOR_J:
    ; j < 8 - i ?
    MOV A, R7
    ADD A, R6
    CJNE A, #8, OK_J
    SJMP END_J
OK_J:
    ; (1 << j)
    MOV A, #1
    MOV B, R7
SHIFT_J:
    JZ SHIFT_DONE
    RL A
    DJNZ B, SHIFT_J
SHIFT_DONE:

    CPL A             
    MOV B, A

    MOV A, MASK       
    ANL A, B
    MOV P2, A         

    LCALL DELAY200MS

    INC R7
    SJMP FOR_J

END_J:
    MOV MASK, BASE     ; mask = base
    INC R6
    SJMP FOR_I


DELAY200MS:
    MOV R2, #200
D2:
    MOV R1, #200
D1:
    NOP
    NOP
    NOP
    DJNZ R1, D1
    DJNZ R2, D2
    RET

END
