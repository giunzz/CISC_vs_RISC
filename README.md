# THỰC TẬP KIẾN TRÚC VÀ TỔ CHỨC MÁY TÍNH – 8051

## 1. Giới thiệu

- Link tải KeilC (AT89C51) và Proteus:  [KeilC & Proteus](https://drive.google.com/drive/folders/1FDUzu3VYYfmMQK9li1eveh-XFw6xOYeM?usp=sharing)

- Lập trình vi điều khiển 8051 bằng ASM và C.

- Kit thực hành Proteus:  [KitTT_ver8.17](https://github.com/giunzz/COOA_LABS/blob/main/proteus/kitTT_ver8.17.pdsprj)

- Bảng chuyển đổi cơ số: [Link](https://github.com/giunzz/COOA_LABS/blob/main/Bang_Chuyen_Doi_Co_So.xlsx)

  Lưu ý sử dụng chân:
  - LED đơn: **P1**
  - LED 7 đoạn: **P2, P0**
  - UART: **P3.0, P3.1**
  - Ngắt ngoài: **INT0 (P3.2), INT1 (P3.3)**

- Hoàng Ngọc Dung

---

## 2. Báo cáo từng tuần

| Tuần | Nội dung chính | File báo cáo |
|------|----------------|--------------|
| **Tuần 1 – ASM** | Tổng quan 8051, tập lệnh ASM | [Report01](https://github.com/giunzz/COOA_LABS/blob/main/report/HOANGNGOCDUNG_TUAN1.docx) |
| **Tuần 2 – ASM** | Nhập xuất GPIO, Timer – Counter | [Report02](https://github.com/giunzz/COOA_LABS/blob/main/report/HOANGNGOCDUNG_TUAN2.docx) |
| **Tuần 3 – ASM** | Ngắt (Interrupt) | [Report03](https://github.com/giunzz/COOA_LABS/blob/main/report/HOANGNGOCDUNG_TUAN3.docx) |
| **Tuần 4 – C** | Lập trình GPIO với C | [Report04](https://github.com/giunzz/COOA_LABS/blob/main/report/HOANGNGOCDUNG_TUAN4.docx) |
| **Tuần 5 – C** | LED 7 đoạn, Timer/Counter | [Report05](https://github.com/giunzz/COOA_LABS/blob/main/report/HOANGNGOCDUNG_TUAN5.docx) |
| **Tuần 6 – C** | UART – Timer/Counter nâng cao | [Report06](https://github.com/giunzz/COOA_LABS/blob/main/report/HOANGNGOCDUNG_TUAN6.docx) |


## 3. Kiểm tra ASM

**Yêu cầu chương trình:**

1. LED chạy tắt dần từ phải → trái, có trạng thái sáng hết (T mặc định = 1000ms).  
2. Nút A: T = T + 500ms (tối đa 3s).  
3. Nút B: T = T – 500ms (tối thiểu 500ms).  
4. Nút C: LED sáng hết và đứng im. Nút D: chạy lại chế độ 1.  
5. LED 7 đoạn hiển thị số LED sáng và tốc độ hiện tại.

Code ASM kiểm tra:  [Midterm ASM](https://github.com/giunzz/COOA_LABS/tree/main/midterm_asm)

---

## 4. Kiểm tra C – Lần 1

**Đề bài:**

- 3 chế độ LED:  
  1) sáng dần trái→phải ×8  
  2) tắt dần phải→trái ×8  
  3) nháy ×8  
- 1 nút chuyển mode:  
  - mode 0: lặp cả 3  
  - mode 1/2/3: chạy từng chế độ riêng  
- LED 7 đoạn hiển thị mode và số LED sáng  

Code:  [KT C – Lần 1](https://github.com/giunzz/COOA_LABS/blob/main/code_c/kt2.c)

---

## 5. Kiểm tra C – Lần 2

**Đề bài:**

- LED chớp 100ms  
- Button 1: bật LED trong T giây (T = 1 → trở lại ban đầu)  
- Button 2: T = T + 0.5 (giới hạn 0.5 → 2.0s)  
- LED 7 đoạn hiển thị T (ms)

Code:  [KT C – Lần 2](https://github.com/giunzz/COOA_LABS/blob/main/code_c/KT3.c)

---

## 6. Tổng hợp Code

Tổng hợp toàn bộ code ASM & C:  [Overview](https://github.com/giunzz/COOA_LABS/tree/main/Overview)
