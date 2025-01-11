#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
		// Khởi tạo Pipe
    int p[2]; // tạo một mảng p lưu hai file descriptor cho pipe.
    char buf[100];  
    pipe(p); //khởi tạo pipe, trong đó p[0] là đầu đọc, và p[1] là đầu ghi.
	
		// Tách Tiến Trình
		// fork() tạo ra một tiến trình con. Sau dòng lệnh này, ta có hai tiến trình: cha và con.
    int pid = fork(); 
    // pid sẽ chứa ID của tiến trình con trong tiến trình cha, hoặc 0 trong tiến trình con.
    if (pid == 0) // tiến trình con (pid == 0).
    {
        write(p[1], "ping", 1); // gửi một byte ("ping") tới tiến trình cha thông qua pipe.
        printf("%d: received ping\n", getpid()); // in ra thông báo với ID của tiến trình con, cho biết nó đã nhận thông điệp từ tiến trình cha.
    }
    //Sau khi in ra thông báo, tiến trình con kết thúc.
    
    else // Tiến Trình Cha:
    {
        wait(0); // đợi tiến trình con hoàn thành.
        read(p[0], buf, 1); // đọc byte mà tiến trình con gửi và lưu vào buf.
        printf("%d: received pong\n", getpid()); // in ra thông báo với ID của tiến trình cha, cho biết nó đã nhận phản hồi ("pong") từ tiến trình con.
    }
}