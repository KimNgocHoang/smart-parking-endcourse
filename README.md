# Bãi đậu xe thông minh
HỆ THỐNG BÃI ĐỖ XE THÔNG MINH: là hệ thống sử dụng cảm biến để mở và đóng barie tự động khi có xe đi ra hoặc vào trong bãi đỗ hoặc có thể điều khiển trên web. Hiển thị thông tin số xe có trong bãi, vị trí đỗ còn trống hay đã có xe đỗ trên màn hình lcd và trên web. 
Thành phần:
+ ESP32 
+ Cảm biến hồng ngoại
+ Động cơ Servo
+ LCD
Hướng dẫn cài đặt:
- Đầu tiên cài đặt thư viện của ESP32, thư viện của Servo, ESPAsyncWebServer. Cài đặt SPIFFS Tools
- Khi tải thư viện xong vào File -> Preferences. Trong cửa sổ Preferences, nhập đường dẫn sau vào ô Additional Boards Manager URLs: https://dl.espressif.com/dl/package_esp32_index.json rồi nhấn OK.
- Trên thanh công cụ, vào Tools -> Board -> DOIT ESP32 DEVKIT V1.
- Sau đó chọn cổng COM tương ứng bằng cách vào Tools -> Port -> cổng COM. 
Trình tự chạy chương trình:
- Mở file testCK.ino, tiến hành điền vào tên và mật khẩu wifi. Lưu và nhấn vào upload, để upload chương trình. Sau khi chương trình hiện "connecting..." thì nhấn giữ nút boot trên ESP32 khoảng 2s, đợi khi chương trình hiện chữ "Hard resetting via RTS pin...". 
- Sau đó vào tool và chọn ESP32 Sketch Data Upload, khi có thông báo là "connecting...___.." thì nhấn giữ nút boot trên ESP32 khoảng 2s, đợi khi chương trình hiện chữ "Hard resetting.." 
- Sau đó nhấn EN để kích hoạt chương trình. 
- Cuối cùng mở Serial Monitor để xem địa chỉ IP, gán IP đó vào trình duyệt (lưu ý wifi của client và wifi điền trong file testCK.ino phải giống nhau).

# Sơ đồ lắp mạch
![smartcart](https://user-images.githubusercontent.com/80655006/194472178-e836f672-cdac-44e7-a17c-dd02810862c2.png)
