ORG 0000H 
MOV 30H, #05H     
MOV R1, 30H       
MOV R2, #00H      
CLR A             
LOOP:
MOV A, R1         
ADD A, R2         
MOV R2, A         
DJNZ R1, LOOP     
MOV 40H, R2       
MOV P1, R2        
JMP $             
END
