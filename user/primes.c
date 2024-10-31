#include "kernel/types.h"
#include "user/user.h"

void prime_sieve(int* fd) __attribute__((noreturn));

void prime_sieve(int* fd)
{
    int prime;

    if (read(fd[0], &prime, sizeof(int)) == 0) {
        close(fd[0]); // Đóng pipe đọc
        exit(0); // Không còn dữ liệu để xử lý
    } else {
        fprintf(2, "prime %d\n", prime); // In ra số nguyên tố tìm được
        int p[2];
        pipe(p); // Tạo pipe cho quá trình tiếp theo

        if (fork() == 0) { // Tiến trình con
            close(fd[0]); // Đóng pipe đọc từ tiến trình trước
            close(p[1]); // Đóng đầu ghi của pipe mới
            prime_sieve(p); // Đệ quy sang tiến trình tiếp theo
            exit(0);
        } else { // Tiến trình cha
            close(p[0]); // Đóng đầu đọc của pipe mới
            
            int i;
            while (read(fd[0], &i, sizeof(int)) > 0) {
                if (i % prime != 0) { // Chỉ ghi số không chia hết cho prime
                    write(p[1], &i, sizeof(int));
                }
            }
            
            close(p[1]); // Đóng đầu ghi khi hoàn thành
            close(fd[0]); // Đóng đầu đọc hiện tại
            wait(0); // Đợi tiến trình con hoàn thành
            exit(0);
        }
    }
}

int main(int argc, char *argv[])
{
    int N = 280; // Giới hạn số nguyên tố cần tìm
    int fd[2];

    pipe(fd); // Tạo pipe đầu tiên

    if (fork() == 0) { // Tiến trình con để xử lý các số nguyên tố
        close(fd[1]); // Đóng đầu ghi vì tiến trình này chỉ đọc
        prime_sieve(fd); // Gọi hàm xử lý
    } else { // Tiến trình cha ghi số vào pipe
        for (int i = 2; i <= N; i++) {
            write(fd[1], &i, sizeof(int)); // Ghi các số 2 đến N vào pipe
        }
        close(fd[0]); // Đóng đầu đọc
        close(fd[1]); // Đóng đầu ghi
        wait(0); // Đợi tiến trình con hoàn thành
    }
    exit(0); // Kết thúc chương trình
}
