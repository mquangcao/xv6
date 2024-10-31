#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char* argv[])
{
    int p1[2]; // Pipe từ tiến trình cha đến tiến trình con
    int p2[2]; // Pipe từ tiến trình con đến tiến trình cha
    char byte = 'A'; // Byte để gửi
    char bytefer;

    // Tạo hai pipe và lưu các file descriptor vào p1 và p2
    if (pipe(p1) < 0 || pipe(p2) < 0) {
        fprintf(2, "Error: Cannot create pipes\n");
        exit(1);
    }

    int pid = fork();

    if (pid > 0) { // Tiến trình cha
        // Ghi byte "ping" vào pipe p1[1]
        if (write(p1[1], &byte, 1) < 0) {
            fprintf(2, "Error: Parent cannot write to pipe\n");
            close(p1[0]);
            close(p1[1]);
            close(p2[0]);
            close(p2[1]);
            exit(1);
        }
        close(p1[1]); // Đóng p1[1] sau khi ghi xong

        // Chờ tiến trình con hoàn thành
        wait((int *) 0);

        // Đọc byte từ pipe p2[0] (từ con gửi về)
        if (read(p2[0], &bytefer, 1) < 0) {
            fprintf(2, "Error: Parent cannot read from pipe\n");
            close(p1[0]);
            close(p2[0]);
            exit(1);
        }
        printf("%d: received pong\n", getpid());
        close(p2[0]); // Đóng p2[0] sau khi đọc xong

    } else if (pid == 0) { // Tiến trình con
        // Đọc byte từ pipe p1[0] (từ cha gửi xuống)
        if (read(p1[0], &bytefer, 1) < 0) {
            fprintf(2, "Error: Child cannot read from pipe\n");
            close(p1[0]);
            close(p2[1]);
            exit(1);
        }
        close(p1[0]); // Đóng p1[0] sau khi đọc xong
        printf("%d: received ping\n", getpid());

        // Ghi byte "pong" vào pipe p2[1]
        bytefer = 'B';
        if (write(p2[1], &bytefer, 1) < 0) {
            fprintf(2, "Error: Child cannot write to pipe\n");
            close(p2[1]);
            exit(1);
        }
        close(p2[1]); // Đóng p2[1] sau khi ghi xong

        exit(0);

    } else { // fork() thất bại
        fprintf(2, "Error: fork failed\n");
        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);
        exit(1);
    }

    exit(0);
}
