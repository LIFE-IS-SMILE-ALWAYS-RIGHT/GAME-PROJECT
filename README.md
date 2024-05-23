**BRICK BREAKER** là một trò chơi đơn giản với nhiệm vụ phá hết những viên gạch có sẵn


![Screenshot (56)](https://github.com/LIFE-IS-SMILE-ALWAYS-RIGHT/GAME-PROJECT/assets/160494904/4c3a8511-5ee2-4f47-90ce-02e9c64c7042)



**CÁCH CHƠI**
Rất đơn giản, bạn chỉ cần dùng hai mũi tên trái và phải để điều khiển bệ đỡ để. Hãy khéo léo điều khiển bệ để quả bóng phá hết những viên gạch


**CÁCH CHẠY CHƯƠNG TRÌNH**
Cài đặt toàn bộ file và chạy trên Code::Blocks 20.03
Bấm vào ô lệch "Build and run" hoặc nut F9 để chạy chương trình



**CÁC THƯ VIỆN CẦN THIẾT ĐỂ CHẠY CHƯƠNG TRÌNH**
Dùng Google tìm kiếm các thư viện đã được liệt kê sẵn trong file library. Tải về và giải nén. Mở phần Settings/Compiler. Copy các file thích hợp
Ở phần Linker settings, ô Other linker options, hãy chép vào: -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

![Screenshot (57)](https://github.com/LIFE-IS-SMILE-ALWAYS-RIGHT/GAME-PROJECT/assets/160494904/0901c5a1-cd0f-49c4-91b3-15568aa37271)


Ở phần Search directories, ô Compiler, thêm đường dẫn:  
…..\x86_64-w64-mingw32\include\SDL2 
Ở phần Search directories, ô Linker, thêm đường dẫn:
…..\x86_64-w64-mingw32\lib 



![Screenshot (58)](https://github.com/LIFE-IS-SMILE-ALWAYS-RIGHT/GAME-PROJECT/assets/160494904/cea9f73f-9388-4ecb-86ca-4d961225802a)
![Screenshot (59)](https://github.com/LIFE-IS-SMILE-ALWAYS-RIGHT/GAME-PROJECT/assets/160494904/1998fc27-768f-43ac-9416-03fd79c300a8)

Việc cuối cùng hãy copy file dll vào thư mục mã nguồn project (nơi có các file .cpp)
…..\x86_64-w64-mingw32\bin\SDL2.dll
Đừng quên việc tải ảnh về và đưa vào thư mục mã nguồn, thế là bạn có thể chạy chương trình được rồi. Dưới đây là những gì file mã nguồn cần có
![Screenshot (60)](https://github.com/LIFE-IS-SMILE-ALWAYS-RIGHT/GAME-PROJECT/assets/160494904/759b38cf-9e94-406a-834a-2f3dbacd1380)

**TƯ LIỆU VÀ THAM KHẢO**
Về chương trình:

Chương trình được viết bằng ngôn ngữ lập trình C++
Sử dụng thêm thư viện đồ họa SDL 
Về hình ảnh các bạn có thể thay thế bằng cách tìm đoạn code sau và thay "sky 1" bằng tên ảnh mà bạn muốn, nhớ đưa ảnh vào file mã nguồn nhé
SDL_Texture* background = loadTexture("sky 1.jpg", renderer);







