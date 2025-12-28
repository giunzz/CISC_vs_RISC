Bắt đầu

Khai báo:
    SEG_CODE[8]  // Mảng mã LED 7 đoạn cho các số 1 → 8
    i, bit       // Biến đếm
    DATA_PIN     // Chân dữ liệu (ví dụ P1.0)
    CLK_PIN      // Chân xung clock (ví dụ P1.1)
    LATCH_PIN    // Chân chốt dữ liệu (ví dụ P1.2)

Khởi tạo:
    Cấu hình DATA_PIN, CLK_PIN, LATCH_PIN là OUTPUT
    Gán mã hiển thị cho SEG_CODE[1] → SEG_CODE[8]

Vòng lặp vô hạn:
    Cho i chạy từ 1 đến 8:
        Lấy mã hiển thị = SEG_CODE[i]

        Cho bit chạy từ 0 đến 7:
            Xuất bit hiện tại của mã hiển thị ra DATA_PIN
            Tạo xung CLK_PIN (0 → 1 → 0) để dịch bit

        Tạo xung LATCH_PIN để chốt dữ liệu ra LED 7 đoạn
        Tạo độ trễ để quan sát LED

    Kết thúc vòng for

Quay lại vòng lặp vô hạn

Kết thúc
