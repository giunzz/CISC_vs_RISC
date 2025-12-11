ORG 0000H
MOV DPTR, #TABLE     
MOV A, #00H          
MOVC A, @A+DPTR      
MOV R0, A            

MOV A, #01H          
MOVC A, @A+DPTR      
MOV R1, A            

MOV A, R0
CLR C                
SUBB A, R1           

MOV P1, A

JMP $            
	
ORG 0100H 
TABLE: 
		DB 0AH, 03H
END 
	
