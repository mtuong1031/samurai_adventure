[[English]](README_en.md)

# Samurai .......

  Bài tập lớn môn Lập Trình Nâng Cao của Trường Đại học Công Nghệ - Đại Học Quốc Gia Hà Nội

# Giới thiệu game

... là tựa game thuộc thể loại Roguelike. Chìm đắm vào hành trình khám phá hầm ngục, cố gắng tiêu diệt tất cả kẻ thù và những con Boss khó nhằn. Hãy cùng mình khám phá tựa game này.


- [Thông tin sinh viên](#thông-tin-sinh-viên)
- [Cách tải game](#cách-tải-game)
    * [Cách 1: Không bao gồm code.](#cách-1-không-bao-gồm-code)
    * [Cách 2: Bao gồm code và có thể biên dịch.](#cách-2-bao-gồm-code-và-có-thể-biên-dịch)
- [Bắt đầu game](#bắt-đầu-game)
- [Cách chơi](#cách-chơi)
- [Chiến thắng và thất bại](#chiến-thắng-và-thất-bại)
- [Về đồ họa của game:](#về-đồ-họa-của-game)
- [Về source code game](#về-source-code-game)

# Thông tin sinh viên

  Họ tên : Lù Minh Tường

  Mã SV: 23020705

  Lớp: K68 - J

  Email: 23020705@gmail.com

# Cách tải game

## Cách 1: Không bao gồm code.

Tải game (được nén thành .zip) tại link
sau: https://github.com/nmhienbn/PVZ-Kaito-NMH-Edition/releases/tag/published<br/>
Cách này tiết kiệm bộ nhớ và thời gian tải đi rất nhiều (khoảng 103MB).<br/>
Giải nén game vào một thư mục và bật pvz.exe lên và chơi.<br/>

## Cách 2: Bao gồm code và có thể biên dịch.

**Bước 1:** Clone repo này về (Khoảng 800 MB - do có cả lịch sử các phiên bản). <br/>
Hoặc Chọn Code -> Download Zip (Khoảng 90MB)

![image](image/readme/1.png)

- Tất cả code đều nằm trong thư mục src.<br/>
- Có thể nhảy đến bước 5 để chơi luôn.

**Bước 2:** Cài mingw. Ở đây tôi cài đặt phiên bản MinGW-W64
GCC-8.1.0 [x86_64-posix-seh](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/8.1.0/threads-posix/seh/x86_64-8.1.0-release-posix-seh-rt_v6-rev0.7z/download).<br/>
Một số chú ý: <br/>

- Nếu sử dụng trình biên dịch khác thì phải dịch lại icon: vào thư mục icon rồi dùng
  lệnh `windres main.rc -O coff main.res` trong cmd.<br/>
- Phải xóa ba dll của c++ trong thư mục chứa pvz.exe: `libgcc_s_seh-1.dll`, `libstdc++-6.dll`, `libwinpthread-1.dll`. Ba
  dll này chỉ dành cho người chơi không cài sẵn c++ trong máy.

**Bước 3:** Cài GNU make

**Bước 4:** Mở cmd trong thư mục chứa Makefile và gõ lệnh make.

**Bước 5:** Bật pvz.exe và chơi

# Bắt đầu game

Chờ một chút rồi click vào bất kì điểm nào trên màn hình để bắt đầu chơi game.
<div style="text-align: center;">

![ezgif com-video-to-gif](resources/preview/loading.gif)

</div>

# Cách chơi

  Bạn sẽ bắt đầu cùng nhân vật cơ bản là ... với các kĩ năng là sử dụng kiếm và khả năng phòng ra lưỡi kiếm ma thuật tấn công từ xa. 

  ![main](./image/readme/main.png)

  Sử dụng các phím W A S D để di chuyển và hai phím I J để tấn công.

  Trong quá trình chiến đấu, bạn có thể nhận những chiếc rương báu để bổ sung HP.

  Hãy cố gắng sống sót và tiêu diệt Boss của hầm ngục để dành chiến thắng.  

![boss](./image/readme/boss.png)

# Chiến thắng và thất bại

- Bạn sẽ chiến thắng nếu tiêu diệt Boss hầm ngục.

<div style="text-align: center;">

![image](resources/preview/win.png)
</div>

- Bạn sẽ thất bại nếu hết HP.

<div style="text-align: center;">

![image](resources/preview/lose.png)
</div>

---

# Về đồ họa của game:

  Các tài nguyên đồ họa, hình ảnh trong game được sử dụng từ các nguồn miễn phí trên trang web: [itch.io](https://itch.io/) và được chỉnh sửa thông qua [App Aseprite](https://www.aseprite.org/).

# Về source code game:

  Được tham khảo và phát triển từ Tutorial [How To Make A Game] của Youtuber: [Let's Make Games](https://www.youtube.com/playlist?list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx).

- Folder ECS: Hệ thống ECS và các thành phần liên quan.
    * ECS.cpp và ECS.h: Hệ thống ECS (Entity-Component-System) chia mọi thứ trong game thành 3 khái niệm chính: Entity, Component và System để dễ quản lí và thao tác.
    * Component.h: Khai báo các thư viện trong ECS.
    * SpriteComponent.h: Quản lí các thành phần hình ảnh trong ECS.
    * TransformComponent.h: Quản lí tọa độ, sự thay đổi tọa độ của các thành phần ECS
    * TileComponent.h
- Folder assets: Chứa tài nguyên về các thành phần của game:
    
- Folder img: Chứa các tài nguyên hình ảnh
- Folder music: Chứa các tài nguyên âm thanh
- AssetManager.cpp và AssetManager.h: Quàn lí các thành phần chính cấu thành nên game
- TextureManager.cpp và TextureManager.h: Quản lí thành phần texture.
- Collision.cpp và Collision.h: Quản lí va chạm.
- Map.cpp và Map.h: Quản lí map cũng như kĩ thuật tilemap.
- Game.cpp và Game.h: quản lí các thay đổi, sự kiện của trò chơi.
- game.hpp và game.cpp: hàm main() của trò chơi.
