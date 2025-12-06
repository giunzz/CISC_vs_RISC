# Reduced Instruction Set Computer (RISC)

### Mô phỏng 

Mô phỏng chương trình theo 8051 tập lệnh CISC và PIC16F8(RISC)
- Cho 4 led tắt dần từ phải sang trái, rồi lặp lại. Có trạng thái “Sáng hết”. Tốc độ T ms (mặc định T = 1000 ms).
- Nhấn nút A, T = T + 500 ms (tối đa 3 s).
- Nhấ Nút B , T = T - 500ms
- Nhấn nút C, Led sáng hết rồi đứng im. Nhấn nút D, Led chạy lại như yêu cầu 1.

### So sánh 
Chu kỳ lệnh, số lệnh , pieline, MIPS ,CPI,...
CPU time 


### Chú thích code 
P3.2: Ngắt (đứng yên 4 led sáng)
P3.0 Tang T
P3.3 Giam t
P3.1 Quy lai ban dau 

Delay 10ms -> inner 250x2 -> out 500x20 
T_delay : mac dinh 100 -> tong 1s (dùng tăng giảm)