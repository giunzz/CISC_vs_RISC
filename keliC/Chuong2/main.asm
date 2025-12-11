; Gan 2 gia tri khong dau vao 2 thanh ghi R0, R1 . Tinh tong hieu tich va xuat ra port 0,1,2,3
ORG 0000H 
MOV R0, #0AH 
MOV R1, #05H


MOV A, R0 
ADD A, R1 
MOV P0, A 


MOV A, R0 
CLR C 
SUBB A, R1 
MOV P1, A 


MOV A, R0 
MOV B, R1
MUL AB 
MOV P2, A 
MOV P3, B 

JMP $ 
END 