🎮 EMBEDDED HANDHELD CONSOLE (DNU-EHC)

🌟 Tóm Tắt Dự Án (Project Abstract)

Dự án Máy Chơi Game Cầm Tay là một bài tập lớn thuộc học phần Hệ Thống Nhúng tại Trường Đại học Đại Nam.
Mục tiêu cốt lõi là thiết kế và triển khai một hệ thống nhúng hoàn chỉnh, tự chủ từ phần cứng đến phần mềm, sử dụng vi điều khiển Arduino Uno.

Dự án này nhằm chứng minh khả năng tích hợp hệ thống, rèn luyện tư duy giải quyết vấn đề thực tế, và vận dụng các kiến thức nền tảng về kiến trúc vi điều khiển và giao tiếp ngoại vi.

🛠️ Công Nghệ và Kiến Trúc (Technology Stack & Architecture)
1. Phần cứng (Hardware)

Vi điều khiển chính: Arduino Uno

Thiết bị ngoại vi: Màn hình TFT/ST7789, 4 nút điều hướng, 1 nút chức năng, loa nhỏ

Kiến trúc: Tích hợp phức tạp, yêu cầu phối hợp nhịp nhàng giữa các thành phần để tối ưu hóa hiệu suất đồ họa và quản lý năng lượng

2. Phần mềm (Software)

Ngôn ngữ lập trình: C / C++ (Arduino)

IDE: Arduino IDE

Yêu cầu tối ưu hóa: Quản lý tài nguyên chặt chẽ do hạn chế về bộ nhớ và tốc độ của Arduino Uno

⚙️ Yêu Cầu Kỹ Thuật (Technical Requirements)
Chức năng	Mô tả chi tiết
Trò chơi mẫu	Chạy ít nhất một trò chơi cơ bản (ví dụ Snake)
Xử lý đầu vào	4 nút điều hướng + 1 nút chức năng để tương tác
Hiển thị	Cập nhật trạng thái trò chơi, điểm số và đồ họa cơ bản theo thời gian thực
Phản hồi	Cung cấp âm thanh đơn giản (buzzer) để tăng tính tương tác
Giao tiếp	Kết nối và giao tiếp giữa Arduino và các thiết bị ngoại vi (TFT, nút, loa)
📂 Cấu Trúc Thư Mục (Folder Structure)
DNU-Embedded-Handheld-Console/
├── src/           # Code Arduino chính
│   ├── main.ino
│   ├── game/
│   └── display/
├── images/        # Hình minh họa, GIF demo
│   └── demo.png
├── docs/          # Tài liệu, sơ đồ mạch, datasheet
├── videos/        # Video demo
└── README.md



🚀 Hướng Dẫn Cài Đặt (Setup Instructions)

Clone repo về máy:

git clone https://github.com/YourUsername/DNU-Embedded-Handheld-Console.git
cd DNU-Embedded-Handheld-Console


Cài đặt Arduino IDE và các thư viện:

Adafruit GFX

Adafruit ST7789

Kết nối Arduino Uno theo sơ đồ mạch trong /docs

Upload code từ /src/main.ino

Thưởng thức trò chơi! 🎮

📌 Liên Hệ (Contact)

Tác giả: Noname

Email: thangxeom88@gmail.com
GitHub: https://github.com/vdt143205-sketch
