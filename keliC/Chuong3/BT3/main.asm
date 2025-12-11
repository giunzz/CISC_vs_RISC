SEGMENT_PORT EQU P0
SELECT_PORT EQU P2
LED7SEG DATA 30H
VALUE DATA 31H

ORG 0000H
JMP MAIN

ORG 0030H
MAIN:
        MOV LED7SEG, #0
        MOV VALUE, #0
        
LOOP:
        MOV A, LED7SEG
        RL A
        RL A
        MOV SELECT_PORT, A
        
        MOV DPTR, #DIGIT_PATTERNS
        MOV A, VALUE
        MOVC A, @A+DPTR
        MOV SEGMENT_PORT, A
        CALL DELAY_2MS
        
        INC LED7SEG
        ANL A, #07H
        MOV LED7SEG, A
        
        INC VALUE
        ANL A, #07H
        MOV VALUE, A
        
        JMP LOOP

DELAY_2MS:
        MOV R2, #4
D2_OUTER:
        MOV R1, #250
D2_INNER:
        DJNZ R1, D2_INNER
        DJNZ R2, D2_OUTER
        RET
        
DIGIT_PATTERNS:
        DB 03Fh, 006h, 05Bh, 04Fh, 066h, 06Dh, 07Dh, 007h, 07Fh, 06Fh
        
END