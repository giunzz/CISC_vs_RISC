ORG 0000H        

MAIN:            
    MOV DPTR, #07FFAH    
    MOV R3, #08H         
    MOV A, #01H          
REP1:
    MOVX @DPTR, A         
    RL A                  
    INC DPTR              
    DJNZ R3, REP1         

    MOV DPTR, #07FFAH     
    MOV R3, #08H

REP2:
    MOVX A, @DPTR         
    MOV P1, A             
    CALL DELAY           
    INC DPTR
    DJNZ R3, REP2

    SJMP MAIN             
DELAY:
    MOV R1, #200          
LAP1:
    MOV R0, #200          
LAP:
    NOP                   
    DJNZ R0, LAP
    DJNZ R1, LAP1
    RET

END
